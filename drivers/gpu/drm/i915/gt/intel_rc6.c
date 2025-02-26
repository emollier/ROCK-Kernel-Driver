/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright © 2019 Intel Corporation
 */

#include <linux/pm_runtime.h>

#include "i915_drv.h"
#include "intel_gt.h"
#include "intel_gt_pm.h"
#include "intel_rc6.h"
#include "intel_sideband.h"

/**
 * DOC: RC6
 *
 * RC6 is a special power stage which allows the GPU to enter an very
 * low-voltage mode when idle, using down to 0V while at this stage.  This
 * stage is entered automatically when the GPU is idle when RC6 support is
 * enabled, and as soon as new workload arises GPU wakes up automatically as
 * well.
 *
 * There are different RC6 modes available in Intel GPU, which differentiate
 * among each other with the latency required to enter and leave RC6 and
 * voltage consumed by the GPU in different states.
 *
 * The combination of the following flags define which states GPU is allowed
 * to enter, while RC6 is the normal RC6 state, RC6p is the deep RC6, and
 * RC6pp is deepest RC6. Their support by hardware varies according to the
 * GPU, BIOS, chipset and platform. RC6 is usually the safest one and the one
 * which brings the most power savings; deeper states save more power, but
 * require higher latency to switch to and wake up.
 */

static struct intel_gt *rc6_to_gt(struct intel_rc6 *rc6)
{
	return container_of(rc6, struct intel_gt, rc6);
}

static struct intel_uncore *rc6_to_uncore(struct intel_rc6 *rc)
{
	return rc6_to_gt(rc)->uncore;
}

static struct drm_i915_private *rc6_to_i915(struct intel_rc6 *rc)
{
	return rc6_to_gt(rc)->i915;
}

static inline void set(struct intel_uncore *uncore, i915_reg_t reg, u32 val)
{
	intel_uncore_write_fw(uncore, reg, val);
}

static void gen11_rc6_enable(struct intel_rc6 *rc6)
{
	struct intel_uncore *uncore = rc6_to_uncore(rc6);
	struct intel_engine_cs *engine;
	enum intel_engine_id id;

	/* 2b: Program RC6 thresholds.*/
	set(uncore, GEN6_RC6_WAKE_RATE_LIMIT, 54 << 16 | 85);
	set(uncore, GEN10_MEDIA_WAKE_RATE_LIMIT, 150);

	set(uncore, GEN6_RC_EVALUATION_INTERVAL, 125000); /* 12500 * 1280ns */
	set(uncore, GEN6_RC_IDLE_HYSTERSIS, 25); /* 25 * 1280ns */
	for_each_engine(engine, rc6_to_gt(rc6), id)
		set(uncore, RING_MAX_IDLE(engine->mmio_base), 10);

	set(uncore, GUC_MAX_IDLE_COUNT, 0xA);

	set(uncore, GEN6_RC_SLEEP, 0);

	set(uncore, GEN6_RC6_THRESHOLD, 50000); /* 50/125ms per EI */

	/*
	 * 2c: Program Coarse Power Gating Policies.
	 *
	 * Bspec's guidance is to use 25us (really 25 * 1280ns) here. What we
	 * use instead is a more conservative estimate for the maximum time
	 * it takes us to service a CS interrupt and submit a new ELSP - that
	 * is the time which the GPU is idle waiting for the CPU to select the
	 * next request to execute. If the idle hysteresis is less than that
	 * interrupt service latency, the hardware will automatically gate
	 * the power well and we will then incur the wake up cost on top of
	 * the service latency. A similar guide from plane_state is that we
	 * do not want the enable hysteresis to less than the wakeup latency.
	 *
	 * igt/gem_exec_nop/sequential provides a rough estimate for the
	 * service latency, and puts it around 10us for Broadwell (and other
	 * big core) and around 40us for Broxton (and other low power cores).
	 * [Note that for legacy ringbuffer submission, this is less than 1us!]
	 * However, the wakeup latency on Broxton is closer to 100us. To be
	 * conservative, we have to factor in a context switch on top (due
	 * to ksoftirqd).
	 */
	set(uncore, GEN9_MEDIA_PG_IDLE_HYSTERESIS, 250);
	set(uncore, GEN9_RENDER_PG_IDLE_HYSTERESIS, 250);

	/* 3a: Enable RC6 */
	set(uncore, GEN6_RC_CONTROL,
	    GEN6_RC_CTL_HW_ENABLE |
	    GEN6_RC_CTL_RC6_ENABLE |
	    GEN6_RC_CTL_EI_MODE(1));

	set(uncore, GEN9_PG_ENABLE,
	    GEN9_RENDER_PG_ENABLE |
	    GEN9_MEDIA_PG_ENABLE |
	    GEN11_MEDIA_SAMPLER_PG_ENABLE);
}

static void gen9_rc6_enable(struct intel_rc6 *rc6)
{
	struct intel_uncore *uncore = rc6_to_uncore(rc6);
	struct intel_engine_cs *engine;
	enum intel_engine_id id;
	u32 rc6_mode;

	/* 2b: Program RC6 thresholds.*/
	if (INTEL_GEN(rc6_to_i915(rc6)) >= 10) {
		set(uncore, GEN6_RC6_WAKE_RATE_LIMIT, 54 << 16 | 85);
		set(uncore, GEN10_MEDIA_WAKE_RATE_LIMIT, 150);
	} else if (IS_SKYLAKE(rc6_to_i915(rc6))) {
		/*
		 * WaRsDoubleRc6WrlWithCoarsePowerGating:skl Doubling WRL only
		 * when CPG is enabled
		 */
		set(uncore, GEN6_RC6_WAKE_RATE_LIMIT, 108 << 16);
	} else {
		set(uncore, GEN6_RC6_WAKE_RATE_LIMIT, 54 << 16);
	}

	set(uncore, GEN6_RC_EVALUATION_INTERVAL, 125000); /* 12500 * 1280ns */
	set(uncore, GEN6_RC_IDLE_HYSTERSIS, 25); /* 25 * 1280ns */
	for_each_engine(engine, rc6_to_gt(rc6), id)
		set(uncore, RING_MAX_IDLE(engine->mmio_base), 10);

	set(uncore, GUC_MAX_IDLE_COUNT, 0xA);

	set(uncore, GEN6_RC_SLEEP, 0);

	/*
	 * 2c: Program Coarse Power Gating Policies.
	 *
	 * Bspec's guidance is to use 25us (really 25 * 1280ns) here. What we
	 * use instead is a more conservative estimate for the maximum time
	 * it takes us to service a CS interrupt and submit a new ELSP - that
	 * is the time which the GPU is idle waiting for the CPU to select the
	 * next request to execute. If the idle hysteresis is less than that
	 * interrupt service latency, the hardware will automatically gate
	 * the power well and we will then incur the wake up cost on top of
	 * the service latency. A similar guide from plane_state is that we
	 * do not want the enable hysteresis to less than the wakeup latency.
	 *
	 * igt/gem_exec_nop/sequential provides a rough estimate for the
	 * service latency, and puts it around 10us for Broadwell (and other
	 * big core) and around 40us for Broxton (and other low power cores).
	 * [Note that for legacy ringbuffer submission, this is less than 1us!]
	 * However, the wakeup latency on Broxton is closer to 100us. To be
	 * conservative, we have to factor in a context switch on top (due
	 * to ksoftirqd).
	 */
	set(uncore, GEN9_MEDIA_PG_IDLE_HYSTERESIS, 250);
	set(uncore, GEN9_RENDER_PG_IDLE_HYSTERESIS, 250);

	/* 3a: Enable RC6 */
	set(uncore, GEN6_RC6_THRESHOLD, 37500); /* 37.5/125ms per EI */

	/* WaRsUseTimeoutMode:cnl (pre-prod) */
	if (IS_CNL_REVID(rc6_to_i915(rc6), CNL_REVID_A0, CNL_REVID_C0))
		rc6_mode = GEN7_RC_CTL_TO_MODE;
	else
		rc6_mode = GEN6_RC_CTL_EI_MODE(1);

	set(uncore, GEN6_RC_CONTROL,
	    GEN6_RC_CTL_HW_ENABLE |
	    GEN6_RC_CTL_RC6_ENABLE |
	    rc6_mode);

	set(uncore, GEN9_PG_ENABLE,
	    GEN9_RENDER_PG_ENABLE | GEN9_MEDIA_PG_ENABLE);
}

static void gen8_rc6_enable(struct intel_rc6 *rc6)
{
	struct intel_uncore *uncore = rc6_to_uncore(rc6);
	struct intel_engine_cs *engine;
	enum intel_engine_id id;

	/* 2b: Program RC6 thresholds.*/
	set(uncore, GEN6_RC6_WAKE_RATE_LIMIT, 40 << 16);
	set(uncore, GEN6_RC_EVALUATION_INTERVAL, 125000); /* 12500 * 1280ns */
	set(uncore, GEN6_RC_IDLE_HYSTERSIS, 25); /* 25 * 1280ns */
	for_each_engine(engine, rc6_to_gt(rc6), id)
		set(uncore, RING_MAX_IDLE(engine->mmio_base), 10);
	set(uncore, GEN6_RC_SLEEP, 0);
	set(uncore, GEN6_RC6_THRESHOLD, 625); /* 800us/1.28 for TO */

	/* 3: Enable RC6 */
	set(uncore, GEN6_RC_CONTROL,
	    GEN6_RC_CTL_HW_ENABLE |
	    GEN7_RC_CTL_TO_MODE |
	    GEN6_RC_CTL_RC6_ENABLE);
}

static void gen6_rc6_enable(struct intel_rc6 *rc6)
{
	struct intel_uncore *uncore = rc6_to_uncore(rc6);
	struct drm_i915_private *i915 = rc6_to_i915(rc6);
	struct intel_engine_cs *engine;
	enum intel_engine_id id;
	u32 rc6vids, rc6_mask;
	int ret;

	set(uncore, GEN6_RC1_WAKE_RATE_LIMIT, 1000 << 16);
	set(uncore, GEN6_RC6_WAKE_RATE_LIMIT, 40 << 16 | 30);
	set(uncore, GEN6_RC6pp_WAKE_RATE_LIMIT, 30);
	set(uncore, GEN6_RC_EVALUATION_INTERVAL, 125000);
	set(uncore, GEN6_RC_IDLE_HYSTERSIS, 25);

	for_each_engine(engine, rc6_to_gt(rc6), id)
		set(uncore, RING_MAX_IDLE(engine->mmio_base), 10);

	set(uncore, GEN6_RC_SLEEP, 0);
	set(uncore, GEN6_RC1e_THRESHOLD, 1000);
	if (IS_IVYBRIDGE(i915))
		set(uncore, GEN6_RC6_THRESHOLD, 125000);
	else
		set(uncore, GEN6_RC6_THRESHOLD, 50000);
	set(uncore, GEN6_RC6p_THRESHOLD, 150000);
	set(uncore, GEN6_RC6pp_THRESHOLD, 64000); /* unused */

	/* We don't use those on Haswell */
	rc6_mask = GEN6_RC_CTL_RC6_ENABLE;
	if (HAS_RC6p(i915))
		rc6_mask |= GEN6_RC_CTL_RC6p_ENABLE;
	if (HAS_RC6pp(i915))
		rc6_mask |= GEN6_RC_CTL_RC6pp_ENABLE;
	set(uncore, GEN6_RC_CONTROL,
	    rc6_mask |
	    GEN6_RC_CTL_EI_MODE(1) |
	    GEN6_RC_CTL_HW_ENABLE);

	rc6vids = 0;
	ret = sandybridge_pcode_read(i915, GEN6_PCODE_READ_RC6VIDS,
				     &rc6vids, NULL);
	if (IS_GEN(i915, 6) && ret) {
		DRM_DEBUG_DRIVER("Couldn't check for BIOS workaround\n");
	} else if (IS_GEN(i915, 6) &&
		   (GEN6_DECODE_RC6_VID(rc6vids & 0xff) < 450)) {
		DRM_DEBUG_DRIVER("You should update your BIOS. Correcting minimum rc6 voltage (%dmV->%dmV)\n",
				 GEN6_DECODE_RC6_VID(rc6vids & 0xff), 450);
		rc6vids &= 0xffff00;
		rc6vids |= GEN6_ENCODE_RC6_VID(450);
		ret = sandybridge_pcode_write(i915, GEN6_PCODE_WRITE_RC6VIDS, rc6vids);
		if (ret)
			DRM_ERROR("Couldn't fix incorrect rc6 voltage\n");
	}
}

/* Check that the pcbr address is not empty. */
static int chv_rc6_init(struct intel_rc6 *rc6)
{
	struct intel_uncore *uncore = rc6_to_uncore(rc6);
	resource_size_t pctx_paddr, paddr;
	resource_size_t pctx_size = 32 * SZ_1K;
	u32 pcbr;

	pcbr = intel_uncore_read(uncore, VLV_PCBR);
	if ((pcbr >> VLV_PCBR_ADDR_SHIFT) == 0) {
		DRM_DEBUG_DRIVER("BIOS didn't set up PCBR, fixing up\n");
		paddr = rc6_to_i915(rc6)->dsm.end + 1 - pctx_size;
		GEM_BUG_ON(paddr > U32_MAX);

		pctx_paddr = (paddr & ~4095);
		intel_uncore_write(uncore, VLV_PCBR, pctx_paddr);
	}

	return 0;
}

static int vlv_rc6_init(struct intel_rc6 *rc6)
{
	struct drm_i915_private *i915 = rc6_to_i915(rc6);
	struct intel_uncore *uncore = rc6_to_uncore(rc6);
	struct drm_i915_gem_object *pctx;
	resource_size_t pctx_paddr;
	resource_size_t pctx_size = 24 * SZ_1K;
	u32 pcbr;

	pcbr = intel_uncore_read(uncore, VLV_PCBR);
	if (pcbr) {
		/* BIOS set it up already, grab the pre-alloc'd space */
		resource_size_t pcbr_offset;

		pcbr_offset = (pcbr & ~4095) - i915->dsm.start;
		pctx = i915_gem_object_create_stolen_for_preallocated(i915,
								      pcbr_offset,
								      I915_GTT_OFFSET_NONE,
								      pctx_size);
		if (IS_ERR(pctx))
			return PTR_ERR(pctx);

		goto out;
	}

	DRM_DEBUG_DRIVER("BIOS didn't set up PCBR, fixing up\n");

	/*
	 * From the Gunit register HAS:
	 * The Gfx driver is expected to program this register and ensure
	 * proper allocation within Gfx stolen memory.  For example, this
	 * register should be programmed such than the PCBR range does not
	 * overlap with other ranges, such as the frame buffer, protected
	 * memory, or any other relevant ranges.
	 */
	pctx = i915_gem_object_create_stolen(i915, pctx_size);
	if (IS_ERR(pctx)) {
		DRM_DEBUG("not enough stolen space for PCTX, disabling\n");
		return PTR_ERR(pctx);
	}

	GEM_BUG_ON(range_overflows_t(u64,
				     i915->dsm.start,
				     pctx->stolen->start,
				     U32_MAX));
	pctx_paddr = i915->dsm.start + pctx->stolen->start;
	intel_uncore_write(uncore, VLV_PCBR, pctx_paddr);

out:
	rc6->pctx = pctx;
	return 0;
}

static void chv_rc6_enable(struct intel_rc6 *rc6)
{
	struct intel_uncore *uncore = rc6_to_uncore(rc6);
	struct intel_engine_cs *engine;
	enum intel_engine_id id;

	/* 2a: Program RC6 thresholds.*/
	set(uncore, GEN6_RC6_WAKE_RATE_LIMIT, 40 << 16);
	set(uncore, GEN6_RC_EVALUATION_INTERVAL, 125000); /* 12500 * 1280ns */
	set(uncore, GEN6_RC_IDLE_HYSTERSIS, 25); /* 25 * 1280ns */

	for_each_engine(engine, rc6_to_gt(rc6), id)
		set(uncore, RING_MAX_IDLE(engine->mmio_base), 10);
	set(uncore, GEN6_RC_SLEEP, 0);

	/* TO threshold set to 500 us (0x186 * 1.28 us) */
	set(uncore, GEN6_RC6_THRESHOLD, 0x186);

	/* Allows RC6 residency counter to work */
	set(uncore, VLV_COUNTER_CONTROL,
	    _MASKED_BIT_ENABLE(VLV_COUNT_RANGE_HIGH |
			       VLV_MEDIA_RC6_COUNT_EN |
			       VLV_RENDER_RC6_COUNT_EN));

	/* 3: Enable RC6 */
	set(uncore, GEN6_RC_CONTROL, GEN7_RC_CTL_TO_MODE);
}

static void vlv_rc6_enable(struct intel_rc6 *rc6)
{
	struct intel_uncore *uncore = rc6_to_uncore(rc6);
	struct intel_engine_cs *engine;
	enum intel_engine_id id;

	set(uncore, GEN6_RC6_WAKE_RATE_LIMIT, 0x00280000);
	set(uncore, GEN6_RC_EVALUATION_INTERVAL, 125000);
	set(uncore, GEN6_RC_IDLE_HYSTERSIS, 25);

	for_each_engine(engine, rc6_to_gt(rc6), id)
		set(uncore, RING_MAX_IDLE(engine->mmio_base), 10);

	set(uncore, GEN6_RC6_THRESHOLD, 0x557);

	/* Allows RC6 residency counter to work */
	set(uncore, VLV_COUNTER_CONTROL,
	    _MASKED_BIT_ENABLE(VLV_COUNT_RANGE_HIGH |
			       VLV_MEDIA_RC0_COUNT_EN |
			       VLV_RENDER_RC0_COUNT_EN |
			       VLV_MEDIA_RC6_COUNT_EN |
			       VLV_RENDER_RC6_COUNT_EN));

	set(uncore, GEN6_RC_CONTROL,
	    GEN7_RC_CTL_TO_MODE | VLV_RC_CTL_CTX_RST_PARALLEL);
}

static bool bxt_check_bios_rc6_setup(struct intel_rc6 *rc6)
{
	struct intel_uncore *uncore = rc6_to_uncore(rc6);
	struct drm_i915_private *i915 = rc6_to_i915(rc6);
	u32 rc6_ctx_base, rc_ctl, rc_sw_target;
	bool enable_rc6 = true;

	rc_ctl = intel_uncore_read(uncore, GEN6_RC_CONTROL);
	rc_sw_target = intel_uncore_read(uncore, GEN6_RC_STATE);
	rc_sw_target &= RC_SW_TARGET_STATE_MASK;
	rc_sw_target >>= RC_SW_TARGET_STATE_SHIFT;
	DRM_DEBUG_DRIVER("BIOS enabled RC states: "
			 "HW_CTRL %s HW_RC6 %s SW_TARGET_STATE %x\n",
			 onoff(rc_ctl & GEN6_RC_CTL_HW_ENABLE),
			 onoff(rc_ctl & GEN6_RC_CTL_RC6_ENABLE),
			 rc_sw_target);

	if (!(intel_uncore_read(uncore, RC6_LOCATION) & RC6_CTX_IN_DRAM)) {
		DRM_DEBUG_DRIVER("RC6 Base location not set properly.\n");
		enable_rc6 = false;
	}

	/*
	 * The exact context size is not known for BXT, so assume a page size
	 * for this check.
	 */
	rc6_ctx_base =
		intel_uncore_read(uncore, RC6_CTX_BASE) & RC6_CTX_BASE_MASK;
	if (!(rc6_ctx_base >= i915->dsm_reserved.start &&
	      rc6_ctx_base + PAGE_SIZE < i915->dsm_reserved.end)) {
		DRM_DEBUG_DRIVER("RC6 Base address not as expected.\n");
		enable_rc6 = false;
	}

	if (!((intel_uncore_read(uncore, PWRCTX_MAXCNT_RCSUNIT) & IDLE_TIME_MASK) > 1 &&
	      (intel_uncore_read(uncore, PWRCTX_MAXCNT_VCSUNIT0) & IDLE_TIME_MASK) > 1 &&
	      (intel_uncore_read(uncore, PWRCTX_MAXCNT_BCSUNIT) & IDLE_TIME_MASK) > 1 &&
	      (intel_uncore_read(uncore, PWRCTX_MAXCNT_VECSUNIT) & IDLE_TIME_MASK) > 1)) {
		DRM_DEBUG_DRIVER("Engine Idle wait time not set properly.\n");
		enable_rc6 = false;
	}

	if (!intel_uncore_read(uncore, GEN8_PUSHBUS_CONTROL) ||
	    !intel_uncore_read(uncore, GEN8_PUSHBUS_ENABLE) ||
	    !intel_uncore_read(uncore, GEN8_PUSHBUS_SHIFT)) {
		DRM_DEBUG_DRIVER("Pushbus not setup properly.\n");
		enable_rc6 = false;
	}

	if (!intel_uncore_read(uncore, GEN6_GFXPAUSE)) {
		DRM_DEBUG_DRIVER("GFX pause not setup properly.\n");
		enable_rc6 = false;
	}

	if (!intel_uncore_read(uncore, GEN8_MISC_CTRL0)) {
		DRM_DEBUG_DRIVER("GPM control not setup properly.\n");
		enable_rc6 = false;
	}

	return enable_rc6;
}

static bool rc6_supported(struct intel_rc6 *rc6)
{
	struct drm_i915_private *i915 = rc6_to_i915(rc6);

	if (!HAS_RC6(i915))
		return false;

	if (intel_vgpu_active(i915))
		return false;

	if (is_mock_gt(rc6_to_gt(rc6)))
		return false;

	if (IS_GEN9_LP(i915) && !bxt_check_bios_rc6_setup(rc6)) {
		dev_notice(i915->drm.dev,
			   "RC6 and powersaving disabled by BIOS\n");
		return false;
	}

	return true;
}

static void rpm_get(struct intel_rc6 *rc6)
{
	GEM_BUG_ON(rc6->wakeref);
	pm_runtime_get_sync(&rc6_to_i915(rc6)->drm.pdev->dev);
	rc6->wakeref = true;
}

static void rpm_put(struct intel_rc6 *rc6)
{
	GEM_BUG_ON(!rc6->wakeref);
	pm_runtime_put(&rc6_to_i915(rc6)->drm.pdev->dev);
	rc6->wakeref = false;
}

static bool intel_rc6_ctx_corrupted(struct intel_rc6 *rc6)
{
	return !intel_uncore_read(rc6_to_uncore(rc6), GEN8_RC6_CTX_INFO);
}

static void intel_rc6_ctx_wa_init(struct intel_rc6 *rc6)
{
	struct drm_i915_private *i915 = rc6_to_i915(rc6);

	if (!NEEDS_RC6_CTX_CORRUPTION_WA(i915))
		return;

	if (intel_rc6_ctx_corrupted(rc6)) {
		DRM_INFO("RC6 context corrupted, disabling runtime power management\n");
		rc6->ctx_corrupted = true;
	}
}

/**
 * intel_rc6_ctx_wa_resume - system resume sequence for the RC6 CTX WA
 * @rc6: rc6 state
 *
 * Perform any steps needed to re-init the RC6 CTX WA after system resume.
 */
void intel_rc6_ctx_wa_resume(struct intel_rc6 *rc6)
{
	if (rc6->ctx_corrupted && !intel_rc6_ctx_corrupted(rc6)) {
		DRM_INFO("RC6 context restored, re-enabling runtime power management\n");
		rc6->ctx_corrupted = false;
	}
}

/**
 * intel_rc6_ctx_wa_check - check for a new RC6 CTX corruption
 * @rc6: rc6 state
 *
 * Check if an RC6 CTX corruption has happened since the last check and if so
 * disable RC6 and runtime power management.
*/
void intel_rc6_ctx_wa_check(struct intel_rc6 *rc6)
{
	struct drm_i915_private *i915 = rc6_to_i915(rc6);

	if (!NEEDS_RC6_CTX_CORRUPTION_WA(i915))
		return;

	if (rc6->ctx_corrupted)
		return;

	if (!intel_rc6_ctx_corrupted(rc6))
		return;

	DRM_NOTE("RC6 context corruption, disabling runtime power management\n");

	intel_rc6_disable(rc6);
	rc6->ctx_corrupted = true;

	return;
}

static void __intel_rc6_disable(struct intel_rc6 *rc6)
{
	struct drm_i915_private *i915 = rc6_to_i915(rc6);
	struct intel_uncore *uncore = rc6_to_uncore(rc6);

	intel_uncore_forcewake_get(uncore, FORCEWAKE_ALL);
	if (INTEL_GEN(i915) >= 9)
		set(uncore, GEN9_PG_ENABLE, 0);
	set(uncore, GEN6_RC_CONTROL, 0);
	set(uncore, GEN6_RC_STATE, 0);
	intel_uncore_forcewake_put(uncore, FORCEWAKE_ALL);
}

void intel_rc6_init(struct intel_rc6 *rc6)
{
	struct drm_i915_private *i915 = rc6_to_i915(rc6);
	int err;

	/* Disable runtime-pm until we can save the GPU state with rc6 pctx */
	rpm_get(rc6);

	if (!rc6_supported(rc6))
		return;

	intel_rc6_ctx_wa_init(rc6);

	if (IS_CHERRYVIEW(i915))
		err = chv_rc6_init(rc6);
	else if (IS_VALLEYVIEW(i915))
		err = vlv_rc6_init(rc6);
	else
		err = 0;

	/* Sanitize rc6, ensure it is disabled before we are ready. */
	__intel_rc6_disable(rc6);

	rc6->supported = err == 0;
}

void intel_rc6_sanitize(struct intel_rc6 *rc6)
{
	if (rc6->enabled) { /* unbalanced suspend/resume */
		rpm_get(rc6);
		rc6->enabled = false;
	}

	if (rc6->supported)
		__intel_rc6_disable(rc6);
}

void intel_rc6_enable(struct intel_rc6 *rc6)
{
	struct drm_i915_private *i915 = rc6_to_i915(rc6);
	struct intel_uncore *uncore = rc6_to_uncore(rc6);

	if (!rc6->supported)
		return;

	GEM_BUG_ON(rc6->enabled);

	if (rc6->ctx_corrupted)
		return;

	intel_uncore_forcewake_get(uncore, FORCEWAKE_ALL);

	if (IS_CHERRYVIEW(i915))
		chv_rc6_enable(rc6);
	else if (IS_VALLEYVIEW(i915))
		vlv_rc6_enable(rc6);
	else if (INTEL_GEN(i915) >= 11)
		gen11_rc6_enable(rc6);
	else if (INTEL_GEN(i915) >= 9)
		gen9_rc6_enable(rc6);
	else if (IS_BROADWELL(i915))
		gen8_rc6_enable(rc6);
	else if (INTEL_GEN(i915) >= 6)
		gen6_rc6_enable(rc6);

	intel_uncore_forcewake_put(uncore, FORCEWAKE_ALL);

	/* rc6 is ready, runtime-pm is go! */
	rpm_put(rc6);
	rc6->enabled = true;
}

void intel_rc6_disable(struct intel_rc6 *rc6)
{
	if (!rc6->enabled)
		return;

	rpm_get(rc6);
	rc6->enabled = false;

	__intel_rc6_disable(rc6);
}

void intel_rc6_fini(struct intel_rc6 *rc6)
{
	struct drm_i915_gem_object *pctx;

	intel_rc6_disable(rc6);

	pctx = fetch_and_zero(&rc6->pctx);
	if (pctx)
		i915_gem_object_put(pctx);

	if (rc6->wakeref)
		rpm_put(rc6);
}

static u64 vlv_residency_raw(struct intel_uncore *uncore, const i915_reg_t reg)
{
	u32 lower, upper, tmp;
	int loop = 2;

	/*
	 * The register accessed do not need forcewake. We borrow
	 * uncore lock to prevent concurrent access to range reg.
	 */
	lockdep_assert_held(&uncore->lock);

	/*
	 * vlv and chv residency counters are 40 bits in width.
	 * With a control bit, we can choose between upper or lower
	 * 32bit window into this counter.
	 *
	 * Although we always use the counter in high-range mode elsewhere,
	 * userspace may attempt to read the value before rc6 is initialised,
	 * before we have set the default VLV_COUNTER_CONTROL value. So always
	 * set the high bit to be safe.
	 */
	set(uncore, VLV_COUNTER_CONTROL,
	    _MASKED_BIT_ENABLE(VLV_COUNT_RANGE_HIGH));
	upper = intel_uncore_read_fw(uncore, reg);
	do {
		tmp = upper;

		set(uncore, VLV_COUNTER_CONTROL,
		    _MASKED_BIT_DISABLE(VLV_COUNT_RANGE_HIGH));
		lower = intel_uncore_read_fw(uncore, reg);

		set(uncore, VLV_COUNTER_CONTROL,
		    _MASKED_BIT_ENABLE(VLV_COUNT_RANGE_HIGH));
		upper = intel_uncore_read_fw(uncore, reg);
	} while (upper != tmp && --loop);

	/*
	 * Everywhere else we always use VLV_COUNTER_CONTROL with the
	 * VLV_COUNT_RANGE_HIGH bit set - so it is safe to leave it set
	 * now.
	 */

	return lower | (u64)upper << 8;
}

u64 intel_rc6_residency_ns(struct intel_rc6 *rc6, const i915_reg_t reg)
{
	struct drm_i915_private *i915 = rc6_to_i915(rc6);
	struct intel_uncore *uncore = rc6_to_uncore(rc6);
	u64 time_hw, prev_hw, overflow_hw;
	unsigned int fw_domains;
	unsigned long flags;
	unsigned int i;
	u32 mul, div;

	if (!rc6->supported)
		return 0;

	/*
	 * Store previous hw counter values for counter wrap-around handling.
	 *
	 * There are only four interesting registers and they live next to each
	 * other so we can use the relative address, compared to the smallest
	 * one as the index into driver storage.
	 */
	i = (i915_mmio_reg_offset(reg) -
	     i915_mmio_reg_offset(GEN6_GT_GFX_RC6_LOCKED)) / sizeof(u32);
	if (WARN_ON_ONCE(i >= ARRAY_SIZE(rc6->cur_residency)))
		return 0;

	fw_domains = intel_uncore_forcewake_for_reg(uncore, reg, FW_REG_READ);

	spin_lock_irqsave(&uncore->lock, flags);
	intel_uncore_forcewake_get__locked(uncore, fw_domains);

	/* On VLV and CHV, residency time is in CZ units rather than 1.28us */
	if (IS_VALLEYVIEW(i915) || IS_CHERRYVIEW(i915)) {
		mul = 1000000;
		div = i915->czclk_freq;
		overflow_hw = BIT_ULL(40);
		time_hw = vlv_residency_raw(uncore, reg);
	} else {
		/* 833.33ns units on Gen9LP, 1.28us elsewhere. */
		if (IS_GEN9_LP(i915)) {
			mul = 10000;
			div = 12;
		} else {
			mul = 1280;
			div = 1;
		}

		overflow_hw = BIT_ULL(32);
		time_hw = intel_uncore_read_fw(uncore, reg);
	}

	/*
	 * Counter wrap handling.
	 *
	 * But relying on a sufficient frequency of queries otherwise counters
	 * can still wrap.
	 */
	prev_hw = rc6->prev_hw_residency[i];
	rc6->prev_hw_residency[i] = time_hw;

	/* RC6 delta from last sample. */
	if (time_hw >= prev_hw)
		time_hw -= prev_hw;
	else
		time_hw += overflow_hw - prev_hw;

	/* Add delta to RC6 extended raw driver copy. */
	time_hw += rc6->cur_residency[i];
	rc6->cur_residency[i] = time_hw;

	intel_uncore_forcewake_put__locked(uncore, fw_domains);
	spin_unlock_irqrestore(&uncore->lock, flags);

	return mul_u64_u32_div(time_hw, mul, div);
}

u64 intel_rc6_residency_us(struct intel_rc6 *rc6, i915_reg_t reg)
{
	return DIV_ROUND_UP_ULL(intel_rc6_residency_ns(rc6, reg), 1000);
}

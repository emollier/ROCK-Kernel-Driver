/*
 * Copyright 2012-16 Advanced Micro Devices, Inc.
 * Copyright 2019 Raptor Engineering, LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: AMD
 *
 */

#ifndef _OS_TYPES_H_
#define _OS_TYPES_H_

#include <linux/kgdb.h>
#include <linux/kref.h>
#include <linux/types.h>
#include <linux/slab.h>

#include <asm/byteorder.h>

#include <drm/drm_print.h>

#include "cgs_common.h"

#if defined(__BIG_ENDIAN) && !defined(BIGENDIAN_CPU)
#define BIGENDIAN_CPU
#elif defined(__LITTLE_ENDIAN) && !defined(LITTLEENDIAN_CPU)
#define LITTLEENDIAN_CPU
#endif

#undef FRAME_SIZE

#define dm_output_to_console(fmt, ...) DRM_DEBUG_KMS(fmt, ##__VA_ARGS__)

#define dm_error(fmt, ...) DRM_ERROR(fmt, ##__VA_ARGS__)

#if defined(CONFIG_DRM_AMD_DC_DCN1_0)
#if defined(CONFIG_X86)
#if defined(HAVE_ASM_FPU_API_H)
#include <asm/fpu/api.h>
#else
#include <asm/i387.h>
#endif
#define DC_FP_START() kernel_fpu_begin()
#define DC_FP_END() kernel_fpu_end()
#elif defined(CONFIG_PPC64)
#include <asm/switch_to.h>
#include <asm/cputable.h>
#define DC_FP_START() { \
	if (cpu_has_feature(CPU_FTR_VSX_COMP)) { \
		preempt_disable(); \
		enable_kernel_vsx(); \
	} else if (cpu_has_feature(CPU_FTR_ALTIVEC_COMP)) { \
		preempt_disable(); \
		enable_kernel_altivec(); \
	} else if (!cpu_has_feature(CPU_FTR_FPU_UNAVAILABLE)) { \
		preempt_disable(); \
		enable_kernel_fp(); \
	} \
}
#define DC_FP_END() { \
	if (cpu_has_feature(CPU_FTR_VSX_COMP)) { \
		disable_kernel_vsx(); \
		preempt_enable(); \
	} else if (cpu_has_feature(CPU_FTR_ALTIVEC_COMP)) { \
		disable_kernel_altivec(); \
		preempt_enable(); \
	} else if (!cpu_has_feature(CPU_FTR_FPU_UNAVAILABLE)) { \
		disable_kernel_fp(); \
		preempt_enable(); \
	} \
}
#endif
#endif

/*
 *
 * general debug capabilities
 *
 */
#if defined(CONFIG_HAVE_KGDB) || defined(CONFIG_KGDB)
#define ASSERT_CRITICAL(expr) do {	\
	if (WARN_ON(!(expr))) { \
		kgdb_breakpoint(); \
	} \
} while (0)
#else
#define ASSERT_CRITICAL(expr) do {	\
	if (WARN_ON(!(expr))) { \
		; \
	} \
} while (0)
#endif

#if defined(CONFIG_DEBUG_KERNEL_DC)
#define ASSERT(expr) ASSERT_CRITICAL(expr)

#else
#define ASSERT(expr) WARN_ON(!(expr))
#endif

#define BREAK_TO_DEBUGGER() ASSERT(0)

#define DC_ERR(...)  do { \
	dm_error(__VA_ARGS__); \
	BREAK_TO_DEBUGGER(); \
} while (0)

#endif /* _OS_TYPES_H_ */

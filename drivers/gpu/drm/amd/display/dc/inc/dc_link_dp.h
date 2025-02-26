/*
 * Copyright 2015 Advanced Micro Devices, Inc.
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

#ifndef __DC_LINK_DP_H__
#define __DC_LINK_DP_H__

#define LINK_TRAINING_ATTEMPTS 4
#define LINK_TRAINING_RETRY_DELAY 50 /* ms */
#define LINK_AUX_DEFAULT_EXTENDED_TIMEOUT_PERIOD 3200 /*us*/
#define LINK_AUX_DEFAULT_TIMEOUT_PERIOD 552 /*us*/

struct dc_link;
struct dc_stream_state;
struct dc_link_settings;

bool dp_verify_link_cap(
	struct dc_link *link,
	struct dc_link_settings *known_limit_link_setting,
	int *fail_count);

bool dp_verify_link_cap_with_retries(
	struct dc_link *link,
	struct dc_link_settings *known_limit_link_setting,
	int attempts);

bool dp_verify_mst_link_cap(
	struct dc_link *link);

bool dp_validate_mode_timing(
	struct dc_link *link,
	const struct dc_crtc_timing *timing);

void decide_link_settings(
	struct dc_stream_state *stream,
	struct dc_link_settings *link_setting);

bool perform_link_training_with_retries(
	const struct dc_link_settings *link_setting,
	bool skip_video_pattern,
	int attempts,
	struct pipe_ctx *pipe_ctx,
	enum signal_type signal);

bool is_mst_supported(struct dc_link *link);

bool detect_dp_sink_caps(struct dc_link *link);

void detect_edp_sink_caps(struct dc_link *link);

bool is_dp_active_dongle(const struct dc_link *link);

void dp_enable_mst_on_sink(struct dc_link *link, bool enable);

enum dp_panel_mode dp_get_panel_mode(struct dc_link *link);
void dp_set_panel_mode(struct dc_link *link, enum dp_panel_mode panel_mode);

bool dp_overwrite_extended_receiver_cap(struct dc_link *link);

void dpcd_set_source_specific_data(struct dc_link *link);

#ifdef CONFIG_DRM_AMD_DC_DSC_SUPPORT
void dp_set_fec_ready(struct dc_link *link, bool ready);
void dp_set_fec_enable(struct dc_link *link, bool enable);
bool dp_set_dsc_enable(struct pipe_ctx *pipe_ctx, bool enable);
bool dp_set_dsc_pps_sdp(struct pipe_ctx *pipe_ctx, bool enable);
void dp_set_dsc_on_stream(struct pipe_ctx *pipe_ctx, bool enable);
bool dp_update_dsc_config(struct pipe_ctx *pipe_ctx);
#endif

#endif /* __DC_LINK_DP_H__ */

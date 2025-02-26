/*
 * Copyright 2012-15 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
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

#ifndef __DC_STREAM_ENCODER_DCN10_H__
#define __DC_STREAM_ENCODER_DCN10_H__

#include "stream_encoder.h"

#define DCN10STRENC_FROM_STRENC(stream_encoder)\
	container_of(stream_encoder, struct dcn10_stream_encoder, base)

#define SE_COMMON_DCN_REG_LIST(id) \
	SRI(AFMT_CNTL, DIG, id), \
	SRI(AFMT_GENERIC_0, DIG, id), \
	SRI(AFMT_GENERIC_1, DIG, id), \
	SRI(AFMT_GENERIC_2, DIG, id), \
	SRI(AFMT_GENERIC_3, DIG, id), \
	SRI(AFMT_GENERIC_4, DIG, id), \
	SRI(AFMT_GENERIC_5, DIG, id), \
	SRI(AFMT_GENERIC_6, DIG, id), \
	SRI(AFMT_GENERIC_7, DIG, id), \
	SRI(AFMT_GENERIC_HDR, DIG, id), \
	SRI(AFMT_INFOFRAME_CONTROL0, DIG, id), \
	SRI(AFMT_VBI_PACKET_CONTROL, DIG, id), \
	SRI(AFMT_VBI_PACKET_CONTROL1, DIG, id), \
	SRI(AFMT_AUDIO_PACKET_CONTROL, DIG, id), \
	SRI(AFMT_AUDIO_PACKET_CONTROL2, DIG, id), \
	SRI(AFMT_AUDIO_SRC_CONTROL, DIG, id), \
	SRI(AFMT_60958_0, DIG, id), \
	SRI(AFMT_60958_1, DIG, id), \
	SRI(AFMT_60958_2, DIG, id), \
	SRI(DIG_FE_CNTL, DIG, id), \
	SRI(HDMI_CONTROL, DIG, id), \
	SRI(HDMI_DB_CONTROL, DIG, id), \
	SRI(HDMI_GC, DIG, id), \
	SRI(HDMI_GENERIC_PACKET_CONTROL0, DIG, id), \
	SRI(HDMI_GENERIC_PACKET_CONTROL1, DIG, id), \
	SRI(HDMI_GENERIC_PACKET_CONTROL2, DIG, id), \
	SRI(HDMI_GENERIC_PACKET_CONTROL3, DIG, id), \
	SRI(HDMI_INFOFRAME_CONTROL0, DIG, id), \
	SRI(HDMI_INFOFRAME_CONTROL1, DIG, id), \
	SRI(HDMI_VBI_PACKET_CONTROL, DIG, id), \
	SRI(HDMI_AUDIO_PACKET_CONTROL, DIG, id),\
	SRI(HDMI_ACR_PACKET_CONTROL, DIG, id),\
	SRI(HDMI_ACR_32_0, DIG, id),\
	SRI(HDMI_ACR_32_1, DIG, id),\
	SRI(HDMI_ACR_44_0, DIG, id),\
	SRI(HDMI_ACR_44_1, DIG, id),\
	SRI(HDMI_ACR_48_0, DIG, id),\
	SRI(HDMI_ACR_48_1, DIG, id),\
	SRI(DP_DB_CNTL, DP, id), \
	SRI(DP_MSA_MISC, DP, id), \
	SRI(DP_MSA_COLORIMETRY, DP, id), \
	SRI(DP_MSA_TIMING_PARAM1, DP, id), \
	SRI(DP_MSA_TIMING_PARAM2, DP, id), \
	SRI(DP_MSA_TIMING_PARAM3, DP, id), \
	SRI(DP_MSA_TIMING_PARAM4, DP, id), \
	SRI(DP_MSE_RATE_CNTL, DP, id), \
	SRI(DP_MSE_RATE_UPDATE, DP, id), \
	SRI(DP_PIXEL_FORMAT, DP, id), \
	SRI(DP_SEC_CNTL, DP, id), \
	SRI(DP_SEC_CNTL2, DP, id), \
	SRI(DP_SEC_CNTL6, DP, id), \
	SRI(DP_STEER_FIFO, DP, id), \
	SRI(DP_VID_M, DP, id), \
	SRI(DP_VID_N, DP, id), \
	SRI(DP_VID_STREAM_CNTL, DP, id), \
	SRI(DP_VID_TIMING, DP, id), \
	SRI(DP_SEC_AUD_N, DP, id), \
	SRI(DP_SEC_TIMESTAMP, DP, id), \
	SRI(DIG_CLOCK_PATTERN, DIG, id)

#define SE_DCN_REG_LIST(id)\
	SE_COMMON_DCN_REG_LIST(id)


struct dcn10_stream_enc_registers {
	uint32_t AFMT_CNTL;
	uint32_t AFMT_AVI_INFO0;
	uint32_t AFMT_AVI_INFO1;
	uint32_t AFMT_AVI_INFO2;
	uint32_t AFMT_AVI_INFO3;
	uint32_t AFMT_GENERIC_0;
	uint32_t AFMT_GENERIC_1;
	uint32_t AFMT_GENERIC_2;
	uint32_t AFMT_GENERIC_3;
	uint32_t AFMT_GENERIC_4;
	uint32_t AFMT_GENERIC_5;
	uint32_t AFMT_GENERIC_6;
	uint32_t AFMT_GENERIC_7;
	uint32_t AFMT_GENERIC_HDR;
	uint32_t AFMT_INFOFRAME_CONTROL0;
	uint32_t AFMT_VBI_PACKET_CONTROL;
	uint32_t AFMT_VBI_PACKET_CONTROL1;
	uint32_t AFMT_AUDIO_PACKET_CONTROL;
	uint32_t AFMT_AUDIO_PACKET_CONTROL2;
	uint32_t AFMT_AUDIO_SRC_CONTROL;
	uint32_t AFMT_60958_0;
	uint32_t AFMT_60958_1;
	uint32_t AFMT_60958_2;
	uint32_t DIG_FE_CNTL;
	uint32_t DIG_FE_CNTL2;
	uint32_t DP_MSE_RATE_CNTL;
	uint32_t DP_MSE_RATE_UPDATE;
	uint32_t DP_PIXEL_FORMAT;
	uint32_t DP_SEC_CNTL;
	uint32_t DP_SEC_CNTL2;
	uint32_t DP_SEC_CNTL6;
	uint32_t DP_STEER_FIFO;
	uint32_t DP_VID_M;
	uint32_t DP_VID_N;
	uint32_t DP_VID_STREAM_CNTL;
	uint32_t DP_VID_TIMING;
	uint32_t DP_SEC_AUD_N;
	uint32_t DP_SEC_TIMESTAMP;
	uint32_t HDMI_CONTROL;
	uint32_t HDMI_GC;
	uint32_t HDMI_GENERIC_PACKET_CONTROL0;
	uint32_t HDMI_GENERIC_PACKET_CONTROL1;
	uint32_t HDMI_GENERIC_PACKET_CONTROL2;
	uint32_t HDMI_GENERIC_PACKET_CONTROL3;
	uint32_t HDMI_GENERIC_PACKET_CONTROL4;
	uint32_t HDMI_GENERIC_PACKET_CONTROL5;
	uint32_t HDMI_INFOFRAME_CONTROL0;
	uint32_t HDMI_INFOFRAME_CONTROL1;
	uint32_t HDMI_VBI_PACKET_CONTROL;
	uint32_t HDMI_AUDIO_PACKET_CONTROL;
	uint32_t HDMI_ACR_PACKET_CONTROL;
	uint32_t HDMI_ACR_32_0;
	uint32_t HDMI_ACR_32_1;
	uint32_t HDMI_ACR_44_0;
	uint32_t HDMI_ACR_44_1;
	uint32_t HDMI_ACR_48_0;
	uint32_t HDMI_ACR_48_1;
	uint32_t DP_DB_CNTL;
	uint32_t DP_MSA_MISC;
	uint32_t DP_MSA_VBID_MISC;
	uint32_t DP_MSA_COLORIMETRY;
	uint32_t DP_MSA_TIMING_PARAM1;
	uint32_t DP_MSA_TIMING_PARAM2;
	uint32_t DP_MSA_TIMING_PARAM3;
	uint32_t DP_MSA_TIMING_PARAM4;
	uint32_t HDMI_DB_CONTROL;
#if defined(CONFIG_DRM_AMD_DC_DCN2_0)
	uint32_t DP_DSC_CNTL;
	uint32_t DP_DSC_BYTES_PER_PIXEL;
	uint32_t DME_CONTROL;
	uint32_t DP_SEC_METADATA_TRANSMISSION;
	uint32_t HDMI_METADATA_PACKET_CONTROL;
	uint32_t DP_SEC_FRAMING4;
#endif
	uint32_t DIG_CLOCK_PATTERN;
};


#define SE_SF(reg_name, field_name, post_fix)\
	.field_name = reg_name ## __ ## field_name ## post_fix

#define SE_COMMON_MASK_SH_LIST_SOC_BASE(mask_sh)\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL, AFMT_GENERIC_INDEX, mask_sh),\
	SE_SF(DIG0_AFMT_GENERIC_HDR, AFMT_GENERIC_HB0, mask_sh),\
	SE_SF(DIG0_AFMT_GENERIC_HDR, AFMT_GENERIC_HB1, mask_sh),\
	SE_SF(DIG0_AFMT_GENERIC_HDR, AFMT_GENERIC_HB2, mask_sh),\
	SE_SF(DIG0_AFMT_GENERIC_HDR, AFMT_GENERIC_HB3, mask_sh),\
	SE_SF(DP0_DP_PIXEL_FORMAT, DP_PIXEL_ENCODING, mask_sh),\
	SE_SF(DP0_DP_PIXEL_FORMAT, DP_COMPONENT_DEPTH, mask_sh),\
	SE_SF(DIG0_HDMI_CONTROL, HDMI_PACKET_GEN_VERSION, mask_sh),\
	SE_SF(DIG0_HDMI_CONTROL, HDMI_KEEPOUT_MODE, mask_sh),\
	SE_SF(DIG0_HDMI_CONTROL, HDMI_DEEP_COLOR_ENABLE, mask_sh),\
	SE_SF(DIG0_HDMI_CONTROL, HDMI_DEEP_COLOR_DEPTH, mask_sh),\
	SE_SF(DIG0_HDMI_CONTROL, HDMI_DATA_SCRAMBLE_EN, mask_sh),\
	SE_SF(DIG0_HDMI_CONTROL, HDMI_NO_EXTRA_NULL_PACKET_FILLED, mask_sh),\
	SE_SF(DIG0_HDMI_VBI_PACKET_CONTROL, HDMI_GC_CONT, mask_sh),\
	SE_SF(DIG0_HDMI_VBI_PACKET_CONTROL, HDMI_GC_SEND, mask_sh),\
	SE_SF(DIG0_HDMI_VBI_PACKET_CONTROL, HDMI_NULL_SEND, mask_sh),\
	SE_SF(DIG0_HDMI_INFOFRAME_CONTROL0, HDMI_AUDIO_INFO_SEND, mask_sh),\
	SE_SF(DIG0_AFMT_INFOFRAME_CONTROL0, AFMT_AUDIO_INFO_UPDATE, mask_sh),\
	SE_SF(DIG0_HDMI_INFOFRAME_CONTROL1, HDMI_AUDIO_INFO_LINE, mask_sh),\
	SE_SF(DIG0_HDMI_GC, HDMI_GC_AVMUTE, mask_sh),\
	SE_SF(DP0_DP_MSE_RATE_CNTL, DP_MSE_RATE_X, mask_sh),\
	SE_SF(DP0_DP_MSE_RATE_CNTL, DP_MSE_RATE_Y, mask_sh),\
	SE_SF(DP0_DP_MSE_RATE_UPDATE, DP_MSE_RATE_UPDATE_PENDING, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL, DP_SEC_GSP0_ENABLE, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL, DP_SEC_STREAM_ENABLE, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL, DP_SEC_GSP1_ENABLE, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL, DP_SEC_GSP2_ENABLE, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL, DP_SEC_GSP3_ENABLE, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL, DP_SEC_MPG_ENABLE, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL2, DP_SEC_GSP4_SEND, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL2, DP_SEC_GSP4_SEND_PENDING, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL4, DP_SEC_GSP4_LINE_NUM, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL2, DP_SEC_GSP4_SEND_ANY_LINE, mask_sh),\
	SE_SF(DP0_DP_VID_STREAM_CNTL, DP_VID_STREAM_DIS_DEFER, mask_sh),\
	SE_SF(DP0_DP_VID_STREAM_CNTL, DP_VID_STREAM_ENABLE, mask_sh),\
	SE_SF(DP0_DP_VID_STREAM_CNTL, DP_VID_STREAM_STATUS, mask_sh),\
	SE_SF(DP0_DP_STEER_FIFO, DP_STEER_FIFO_RESET, mask_sh),\
	SE_SF(DP0_DP_VID_TIMING, DP_VID_M_N_GEN_EN, mask_sh),\
	SE_SF(DP0_DP_VID_N, DP_VID_N, mask_sh),\
	SE_SF(DP0_DP_VID_M, DP_VID_M, mask_sh),\
	SE_SF(DIG0_DIG_FE_CNTL, DIG_START, mask_sh),\
	SE_SF(DIG0_AFMT_AUDIO_SRC_CONTROL, AFMT_AUDIO_SRC_SELECT, mask_sh),\
	SE_SF(DIG0_AFMT_AUDIO_PACKET_CONTROL2, AFMT_AUDIO_CHANNEL_ENABLE, mask_sh),\
	SE_SF(DIG0_HDMI_AUDIO_PACKET_CONTROL, HDMI_AUDIO_PACKETS_PER_LINE, mask_sh),\
	SE_SF(DIG0_HDMI_AUDIO_PACKET_CONTROL, HDMI_AUDIO_DELAY_EN, mask_sh),\
	SE_SF(DIG0_AFMT_AUDIO_PACKET_CONTROL, AFMT_60958_CS_UPDATE, mask_sh),\
	SE_SF(DIG0_AFMT_AUDIO_PACKET_CONTROL2, AFMT_AUDIO_LAYOUT_OVRD, mask_sh),\
	SE_SF(DIG0_AFMT_AUDIO_PACKET_CONTROL2, AFMT_60958_OSF_OVRD, mask_sh),\
	SE_SF(DIG0_HDMI_ACR_PACKET_CONTROL, HDMI_ACR_AUTO_SEND, mask_sh),\
	SE_SF(DIG0_HDMI_ACR_PACKET_CONTROL, HDMI_ACR_SOURCE, mask_sh),\
	SE_SF(DIG0_HDMI_ACR_PACKET_CONTROL, HDMI_ACR_AUDIO_PRIORITY, mask_sh),\
	SE_SF(DIG0_HDMI_ACR_32_0, HDMI_ACR_CTS_32, mask_sh),\
	SE_SF(DIG0_HDMI_ACR_32_1, HDMI_ACR_N_32, mask_sh),\
	SE_SF(DIG0_HDMI_ACR_44_0, HDMI_ACR_CTS_44, mask_sh),\
	SE_SF(DIG0_HDMI_ACR_44_1, HDMI_ACR_N_44, mask_sh),\
	SE_SF(DIG0_HDMI_ACR_48_0, HDMI_ACR_CTS_48, mask_sh),\
	SE_SF(DIG0_HDMI_ACR_48_1, HDMI_ACR_N_48, mask_sh),\
	SE_SF(DIG0_AFMT_60958_0, AFMT_60958_CS_CHANNEL_NUMBER_L, mask_sh),\
	SE_SF(DIG0_AFMT_60958_0, AFMT_60958_CS_CLOCK_ACCURACY, mask_sh),\
	SE_SF(DIG0_AFMT_60958_1, AFMT_60958_CS_CHANNEL_NUMBER_R, mask_sh),\
	SE_SF(DIG0_AFMT_60958_2, AFMT_60958_CS_CHANNEL_NUMBER_2, mask_sh),\
	SE_SF(DIG0_AFMT_60958_2, AFMT_60958_CS_CHANNEL_NUMBER_3, mask_sh),\
	SE_SF(DIG0_AFMT_60958_2, AFMT_60958_CS_CHANNEL_NUMBER_4, mask_sh),\
	SE_SF(DIG0_AFMT_60958_2, AFMT_60958_CS_CHANNEL_NUMBER_5, mask_sh),\
	SE_SF(DIG0_AFMT_60958_2, AFMT_60958_CS_CHANNEL_NUMBER_6, mask_sh),\
	SE_SF(DIG0_AFMT_60958_2, AFMT_60958_CS_CHANNEL_NUMBER_7, mask_sh),\
	SE_SF(DP0_DP_SEC_AUD_N, DP_SEC_AUD_N, mask_sh),\
	SE_SF(DP0_DP_SEC_TIMESTAMP, DP_SEC_TIMESTAMP_MODE, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL, DP_SEC_ASP_ENABLE, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL, DP_SEC_ATP_ENABLE, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL, DP_SEC_AIP_ENABLE, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL, DP_SEC_ACM_ENABLE, mask_sh),\
	SE_SF(DIG0_AFMT_AUDIO_PACKET_CONTROL, AFMT_AUDIO_SAMPLE_SEND, mask_sh),\
	SE_SF(DIG0_AFMT_CNTL, AFMT_AUDIO_CLOCK_EN, mask_sh),\
	SE_SF(DIG0_HDMI_CONTROL, HDMI_CLOCK_CHANNEL_RATE, mask_sh),\
	SE_SF(DIG0_DIG_FE_CNTL, TMDS_PIXEL_ENCODING, mask_sh),\
	SE_SF(DIG0_DIG_FE_CNTL, TMDS_COLOR_FORMAT, mask_sh),\
	SE_SF(DIG0_DIG_FE_CNTL, DIG_STEREOSYNC_SELECT, mask_sh),\
	SE_SF(DIG0_DIG_FE_CNTL, DIG_STEREOSYNC_GATE_EN, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL, AFMT_GENERIC_LOCK_STATUS, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL, AFMT_GENERIC_CONFLICT, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL, AFMT_GENERIC_CONFLICT_CLR, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL1, AFMT_GENERIC0_FRAME_UPDATE_PENDING, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL1, AFMT_GENERIC1_FRAME_UPDATE_PENDING, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL1, AFMT_GENERIC2_FRAME_UPDATE_PENDING, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL1, AFMT_GENERIC3_FRAME_UPDATE_PENDING, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL1, AFMT_GENERIC4_FRAME_UPDATE_PENDING, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL1, AFMT_GENERIC4_IMMEDIATE_UPDATE_PENDING, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL1, AFMT_GENERIC5_FRAME_UPDATE_PENDING, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL1, AFMT_GENERIC6_FRAME_UPDATE_PENDING, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL1, AFMT_GENERIC7_FRAME_UPDATE_PENDING, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL1, AFMT_GENERIC0_FRAME_UPDATE, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL1, AFMT_GENERIC1_FRAME_UPDATE, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL1, AFMT_GENERIC2_FRAME_UPDATE, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL1, AFMT_GENERIC3_FRAME_UPDATE, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL1, AFMT_GENERIC4_FRAME_UPDATE, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL1, AFMT_GENERIC4_IMMEDIATE_UPDATE, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL1, AFMT_GENERIC5_FRAME_UPDATE, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL1, AFMT_GENERIC6_FRAME_UPDATE, mask_sh),\
	SE_SF(DIG0_AFMT_VBI_PACKET_CONTROL1, AFMT_GENERIC7_FRAME_UPDATE, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL, DP_SEC_GSP4_ENABLE, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL, DP_SEC_GSP5_ENABLE, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL, DP_SEC_GSP6_ENABLE, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL, DP_SEC_GSP7_ENABLE, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL2, DP_SEC_GSP7_PPS, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL2, DP_SEC_GSP7_SEND, mask_sh),\
	SE_SF(DP0_DP_SEC_CNTL6, DP_SEC_GSP7_LINE_NUM, mask_sh),\
	SE_SF(DP0_DP_DB_CNTL, DP_DB_DISABLE, mask_sh),\
	SE_SF(DP0_DP_MSA_COLORIMETRY, DP_MSA_MISC0, mask_sh),\
	SE_SF(DP0_DP_MSA_TIMING_PARAM1, DP_MSA_HTOTAL, mask_sh),\
	SE_SF(DP0_DP_MSA_TIMING_PARAM1, DP_MSA_VTOTAL, mask_sh),\
	SE_SF(DP0_DP_MSA_TIMING_PARAM2, DP_MSA_HSTART, mask_sh),\
	SE_SF(DP0_DP_MSA_TIMING_PARAM2, DP_MSA_VSTART, mask_sh),\
	SE_SF(DP0_DP_MSA_TIMING_PARAM3, DP_MSA_HSYNCWIDTH, mask_sh),\
	SE_SF(DP0_DP_MSA_TIMING_PARAM3, DP_MSA_HSYNCPOLARITY, mask_sh),\
	SE_SF(DP0_DP_MSA_TIMING_PARAM3, DP_MSA_VSYNCWIDTH, mask_sh),\
	SE_SF(DP0_DP_MSA_TIMING_PARAM3, DP_MSA_VSYNCPOLARITY, mask_sh),\
	SE_SF(DP0_DP_MSA_TIMING_PARAM4, DP_MSA_HWIDTH, mask_sh),\
	SE_SF(DP0_DP_MSA_TIMING_PARAM4, DP_MSA_VHEIGHT, mask_sh),\
	SE_SF(DIG0_HDMI_DB_CONTROL, HDMI_DB_DISABLE, mask_sh),\
	SE_SF(DP0_DP_VID_TIMING, DP_VID_N_MUL, mask_sh),\
	SE_SF(DIG0_DIG_FE_CNTL, DIG_SOURCE_SELECT, mask_sh),\
	SE_SF(DIG0_DIG_CLOCK_PATTERN, DIG_CLOCK_PATTERN, mask_sh)

#define SE_COMMON_MASK_SH_LIST_SOC(mask_sh)\
	SE_COMMON_MASK_SH_LIST_SOC_BASE(mask_sh)

#define SE_COMMON_MASK_SH_LIST_DCN10(mask_sh)\
	SE_COMMON_MASK_SH_LIST_SOC(mask_sh),\
	SE_SF(DIG0_HDMI_GENERIC_PACKET_CONTROL0, HDMI_GENERIC0_CONT, mask_sh),\
	SE_SF(DIG0_HDMI_GENERIC_PACKET_CONTROL0, HDMI_GENERIC0_SEND, mask_sh),\
	SE_SF(DIG0_HDMI_GENERIC_PACKET_CONTROL0, HDMI_GENERIC0_LINE, mask_sh),\
	SE_SF(DIG0_HDMI_GENERIC_PACKET_CONTROL0, HDMI_GENERIC1_CONT, mask_sh),\
	SE_SF(DIG0_HDMI_GENERIC_PACKET_CONTROL0, HDMI_GENERIC1_SEND, mask_sh),\
	SE_SF(DIG0_HDMI_GENERIC_PACKET_CONTROL0, HDMI_GENERIC1_LINE, mask_sh)


#define SE_REG_FIELD_LIST_DCN1_0(type) \
	type AFMT_GENERIC_INDEX;\
	type AFMT_GENERIC_HB0;\
	type AFMT_GENERIC_HB1;\
	type AFMT_GENERIC_HB2;\
	type AFMT_GENERIC_HB3;\
	type AFMT_GENERIC_LOCK_STATUS;\
	type AFMT_GENERIC_CONFLICT;\
	type AFMT_GENERIC_CONFLICT_CLR;\
	type AFMT_GENERIC0_FRAME_UPDATE_PENDING;\
	type AFMT_GENERIC1_FRAME_UPDATE_PENDING;\
	type AFMT_GENERIC2_FRAME_UPDATE_PENDING;\
	type AFMT_GENERIC3_FRAME_UPDATE_PENDING;\
	type AFMT_GENERIC4_FRAME_UPDATE_PENDING;\
	type AFMT_GENERIC4_IMMEDIATE_UPDATE_PENDING;\
	type AFMT_GENERIC5_FRAME_UPDATE_PENDING;\
	type AFMT_GENERIC6_FRAME_UPDATE_PENDING;\
	type AFMT_GENERIC7_FRAME_UPDATE_PENDING;\
	type AFMT_GENERIC0_FRAME_UPDATE;\
	type AFMT_GENERIC1_FRAME_UPDATE;\
	type AFMT_GENERIC2_FRAME_UPDATE;\
	type AFMT_GENERIC3_FRAME_UPDATE;\
	type AFMT_GENERIC4_FRAME_UPDATE;\
	type AFMT_GENERIC4_IMMEDIATE_UPDATE;\
	type AFMT_GENERIC5_FRAME_UPDATE;\
	type AFMT_GENERIC6_FRAME_UPDATE;\
	type AFMT_GENERIC7_FRAME_UPDATE;\
	type HDMI_GENERIC0_CONT;\
	type HDMI_GENERIC0_SEND;\
	type HDMI_GENERIC0_LINE;\
	type HDMI_GENERIC1_CONT;\
	type HDMI_GENERIC1_SEND;\
	type HDMI_GENERIC1_LINE;\
	type HDMI_GENERIC2_CONT;\
	type HDMI_GENERIC2_SEND;\
	type HDMI_GENERIC2_LINE;\
	type HDMI_GENERIC3_CONT;\
	type HDMI_GENERIC3_SEND;\
	type HDMI_GENERIC3_LINE;\
	type HDMI_GENERIC4_CONT;\
	type HDMI_GENERIC4_SEND;\
	type HDMI_GENERIC4_LINE;\
	type HDMI_GENERIC5_CONT;\
	type HDMI_GENERIC5_SEND;\
	type HDMI_GENERIC5_LINE;\
	type HDMI_GENERIC6_CONT;\
	type HDMI_GENERIC6_SEND;\
	type HDMI_GENERIC6_LINE;\
	type HDMI_GENERIC7_CONT;\
	type HDMI_GENERIC7_SEND;\
	type HDMI_GENERIC7_LINE;\
	type DP_PIXEL_ENCODING;\
	type DP_COMPONENT_DEPTH;\
	type HDMI_PACKET_GEN_VERSION;\
	type HDMI_KEEPOUT_MODE;\
	type HDMI_DEEP_COLOR_ENABLE;\
	type HDMI_CLOCK_CHANNEL_RATE;\
	type HDMI_DEEP_COLOR_DEPTH;\
	type HDMI_GC_CONT;\
	type HDMI_GC_SEND;\
	type HDMI_NULL_SEND;\
	type HDMI_DATA_SCRAMBLE_EN;\
	type HDMI_NO_EXTRA_NULL_PACKET_FILLED;\
	type HDMI_AUDIO_INFO_SEND;\
	type AFMT_AUDIO_INFO_UPDATE;\
	type HDMI_AUDIO_INFO_LINE;\
	type HDMI_GC_AVMUTE;\
	type DP_MSE_RATE_X;\
	type DP_MSE_RATE_Y;\
	type DP_MSE_RATE_UPDATE_PENDING;\
	type DP_SEC_GSP0_ENABLE;\
	type DP_SEC_STREAM_ENABLE;\
	type DP_SEC_GSP1_ENABLE;\
	type DP_SEC_GSP2_ENABLE;\
	type DP_SEC_GSP3_ENABLE;\
	type DP_SEC_GSP4_ENABLE;\
	type DP_SEC_GSP5_ENABLE;\
	type DP_SEC_GSP6_ENABLE;\
	type DP_SEC_GSP7_ENABLE;\
	type DP_SEC_GSP7_PPS;\
	type DP_SEC_GSP7_SEND;\
	type DP_SEC_GSP4_SEND;\
	type DP_SEC_GSP4_SEND_PENDING;\
	type DP_SEC_GSP4_LINE_NUM;\
	type DP_SEC_GSP4_SEND_ANY_LINE;\
	type DP_SEC_MPG_ENABLE;\
	type DP_VID_STREAM_DIS_DEFER;\
	type DP_VID_STREAM_ENABLE;\
	type DP_VID_STREAM_STATUS;\
	type DP_STEER_FIFO_RESET;\
	type DP_VID_M_N_GEN_EN;\
	type DP_VID_N;\
	type DP_VID_M;\
	type DIG_START;\
	type AFMT_AUDIO_SRC_SELECT;\
	type AFMT_AUDIO_CHANNEL_ENABLE;\
	type HDMI_AUDIO_PACKETS_PER_LINE;\
	type HDMI_AUDIO_DELAY_EN;\
	type AFMT_60958_CS_UPDATE;\
	type AFMT_AUDIO_LAYOUT_OVRD;\
	type AFMT_60958_OSF_OVRD;\
	type HDMI_ACR_AUTO_SEND;\
	type HDMI_ACR_SOURCE;\
	type HDMI_ACR_AUDIO_PRIORITY;\
	type HDMI_ACR_CTS_32;\
	type HDMI_ACR_N_32;\
	type HDMI_ACR_CTS_44;\
	type HDMI_ACR_N_44;\
	type HDMI_ACR_CTS_48;\
	type HDMI_ACR_N_48;\
	type AFMT_60958_CS_CHANNEL_NUMBER_L;\
	type AFMT_60958_CS_CLOCK_ACCURACY;\
	type AFMT_60958_CS_CHANNEL_NUMBER_R;\
	type AFMT_60958_CS_CHANNEL_NUMBER_2;\
	type AFMT_60958_CS_CHANNEL_NUMBER_3;\
	type AFMT_60958_CS_CHANNEL_NUMBER_4;\
	type AFMT_60958_CS_CHANNEL_NUMBER_5;\
	type AFMT_60958_CS_CHANNEL_NUMBER_6;\
	type AFMT_60958_CS_CHANNEL_NUMBER_7;\
	type DP_SEC_AUD_N;\
	type DP_SEC_TIMESTAMP_MODE;\
	type DP_SEC_ASP_ENABLE;\
	type DP_SEC_ATP_ENABLE;\
	type DP_SEC_AIP_ENABLE;\
	type DP_SEC_ACM_ENABLE;\
	type DP_SEC_GSP7_LINE_NUM;\
	type AFMT_AUDIO_SAMPLE_SEND;\
	type AFMT_AUDIO_CLOCK_EN;\
	type TMDS_PIXEL_ENCODING;\
	type TMDS_COLOR_FORMAT;\
	type DIG_STEREOSYNC_SELECT;\
	type DIG_STEREOSYNC_GATE_EN;\
	type DP_DB_DISABLE;\
	type DP_MSA_MISC0;\
	type DP_MSA_HTOTAL;\
	type DP_MSA_VTOTAL;\
	type DP_MSA_HSTART;\
	type DP_MSA_VSTART;\
	type DP_MSA_HSYNCWIDTH;\
	type DP_MSA_HSYNCPOLARITY;\
	type DP_MSA_VSYNCWIDTH;\
	type DP_MSA_VSYNCPOLARITY;\
	type DP_MSA_HWIDTH;\
	type DP_MSA_VHEIGHT;\
	type HDMI_DB_DISABLE;\
	type DP_VID_N_MUL;\
	type DP_VID_M_DOUBLE_VALUE_EN;\
	type DIG_SOURCE_SELECT;\
	type DIG_CLOCK_PATTERN

#if defined(CONFIG_DRM_AMD_DC_DCN2_0)
#define SE_REG_FIELD_LIST_DCN2_0(type) \
	type DP_DSC_MODE;\
	type DP_DSC_SLICE_WIDTH;\
	type DP_DSC_BYTES_PER_PIXEL;\
	type DP_VBID6_LINE_REFERENCE;\
	type DP_VBID6_LINE_NUM;\
	type METADATA_ENGINE_EN;\
	type METADATA_HUBP_REQUESTOR_ID;\
	type METADATA_STREAM_TYPE;\
	type DP_SEC_METADATA_PACKET_ENABLE;\
	type DP_SEC_METADATA_PACKET_LINE_REFERENCE;\
	type DP_SEC_METADATA_PACKET_LINE;\
	type HDMI_METADATA_PACKET_ENABLE;\
	type HDMI_METADATA_PACKET_LINE_REFERENCE;\
	type HDMI_METADATA_PACKET_LINE;\
	type DOLBY_VISION_EN;\
	type DP_PIXEL_COMBINE;\
	type DP_SST_SDP_SPLITTING
#endif

struct dcn10_stream_encoder_shift {
	SE_REG_FIELD_LIST_DCN1_0(uint8_t);
#if defined(CONFIG_DRM_AMD_DC_DCN2_0)
	SE_REG_FIELD_LIST_DCN2_0(uint8_t);
#endif
};

struct dcn10_stream_encoder_mask {
	SE_REG_FIELD_LIST_DCN1_0(uint32_t);
#if defined(CONFIG_DRM_AMD_DC_DCN2_0)
	SE_REG_FIELD_LIST_DCN2_0(uint32_t);
#endif
};

struct dcn10_stream_encoder {
	struct stream_encoder base;
	const struct dcn10_stream_enc_registers *regs;
	const struct dcn10_stream_encoder_shift *se_shift;
	const struct dcn10_stream_encoder_mask *se_mask;
};

void dcn10_stream_encoder_construct(
	struct dcn10_stream_encoder *enc1,
	struct dc_context *ctx,
	struct dc_bios *bp,
	enum engine_id eng_id,
	const struct dcn10_stream_enc_registers *regs,
	const struct dcn10_stream_encoder_shift *se_shift,
	const struct dcn10_stream_encoder_mask *se_mask);

void enc1_update_generic_info_packet(
	struct dcn10_stream_encoder *enc1,
	uint32_t packet_index,
	const struct dc_info_packet *info_packet);

void enc1_stream_encoder_dp_set_stream_attribute(
	struct stream_encoder *enc,
	struct dc_crtc_timing *crtc_timing,
	enum dc_color_space output_color_space,
	bool use_vsc_sdp_for_colorimetry,
	uint32_t enable_sdp_splitting);

void enc1_stream_encoder_hdmi_set_stream_attribute(
	struct stream_encoder *enc,
	struct dc_crtc_timing *crtc_timing,
	int actual_pix_clk_khz,
	bool enable_audio);

void enc1_stream_encoder_dvi_set_stream_attribute(
	struct stream_encoder *enc,
	struct dc_crtc_timing *crtc_timing,
	bool is_dual_link);

void enc1_stream_encoder_set_mst_bandwidth(
	struct stream_encoder *enc,
	struct fixed31_32 avg_time_slots_per_mtp);

void enc1_stream_encoder_update_dp_info_packets(
	struct stream_encoder *enc,
	const struct encoder_info_frame *info_frame);

void enc1_stream_encoder_send_immediate_sdp_message(
	struct stream_encoder *enc,
	const uint8_t *custom_sdp_message,
				unsigned int sdp_message_size);

void enc1_stream_encoder_stop_dp_info_packets(
	struct stream_encoder *enc);

void enc1_stream_encoder_dp_blank(
	struct stream_encoder *enc);

void enc1_stream_encoder_dp_unblank(
	struct stream_encoder *enc,
	const struct encoder_unblank_param *param);

void enc1_setup_stereo_sync(
	struct stream_encoder *enc,
	int tg_inst, bool enable);

void enc1_stream_encoder_set_avmute(
	struct stream_encoder *enc,
	bool enable);

void enc1_se_audio_mute_control(
	struct stream_encoder *enc,
	bool mute);

void enc1_se_dp_audio_setup(
	struct stream_encoder *enc,
	unsigned int az_inst,
	struct audio_info *info);

void enc1_se_dp_audio_enable(
	struct stream_encoder *enc);

void enc1_se_dp_audio_disable(
	struct stream_encoder *enc);

void enc1_se_hdmi_audio_setup(
	struct stream_encoder *enc,
	unsigned int az_inst,
	struct audio_info *info,
	struct audio_crtc_info *audio_crtc_info);

void enc1_se_hdmi_audio_disable(
	struct stream_encoder *enc);

void enc1_dig_connect_to_otg(
	struct stream_encoder *enc,
	int tg_inst);

unsigned int enc1_dig_source_otg(
	struct stream_encoder *enc);

void enc1_stream_encoder_set_stream_attribute_helper(
	struct dcn10_stream_encoder *enc1,
	struct dc_crtc_timing *crtc_timing);

void enc1_se_enable_audio_clock(
	struct stream_encoder *enc,
	bool enable);

void enc1_se_enable_dp_audio(
	struct stream_encoder *enc);

void get_audio_clock_info(
	enum dc_color_depth color_depth,
	uint32_t crtc_pixel_clock_100Hz,
	uint32_t actual_pixel_clock_100Hz,
	struct audio_clock_info *audio_clock_info);

void enc1_reset_hdmi_stream_attribute(
	struct stream_encoder *enc);

bool enc1_stream_encoder_dp_get_pixel_format(
	struct stream_encoder *enc,
	enum dc_pixel_encoding *encoding,
	enum dc_color_depth *depth);

#endif /* __DC_STREAM_ENCODER_DCN10_H__ */

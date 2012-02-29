/*
 * Copyright © 2011 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    Zhou Chang <chang.zhou@intel.com>
 *    Xiang, Haihao <haihao.xiang@intel.com>
 *
 */

#ifndef _GEN7_MFC_H_
#define _GEN7_MFC_H_

#include <drm.h>
#include <i915_drm.h>
#include <intel_bufmgr.h>

struct encode_state;

#define MAX_MFC_REFERENCE_SURFACES      16
#define NUM_MFC_DMV_BUFFERS             34

struct gen7_mfc_avc_surface_aux
{
    dri_bo *dmv_top;
    dri_bo *dmv_bottom;
};

struct gen7_mfc_context
{
    struct {
        unsigned int width;
        unsigned int height;
        unsigned int w_pitch;
        unsigned int h_pitch;
    } surface_state;

    //MFX_PIPE_BUF_ADDR_STATE
    struct {
        dri_bo *bo;                            	
    } post_deblocking_output;           //OUTPUT: reconstructed picture                           
    
    struct {  
        dri_bo *bo;							   	
    } pre_deblocking_output;            //OUTPUT: reconstructed picture with deblocked                           

    struct {
        dri_bo *bo;
    } uncompressed_picture_source;      //INPUT: original compressed image

    struct {
        dri_bo *bo;							  	
    } intra_row_store_scratch_buffer;   //INTERNAL:

    struct {
        dri_bo *bo;
    } macroblock_status_buffer;         //INTERNAL:

    struct {
        dri_bo *bo;								
    } deblocking_filter_row_store_scratch_buffer;       //INTERNAL:

    struct {                                    
        dri_bo *bo; 
    } reference_surfaces[MAX_MFC_REFERENCE_SURFACES];   //INTERNAL: refrence surfaces

    //MFX_IND_OBJ_BASE_ADDR_STATE
    struct{
        dri_bo *bo;
    } mfc_indirect_mv_object;           //INPUT: the blocks' mv info

    struct {
        dri_bo *bo;
        int offset;
        int end_offset;
    } mfc_indirect_pak_bse_object;      //OUTPUT: the compressed bitstream 

    //MFX_BSP_BUF_BASE_ADDR_STATE
    struct {
        dri_bo *bo;
    } bsd_mpc_row_store_scratch_buffer; //INTERNAL:
	
    //MFX_AVC_DIRECTMODE_STATE
    struct {
        dri_bo *bo;
    } direct_mv_buffers[NUM_MFC_DMV_BUFFERS];   //INTERNAL: 0-31 as input,32 and 33 as output

    //Bit rate tracking context
    struct {
        unsigned int QpPrimeY;
        unsigned int MaxQpNegModifier;
        unsigned int MaxQpPosModifier;
        unsigned char MaxSizeInWord;
        unsigned char TargetSizeInWord;
        unsigned char Correct[6];
        unsigned char GrowInit;
        unsigned char GrowResistance;
        unsigned char ShrinkInit;
        unsigned char ShrinkResistance; 

        unsigned int target_mb_size;
        unsigned int target_frame_size;
    } bit_rate_control_context[2];      //INTERNAL: 0 for intra frames, 1 for inter frames.

    // HRD control context
    struct {
        int i_cpb_cnt;
        int i_bit_rate_scale;
        int i_cpb_size_scale;
        int i_cbr_flag;
        int i_bit_rate_value;
        int i_cpb_size_value;
        int i_bit_rate_unscaled;
        int i_cpb_size_unscaled;

        int i_initial_cpb_removal_delay_length;
        int i_cpb_removal_delay_length;
        int i_dpb_output_delay_length;
        int i_time_offset_length;            

    }vui_hrd;

    struct {
        double cpb_initial_arrival_time;
        double cpb_final_arrival_time;
        double cpb_removal_time;

        double dpb_output_time;
    }hrd_frame_result;

};

#endif	/* _GEN7_MFC_BCS_H_ */

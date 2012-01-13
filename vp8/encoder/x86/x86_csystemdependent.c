/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */


#include "vpx_config.h"
#include "vpx_rtcd.h"
#include "vpx_ports/x86.h"
#include "vp8/encoder/variance.h"
#include "vp8/encoder/onyx_int.h"


#if HAVE_MMX
void vp8_short_fdct8x4_mmx(short *input, short *output, int pitch)
{
    vp8_short_fdct4x4_mmx(input,   output,    pitch);
    vp8_short_fdct4x4_mmx(input + 4, output + 16, pitch);
}

int vp8_fast_quantize_b_impl_mmx(short *coeff_ptr, short *zbin_ptr,
                                 short *qcoeff_ptr, short *dequant_ptr,
                                 short *scan_mask, short *round_ptr,
                                 short *quant_ptr, short *dqcoeff_ptr);
void vp8_fast_quantize_b_mmx(BLOCK *b, BLOCKD *d)
{
    short *scan_mask   = vp8_default_zig_zag_mask;//d->scan_order_mask_ptr;
    short *coeff_ptr   = b->coeff;
    short *zbin_ptr    = b->zbin;
    short *round_ptr   = b->round;
    short *quant_ptr   = b->quant_fast;
    short *qcoeff_ptr  = d->qcoeff;
    short *dqcoeff_ptr = d->dqcoeff;
    short *dequant_ptr = d->dequant;

    *d->eob = (char)vp8_fast_quantize_b_impl_mmx(
                                                 coeff_ptr,
                                                 zbin_ptr,
                                                 qcoeff_ptr,
                                                 dequant_ptr,
                                                 scan_mask,

                                                 round_ptr,
                                                 quant_ptr,
                                                 dqcoeff_ptr
                                                 );
}

int vp8_mbblock_error_mmx_impl(short *coeff_ptr, short *dcoef_ptr, int dc);
int vp8_mbblock_error_mmx(MACROBLOCK *mb, int dc)
{
    short *coeff_ptr =  mb->block[0].coeff;
    short *dcoef_ptr =  mb->e_mbd.block[0].dqcoeff;
    return vp8_mbblock_error_mmx_impl(coeff_ptr, dcoef_ptr, dc);
}

int vp8_mbuverror_mmx_impl(short *s_ptr, short *d_ptr);
int vp8_mbuverror_mmx(MACROBLOCK *mb)
{
    short *s_ptr = &mb->coeff[256];
    short *d_ptr = &mb->e_mbd.dqcoeff[256];
    return vp8_mbuverror_mmx_impl(s_ptr, d_ptr);
}

void vp8_subtract_b_mmx_impl(unsigned char *z,  int src_stride,
                             short *diff, unsigned char *predictor,
                             int pitch);
void vp8_subtract_b_mmx(BLOCK *be, BLOCKD *bd, int pitch)
{
    unsigned char *z = *(be->base_src) + be->src;
    unsigned int  src_stride = be->src_stride;
    short *diff = &be->src_diff[0];
    unsigned char *predictor = &bd->predictor[0];
    vp8_subtract_b_mmx_impl(z, src_stride, diff, predictor, pitch);
}

#endif

#if HAVE_SSE2
int vp8_mbblock_error_xmm_impl(short *coeff_ptr, short *dcoef_ptr, int dc);
int vp8_mbblock_error_xmm(MACROBLOCK *mb, int dc)
{
    short *coeff_ptr =  mb->block[0].coeff;
    short *dcoef_ptr =  mb->e_mbd.block[0].dqcoeff;
    return vp8_mbblock_error_xmm_impl(coeff_ptr, dcoef_ptr, dc);
}

int vp8_mbuverror_xmm_impl(short *s_ptr, short *d_ptr);
int vp8_mbuverror_xmm(MACROBLOCK *mb)
{
    short *s_ptr = &mb->coeff[256];
    short *d_ptr = &mb->e_mbd.dqcoeff[256];
    return vp8_mbuverror_xmm_impl(s_ptr, d_ptr);
}

void vp8_subtract_b_sse2_impl(unsigned char *z,  int src_stride,
                             short *diff, unsigned char *predictor,
                             int pitch);
void vp8_subtract_b_sse2(BLOCK *be, BLOCKD *bd, int pitch)
{
    unsigned char *z = *(be->base_src) + be->src;
    unsigned int  src_stride = be->src_stride;
    short *diff = &be->src_diff[0];
    unsigned char *predictor = &bd->predictor[0];
    vp8_subtract_b_sse2_impl(z, src_stride, diff, predictor, pitch);
}

#endif

void vp8_arch_x86_encoder_init(VP8_COMP *cpi)
{
}

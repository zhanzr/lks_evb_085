/*******************************************************************************
 *******************************************************************************/
#ifndef __DSP_PERIPH_H
#define __DSP_PERIPH_H

#include <inttypes.h>

#include "lks32mc08x_lib.h"

typedef struct {
  int16_t hCos;
  int16_t hSin;
} sin_cos_result;

extern sin_cos_result dsp_sin_cos_func(int16_t angle_q_1_15);

#endif

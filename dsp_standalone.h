/*******************************************************************************
 *******************************************************************************/
#ifndef __DSP_STANDALONE_H
#define __DSP_STANDALONE_H

#include <inttypes.h>

#include "lks32mc08x_lib.h"

#define DSP_CODE_MEM_BASE (DSP_BASE + 0x00)
#define DSP_DATA_MEM_BASE (DSP_BASE + 0x0800)
#define DSP_SC_BASE (DSP_BASE + 0x1000)

typedef struct str_arctan_result {
  uint16_t arctan;
  uint16_t mod;
} arctan_result;

typedef struct str_arctan_input {
  uint16_t x;
  uint16_t y;
} arctan_input;

arctan_result cpu_issue_arctan_mod(arctan_input input);

arctan_result dsp_issue_arctan_mod_poll(const uint32_t *code_ptr,
                                        const uint16_t code_u32_len,
                                        const uint32_t *data_ptr,
                                        const uint8_t data_u32_len);

#endif

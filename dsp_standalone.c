#include "dsp_standalone.h"

arctan_result cpu_issue_arctan_mod(arctan_input input) {
  DSP_SC &= (~BIT2);
  DSP_CORDIC_X = input.x;
  DSP_CORDIC_Y = input.y;

  arctan_result res;
  res.arctan = DSP_CORDIC_ARCTAN;
  res.mod = DSP_CORDIC_MOD;

  return res;
}

arctan_result dsp_issue_arctan_mod_poll(const uint32_t *code_ptr,
                                        const uint16_t code_u32_len,
                                        const uint32_t *data_ptr,
                                        const uint8_t data_u32_len) {
  volatile uint16_t arctan, mod;
  uint32_t i;

  // dsp data mem flush
  for (i = 0; i < 64; i++) {
    REG32(DSP_DATA_MEM_BASE + i * 4) = 0;
  }

  // dsp code mem flush
  for (i = 0; i < 512; i++) {
    REG32(DSP_CODE_MEM_BASE + i * 4) = 0;
  }

  // dsp data mem init
  for (i = 0; i < data_u32_len; i++) {
    REG32(DSP_DATA_MEM_BASE + i * 4) = data_ptr[i];
  }

  // dsp code mem init
  // code length 200 half word
  for (i = 0; i < code_u32_len; i++) {
    REG32(DSP_CODE_MEM_BASE + i * 4) = code_ptr[i];
  }

  i = 1;
  while (i--) {
    // [1] dsp_paused, write 0 to start dsp
    // [0] irq write 1 to clear
    DSP_SC = BIT0; // Start DSP
    // wait until irq set and dsp paused
    while ((BIT0 | BIT1) != (DSP_SC & (BIT0 | BIT1)))
      ;
  }

  arctan_result res;
  res.arctan = DSP_CORDIC_ARCTAN;
  res.mod = DSP_CORDIC_MOD;

  return res;
}

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

	// Pause DSP
	DSP_SC |= BIT1;
	// Reset DSP PC
	DSP_SC |= BIT2;
	// Clear the IRQ flag
	DSP_SC |= BIT0;

	// Start DSP
	DSP_SC = BIT0;
	
	// wait until irq set and dsp paused
	while ((BIT0) != (DSP_SC & (BIT0))) {
      __WFI();
	}
	
  arctan_result res;
  res.arctan = REG32(DSP_DATA_MEM_BASE + 5 * 4);
  res.mod = REG32(DSP_DATA_MEM_BASE + 6 * 4);

//	printf("%04X %04X\n", DSP_CORDIC_ARCTAN, DSP_CORDIC_MOD);
//	
//	for(uint32_t j=0; j<16; ++j) {
//		printf("%08X:\t%08X\n", DSP_DATA_MEM_BASE + j*4, REG32(DSP_DATA_MEM_BASE + j*4));
//	}
	
  return res;
}

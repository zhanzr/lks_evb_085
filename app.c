/*******************************************************************************
 *
 *******************************************************************************/
#include <inttypes.h>
#include <stdbool.h>

#include "hardware_config.h"
#include <lks32mc08x.h>

#include "dsp_periph.h"
#include "dsp_standalone.h"

#include <math.h>

/*******************************************************************************
 *******************************************************************************/
extern uint32_t SystemCoreClock;
extern volatile uint32_t g_ticks;

extern void Invers_GPIO(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

uint32_t SysTick_Config_Div8(uint32_t ticks) {
  if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk) {
    return (1UL); /* Reload value impossible */
  }

  SysTick->LOAD = (uint32_t)(ticks - 1UL); /* set reload register */
  NVIC_SetPriority(SysTick_IRQn,
                   (1UL << __NVIC_PRIO_BITS) -
                       1UL); /* set Priority for Systick Interrupt */
  SysTick->VAL = 0UL;        /* Load the SysTick Counter Value */
  SysTick->CTRL =
      SysTick_CTRL_TICKINT_Msk |
      SysTick_CTRL_ENABLE_Msk; /* Enable SysTick IRQ and SysTick Timer */
  return (0UL);                /* Function successful */
}

int stdout_putchar(int ch) {
  UART_SendData(UART0, (uint8_t)ch);
  SoftDelay(0x400);
  return ch;
}

const uint32_t test_code_arctan_simple[] = {
0x0000f01c,
0x0000792b,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x0000b145,
0x0000b186,
0x0000e000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,

};

const uint32_t test_data_arctan_simple[] = {
0x10001BB6,
0x00000000,
};

int main(void) {
  volatile uint32_t tmp_ticks;
  volatile uint32_t delta_ticks;

  volatile uint16_t arctan;
  volatile uint16_t mod;

  Hardware_init();

  /* System timer configuration */
  SysTick_Config_Div8((SystemCoreClock >> 3) / TICK_HZ);

  while (1) {
    arctan_result arctan_res;

    // DSP as a periph
    tmp_ticks = g_ticks;
    arctan_input input;
    input.x = 0x1000;
    input.y = 0x1BB6;
    arctan_res = cpu_issue_arctan_mod(input);
    delta_ticks = g_ticks - tmp_ticks;

    printf("DSP as a periph delta:%u\n", delta_ticks);
    printf("%04X, %04X\n", arctan_res.arctan, arctan_res.mod);

    printf("\n");
    printf("%s\n", __VERSION__);
    printf("\n");

    // DSP as standalone
    tmp_ticks = g_ticks;
    arctan_res = dsp_issue_arctan_mod_poll(
        test_code_arctan_simple, sizeof(test_code_arctan_simple) / sizeof(test_code_arctan_simple[0]),
        test_data_arctan_simple, sizeof(test_data_arctan_simple) / sizeof(test_data_arctan_simple[0]));

    delta_ticks = g_ticks - tmp_ticks;

    printf("DSP standalone delta:%u\n", delta_ticks);
    printf("%04X, %04X\n", arctan_res.arctan, arctan_res.mod);

    printf("\n");
    printf("%s\n", __VERSION__);
    printf("\n");

    tmp_ticks = g_ticks;
    while (300 > (g_ticks - tmp_ticks)) {
      __WFI();
    }
    Invers_GPIO(GPIO2, GPIO_Pin_8);

    tmp_ticks = g_ticks;
    while (300 > (g_ticks - tmp_ticks)) {
      __WFI();
    }
    Invers_GPIO(GPIO3, GPIO_Pin_9);

    while (true) {
      __NOP();
    }
  }
}

/* ------------------------------END OF FILE------------------------------------
 */

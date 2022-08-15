/*******************************************************************************
 *
 *******************************************************************************/
#include <inttypes.h>

#include "hardware_config.h"
#include <lks32mc08x.h>

/*******************************************************************************
 *******************************************************************************/
extern uint32_t SystemCoreClock;
extern uint32_t g_ticks;

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
  SoftDelay(0x280);
  return ch;
}

int main(void) {
  uint32_t tmp_ticks;

  Hardware_init();

  /* System timer configuration */
  SysTick_Config_Div8((SystemCoreClock >> 3) / TICK_HZ);

  while (1) {
    printf("hello %p %u %u\r\n", main, SystemCoreClock, g_ticks);
    tmp_ticks = g_ticks;
    while (500 > (g_ticks - tmp_ticks)) {
      __WFI();
    }
    Invers_GPIO(GPIO2, GPIO_Pin_8);

    tmp_ticks = g_ticks;
    while (500 > (g_ticks - tmp_ticks)) {
      __WFI();
    }
    Invers_GPIO(GPIO3, GPIO_Pin_9);
  }
}

/* ------------------------------END OF FILE------------------------------------
 */

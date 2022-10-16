/*******************************************************************************
 *
 *******************************************************************************/
#include "hardware_config.h"

#if defined(__CC_ARM)
uint32_t SystemCoreClock __attribute__((at(0x20001FFC)));
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
uint32_t SystemCoreClock __attribute__((section(".ARM.__at_0x20001FFC")));
#elif defined(__ICCARM__)
__no_init uint32_t SystemCoreClock;
#elif defined(__GNUC__)
uint32_t SystemCoreClock __attribute__((section(".no_init")));
#elif defined(__TASKING__)
uint32_t SystemCoreClock __at(0x20001FFC);
#endif

void UART_init(void);

/*******************************************************************************
 函数名称：    void Hardware_init(void)
 功能描述：    硬件部分初始化
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void Hardware_init(void) {
  __disable_irq();                 /* 关闭中断 中断总开关 */
  SYS_WR_PROTECT = 0x7a83;         /*使能系统寄存器写操作*/
  FLASH_CFG |= 0x00080000;         /* FLASH预取加速使能*/
  GPIO_init();                     /* GPIO初始化 */
  UART_init();                     /* UART0初始化 */
  SoftDelay(100);                  /* 等待硬件初始化完毕*/
  NVIC_SetPriority(UART0_IRQn, 0); // UART0中断优先级配置
  NVIC_EnableIRQ(UART0_IRQn);      //使能UART0定时器中断
  __enable_irq();                  /* 开启中断 */
}

/*******************************************************************************
 函数名称：    void GPIO_init(void)
 功能描述：    GPIO硬件初始化
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          创建
 *******************************************************************************/

void GPIO_init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_StructInit(&GPIO_InitStruct); //初始化结构体

  //配置UART0_RXD  P0.15
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIO0, &GPIO_InitStruct);
  //配置UART0_TXD  P1.0

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIO1, &GPIO_InitStruct);

  GPIO_PinAFConfig(GPIO0, GPIO_PinSource_15, AF4_UART);
  GPIO_PinAFConfig(GPIO1, GPIO_PinSource_0, AF4_UART);

  //配置按键 start：P1.10
  GPIO_StructInit(&GPIO_InitStruct);        //初始化结构体
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; // GPIO输入模式
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIO1, &GPIO_InitStruct);

  //配置按键 stop：P1.11
  GPIO_StructInit(&GPIO_InitStruct);        //初始化结构体
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; // GPIO输入模式
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIO1, &GPIO_InitStruct);

  //配置LED1：P2.8
  GPIO_StructInit(&GPIO_InitStruct);         //初始化结构体
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; // GPIO输出模式
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIO2, &GPIO_InitStruct);

  //配置LED2：P3.9
  GPIO_StructInit(&GPIO_InitStruct);         //初始化结构体
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; // GPIO输出模式
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIO3, &GPIO_InitStruct);
	
	  //肔ED3：P2.9
  GPIO_StructInit(&GPIO_InitStruct);        
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIO2, &GPIO_InitStruct);
}

/*******************************************************************************
 函数名称：    void UART_init(void)
 功能描述：    UART0寄存器配置
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void UART_init(void) {
  UART_InitTypeDef UART_InitStruct;

  UART_StructInit(&UART_InitStruct);
  UART_InitStruct.BaudRate = 115200;
  UART_InitStruct.WordLength = UART_WORDLENGTH_8b; /* 发送数据长度8位 */
  UART_InitStruct.StopBits = UART_STOPBITS_1b;     /* 停止位1位 */
  UART_InitStruct.FirstSend = UART_FIRSTSEND_LSB;  /* 先发送LSB */
  UART_InitStruct.ParityMode = UART_Parity_NO;     /* 无奇偶校验 */
  UART_InitStruct.IRQEna =
      UART_IRQEna_SendOver | UART_IRQEna_RcvOver; /* 串口中断使能 */
  UART_Init(UART0, &UART_InitStruct);
  UART0_IF = 0xff;
}

/*******************************************************************************
 函数名称：    void Clock_Init(void)
 功能描述：    时钟配置
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void Clock_Init(void) {

  SYS_WR_PROTECT = 0x7a83;      /* 解除系统寄存器写保护 */
  SYS_AFE_REG5 |= BIT15 | BIT0; /* BIT15:PLLPDN  BIT0:ADC_Clock*/
  SoftDelay(100);               /* 等待PLL稳定*/
  SYS_CLK_CFG = 0x000001ff;     /* BIT8:0: CLK_HS,1:PLL  | BIT[7:0]CLK_DIV  |
                                   1ff对应96M时钟 ; 开启所有时钟*/
  SystemCoreClock = 96000000;
}

/*******************************************************************************
 函数名称：    void SystemInit(void)
 功能描述：    硬件系统初始化，调用时钟初始化函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/3/14      V1.0           Howlet Li          创建
 *******************************************************************************/
void SystemInit(void) { Clock_Init(); /* 时钟初始化 */ }

/*******************************************************************************
 函数名称：    void SoftDelay(void)
 功能描述：    软件延时函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void SoftDelay(u32 cnt) {
  volatile u32 t_cnt;

  for (t_cnt = 0; t_cnt < cnt; t_cnt++) {
    __NOP();
  }
}

/*******************************************************************************
 *******************************************************************************/
void Invers_GPIO(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
  u16 reg;
  reg = GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin); //读GPIO电平
  if (reg) {
    GPIO_ResetBits(GPIOx, GPIO_Pin);
  } else {
    GPIO_SetBits(GPIOx, GPIO_Pin);
  }
}

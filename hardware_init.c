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
 �������ƣ�    void Hardware_init(void)
 ����������    Ӳ�����ֳ�ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void Hardware_init(void) {
  __disable_irq();                 /* �ر��ж� �ж��ܿ��� */
  SYS_WR_PROTECT = 0x7a83;         /*ʹ��ϵͳ�Ĵ���д����*/
  FLASH_CFG |= 0x00080000;         /* FLASHԤȡ����ʹ��*/
  GPIO_init();                     /* GPIO��ʼ�� */
  UART_init();                     /* UART0��ʼ�� */
  SoftDelay(100);                  /* �ȴ�Ӳ����ʼ�����*/
  NVIC_SetPriority(UART0_IRQn, 0); // UART0�ж����ȼ�����
  NVIC_EnableIRQ(UART0_IRQn);      //ʹ��UART0��ʱ���ж�
  __enable_irq();                  /* �����ж� */
}

/*******************************************************************************
 �������ƣ�    void GPIO_init(void)
 ����������    GPIOӲ����ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void GPIO_init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_StructInit(&GPIO_InitStruct); //��ʼ���ṹ��

  //����UART0_RXD  P0.15
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIO0, &GPIO_InitStruct);
  //����UART0_TXD  P1.0

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIO1, &GPIO_InitStruct);

  GPIO_PinAFConfig(GPIO0, GPIO_PinSource_15, AF4_UART);
  GPIO_PinAFConfig(GPIO1, GPIO_PinSource_0, AF4_UART);

  //���ð��� start��P1.10
  GPIO_StructInit(&GPIO_InitStruct);        //��ʼ���ṹ��
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; // GPIO����ģʽ
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIO1, &GPIO_InitStruct);

  //���ð��� stop��P1.11
  GPIO_StructInit(&GPIO_InitStruct);        //��ʼ���ṹ��
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; // GPIO����ģʽ
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIO1, &GPIO_InitStruct);

  //����LED1��P2.8
  GPIO_StructInit(&GPIO_InitStruct);         //��ʼ���ṹ��
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; // GPIO���ģʽ
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIO2, &GPIO_InitStruct);

  //����LED2��P3.9
  GPIO_StructInit(&GPIO_InitStruct);         //��ʼ���ṹ��
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; // GPIO���ģʽ
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIO3, &GPIO_InitStruct);
	
	  //�LED3��P2.9
  GPIO_StructInit(&GPIO_InitStruct);        
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIO2, &GPIO_InitStruct);
}

/*******************************************************************************
 �������ƣ�    void UART_init(void)
 ����������    UART0�Ĵ�������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void UART_init(void) {
  UART_InitTypeDef UART_InitStruct;

  UART_StructInit(&UART_InitStruct);
  UART_InitStruct.BaudRate = 115200;
  UART_InitStruct.WordLength = UART_WORDLENGTH_8b; /* �������ݳ���8λ */
  UART_InitStruct.StopBits = UART_STOPBITS_1b;     /* ֹͣλ1λ */
  UART_InitStruct.FirstSend = UART_FIRSTSEND_LSB;  /* �ȷ���LSB */
  UART_InitStruct.ParityMode = UART_Parity_NO;     /* ����żУ�� */
  UART_InitStruct.IRQEna =
      UART_IRQEna_SendOver | UART_IRQEna_RcvOver; /* �����ж�ʹ�� */
  UART_Init(UART0, &UART_InitStruct);
  UART0_IF = 0xff;
}

/*******************************************************************************
 �������ƣ�    void Clock_Init(void)
 ����������    ʱ������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void Clock_Init(void) {

  SYS_WR_PROTECT = 0x7a83;      /* ���ϵͳ�Ĵ���д���� */
  SYS_AFE_REG5 |= BIT15 | BIT0; /* BIT15:PLLPDN  BIT0:ADC_Clock*/
  SoftDelay(100);               /* �ȴ�PLL�ȶ�*/
  SYS_CLK_CFG = 0x000001ff;     /* BIT8:0: CLK_HS,1:PLL  | BIT[7:0]CLK_DIV  |
                                   1ff��Ӧ96Mʱ�� ; ��������ʱ��*/
  SystemCoreClock = 96000000;
}

/*******************************************************************************
 �������ƣ�    void SystemInit(void)
 ����������    Ӳ��ϵͳ��ʼ��������ʱ�ӳ�ʼ������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2016/3/14      V1.0           Howlet Li          ����
 *******************************************************************************/
void SystemInit(void) { Clock_Init(); /* ʱ�ӳ�ʼ�� */ }

/*******************************************************************************
 �������ƣ�    void SoftDelay(void)
 ����������    �����ʱ����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
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
  reg = GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin); //��GPIO��ƽ
  if (reg) {
    GPIO_ResetBits(GPIOx, GPIO_Pin);
  } else {
    GPIO_SetBits(GPIOx, GPIO_Pin);
  }
}

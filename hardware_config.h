/*******************************************************************************
 *
 *******************************************************************************/

/*------------------------------prevent recursive inclusion
 * -------------------*/
#ifndef __HARDWARE_CONFIG_H_
#define __HARDWARE_CONFIG_H_

#include "hardware_init.h"

#define P_HIGH__N_HIGH 0 /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܸߵ�ƽ��Ч */
#define P_HIGH__N_LOW 1  /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܵ͵�ƽ��Ч */

#define CHIP_PART_NUMBER                                                       \
  LKS32MC085 /* оƬ�ͺ�ѡ��ѡ����ȷ��Ӱ��оƬģ��ĳ�ʼ�� */

#if ((CHIP_PART_NUMBER == LKS32MC084D) || (CHIP_PART_NUMBER == LKS32MC086D) || \
     (CHIP_PART_NUMBER == LKS32MC087D))
#define MCPWM_SWAP_FUNCTION 1
#define PRE_DRIVER_POLARITY                                                    \
  P_HIGH__N_HIGH /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܸߵ�ƽ��Ч */
#else
#define PRE_DRIVER_POLARITY                                                    \
  P_HIGH__N_LOW /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܵ͵�ƽ��Ч */
#endif

/* ----------------------PWM Ƶ�ʼ���������-----------------------------------
 */
#define MCU_MCLK (96000000LL)    /* PWMģ��������Ƶ */
#define PWM_MCLK ((u32)MCU_MCLK) /* PWMģ��������Ƶ */
#define PWM_PRSC ((u8)0)         /* PWMģ������Ԥ��Ƶ�� */
#define PWM_FREQ ((u16)16000)    /* PWMն��Ƶ�� */

/* �������PWM ���ڼ�����ֵ */
#define PWM_PERIOD ((u16)(PWM_MCLK / (u32)(2 * PWM_FREQ * (PWM_PRSC + 1))))
/* PFC����PWM ���ڼ�����ֵ */
#define PFC_PERIOD ((u16)(PWM_MCLK / (u32)(2 * PFC_FREQ * (PWM_PRSC + 1))))

#define DEADTIME_NS ((u16)1200) /* ����ʱ�� */
#define DEADTIME                                                               \
  (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)DEADTIME_NS) /     \
        1000000000uL)

#define DEADTIMECOMPVOLTAGE                                                    \
  (u16)(DEADTIME_NS / (1000000000.0 / PWM_FREQ) * MAX_MODULE_VALUE)

/* --------------------------------ADCͨ���Ŷ���------------------------------
 */
#define ADC0_CHANNEL_OPA0 ADC_CHANNEL_0
#define ADC0_CHANNEL_OPA1 ADC_CHANNEL_1
#define ADC0_CHANNEL_OPA2 ADC_CHANNEL_2
#define ADC0_CHANNEL_OPA3 ADC_CHANNEL_3

#define ADC1_CHANNEL_OPA0 ADC_CHANNEL_0
#define ADC1_CHANNEL_OPA1 ADC_CHANNEL_1
#define ADC1_CHANNEL_OPA2 ADC_CHANNEL_2
#define ADC1_CHANNEL_OPA3 ADC_CHANNEL_3

/* ADC���������ʱ��Ӳ����� ------------------------------------------------
 */
/* Porting Application Notice ע��������� ------------------------------------
 */
#define ADC0_CURRETN_A_CHANNEL (ADC0_CHANNEL_OPA0)
#define ADC0_CURRETN_B_CHANNEL (ADC0_CHANNEL_OPA1)
#define ADC0_CURRETN_C_CHANNEL (ADC0_CHANNEL_OPA2)

#define ADC_1SHUNT_CURR_CH (ADC0_CHANNEL_OPA3) /* �������������ͨ�� */

#define ADC_BUS_VOL_CHANNEL (ADC_CHANNEL_4)    /* ĸ�ߵ�ѹADC����ͨ�� */
#define M0_ADC_BUS_CURR_CH (ADC0_CHANNEL_OPA3) /* ĸ�ߵ���ADC����ͨ�� */

#define ADC0_TEMP_CHANNEL (ADC_CHANNEL_7) /* �¶ȼ�� */

#define BEMF_CH_A ADC_CHANNEL_17 /* A�෴���Ƽ��ADCͨ���� */
#define BEMF_CH_B ADC_CHANNEL_16 /* B�෴���Ƽ��ADCͨ���� */
#define BEMF_CH_C ADC_CHANNEL_15 /* C�෴���Ƽ��ADCͨ���� */

/* �����ڻ������������ADCͨ����������궨�� */
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#define GET_ADC0_DATA(value)                                                   \
  { value.nData0 = (s16)ADC0_DAT0; }
#define GET_ADC1_DATA(value)                                                   \
  { value.nData0 = (s16)ADC0_DAT2; }
#else
#define GET_ADC0_DATA(value)                                                   \
  { value.nData0 = (s16)ADC0_DAT0; }
#define GET_ADC1_DATA(value)                                                   \
  { value.nData0 = (s16)ADC0_DAT1; }
#endif

/* ĸ�ߵ�ѹADCͨ����������궨�� */
#define GET_BUS_VOL_ADC_RESULT (ADC0_DAT4)

#define ADC_STATE_RESET()                                                      \
  { ADC0_CFG |= BIT2; } /* ADC0 ״̬����λ,���Լ��������ȷ��ADC����״̬ */
#define ADC_SOFTWARE_TRIG_ONLY()                                               \
  { ADC0_TRIG = 0; } /* ADC����Ϊ��������� */

/* ------------------------------PGA������ض��� ------------------------------
 */
#define OPA0_GIAN (PGA_GAIN_20)
#define OPA1_GIAN (PGA_GAIN_20 << 2)
#define OPA2_GIAN (PGA_GAIN_20 << 4)
#define OPA3_GIAN (PGA_GAIN_20 << 6)

/* ------------------------------DAC������ض��� ------------------------------
 */
#define DAC_RANGE_1V2 1  /* DAC 1.2V���� */
#define DAC_RANGE_3V0 0  /* DAC 3.0V���� */
#define DAC_RANGE_4V85 2 /* DAC 4.85V���� */

#define TICK_HZ 1000L

#endif
/* ------------------------------END OF FILE------------------------------------
 */

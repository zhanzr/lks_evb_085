#include "dsp_periph.h"

/*******************************************************************************
 �������ƣ�    sin_cos_result dsp_sin_cos_func(int16_t angle_q_1_15)
 ����������    ��Ƕȵ����Һ�����ֵ
 ���������    int16_t angle_q_1_15 ���Ƕ�
 ���������    ���Һ�����ֵ
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
sin_cos_result dsp_sin_cos_func(int16_t angle_q_1_15) {
  sin_cos_result result;

  SYS_ModuleClockCmd(SYS_Module_DSP, ENABLE); /* DSPʱ��ʹ��*/
  DSP_SC |= BIT2;                             // sin��cos����
  DSP_CORDIC_THETA = angle_q_1_15;            //�Ƕ�

  result.hSin = DSP_CORDIC_SIN;
  result.hCos = DSP_CORDIC_COS;

  return result;
}

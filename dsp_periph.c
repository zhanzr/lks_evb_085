#include "dsp_periph.h"

/*******************************************************************************
 函数名称：    sin_cos_result dsp_sin_cos_func(int16_t angle_q_1_15)
 功能描述：    求角度的正弦和余弦值
 输入参数：    int16_t angle_q_1_15 ：角度
 输出参数：    正弦和余弦值
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
sin_cos_result dsp_sin_cos_func(int16_t angle_q_1_15) {
  sin_cos_result result;

  SYS_ModuleClockCmd(SYS_Module_DSP, ENABLE); /* DSP时钟使能*/
  DSP_SC |= BIT2;                             // sin与cos计算
  DSP_CORDIC_THETA = angle_q_1_15;            //角度

  result.hSin = DSP_CORDIC_SIN;
  result.hCos = DSP_CORDIC_COS;

  return result;
}

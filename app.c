/*******************************************************************************
 *
 *******************************************************************************/
#include <inttypes.h>

#include "hardware_config.h"
#include <lks32mc08x.h>

#include "dsp_periph.h"

#include "arm_math.h"
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

#define TEST_LOOP_CNT 500
#define TEST_POINT_NUM 360

// for i in range(360):
//    print(int(i*32768/360), end=',')
//(0 ~ 32767) Map to (0~2??)
const uint16_t angle_array_360_0_2Pi[] = {
    0,     91,    182,   273,   364,   455,   546,   637,   728,   819,   910,
    1001,  1092,  1183,  1274,  1365,  1456,  1547,  1638,  1729,  1820,  1911,
    2002,  2093,  2184,  2275,  2366,  2457,  2548,  2639,  2730,  2821,  2912,
    3003,  3094,  3185,  3276,  3367,  3458,  3549,  3640,  3731,  3822,  3913,
    4004,  4096,  4187,  4278,  4369,  4460,  4551,  4642,  4733,  4824,  4915,
    5006,  5097,  5188,  5279,  5370,  5461,  5552,  5643,  5734,  5825,  5916,
    6007,  6098,  6189,  6280,  6371,  6462,  6553,  6644,  6735,  6826,  6917,
    7008,  7099,  7190,  7281,  7372,  7463,  7554,  7645,  7736,  7827,  7918,
    8009,  8100,  8192,  8283,  8374,  8465,  8556,  8647,  8738,  8829,  8920,
    9011,  9102,  9193,  9284,  9375,  9466,  9557,  9648,  9739,  9830,  9921,
    10012, 10103, 10194, 10285, 10376, 10467, 10558, 10649, 10740, 10831, 10922,
    11013, 11104, 11195, 11286, 11377, 11468, 11559, 11650, 11741, 11832, 11923,
    12014, 12105, 12196, 12288, 12379, 12470, 12561, 12652, 12743, 12834, 12925,
    13016, 13107, 13198, 13289, 13380, 13471, 13562, 13653, 13744, 13835, 13926,
    14017, 14108, 14199, 14290, 14381, 14472, 14563, 14654, 14745, 14836, 14927,
    15018, 15109, 15200, 15291, 15382, 15473, 15564, 15655, 15746, 15837, 15928,
    16019, 16110, 16201, 16292, 16384, 16475, 16566, 16657, 16748, 16839, 16930,
    17021, 17112, 17203, 17294, 17385, 17476, 17567, 17658, 17749, 17840, 17931,
    18022, 18113, 18204, 18295, 18386, 18477, 18568, 18659, 18750, 18841, 18932,
    19023, 19114, 19205, 19296, 19387, 19478, 19569, 19660, 19751, 19842, 19933,
    20024, 20115, 20206, 20297, 20388, 20480, 20571, 20662, 20753, 20844, 20935,
    21026, 21117, 21208, 21299, 21390, 21481, 21572, 21663, 21754, 21845, 21936,
    22027, 22118, 22209, 22300, 22391, 22482, 22573, 22664, 22755, 22846, 22937,
    23028, 23119, 23210, 23301, 23392, 23483, 23574, 23665, 23756, 23847, 23938,
    24029, 24120, 24211, 24302, 24393, 24484, 24576, 24667, 24758, 24849, 24940,
    25031, 25122, 25213, 25304, 25395, 25486, 25577, 25668, 25759, 25850, 25941,
    26032, 26123, 26214, 26305, 26396, 26487, 26578, 26669, 26760, 26851, 26942,
    27033, 27124, 27215, 27306, 27397, 27488, 27579, 27670, 27761, 27852, 27943,
    28034, 28125, 28216, 28307, 28398, 28489, 28580, 28672, 28763, 28854, 28945,
    29036, 29127, 29218, 29309, 29400, 29491, 29582, 29673, 29764, 29855, 29946,
    30037, 30128, 30219, 30310, 30401, 30492, 30583, 30674, 30765, 30856, 30947,
    31038, 31129, 31220, 31311, 31402, 31493, 31584, 31675, 31766, 31857, 31948,
    32039, 32130, 32221, 32312, 32403, 32494, 32585, 32676};

// for i in range(-180, 180):
//    print(int(i*32768/180), end=',')
//(-32768 ~ 32767) Map to (?C??~??)
const int16_t angle_array_360_negPi_PosPi[] = {
    -32768, -32585, -32403, -32221, -32039, -31857, -31675, -31493, -31311,
    -31129, -30947, -30765, -30583, -30401, -30219, -30037, -29855, -29673,
    -29491, -29309, -29127, -28945, -28763, -28580, -28398, -28216, -28034,
    -27852, -27670, -27488, -27306, -27124, -26942, -26760, -26578, -26396,
    -26214, -26032, -25850, -25668, -25486, -25304, -25122, -24940, -24758,
    -24576, -24393, -24211, -24029, -23847, -23665, -23483, -23301, -23119,
    -22937, -22755, -22573, -22391, -22209, -22027, -21845, -21663, -21481,
    -21299, -21117, -20935, -20753, -20571, -20388, -20206, -20024, -19842,
    -19660, -19478, -19296, -19114, -18932, -18750, -18568, -18386, -18204,
    -18022, -17840, -17658, -17476, -17294, -17112, -16930, -16748, -16566,
    -16384, -16201, -16019, -15837, -15655, -15473, -15291, -15109, -14927,
    -14745, -14563, -14381, -14199, -14017, -13835, -13653, -13471, -13289,
    -13107, -12925, -12743, -12561, -12379, -12196, -12014, -11832, -11650,
    -11468, -11286, -11104, -10922, -10740, -10558, -10376, -10194, -10012,
    -9830,  -9648,  -9466,  -9284,  -9102,  -8920,  -8738,  -8556,  -8374,
    -8192,  -8009,  -7827,  -7645,  -7463,  -7281,  -7099,  -6917,  -6735,
    -6553,  -6371,  -6189,  -6007,  -5825,  -5643,  -5461,  -5279,  -5097,
    -4915,  -4733,  -4551,  -4369,  -4187,  -4004,  -3822,  -3640,  -3458,
    -3276,  -3094,  -2912,  -2730,  -2548,  -2366,  -2184,  -2002,  -1820,
    -1638,  -1456,  -1274,  -1092,  -910,   -728,   -546,   -364,   -182,
    0,      182,    364,    546,    728,    910,    1092,   1274,   1456,
    1638,   1820,   2002,   2184,   2366,   2548,   2730,   2912,   3094,
    3276,   3458,   3640,   3822,   4004,   4187,   4369,   4551,   4733,
    4915,   5097,   5279,   5461,   5643,   5825,   6007,   6189,   6371,
    6553,   6735,   6917,   7099,   7281,   7463,   7645,   7827,   8009,
    8192,   8374,   8556,   8738,   8920,   9102,   9284,   9466,   9648,
    9830,   10012,  10194,  10376,  10558,  10740,  10922,  11104,  11286,
    11468,  11650,  11832,  12014,  12196,  12379,  12561,  12743,  12925,
    13107,  13289,  13471,  13653,  13835,  14017,  14199,  14381,  14563,
    14745,  14927,  15109,  15291,  15473,  15655,  15837,  16019,  16201,
    16384,  16566,  16748,  16930,  17112,  17294,  17476,  17658,  17840,
    18022,  18204,  18386,  18568,  18750,  18932,  19114,  19296,  19478,
    19660,  19842,  20024,  20206,  20388,  20571,  20753,  20935,  21117,
    21299,  21481,  21663,  21845,  22027,  22209,  22391,  22573,  22755,
    22937,  23119,  23301,  23483,  23665,  23847,  24029,  24211,  24393,
    24576,  24758,  24940,  25122,  25304,  25486,  25668,  25850,  26032,
    26214,  26396,  26578,  26760,  26942,  27124,  27306,  27488,  27670,
    27852,  28034,  28216,  28398,  28580,  28763,  28945,  29127,  29309,
    29491,  29673,  29855,  30037,  30219,  30401,  30583,  30765,  30947,
    31129,  31311,  31493,  31675,  31857,  32039,  32221,  32403,  32585};

//for i in range(360):
//    print('%.3f' % (i*math.pi/180), end=',')
const float32_t angle_array_360_0_2Pi_f32[] = {
    0.000, 0.017, 0.035, 0.052, 0.070, 0.087, 0.105, 0.122, 0.140, 0.157, 0.175,
    0.192, 0.209, 0.227, 0.244, 0.262, 0.279, 0.297, 0.314, 0.332, 0.349, 0.367,
    0.384, 0.401, 0.419, 0.436, 0.454, 0.471, 0.489, 0.506, 0.524, 0.541, 0.559,
    0.576, 0.593, 0.611, 0.628, 0.646, 0.663, 0.681, 0.698, 0.716, 0.733, 0.750,
    0.768, 0.785, 0.803, 0.820, 0.838, 0.855, 0.873, 0.890, 0.908, 0.925, 0.942,
    0.960, 0.977, 0.995, 1.012, 1.030, 1.047, 1.065, 1.082, 1.100, 1.117, 1.134,
    1.152, 1.169, 1.187, 1.204, 1.222, 1.239, 1.257, 1.274, 1.292, 1.309, 1.326,
    1.344, 1.361, 1.379, 1.396, 1.414, 1.431, 1.449, 1.466, 1.484, 1.501, 1.518,
    1.536, 1.553, 1.571, 1.588, 1.606, 1.623, 1.641, 1.658, 1.676, 1.693, 1.710,
    1.728, 1.745, 1.763, 1.780, 1.798, 1.815, 1.833, 1.850, 1.868, 1.885, 1.902,
    1.920, 1.937, 1.955, 1.972, 1.990, 2.007, 2.025, 2.042, 2.059, 2.077, 2.094,
    2.112, 2.129, 2.147, 2.164, 2.182, 2.199, 2.217, 2.234, 2.251, 2.269, 2.286,
    2.304, 2.321, 2.339, 2.356, 2.374, 2.391, 2.409, 2.426, 2.443, 2.461, 2.478,
    2.496, 2.513, 2.531, 2.548, 2.566, 2.583, 2.601, 2.618, 2.635, 2.653, 2.670,
    2.688, 2.705, 2.723, 2.740, 2.758, 2.775, 2.793, 2.810, 2.827, 2.845, 2.862,
    2.880, 2.897, 2.915, 2.932, 2.950, 2.967, 2.985, 3.002, 3.019, 3.037, 3.054,
    3.072, 3.089, 3.107, 3.124, 3.142, 3.159, 3.176, 3.194, 3.211, 3.229, 3.246,
    3.264, 3.281, 3.299, 3.316, 3.334, 3.351, 3.368, 3.386, 3.403, 3.421, 3.438,
    3.456, 3.473, 3.491, 3.508, 3.526, 3.543, 3.560, 3.578, 3.595, 3.613, 3.630,
    3.648, 3.665, 3.683, 3.700, 3.718, 3.735, 3.752, 3.770, 3.787, 3.805, 3.822,
    3.840, 3.857, 3.875, 3.892, 3.910, 3.927, 3.944, 3.962, 3.979, 3.997, 4.014,
    4.032, 4.049, 4.067, 4.084, 4.102, 4.119, 4.136, 4.154, 4.171, 4.189, 4.206,
    4.224, 4.241, 4.259, 4.276, 4.294, 4.311, 4.328, 4.346, 4.363, 4.381, 4.398,
    4.416, 4.433, 4.451, 4.468, 4.485, 4.503, 4.520, 4.538, 4.555, 4.573, 4.590,
    4.608, 4.625, 4.643, 4.660, 4.677, 4.695, 4.712, 4.730, 4.747, 4.765, 4.782,
    4.800, 4.817, 4.835, 4.852, 4.869, 4.887, 4.904, 4.922, 4.939, 4.957, 4.974,
    4.992, 5.009, 5.027, 5.044, 5.061, 5.079, 5.096, 5.114, 5.131, 5.149, 5.166,
    5.184, 5.201, 5.219, 5.236, 5.253, 5.271, 5.288, 5.306, 5.323, 5.341, 5.358,
    5.376, 5.393, 5.411, 5.428, 5.445, 5.463, 5.480, 5.498, 5.515, 5.533, 5.550,
    5.568, 5.585, 5.603, 5.620, 5.637, 5.655, 5.672, 5.690, 5.707, 5.725, 5.742,
    5.760, 5.777, 5.794, 5.812, 5.829, 5.847, 5.864, 5.882, 5.899, 5.917, 5.934,
    5.952, 5.969, 5.986, 6.004, 6.021, 6.039, 6.056, 6.074, 6.091, 6.109, 6.126,
    6.144, 6.161, 6.178, 6.196, 6.213, 6.231, 6.248, 6.266};

int main(void) {
  volatile uint32_t tmp_ticks;
  volatile uint32_t delta_ticks;

  Hardware_init();

  /* System timer configuration */
  SysTick_Config_Div8((SystemCoreClock >> 3) / TICK_HZ);

  while (1) {
    uint32_t test_loop_count = TEST_LOOP_CNT;
    // DSP sine & cosine
    tmp_ticks = g_ticks;
    while (--test_loop_count) {
      for (uint32_t idx = 0; idx < TEST_POINT_NUM; ++idx) {
        volatile float sin_Vlaue;
        volatile float cos_Vlaue;
        int16_t angle_q1_15 = angle_array_360_negPi_PosPi[idx];
        sin_cos_result result = dsp_sin_cos_func(angle_q1_15);
        //			sin_Vlaue = (float)(result.hSin) / 0x7FFF;
        //			cos_Vlaue = (float)(result.hCos) / 0x7FFF;

        //			printf("%.3f\t%.3f\t%.3f\n",
        //		(angle_q1_15*180.0)/32768,
        //		sin_Vlaue, cos_Vlaue);
      }
    }
    delta_ticks = g_ticks - tmp_ticks;

    printf("DSP device -Pi~Pi %d, delta:%u, loop count: %u \n",
           sizeof(angle_array_360_negPi_PosPi) /
               sizeof(angle_array_360_negPi_PosPi[0]),
           delta_ticks, TEST_LOOP_CNT);

    printf("\n");
    printf("%s\n", __VERSION__);
    printf("\n");

    // CMSIS DSP sine & cosine
    test_loop_count = TEST_LOOP_CNT;
    tmp_ticks = g_ticks;
    while (--test_loop_count) {
      for (uint32_t idx = 0; idx < TEST_POINT_NUM; ++idx) {
        volatile q15_t result_q15[2];
        volatile float32_t result_f32[2];
        q15_t angle_q1_15 = angle_array_360_0_2Pi[idx];
        result_q15[0] = arm_sin_q15(angle_q1_15);
        result_q15[1] = arm_cos_q15(angle_q1_15);
        //			arm_q15_to_float(result_q15, result_f32, 2);
        //			printf("%.3f\t%.3f\t%.3f\n",
        //		((uint16_t)angle_q1_15*360.0)/32768,
        //		result_f32[0], result_f32[1]);
      }
    }
    delta_ticks = g_ticks - tmp_ticks;

    printf("CMSIS DSP 0~2Pi %d, delta:%u, loop count: %u \n",
           sizeof(angle_array_360_0_2Pi) / sizeof(angle_array_360_0_2Pi[0]),
           delta_ticks, TEST_LOOP_CNT);

    printf("\n");
    printf("%s\n", __VERSION__);
    printf("%s\n", __VERSION__);
    printf("%s\n", __VERSION__);
    printf("\n");

    // Math soft float point library sine & cosine (single precision)
    test_loop_count = TEST_LOOP_CNT;
    tmp_ticks = g_ticks;
    while (--test_loop_count) {
      for (uint32_t idx = 0; idx < TEST_POINT_NUM; ++idx) {
        volatile float32_t sin_vlaue_f32;
        volatile float32_t cos_vlaue_f32;

        float32_t angle_f = angle_array_360_0_2Pi_f32[idx];
        sin_vlaue_f32 = sinf(angle_f);
        cos_vlaue_f32 = cosf(angle_f);
      }
    }
    delta_ticks = g_ticks - tmp_ticks;

    printf("Soft float32 0~2Pi %d, delta:%u, loop count: %u \n",
           sizeof(angle_array_360_0_2Pi_f32) /
               sizeof(angle_array_360_0_2Pi_f32[0]),
           delta_ticks, TEST_LOOP_CNT);

    printf("\n");
    printf("%s\n", __VERSION__);
    printf("%s\n", __VERSION__);
    printf("%s\n", __VERSION__);
    printf("\n");

    // Math soft float point library sine & cosine (double precision)
    test_loop_count = TEST_LOOP_CNT;
    tmp_ticks = g_ticks;
    while (--test_loop_count) {
      for (uint32_t idx = 0; idx < TEST_POINT_NUM; ++idx) {
        volatile float64_t sin_vlaue_f64;
        volatile float64_t cos_vlaue_f64;

        float64_t angle_f64 = (double)angle_array_360_0_2Pi_f32[idx];
        sin_vlaue_f64 = sin(angle_f64);
        cos_vlaue_f64 = cos(angle_f64);
      }
    }
    delta_ticks = g_ticks - tmp_ticks;

    printf("Soft float64 0~2Pi %d, delta:%u, loop count: %u \n",
           sizeof(angle_array_360_0_2Pi_f32) /
               sizeof(angle_array_360_0_2Pi_f32[0]),
           delta_ticks, TEST_LOOP_CNT);

    printf("\n");
    printf("%s\n", __VERSION__);
    printf("%s\n", __VERSION__);
    printf("%s\n", __VERSION__);
    printf("\n");

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

    while (1) {
      __NOP();
    }
  }
}

/* ------------------------------END OF FILE------------------------------------
 */

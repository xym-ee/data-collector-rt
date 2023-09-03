#ifndef __POWER_H
#define __POWER_H


#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>




#define POWER_THREAD_STACK_SIZE   1024
#define POWER_THREAD_PRIORITY     16
#define POWER_THREAD_TIMESLICE    10


/*---------------------------------------- GPIO相关 --------------------------------------------*/

#define OUT_24V_EN_PIN          GET_PIN(D, 10)
#define SER_5V_EN1_PIN          GET_PIN(D, 4)
#define SER_5V_EN2_PIN          GET_PIN(D, 3)
#define SER_5V_EN3_PIN          GET_PIN(D, 1)
#define SER_5V_EN4_PIN          GET_PIN(D, 0)
#define SER_5V_EN5_PIN          GET_PIN(A, 15)
#define IIC_5V_EN1_PIN          GET_PIN(C, 8)
#define IIC_5V_EN2_PIN          GET_PIN(D, 15)
#define PC__12V_EN_PIN          GET_PIN(E, 15)
#define CAM_12V_EN_PIN          GET_PIN(E, 14)

//  5V       PIN_HIGH 开启输出
//  PC12V，  PIN_HIGH 开启输出
//  CAM12V， PIN_HIGH 开启输出

/*---------------------------------------- ADC相关 --------------------------------------------*/

#define ADC_DEV_NAME            "adc1"      /* ADC 设备名称 */
#define REFER_VOLTAGE           33         /* 参考电压 3.30V,数据精度乘以100保留2位小数*/
#define CONVERT_BITS            (1 << 12)   /* 转换位数为12位 */



#define CHANNEL_OUT_24V         5
#define CHANNEL_LED_18V         4
#define CHANNEL_CAM_12V         0
#define CHANNEL_PC_12V          1
#define CHANNEL_OUT_5V          11

#define K_OUT_24V               (REFER_VOLTAGE*(11.0f + 1.5f)/1.5f/CONVERT_BITS)
#define K_LED_18V               (REFER_VOLTAGE*(12.0f + 1.5f)/1.5f/CONVERT_BITS)
#define K_CAM_12V               (REFER_VOLTAGE*(7.5f + 1.5f)/1.5f/CONVERT_BITS)
#define K_PC_12V                (REFER_VOLTAGE*(7.5f + 1.5f)/1.5f/CONVERT_BITS)
#define K_OUT_5V                (REFER_VOLTAGE*(1.5f + 1.0f)/1.0f/CONVERT_BITS)






#endif

#ifndef __IIC_DISTANCE_H
#define __IIC_DISTANCE_H


#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>



#define DISTANCE_PRIORITY         5
#define DISTANCE_STACK_SIZE       1024
#define DISTANCE_TIMESLICE        5




/*
    IIC3_SCL_PA8    8
    IIC3_SDA_PC9    41
*/

#define DISTANCE_I2C_BUS_NAME               "i2c3"                              /* 传感器连接的I2C总线设备名称 */


#define DISTANCE_FETCH_CMD                  {0x5A, 0x05, 0x00, 0x01, 0x60}


#define DISTANCE_SENSOR1_ADDR               0x50                                /* 从机地址 */
#define DISTANCE_SENSOR2_ADDR               0x51                                /* 从机地址 */
#define DISTANCE_SENSOR3_ADDR               0x52                                /* 从机地址 */




#endif


#ifndef __IIC_DEV_H
#define __IIC_DEV_H


#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>




#define TEMP1_PRIORITY         5
#define TEMP1_STACK_SIZE       1024
#define TEMP1_TIMESLICE        5





/*
    IIC3_SCL_PA8    8
    IIC3_SDA_PC9    41
*/

#define MCP9808_I2C_BUS_NAME            "i2c3"  /* 传感器连接的I2C总线设备名称 */
#define MCP9808_ADDR                    0x18    /* 从机地址 */
#define MCP9808_MODE_TA                 0x05    /* 室温寄存器地址 */





#endif


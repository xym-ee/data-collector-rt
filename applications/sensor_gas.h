#ifndef __SENSOR_GAS_H__
#define __SENSOR_GAS_H__

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>


#define GAS_SEND_PRIORITY           5
#define GAS_SEND_STACK_SIZE         256
#define GAS_SEND_TIMESLICE          5


#define GAS1_UART_DEV               "uart1"

#define GAS1_RX_PRIORITY           5
#define GAS1_RX_STACK_SIZE         256
#define GAS1_RX_TIMESLICE          5


#endif



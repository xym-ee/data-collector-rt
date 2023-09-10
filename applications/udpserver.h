#ifndef __UDPSERVER_H
#define __UDPSERVER_H



#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>


//MCU 开启端口5500 接收消息
#define UDP_RECIVE_PORT     5500


#define NETDEV_NAME "W5500"


#define     UDPSERVER_THREAD_STACK_SIZE   2048
#define     UDPSERVER_THREAD_PRIORITY     16
#define     UDPSERVER_THREAD_TIMESLICE    10


typedef enum{
    RMSG_IDDLE = 0,
    RMSG_ID_POWER,
    RMSG_ID_LIGHT,

}recv_msg_id_t;



#endif

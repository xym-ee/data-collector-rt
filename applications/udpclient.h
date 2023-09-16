#ifndef __UDPCLIENT_H
#define __UDPCLIENT_H


#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

//MCU连接服务器 5501 端口发送消息
#define     UDP_SERVER_IP                   "192.168.1.111"
#define     UDP_SERVER_PORT                 5501


#define     UDPCLIENT_THREAD_STACK_SIZE     2048
#define     UDPCLIENT_THREAD_PRIORITY       16
#define     UDPCLIENT_THREAD_TIMESLICE      10



typedef enum{
    SMSG_IDDLE = 0,
    SMSG_ID_VOLTAGE,
    SMSG_ID_OPEN_STATUS,
    SMSG_ID_TEMP,
    SMSG_ID_GAS,
    SMSG_ID_DISTANCE,

}send_msg_id_t;



#endif

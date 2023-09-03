#ifndef __UDPCLIENT_H
#define __UDPCLIENT_H


#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>


#define UDP_SERVER_IP       "10.0.0.9"
#define UDP_SERVER_PORT     5500


#define     UDPCLIENT_THREAD_STACK_SIZE   2048
#define     UDPCLIENT_THREAD_PRIORITY     16
#define     UDPCLIENT_THREAD_TIMESLICE    10



typedef enum{
    SMSG_IDDLE = 0,
    SMSG_ID_VOLTAGE,
    SMSG_ID_STATUS,
    SMSG_ID_DISTANCE,
    SMSG_ID_GAS,

}send_msg_id_t;



#endif

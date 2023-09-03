#ifndef __LIGHT_H
#define __LIGHT_H

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>




#define     LIGHT_THREAD_STACK_SIZE   512
#define     LIGHT_THREAD_PRIORITY     16
#define     LIGHT_THREAD_TIMESLICE    10


#define     LIGHT_B1_PIN            GET_PIN(E, 12)
#define     LIGHT_B2_PIN            GET_PIN(E, 13)

#define     LIGHT_L0_PIN            GET_PIN(E, 11)
#define     LIGHT_L1_PIN            GET_PIN(E, 10)
#define     LIGHT_L2_PIN            GET_PIN(E, 9)
#define     LIGHT_L3_PIN            GET_PIN(E, 8)
#define     LIGHT_L4_PIN            GET_PIN(E, 7)
#define     LIGHT_L5_PIN            GET_PIN(B, 2)
#define     LIGHT_L6_PIN            GET_PIN(C, 5)
#define     LIGHT_L7_PIN            GET_PIN(C, 4)

#define     LIGHT_PROTECT_PIN


#endif

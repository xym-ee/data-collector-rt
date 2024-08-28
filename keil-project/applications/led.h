#ifndef __LED_H
#define __LED_H

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>




#define     LED_THREAD_STACK_SIZE   256
#define     LED_THREAD_PRIORITY     16
#define     LED_THREAD_TIMESLICE    10


/* defined the LED0 pin: PC13 */
#define LED0_PIN    GET_PIN(C, 13)


#endif

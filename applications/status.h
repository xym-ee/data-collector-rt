#ifndef __STATUS_H
#define __STATUS_H


#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

/*-------------------- 电源状态 -------------------*/

typedef enum {
    POWER_OFF = 0,
    POWER_ON,
} power_status_t;

typedef struct {
    power_status_t out_24v_en;
    power_status_t pc_12v_en;
    power_status_t cam_12v_en;
    
    power_status_t ser_5v_en1;
    power_status_t ser_5v_en2;    
    power_status_t ser_5v_en3;    
    power_status_t ser_5v_en4;    
    power_status_t ser_5v_en5;
    power_status_t iic_5v_en1;
    power_status_t iic_5v_en2;

    
    rt_uint8_t voltage_24v;
    rt_uint8_t voltage_led_18v;
    rt_uint8_t voltage_cam_12v;
    rt_uint8_t voltage_pc_12v;
    rt_uint8_t voltage_5v;
    
} power_t;



/*-------------------- 灯光状态 -------------------*/

typedef enum {
    LIGHT_OFF = 0,
    LIGHT_ON,
} light_status_t;


typedef enum {
    LIGHT_17V = 0,
    LIGHT_17_5V,
    LIGHT_18V,
    LIGHT_18_5V,
} light_level_t;


typedef struct {
    light_level_t   level;
    rt_uint8_t protect;    
    
    light_status_t light0;
    light_status_t light1;
    light_status_t light2;
    light_status_t light3;
    light_status_t light4;
    light_status_t light5;
    light_status_t light6;
    light_status_t light7;
} light_t;


/*-------------------- 传感器状态 -------------------*/

typedef struct {
    rt_int16_t temp;
    
    rt_uint16_t gas1;
    rt_uint16_t gas2;

    rt_uint16_t distance1;      /* 单位 cm */
    rt_uint16_t distance2;
    rt_uint16_t distance3;
} seneor_t;


/*------------------------------------------------------------------*/


typedef struct {
    power_t power;
    light_t light;
    
    seneor_t sensor;
    
} status_t;



extern status_t status;

rt_uint8_t sum_check(rt_uint8_t *p, rt_uint8_t num);




#define     STATUS_THREAD_STACK_SIZE   512
#define     STATUS_THREAD_PRIORITY     16
#define     STATUS_THREAD_TIMESLICE    10


#endif

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "status.h"

#define BEEP_PIN    GET_PIN(E, 1)

int main(void)
{
    rt_uint32_t time_count = 0;

    
    
    while(1)
    {   
        time_count++;

        
        rt_kprintf("\x1b[2J\x1b[H");
        
        rt_kprintf("system time: %6d s  ", time_count);
        rt_kprintf("temperature: {%d.%01d} \n", status.sensor.temp/10, status.sensor.temp%10);

        rt_kprintf("power status: ");
        rt_kprintf("24v{%d} | 12V{%d %d} | ", status.power.out_24v_en, status.power.pc_12v_en, status.power.cam_12v_en);
        rt_kprintf("5v{%d %d ", status.power.iic_5v_en2, status.power.iic_5v_en1);
        rt_kprintf("%d %d", status.power.ser_5v_en5, status.power.ser_5v_en4);     
        rt_kprintf(" %d %d %d}\n", status.power.ser_5v_en3, status.power.ser_5v_en2, status.power.ser_5v_en1);
        
        
        rt_kprintf("light status:");
        rt_kprintf("{%d %d %d %d ", status.light.light7, status.light.light6,status.light.light5, status.light.light4);
        rt_kprintf("%d %d %d %d} level{%d}\n", status.light.light3, status.light.light2,status.light.light1, status.light.light0,status.light.level);    
    
        rt_kprintf("gas concentration: {%d.%01dppm} {%d.%01dppm}\n", status.sensor.gas1/10, status.sensor.gas1%10, status.sensor.gas2/10, status.sensor.gas2%10);
        
        rt_kprintf("distance: {%dcm} {%dcm} {%dcm}\n", status.sensor.distance1, status.sensor.distance2, status.sensor.distance3);
        
    
        rt_thread_mdelay(1000);
    }
    
}

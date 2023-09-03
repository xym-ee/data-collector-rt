#include "status.h"



status_t status = {
    
    .power.out_24v_en = POWER_OFF,
    .power.pc_12v_en = POWER_OFF,
    .power.cam_12v_en = POWER_OFF,
    .power.ser_5v_en1 = POWER_OFF,
    .power.ser_5v_en2 = POWER_OFF,
    .power.ser_5v_en3 = POWER_OFF,
    .power.ser_5v_en4 = POWER_OFF,
    .power.ser_5v_en5 = POWER_OFF,
    .power.iic_5v_en1 = POWER_OFF,
    .power.iic_5v_en2 = POWER_OFF,

    
    .light.level = LIGHT_17V,
    .light.protect = 1,
    .light.light0 = LIGHT_OFF,
    .light.light1 = LIGHT_OFF,
    .light.light2 = LIGHT_OFF,
    .light.light3 = LIGHT_OFF,
    .light.light4 = LIGHT_OFF,
    .light.light5 = LIGHT_OFF,
    .light.light6 = LIGHT_OFF,
    .light.light7 = LIGHT_OFF,
};



rt_uint8_t sum_check(rt_uint8_t *p, rt_uint8_t num)
{
    rt_uint8_t sum = 0;
    
    for (int i = 0; i<num; i++)
        sum += p[i];

    return sum;
}


/*--------------------------  数据处理线程  ---------------------------*/

static void status_thread_entry(void *parameter)
{

    while (1)
    {
        /* 供电状态调试 */
//        rt_kprintf("24v %d \n12: %d %d \n", status.power.out_24v_en, status.power.pc_12v_en, status.power.cam_12v_en);
//        rt_kprintf("5v %d %d ", status.power.iic_5v_en2, status.power.iic_5v_en1);
//        rt_kprintf("%d %d", status.power.ser_5v_en5, status.power.ser_5v_en4);     
//        rt_kprintf(" %d %d %d\n", status.power.ser_5v_en3, status.power.ser_5v_en2, status.power.ser_5v_en1);
        
        /* 灯光状态调试 */
//        rt_kprintf("level: %d\n", status.light.level);
//        rt_kprintf("%d %d %d %d ", status.light.light7, status.light.light6,status.light.light5, status.light.light4);
//        rt_kprintf("%d %d %d %d\n", status.light.light3, status.light.light2,status.light.light1, status.light.light0);
//        
        rt_thread_mdelay(2000);
    }
}

int status_init(void)
{
    rt_err_t ret = RT_EOK;

    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("status", 
                                        status_thread_entry, 
                                        RT_NULL, 
                                        STATUS_THREAD_STACK_SIZE, 
                                        STATUS_THREAD_PRIORITY, 
										STATUS_THREAD_TIMESLICE);
    
    /* 创建成功则启动线程 */
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        ret = RT_ERROR;
    }

    return ret;
}

/* 导出命令 or 自动初始化 */
INIT_APP_EXPORT(status_init);











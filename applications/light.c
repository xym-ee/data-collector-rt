#include "light.h"
#include "status.h"


/*--------------------------  数据处理线程  ---------------------------*/

static void light_thread_entry(void *parameter)
{
    /*----------------------- 电源开关引脚配置 ------------------*/
    rt_pin_mode(LIGHT_B1_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LIGHT_B2_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LIGHT_L0_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LIGHT_L1_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LIGHT_L2_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LIGHT_L3_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LIGHT_L4_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LIGHT_L5_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LIGHT_L6_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LIGHT_L7_PIN, PIN_MODE_OUTPUT);
    
    rt_pin_write(LIGHT_B1_PIN, PIN_LOW);
    rt_pin_write(LIGHT_B2_PIN, PIN_LOW);
    rt_pin_write(LIGHT_L0_PIN, PIN_LOW);
    rt_pin_write(LIGHT_L1_PIN, PIN_LOW);
    rt_pin_write(LIGHT_L2_PIN, PIN_LOW);
    rt_pin_write(LIGHT_L3_PIN, PIN_LOW);
    rt_pin_write(LIGHT_L4_PIN, PIN_LOW);
    rt_pin_write(LIGHT_L5_PIN, PIN_LOW);
    rt_pin_write(LIGHT_L6_PIN, PIN_LOW);
    rt_pin_write(LIGHT_L7_PIN, PIN_LOW);    
    

    while (1)
    {
        switch (status.light.level)
        {
            case LIGHT_17V : 
                rt_pin_write(LIGHT_B1_PIN, PIN_LOW);
                rt_pin_write(LIGHT_B2_PIN, PIN_LOW);
                break;
            case LIGHT_17_5V : 
                rt_pin_write(LIGHT_B1_PIN, PIN_LOW);
                rt_pin_write(LIGHT_B2_PIN, PIN_HIGH);
                break;
            case LIGHT_18V :                 
                rt_pin_write(LIGHT_B1_PIN, PIN_HIGH);
                rt_pin_write(LIGHT_B2_PIN, PIN_LOW);
                break;
            case LIGHT_18_5V :                 
                rt_pin_write(LIGHT_B1_PIN, PIN_HIGH);
                rt_pin_write(LIGHT_B2_PIN, PIN_HIGH);
                break;
            default : ;break;        
        }
        
        if (status.light.light0 == LIGHT_ON)
            rt_pin_write(LIGHT_L0_PIN, PIN_HIGH);
        else
            rt_pin_write(LIGHT_L0_PIN, PIN_LOW);
        
        if (status.light.light1 == LIGHT_ON)
            rt_pin_write(LIGHT_L1_PIN, PIN_HIGH);
        else
            rt_pin_write(LIGHT_L1_PIN, PIN_LOW);        
        
        if (status.light.light2 == LIGHT_ON)
            rt_pin_write(LIGHT_L2_PIN, PIN_HIGH);
        else
            rt_pin_write(LIGHT_L2_PIN, PIN_LOW);
        
        if (status.light.light3 == LIGHT_ON)
            rt_pin_write(LIGHT_L3_PIN, PIN_HIGH);
        else
            rt_pin_write(LIGHT_L3_PIN, PIN_LOW);        
        
        if (status.light.light4 == LIGHT_ON)
            rt_pin_write(LIGHT_L4_PIN, PIN_HIGH);
        else
            rt_pin_write(LIGHT_L4_PIN, PIN_LOW);

        if (status.light.light5 == LIGHT_ON)
            rt_pin_write(LIGHT_L5_PIN, PIN_HIGH);
        else
            rt_pin_write(LIGHT_L5_PIN, PIN_LOW);

        if (status.light.light6 == LIGHT_ON)
            rt_pin_write(LIGHT_L6_PIN, PIN_HIGH);
        else
            rt_pin_write(LIGHT_L6_PIN, PIN_LOW);

        if (status.light.light7 == LIGHT_ON)
            rt_pin_write(LIGHT_L7_PIN, PIN_HIGH);
        else
            rt_pin_write(LIGHT_L7_PIN, PIN_LOW);
        
        rt_thread_mdelay(2000);
    }
}

int light_init(void)
{
    rt_err_t ret = RT_EOK;

    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("light", 
                                        light_thread_entry, 
                                        RT_NULL, 
                                        LIGHT_THREAD_STACK_SIZE, 
                                        LIGHT_THREAD_PRIORITY, 
										LIGHT_THREAD_TIMESLICE);
    
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
INIT_APP_EXPORT(light_init);






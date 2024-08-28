#include "led.h"



/*--------------------------  数据处理线程  ---------------------------*/

static void led_thread_entry(void *parameter)
{
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    while (1)
    {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(800);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(200);
    }
}

int led_init(void)
{
    rt_err_t ret = RT_EOK;

    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("led", 
                                        led_thread_entry, 
                                        RT_NULL, 
                                        LED_THREAD_STACK_SIZE, 
                                        LED_THREAD_PRIORITY, 
										LED_THREAD_TIMESLICE);
    
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
INIT_APP_EXPORT(led_init);






#include "light.h"



/*--------------------------  数据处理线程  ---------------------------*/

static void light_thread_entry(void *parameter)
{

    while (1)
    {

        
        rt_thread_mdelay(200);
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






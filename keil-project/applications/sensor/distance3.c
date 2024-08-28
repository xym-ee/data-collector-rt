#include "sensor/distance3.h"

#include "status.h"

/*--------------------------  控制块  ---------------------------*/

/* 串口设备句柄 */
static rt_device_t serial;

/* 串口接收消息结构*/
struct rx_msg
{
    rt_device_t dev;
    rt_size_t size;
};


/* 消息队列控制块 */
static struct rt_messagequeue rx_mq;


/*--------------------------  DMA中断回调  ---------------------------*/

/* 接收数据回调函数 */
static rt_err_t sbus_uart_input(rt_device_t dev, rt_size_t size)
{
    struct rx_msg msg;
    rt_err_t result;
    msg.dev = dev;
    msg.size = size;

    result = rt_mq_send(&rx_mq, &msg, sizeof(msg));
    if ( result == -RT_EFULL)
    {
        /* 消息队列满 */
        rt_kprintf("message queue full！\n");
    }
    return result;
}

/*--------------------------  数据处理线程  ---------------------------*/

static void sbus_thread_entry(void *parameter)
{
    struct rx_msg msg;
    rt_err_t result;
    rt_uint32_t rx_length;
	
    rt_uint8_t rx_buffer[RT_SERIAL_RB_BUFSZ + 1];
    rt_uint8_t* byte = rx_buffer;                   /* 缓冲区别名 */
    
    rt_uint8_t data_count = 0;
    
    while (1)
    {
        rt_memset(&msg, 0, sizeof(msg));
        
        /* 从消息队列中读取消息*/
        result = rt_mq_recv(&rx_mq, &msg, sizeof(msg), RT_WAITING_FOREVER);
        if (result == RT_EOK)
        {
            /* 从串口读取数据*/
            rx_length = rt_device_read(msg.dev, 0, &rx_buffer[data_count], msg.size);
            data_count = data_count + rx_length;
			
            
//            for (int i=0; i<rx_length; i++)
//                rt_kprintf("%02x ", byte[i]);
//            rt_kprintf("\n");
            
			//rt_kprintf("%d\n",rx_length);
            
            /* 可以使用状态机解包 */
            if (byte[0] == 0x59)
            {
                /* 满足帧头才判断帧长 */
                if (data_count >= 9) 
                {
                    for (int i=0; i<9; i++)
                        rt_kprintf("%02x ", byte[i]);
                    rt_kprintf("\n");
                    
                    
                    status.sensor.distance1 = byte[3] << 8 | byte[2];
                    
                    rt_kprintf("%d\n", status.sensor.distance1);

                    
                    /* 准备下一数据帧接收 */
                    data_count = 0;                
                }
            }
            else /*帧头不对，重新接收等待帧头*/
            {
                data_count = 0;
            }
        }
    }
}

int dis1_init(void)
{
    rt_err_t ret = RT_EOK;
    static char msg_pool[256];
    
    /* sbus串口的初始化配置参数 */
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;  
    config.baud_rate = BAUD_RATE_115200;
    config.data_bits = DATA_BITS_8;
    config.parity    = PARITY_NONE;
    config.stop_bits = STOP_BITS_1;
    
    /* 查找串口设备 */
    serial = rt_device_find(SBUS_UART);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", SBUS_UART);
        return RT_ERROR;
    }

    /* 初始化消息队列 */
    rt_mq_init(&rx_mq, "mq_dis1",
               msg_pool,                 /* 存放消息的缓冲区 */
               sizeof(struct rx_msg),    /* 一条消息的最大长度 */
               sizeof(msg_pool),         /* 存放消息的缓冲区大小 */
               RT_IPC_FLAG_FIFO);        /* 如果有多个线程等待，按照先来先得到的方法分配消息 */

    
    rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);      
    
    /* 以 DMA 接收及轮询发送方式打开串口设备 */
    rt_device_open(serial, RT_DEVICE_FLAG_DMA_RX);
    
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial, sbus_uart_input);

    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("dis1", 
                                        sbus_thread_entry, 
                                        RT_NULL, 
                                        SBUS_THREAD_STACK_SIZE, 
                                        SBUS_THREAD_PRIORITY, SBUS_THREAD_TIMESLICE);
    
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
INIT_APP_EXPORT(dis1_init);

/*--------------------------  调试输出线程  ---------------------------*/




#include "sensor/gas1.h"
#include "status.h"


/*
    气体传感器
    
    波特率9600，自动发送数据到MCU，1 Hz，格式 
    FF 01 07 02 HH LL 00 00 00 SUM
    byte[3] 为分辨率
        - 0x00  1
        - 0x01  0.1
        - 0x02  0.01
        - 0x03  0.001
    
    如一次数据为
    FF 01 07 02 00 55 00 00 00 5E
    
    ((0x00<<8) | 0x55 )*0.01 = 0.85 ppm
    
*/



/*---------------------------------------  气体传感器1 DMA接收  --------------------------------------*/
static rt_thread_t sensor_gas1 = RT_NULL;

/* 串口接收消息结构*/
struct rx_msg
{
    rt_device_t dev;
    rt_size_t size;
};

/* 串口设备句柄 */
static rt_device_t gas1_serial;
/* 消息队列控制块 */
static struct rt_messagequeue rx_mq;


/* 接收数据回调函数 */
static rt_err_t gas1_input(rt_device_t dev, rt_size_t size)
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

static void gas1_rx_thread(void *parameter)
{
    struct rx_msg msg;
    rt_err_t result;
    rt_uint32_t rx_length;
    rt_uint8_t rx_buffer[12];
    
    rt_uint8_t *byte = rx_buffer;
    
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
            
            if (byte[0] == 0xFF)
            {
                if (data_count >= 10) 
                {
                    status.sensor.gas1 =  (byte[4]<<8 | byte[5]);
                    
//                    for (int i=0; i<data_count; i++)
//                    {
//                        rt_kprintf("%02x ", rx_buffer[i]);
//                    }
//                    rt_kprintf("\n");

                    
                    data_count = 0;
                }
            }
            else
            {
                data_count = 0;
            }
        }
    }
}

/*----------------------------- 线程启动 -----------------------------*/
int gas1_start(void)
{
    rt_err_t ret = RT_EOK;
    
    /*------------------ DMA接收线程 ---------------------*/
    static char msg_pool[256];
    
    /* 查找串口设备 */
    gas1_serial = rt_device_find(GAS1_UART_DEV);
    if (!gas1_serial)
    {
        rt_kprintf("find %s failed!\n", GAS1_UART_DEV);
        return RT_ERROR;
    }
    
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;  /* 初始化配置参数 */
    config.baud_rate = BAUD_RATE_9600;        //修改波特率为 9600
    rt_device_control(gas1_serial, RT_DEVICE_CTRL_CONFIG, &config);

    /* 初始化消息队列 */
    rt_mq_init(&rx_mq, "mq_gas1",
               msg_pool,                 /* 存放消息的缓冲区 */
               sizeof(struct rx_msg),    /* 一条消息的最大长度 */
               sizeof(msg_pool),         /* 存放消息的缓冲区大小 */
               RT_IPC_FLAG_FIFO);        /* 如果有多个线程等待，按照先来先得到的方法分配消息 */

    /* 以 DMA 接收及轮询发送方式打开串口设备 */
    rt_device_open(gas1_serial, RT_DEVICE_FLAG_DMA_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(gas1_serial, gas1_input);

               
    /* 创建 serial 线程 */
    sensor_gas1 = rt_thread_create("gas1",
                          gas1_rx_thread, 
                          RT_NULL,
                          GAS1_RX_STACK_SIZE,
                          GAS1_RX_PRIORITY, 
                          GAS1_RX_TIMESLICE); 
               
               
    /* 创建成功则启动线程 */
    if (sensor_gas1!= RT_NULL)
    {
        rt_thread_startup(sensor_gas1);
    }
    else
    {
        ret = RT_ERROR;
    }

    return ret;
}

INIT_APP_EXPORT(gas1_start);










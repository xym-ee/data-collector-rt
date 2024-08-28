#include "sensor/iic_dev.h"

#include "status.h"


static struct rt_i2c_bus_device *i2c_bus = RT_NULL;     /* I2C总线设备句柄 */


/*---------------------------- 传感器查询线程 -------------------------------*/

static rt_thread_t temperature1 = RT_NULL;

static void temperature1_thread(void *parameter)
{
    rt_uint8_t recv_buf[2] = {};
    
    rt_uint8_t send_buf[1] = {MCP9808_MODE_TA};
    
    float temperature = 0.0;
    
    while (1)
    {
        rt_memset(recv_buf, 0x00, 2);
        
        /* 发送要读的寄存器的地址 */
        rt_i2c_master_send(i2c_bus, MCP9808_ADDR, RT_I2C_WR, send_buf, 1);
        
        /* 读寄存器 */
        rt_i2c_master_recv(i2c_bus, MCP9808_ADDR, RT_I2C_RD, recv_buf, 2);
        
        /* 高字节高位3bit为标志位 */
        recv_buf[0] = recv_buf[0] & 0x1F;       //Clear flag bits
        
        /* 高字节 bit4 为符号位 */
        if ((recv_buf[0] & 0x10) == 0x10)         //TA < 0°C
        { 
            recv_buf[0] = recv_buf[0] & 0x0F;   //Clear SIGN
            temperature = 256.0 - (recv_buf[0] * 16.0 + recv_buf[1] / 16.0);
        }
        else    //TA ³ 0°C
        {
            temperature = (recv_buf[0] * 16.0 + recv_buf[1] / 16.0);
        }
        
        status.sensor.temp = temperature*10;
        
        rt_thread_mdelay(1000);
    }
}


/*------------------------------  线程启动  -----------------------------*/

int temperature_start(void)
{
    i2c_bus = (struct rt_i2c_bus_device *)rt_device_find("i2c3");

    if (i2c_bus == RT_NULL)
    {
        rt_kprintf("can't find %s device!\n", "i2c3");
    }
    
    temperature1 = rt_thread_create("temp1",
                          temperature1_thread, 
                          RT_NULL,
                          TEMP1_STACK_SIZE,
                          TEMP1_PRIORITY, 
                          TEMP1_TIMESLICE); 
    if (temperature1 != RT_NULL)
        rt_thread_startup(temperature1);
    return 0;
}

INIT_APP_EXPORT(temperature_start);

/*------------------------------------ 一些内部函数 ---------------------------------------*/




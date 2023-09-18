#include "sensor/iic_distance.h"

#include "status.h"


static struct rt_i2c_bus_device *i2c_bus = RT_NULL;     /* I2C总线设备句柄 */



static rt_err_t write_reg(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t *data);

static rt_err_t read_reg(struct rt_i2c_bus_device *bus, rt_uint8_t len, rt_uint8_t *buf);

/*---------------------------- 传感器查询线程 -------------------------------*/

static rt_thread_t temperature1 = RT_NULL;

static void temperature1_thread(void *parameter)
{
    rt_uint8_t recive_buf[9] = {};
    
    rt_uint16_t distance = 0;

    while (1)
    {
        write_reg(i2c_bus, 0x05, RT_NULL);          /* 发送命令 */
        read_reg(i2c_bus, 9, recive_buf);           /* 获取传感器数据 */

        
        for (int i=0; i<9; i++)
        {
            rt_kprintf("%02x ", recive_buf[i]);
        
        }
        
        rt_kprintf("\n");
        
        
        status.sensor.distance1 = recive_buf[3] << 8 | recive_buf[2];
        
        rt_kprintf("%d\n", status.sensor.distance1);
        
//        
        

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

/* 写传感器寄存器 */
static rt_err_t write_reg(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t *data)
{
    rt_uint8_t buf[5] = DISTANCE_FETCH_CMD;
    
    struct rt_i2c_msg msgs;
    
    msgs.addr   = DISTANCE1_ADDR;
    msgs.flags  = RT_I2C_WR;
    msgs.buf    = buf;
    msgs.len    = 5;

    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(bus, &msgs, 1) == 1)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

/* 读传感器寄存器数据 */
static rt_err_t read_reg(struct rt_i2c_bus_device *bus, rt_uint8_t len, rt_uint8_t *buf)
{
    struct rt_i2c_msg msgs;

    msgs.addr   = DISTANCE1_ADDR;           /* 从机地址 */
    msgs.flags  = RT_I2C_RD;                /* 读标志 */
    msgs.buf    = buf;                      /* 读写数据缓冲区指针　*/
    msgs.len    = len;                      /* 读写数据字节数 */

    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(bus, &msgs, 1) == 1)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}




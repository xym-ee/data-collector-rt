#include "sensor/iic_distance.h"

#include "status.h"


static struct rt_i2c_bus_device *i2c_bus = RT_NULL;     /* I2C总线设备句柄 */


/*---------------------------- 传感器查询线程 -------------------------------*/

static rt_thread_t distance = RT_NULL;

static void distance_thread(void *parameter)
{
    rt_uint8_t recv_buf[9] = {};
        
    rt_uint8_t send_buf[5] = DISTANCE_FETCH_CMD;
    
    while (1)
    {
        /*----------------------------------------------------------------------------*/
        rt_memset(recv_buf, 0x00, 9);
        
        rt_i2c_master_send(i2c_bus, DISTANCE_SENSOR1_ADDR, RT_I2C_WR, send_buf, 5);
        
        rt_i2c_master_recv(i2c_bus, DISTANCE_SENSOR1_ADDR, RT_I2C_RD, recv_buf, 9);
        
        status.sensor.distance1 = recv_buf[3] << 8 | recv_buf[2];
        
        
        /*----------------------------------------------------------------------------*/
        rt_memset(recv_buf, 0x00, 9);
        
        rt_i2c_master_send(i2c_bus, DISTANCE_SENSOR2_ADDR, RT_I2C_WR, send_buf, 5);
        
        rt_i2c_master_recv(i2c_bus, DISTANCE_SENSOR2_ADDR, RT_I2C_RD, recv_buf, 9);
        
        status.sensor.distance2 = recv_buf[3] << 8 | recv_buf[2];        
        
        
        
        /*----------------------------------------------------------------------------*/
        rt_memset(recv_buf, 0x00, 9);
        
        rt_i2c_master_send(i2c_bus, DISTANCE_SENSOR3_ADDR, RT_I2C_WR, send_buf, 5);
        
        rt_i2c_master_recv(i2c_bus, DISTANCE_SENSOR3_ADDR, RT_I2C_RD, recv_buf, 9);
        
        status.sensor.distance3 = recv_buf[3] << 8 | recv_buf[2];          
        
        
        
        rt_thread_mdelay(1000);
    }
}


/*------------------------------  线程启动  -----------------------------*/

int distance_start(void)
{
    i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(DISTANCE_I2C_BUS_NAME);

    if (i2c_bus == RT_NULL)
    {
        rt_kprintf("can't find %s device!\n", DISTANCE_I2C_BUS_NAME);
    }    
    
    distance = rt_thread_create("distance",
                          distance_thread, 
                          RT_NULL,
                          DISTANCE_STACK_SIZE,
                          DISTANCE_PRIORITY, 
                          DISTANCE_TIMESLICE); 
    if (distance != RT_NULL)
        rt_thread_startup(distance);
    return 0;
}

INIT_APP_EXPORT(distance_start);

/*------------------------------------ 一些内部函数 ---------------------------------------*/



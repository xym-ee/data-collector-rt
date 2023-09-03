#include "power.h"
#include "status.h"




/*--------------------------  电源相关处理线程  ---------------------------*/
//电压检测与通断控制。

static void power_thread_entry(void *parameter)
{
    /*----------------------- 电源开关引脚配置 ------------------*/
    rt_pin_mode(OUT_24V_EN_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(PC__12V_EN_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(CAM_12V_EN_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(SER_5V_EN1_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(SER_5V_EN2_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(SER_5V_EN3_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(SER_5V_EN4_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(SER_5V_EN5_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(IIC_5V_EN1_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(IIC_5V_EN2_PIN, PIN_MODE_OUTPUT);

    
    rt_pin_write(OUT_24V_EN_PIN, PIN_LOW);
    rt_pin_write(PC__12V_EN_PIN, PIN_LOW);
    rt_pin_write(CAM_12V_EN_PIN, PIN_LOW);
    rt_pin_write(SER_5V_EN1_PIN, PIN_LOW);
    rt_pin_write(SER_5V_EN2_PIN, PIN_LOW);
    rt_pin_write(SER_5V_EN3_PIN, PIN_LOW);
    rt_pin_write(SER_5V_EN4_PIN, PIN_LOW);
    rt_pin_write(SER_5V_EN5_PIN, PIN_LOW);
    rt_pin_write(IIC_5V_EN1_PIN, PIN_LOW);
    rt_pin_write(IIC_5V_EN2_PIN, PIN_LOW);

    
    /*---------------------------- 电压检测硬件配置 ---------*/
    rt_adc_device_t adc_dev;

    /* 查找设备 */
    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
    if (adc_dev == RT_NULL)
    {
        rt_kprintf("adc sample run failed! can't find %s device!\n", ADC_DEV_NAME);
    }
    
    /* 使能设备 */
    rt_adc_enable(adc_dev, CHANNEL_OUT_24V);
    rt_adc_enable(adc_dev, CHANNEL_LED_18V);
    rt_adc_enable(adc_dev, CHANNEL_CAM_12V);
    rt_adc_enable(adc_dev, CHANNEL_PC_12V);
    rt_adc_enable(adc_dev, CHANNEL_OUT_5V);

    
    while (1)
    {
        /*------------------------------- 电源输出控制 ---------------------------------*/

        if (status.power.out_24v_en == POWER_ON)
            rt_pin_write(OUT_24V_EN_PIN, PIN_HIGH);
        else
            rt_pin_write(OUT_24V_EN_PIN, PIN_LOW);
            
        /* 12V */
        
        if (status.power.pc_12v_en == POWER_ON)
            rt_pin_write(PC__12V_EN_PIN, PIN_HIGH);
        else
            rt_pin_write(PC__12V_EN_PIN, PIN_LOW);
        
        if (status.power.cam_12v_en == POWER_ON)
            rt_pin_write(CAM_12V_EN_PIN, PIN_HIGH);
        else
            rt_pin_write(CAM_12V_EN_PIN, PIN_LOW);
           
        /* SER_5V */
        
        if (status.power.ser_5v_en1 == POWER_ON)
            rt_pin_write(SER_5V_EN1_PIN, PIN_HIGH);
        else
            rt_pin_write(SER_5V_EN1_PIN, PIN_LOW);
        
        if (status.power.ser_5v_en2 == POWER_ON)
            rt_pin_write(SER_5V_EN2_PIN, PIN_HIGH);
        else
            rt_pin_write(SER_5V_EN2_PIN, PIN_LOW);        
        
         if (status.power.ser_5v_en3 == POWER_ON)
            rt_pin_write(SER_5V_EN3_PIN, PIN_HIGH);
        else
            rt_pin_write(SER_5V_EN3_PIN, PIN_LOW);       
        
        if (status.power.ser_5v_en4 == POWER_ON)
            rt_pin_write(SER_5V_EN4_PIN, PIN_HIGH);
        else
            rt_pin_write(SER_5V_EN4_PIN, PIN_LOW);        
        
        if (status.power.ser_5v_en5 == POWER_ON)
            rt_pin_write(SER_5V_EN5_PIN, PIN_HIGH);
        else
            rt_pin_write(SER_5V_EN5_PIN, PIN_LOW);        
            
        /* IIC_5V */
        
        if (status.power.iic_5v_en1 == POWER_ON)
            rt_pin_write(IIC_5V_EN1_PIN, PIN_HIGH);
        else
            rt_pin_write(IIC_5V_EN1_PIN, PIN_LOW);

        if (status.power.iic_5v_en2 == POWER_ON)
            rt_pin_write(IIC_5V_EN2_PIN, PIN_HIGH);
        else
            rt_pin_write(IIC_5V_EN2_PIN, PIN_LOW);

        
        /*------------------------------- 电压检测 ---------------------------------*/

        /* 读取采样值 */
        status.power.voltage_24v = rt_adc_read(adc_dev, CHANNEL_OUT_24V) * K_OUT_24V;
        status.power.voltage_led_18v = rt_adc_read(adc_dev, CHANNEL_LED_18V) * K_LED_18V;
        status.power.voltage_cam_12v = rt_adc_read(adc_dev, CHANNEL_CAM_12V) * K_CAM_12V;
        status.power.voltage_pc_12v = rt_adc_read(adc_dev, CHANNEL_PC_12V) * K_PC_12V;
        status.power.voltage_5v = rt_adc_read(adc_dev, CHANNEL_OUT_5V) * K_OUT_5V;

        
//        rt_kprintf("24v : %d \n", status.power.voltage_24v);
//        rt_kprintf("18v : %d \n", status.power.voltage_led_18v);
//        rt_kprintf("12v : %d \n", status.power.voltage_cam_12v);
//        rt_kprintf("12v : %d \n", status.power.voltage_pc_12v);
//        rt_kprintf("5v : %d \n", status.power.voltage_5v);


        rt_thread_mdelay(2000);
    }
}

int power_init(void)
{
    rt_err_t ret = RT_EOK;

    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("power", 
                                        power_thread_entry, 
                                        RT_NULL, 
                                        POWER_THREAD_STACK_SIZE, 
                                        POWER_THREAD_PRIORITY, 
										POWER_THREAD_TIMESLICE);
    
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
INIT_APP_EXPORT(power_init);




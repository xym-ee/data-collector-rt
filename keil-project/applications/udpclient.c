#include "udpclient.h"

#include <rtthread.h>
#include <sys/socket.h> /* 使用BSD socket，需要包含sockets.h头文件 */
#include <netdb.h>
#include <string.h>
#include <finsh.h>

#include "status.h"


/*--------------------------  数据处理线程  ---------------------------*/


static void udpclient_thread_entry(void *parameter)
{
    rt_thread_mdelay(4000);
    
    rt_uint8_t send_data[20] = {0x55, 0x55};
    
    int sock;
    struct hostent *host;
    struct sockaddr_in server_addr;

    host = (struct hostent *) gethostbyname(UDP_SERVER_IP);

    /* 创建一个socket，类型是SOCK_DGRAM，UDP类型 */
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        rt_kprintf("Socket error\n");
        return;
    }
    
    /* 初始化预连接的服务端地址 */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
        
    /* 总计发送count次数据 */
    while (1)
    {        
        /*----------------------------------  电压信息上报 --------------------------------*/
        send_data[2] = SMSG_ID_VOLTAGE;     /* 发送消息ID   */
        send_data[3] = 8;                   /* 发送数据长度 */
        
        send_data[4] = status.power.voltage_24v;                    /* 24V */
        send_data[5] = status.power.voltage_led_18v;                /* 18V */
        send_data[6] = status.power.voltage_pc_12v;                /* CAM12V */
        send_data[7] = status.power.voltage_cam_12v;                /* PC12V */
        send_data[8] = status.power.voltage_5v;                     /* 5V */
        send_data[9] = 0;                   
        send_data[10] = 0;
        send_data[11] = 0;
        
        send_data[12] = sum_check(send_data, 12);
        
        sendto(sock, send_data, 13, 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

        
        /*----------------------------------  电源与灯光开启状态上报 --------------------------------*/
        send_data[2] = SMSG_ID_OPEN_STATUS;     /* 发送消息ID   */
        send_data[3] = 8;                       /* 发送数据长度 */
        
        send_data[4] = status.power.out_24v_en;                     
        send_data[5] = (status.power.pc_12v_en<<1) | status.power.cam_12v_en;
        send_data[6] = (status.power.iic_5v_en2<<6) | 
                       (status.power.iic_5v_en1<<5) |
                       (status.power.ser_5v_en5<<4) |
                       (status.power.ser_5v_en4<<3) |
                       (status.power.ser_5v_en3<<2) |
                       (status.power.ser_5v_en2<<1) |
                       (status.power.ser_5v_en1<<0) ;
        send_data[7] = (status.light.light7<<7) | 
                       (status.light.light6<<6) | 
                       (status.light.light5<<5) | 
                       (status.light.light4<<4) | 
                       (status.light.light3<<3) | 
                       (status.light.light2<<2) | 
                       (status.light.light1<<1) | 
                       (status.light.light0<<0) ; 
        send_data[8] = 0;                    
        send_data[9] = 0;                   
        send_data[10] = 0;
        send_data[11] = 0;
        
        send_data[12] = sum_check(send_data, 12);
        
        sendto(sock, send_data, 13, 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
        
        
        /*----------------------------------  温度上报 --------------------------------*/
        send_data[2] = SMSG_ID_TEMP;            /* 发送消息ID   */
        send_data[3] = 8;                       /* 发送数据长度 */
        
        send_data[4] = status.sensor.temp & 0xff;                     
        send_data[5] = (status.sensor.temp>>8)& 0xff;
        send_data[6] = 0;
        send_data[7] = 0; 
        send_data[8] = 0;                    
        send_data[9] = 0;                   
        send_data[10] = 0;
        send_data[11] = 0;
        
        send_data[12] = sum_check(send_data, 12);
        
        sendto(sock, send_data, 13, 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));        
        
        
        /*----------------------------------  气体上报 --------------------------------*/
        send_data[2] = SMSG_ID_GAS;             /* 发送消息ID   */
        send_data[3] = 8;                       /* 发送数据长度 */
        
        send_data[4] = status.sensor.gas1 & 0xff;                     
        send_data[5] = (status.sensor.gas1>>8)& 0xff;
        send_data[6] = status.sensor.gas2 & 0xff;
        send_data[7] = (status.sensor.gas2>>8)& 0xff; 
        send_data[8] = 0;                    
        send_data[9] = 0;                   
        send_data[10] = 0;
        send_data[11] = 0;
        
        send_data[12] = sum_check(send_data, 12);
        
        sendto(sock, send_data, 13, 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));


        /*----------------------------------  测距上报 --------------------------------*/
        send_data[2] = SMSG_ID_DISTANCE;        /* 发送消息ID   */
        send_data[3] = 8;                       /* 发送数据长度 */
        
        send_data[4] = status.sensor.distance1 & 0xff;                     
        send_data[5] = (status.sensor.distance1>>8)& 0xff;
        send_data[6] = status.sensor.distance2 & 0xff;
        send_data[7] = (status.sensor.distance2>>8)& 0xff; 
        send_data[8] = status.sensor.distance3 & 0xff;                    
        send_data[9] = (status.sensor.distance3>>8)& 0xff;                   
        send_data[10] = 0;
        send_data[11] = 0;
        
        send_data[12] = sum_check(send_data, 12);
        
        sendto(sock, send_data, 13, 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));



        rt_thread_mdelay(1000);
    }
    
    /* 关闭这个socket */
    //closesocket(sock);    
}

int udpclient_init(void)
{
    rt_err_t ret = RT_EOK;

    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("client", 
                                        udpclient_thread_entry, 
                                        RT_NULL, 
                                        UDPCLIENT_THREAD_STACK_SIZE, 
                                        UDPCLIENT_THREAD_PRIORITY, 
										UDPCLIENT_THREAD_TIMESLICE);
    
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
INIT_APP_EXPORT(udpclient_init);



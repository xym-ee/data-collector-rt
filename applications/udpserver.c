#include "udpserver.h"

#include <rtthread.h>
#include <sys/socket.h> /* 使用BSD socket，需要包含socket.h头文件 */
#include <netdb.h>
#include <string.h>
#include <finsh.h>


#include "status.h"



static void rmsg_power_handle(rt_uint8_t *data);

static void rmsg_lighr_handle(rt_uint8_t *data);

#define BUFSZ 128


static void udpserver_thread_entry(void *parameter)
{
    
    rt_thread_mdelay(4000);
    
    
    int sock;
    int bytes_read;
    char *recv_data;
    socklen_t addr_len;
    struct sockaddr_in server_addr, client_addr;
    
    /* 分配接收用的数据缓冲 */
    recv_data = rt_malloc(BUFSZ);
    
    if (recv_data == RT_NULL)
    {
        /* 分配内存失败，返回 */
        rt_kprintf("No memory\n");
        return;
    }
    
    /* 创建一个socket，类型是SOCK_DGRAM，UDP类型 */
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        rt_kprintf("Socket error\n");
        /* 释放接收用的数据缓冲 */
        rt_free(recv_data);
        return;
    }
    
    /* 初始化服务端地址 */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_RECIVE_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
    
    /* 绑定socket到服务端地址 */
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        /* 绑定地址失败 */
        rt_kprintf("Bind error\n");
        /* 释放接收用的数据缓冲 */
        rt_free(recv_data);
        return;
    }
    
    addr_len = sizeof(struct sockaddr);
    rt_kprintf("UDPServer Waiting for client on port 5501...\n");
    
    rt_uint8_t msg_length = 0;
    
    
    while (1)
    {
        /* 从sock中收取最大BUFSZ - 1字节数据 */
        bytes_read = recvfrom(sock, recv_data, BUFSZ - 1, 0, (struct sockaddr *)&client_addr, &addr_len);
        
        
        /*------------------------------- 帧头判断 ------------------------------------*/
        if ( (recv_data[0] == 0x55) && (recv_data[1]==0x55) )
        {
            msg_length = recv_data[3];  /* byte3为数据长度 */
            
            
            
            /*------------------------------- 求和校验 ------------------------------------*/
            rt_uint8_t sum = sum_check((rt_uint8_t*)recv_data, 4 + msg_length);
            
            if (recv_data[4 + msg_length] == sum)
            {
                switch(recv_data[2])
                {
                    /* 传入第一个数据的起始地址 */
                    case RMSG_ID_POWER : rmsg_power_handle((rt_uint8_t*)(&recv_data[4])); break;
                    case RMSG_ID_LIGHT : rmsg_lighr_handle((rt_uint8_t*)(&recv_data[5])); break;
                    default : ; break;
                }
            }
            else
            {
                rt_kprintf("sum error~\n");            
            
            }
        
        }
        else
        {
            rt_kprintf("head error~\n");
        }
        
        
        
        
//        /* UDP不同于TCP，它基本不会出现收取的数据失败的情况，除非设置了超时等待 */
//        recv_data[bytes_read] = '\0'; /* 把末端清零 */
//        

        rt_kprintf("recive: ");
        for (int i=0; i<bytes_read; i++)
            rt_kprintf("%02x ", recv_data[i]);
        rt_kprintf("\n");
//        
//        
//        /* 调试用，输出接收的数据 */
//        rt_kprintf("\n(%s:%d) said : ", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
//        rt_kprintf("%s", recv_data);
        
    }
    
//    closesocket(sock);
//    rt_free(recv_data);
}


int udpserver_init(void)
{
    rt_err_t ret = RT_EOK;

    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("server", 
                                        udpserver_thread_entry, 
                                        RT_NULL, 
                                        UDPSERVER_THREAD_STACK_SIZE, 
                                        UDPSERVER_THREAD_PRIORITY, 
										UDPSERVER_THREAD_TIMESLICE);
    
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
INIT_APP_EXPORT(udpserver_init);


/*-----------------------------------------  控制消息处理  ----------------------------------------------*/

static void rmsg_power_handle(rt_uint8_t *data)
{
    
    rt_kprintf("handle: ");
    for (int i=0; i<4; i++)
        rt_kprintf("%02x ", data[i]);
    rt_kprintf("\n");    
    
    
    /*--------------------- 24V 状态更新 ----------------------*/
    switch (data[0]) /* byte1 */
    {
        case 0x00 : status.power.out_24v_en = POWER_OFF; break;
        case 0x01 : status.power.out_24v_en = POWER_ON; break;
        default : ; break;
    }
    
    /*--------------------- 12V 状态更新 ----------------------*/
    switch(data[1]) /* byte2 */
    {
        case 0x00 : /*0b00*/ rt_kprintf("12v case 00\n");
            status.power.pc_12v_en = POWER_OFF;
            status.power.cam_12v_en = POWER_OFF;
            break;
        
        case 0x01 : /*0b01*/rt_kprintf("12v case 01\n");
            status.power.pc_12v_en = POWER_OFF;
            status.power.cam_12v_en = POWER_ON;
            break;
        
        case 0x02 : /*0b10*/rt_kprintf("12v case 02\n");
            status.power.pc_12v_en = POWER_ON;
            status.power.cam_12v_en = POWER_OFF;
            break;
        
        case 0x03 : /*0b11*/rt_kprintf("12v case 03\n");
            status.power.pc_12v_en = POWER_ON;
            status.power.cam_12v_en = POWER_ON;
            break;
        
        default: ; break;
    }
    
    /*--------------------- 5V 状态更新 ----------------------*/
    for (int i=0; i<8; i++)
    {
        if (((data[3] >> i)&0x01) == 1)
            *(&status.power.ser_5v_en1 + i) = POWER_ON;
        else
            *(&status.power.ser_5v_en1 + i) = POWER_OFF;
    }
}


static void rmsg_lighr_handle(rt_uint8_t *data)
{
    /* 亮度状态 */
    switch (data[0])
    {
        case 0 : status.light.level = LIGHT_17V;        break;
        case 1 : status.light.level = LIGHT_17_5V;      break;
        case 2 : status.light.level = LIGHT_18V;        break;
        case 3 : status.light.level = LIGHT_18_5V;      break;
        default: ;break;
    }
    
    /* 开关状态 */
    for (int i=0; i<8; i++)
    {
        if (((data[1] >> i)&0x01) == 1)
            *(&status.light.light0 + i) = LIGHT_ON;
        else
            *(&status.light.light0 + i) = LIGHT_OFF;
    }    
    
}

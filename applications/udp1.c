#include "udp1.h"

#include <rtthread.h>
#include <sys/socket.h> /* 使用BSD socket，需要包含sockets.h头文件 */
#include <netdb.h>
#include <string.h>
#include <finsh.h>

#include "status.h"



int sock;
struct hostent *host;

#define BUFSZ 128

socklen_t addr_len;
struct sockaddr_in server_addr, client_addr;
   

static void udp_recive_thread_entry(void *parameter);


/*--------------------------  数据接收线程  ---------------------------*/
static void udp_recv_thread_entry(void *parameter)
{
    rt_thread_mdelay(4000);
    
    int bytes_read;
    char *recv_data;
    
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
    server_addr.sin_port = htons(UDP_SERVER_PORT);
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
    rt_kprintf("UDPServer Waiting for client on port 5500...\n");
    
    rt_uint8_t msg_length = 0;
    
    
    while (1)
    {
        /* 从sock中收取最大BUFSZ - 1字节数据 */
        bytes_read = recvfrom(sock, recv_data, BUFSZ - 1, 0, (struct sockaddr *)&client_addr, &addr_len);
        
        rt_kprintf("recive: ");
        for (int i=0; i<bytes_read; i++)
            rt_kprintf("%02x ", recv_data[i]);
        rt_kprintf("\n");

    }
}

/*--------------------------  数据发送线程  ---------------------------*/

static void udp_send_thread_entry(void *parameter)
{
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
        
    while (1)
    {
        
        /*----------------------------------  电压信息上报 --------------------------------*/
        send_data[12] = sum_check(send_data, 12);
        
        sendto(sock, send_data, 13, 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
        
        rt_thread_mdelay(1000);
    }
    
    /* 关闭这个socket */
    //closesocket(sock);    
}

static void udp_send_thread_entry(void *parameter)
{
    rt_thread_mdelay(4000);
    
    rt_uint8_t send_data[20] = {0x55, 0x55};
    
    rt_thread_t thread = rt_thread_create("udp_recv",
                                udp_recive_thread_entry, 
                                RT_NULL, 
                                UDP_THREAD_STACK_SIZE, 
                                UDP_THREAD_PRIORITY, 
                                UDP_THREAD_TIMESLICE);
    
    /* 创建成功则启动线程 */
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    
    /* 总计发送count次数据 */
    while (1)
    {
        /*----------------------------------  电压信息上报 --------------------------------*/

        send_data[12] = sum_check(send_data, 12);
        
        sendto(sock, send_data, 13, 0, (struct sockaddr *)&udp_addr, sizeof(struct sockaddr));        
        
        rt_thread_mdelay(1000);
    }
    
}


/*--------------------------  数据处理线程  ---------------------------*/

#define     BUFSZ       128

static void udp_recive_thread_entry(void *parameter)
{
    //rt_thread_mdelay(4000);
    
    int bytes_read;
    socklen_t addr_len;
    
    char *recv_data;
    
    /* 分配接收用的数据缓冲 */
    recv_data = rt_malloc(BUFSZ);
    
    if (recv_data == RT_NULL)
    {
        /* 分配内存失败，返回 */
        rt_kprintf("No memory\n");
        return;
    }
    
    addr_len = sizeof(struct sockaddr);
    
    while (1)
    {
        /* 从sock中收取最大BUFSZ - 1字节数据 */
        bytes_read = recvfrom(sock, recv_data, BUFSZ - 1, 0, (struct sockaddr *)&udp_addr, &addr_len);
        
        rt_kprintf("recive: ");
        for (int i=0; i<bytes_read; i++)
            rt_kprintf("%02x ", recv_data[i]);
        rt_kprintf("\n");
    }
    /* 关闭这个socket */
    //closesocket(sock);    
}


/*--------------------------  任务自动启动  ---------------------------*/

int udp_init(void)
{
    rt_err_t ret = RT_EOK;
    
    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("udp_send",
                                        udp_send_thread_entry, 
                                        RT_NULL, 
                                        UDP_THREAD_STACK_SIZE, 
                                        UDP_THREAD_PRIORITY, 
										UDP_THREAD_TIMESLICE);
    
    /* 创建成功则启动线程 */
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    
    return ret;
}

/* 导出命令 or 自动初始化 */
INIT_APP_EXPORT(udp_init);





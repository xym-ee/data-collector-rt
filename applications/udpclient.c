#include "udpclient.h"



#include <rtthread.h>
#include <sys/socket.h> /* 使用BSD socket，需要包含sockets.h头文件 */
#include <netdb.h>
#include <string.h>
#include <finsh.h>




/*--------------------------  数据处理线程  ---------------------------*/


static void udpclient_thread_entry(void *parameter)
{
    rt_thread_mdelay(4000);
    
    rt_uint8_t send_data[20] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
    
    int sock, port;
    struct hostent *host;
    struct sockaddr_in server_addr;

    port = UDP_SERVER_PORT;
    host = (struct hostent *) gethostbyname(UDP_SERVER_IP);

    /* 创建一个socket，类型是SOCK_DGRAM，UDP类型 */
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        rt_kprintf("Socket error\n");
        return;
    }
    
    /* 初始化预连接的服务端地址 */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
        
    /* 总计发送count次数据 */
    while (1)
    {
        
        
        
        
        
        /* 发送数据到服务远端 */
        sendto(sock, send_data, 12, 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

        rt_thread_mdelay(1000);
    }
    
    /* 关闭这个socket */
    closesocket(sock);    
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



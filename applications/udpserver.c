#include "udpserver.h"

#include <rtthread.h>
#include <sys/socket.h> /* 使用BSD socket，需要包含socket.h头文件 */
#include <netdb.h>
#include <string.h>
#include <finsh.h>



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
    
    while (1)
    {
        
        
        
        
        /* 从sock中收取最大BUFSZ - 1字节数据 */
        bytes_read = recvfrom(sock, recv_data, BUFSZ - 1, 0, (struct sockaddr *)&client_addr, &addr_len);
        
        /* UDP不同于TCP，它基本不会出现收取的数据失败的情况，除非设置了超时等待 */
        recv_data[bytes_read] = '\0'; /* 把末端清零 */
        
        
        
        /* 调试用，输出接收的数据 */
        rt_kprintf("\n(%s:%d) said : ", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        rt_kprintf("%s", recv_data);
        
    }
    
    closesocket(sock);
    rt_free(recv_data);
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


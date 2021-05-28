#include <sys/types.h>
#include <sys/syscall.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <curl/curl.h>  
#include <string.h>  
#include <ctype.h>  
#include <iconv.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>

#include "log.h"


//最大连接数
#define MAX_CONN  1000


//内核已经忽略  随便写
#define MAX_PRE_EVENTS  5

typedef struct 
{
	unsigned char TX_ORDER_NO[32+1];
	unsigned char MOBILE_NO[32+1];
	unsigned char MERCHANT_ID[64+1];
	unsigned char MERCHANT_NAME[64+1];
	unsigned char ORDER_SUBJECT[64+1];
	unsigned char TRANS_AMOUNT[32+1];
	unsigned char ORDER_STATUS[2+1];
	unsigned char ORDER_STATUS_DESC[64+1];
	unsigned char TX_SUB_ORDERID[32+1];
	unsigned char MSG_CODE[32+1];
	unsigned char MSG_TXT[256+1];	
}TMP_STRUCT;


/* 定义常量 */
#define HTTP_DEF_PORT      9876     /* 连接的缺省端口 */
#define HTTP_BUF_SIZE      1024     /* 缓冲区的大小 */
#define HTTP_FILENAME_LEN   256     /* 文件名长度 */
 
/* 定义文件类型对应的 Content-Type */
struct doc_type
{
    char *suffix; /* 文件后缀 */
    char *type;   /* Content-Type */
};
 
struct doc_type file_type[] =
{
    {"html",    "text/html"  },
    {"gif",     "image/gif"  },
    {"jpeg",    "image/jpeg" },
    { NULL,      NULL        }
};
 
char *http_res_hdr_tmpl = "HTTP/1.1 200 OK\r\nServer: Huiyong's Server <0.1>\r\n"
    "Accept-Ranges: bytes\r\nContent-Length: %d\r\nConnection: close\r\n"
    "Content-Type: %s\r\n\r\n";
 
 
/**************************************************************************
 *
 * 函数功能: 根据文件后缀查找对应的 Content-Type.
 *
 * 参数说明: [IN] suffix, 文件名后缀;
 *
 * 返 回 值: 成功返回文件对应的 Content-Type, 失败返回 NULL.
 *
 **************************************************************************/
char *http_get_type_by_suffix(const char *suffix)
{
    struct doc_type *type;
 
    for (type = file_type; type->suffix; type++)
    {
        if (strcmp(type->suffix, suffix) == 0)
            return type->type;
    }
 
    return NULL;
}
 
/**************************************************************************
 *
 * 函数功能: 解析请求行, 得到文件名及其后缀. 请求行格式:
 *           [GET http://www.baidu.com:8080/index.html HTTP/1.1]
 *
 * 参数说明: [IN]  buf, 字符串指针数组;
 *           [IN]  buflen, buf 的长度;
 *           [OUT] file_name, 文件名;
 *           [OUT] suffix, 文件名后缀;
 *
 * 返 回 值: void.
 *
 **************************************************************************/
void http_parse_request_cmd(char *buf, int buflen, char *file_name, char *suffix)
{
    int length = 0;
    char *begin, *end, *bias;
 
    /* 查找 URL 的开始位置 */
    begin = strchr(buf, ' ');
    begin += 1;
         
    /* 查找 URL 的结束位置 */
    end = strchr(begin, ' ');
    *end = 0;
 
    bias = strrchr(begin, '/');
    length = end - bias;
 
    /* 找到文件名的开始位置 */
    if ((*bias == '/') || (*bias == '\\'))
    {
        bias++;
        length--;
    }
 
    /* 得到文件名 */
    if (length > 0)
    {
        memcpy(file_name, bias, length);
        file_name[length] = 0;
 
        begin = strchr(file_name, '.');
        if (begin)
            strcpy(suffix, begin + 1);
    }
}
 
 
/**************************************************************************
 *
 * 函数功能: 向客户端发送 HTTP 响应.
 *
 * 参数说明: [IN]  buf, 字符串指针数组;
 *           [IN]  buf_len, buf 的长度;
 *
 * 返 回 值: 成功返回非0, 失败返回0.
 *
 **************************************************************************/
int http_send_response(int soc, unsigned char *buf, int buf_len)
{
    int hdr_len, send_len;
    char http_header[HTTP_BUF_SIZE];


 /*	POST / HTTP/1.1
	Content-Type: text/plain
	cache-control: no-cache
	Postman-Token: 7b7ed73b-9b7d-4382-a773-ee1a81e2e5c2
	User-Agent: PostmanRuntime/7.6.1

	Host: 192.168.10.202:9876
	accept-encoding: gzip, deflate
	content-length: 416
	Connection: keep-alive


	Content-Type →text/html;charset=UTF-8
	Content-Language →zh-CN
	Transfer-Encoding →chunked
	Date →Mon, 13 May 2019 08:57:35 GMT

	
	HTTP/1.1 200 OK\r\nServer: Huiyong's Server <0.1>\r\n"
		"Accept-Ranges: bytes\r\nContent-Length: %d\r\nConnection: close\r\n"
		"Content-Type: %s\r\n\r\n"

*/
	hdr_len = snprintf(http_header,sizeof(http_header)-1,
	"HTTP/1.1 200 OK\r\n"
	"Server: xxxx's Server <0.1>\r\n"
	"Content-Type: text/plain\n"
	"Content-Length: %d\r\n"
	"\r\n"
	"%s",
	buf_len,
	buf
	);

 	printf("发送报文===[%s]\n", http_header);
	send_len = send(soc, http_header, hdr_len, 0);
	if(send_len < 0)
	{
		close(soc);
		printf("[Web] Fail to send, error\n");
		return 0;
	}
	printf("send_len===[%d]\n", send_len);

	sleep(1);
	close(soc);
	
//Content-Type →text/html;charset=UTF-8
 
    // 构造 HTTP 首部，并发送
 /*   hdr_len = sprintf(http_header, http_res_hdr_tmpl, buf_len, "text/plain");
    send_len = send(soc, http_header, hdr_len, 0);
    //send_len=1;
    if (send_len < 0)
    {
        close(soc);
        printf("[Web] Fail to send, error\n");
        return 0;
    }
 printf("[Web]  HTTP 头  发送 成功....buf==[%s], buf_len==[%d]\n", buf, buf_len);
	do // 发送文件, HTTP 的消息体 
	{
	
		send_len = send(soc, buf, buf_len, 0);
		
		buf_len -= send_len;
		buf += send_len;
printf("[Web] buf==[%s], buf_len==[%d], send_len===[%d]\n", buf, buf_len, send_len);		
	}while(buf_len > 0);
 */
    close(soc);
     
    return 0;
}
 
int set_fd_nonb(int fd)
{
	int	flags;

	if ((flags = fcntl(fd, F_GETFL, 0)) < 0)
		return -1;

	if ( fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		return -1;

	return 0;
}

int set_svr_opt(int sockfd)
{	
	int keepAlive=0, keepIdle=0, keepInterval=0, keepCount=0, n=0, on=1, ru=1;
	struct linger lg;	  

	lg.l_onoff = 1;		
	lg.l_linger = 0;		

	keepAlive = 1;
	keepIdle = 1000;
	keepInterval = 10;
	keepCount = 10;

	n=1;

	if(setsockopt(sockfd, IPPROTO_TCP, SO_KEEPALIVE, (void*)&keepAlive, sizeof(keepAlive))) 
		return -1;
	if(setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle))) 
		return -1;
	if(setsockopt(sockfd, IPPROTO_TCP,TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval))) 
		return -1;
	if(setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount))) 
		return -1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) 
		return -1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &ru, sizeof(ru))) 
		return -1;

	if(setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &lg, sizeof(lg))) 
		return -1;

	if(setsockopt(sockfd, SOL_SOCKET, SO_RCVLOWAT, (char *)(&n), sizeof(n))) 
		return -1;
	
	return 0;
}
 


int initSerSock(int *lis_fd)
{
    int listenfd, epollfd;
    struct sockaddr_in servaddr;
	struct epoll_event event;

    while(1)
    {
        listenfd = socket(AF_INET, SOCK_STREAM, 0);
        if (listenfd < 0) 
        {
            showmsg_pth(ERROR,"socket failed..... errno:%d-%s...\n", errno, strerror(errno));
            continue;
        }
        break;
    }
 
    if(set_svr_opt(listenfd)) 
    {
        showmsg_pth(ERROR," set_ver_opt failed...... errno:%d-%s...\n", errno, strerror(errno));
        close(listenfd);
        return -1;
    }

   /* if(set_fd_nonb(listenfd))
    {
        showmsg_pth(ERROR," set_fd_nonb failed...... errno:%d-%s...\n", errno, strerror(errno));    
        close(listenfd);
        return -1;
    }*/
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(HTTP_DEF_PORT);
    servaddr.sin_family = AF_INET;
    
    if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)))
    {
        showmsg_pth(ERROR," bind failed...... errno:%d-%s...\n", errno, strerror(errno));     
        close(listenfd);
        return -1;
    }

    if(listen(listenfd, MAX_CONN)==-1)
    {
        showmsg_pth(ERROR," listen failed...... errno:%d-%s...\n", errno, strerror(errno));
        close(listenfd);
        return -1;
    }   


	epollfd=epoll_create(MAX_PRE_EVENTS);
	event.events=EPOLLIN|EPOLLET|EPOLLRDHUP;
	event.data.fd=listenfd;

	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event)<0)
	{
		close(listenfd);
		close(epollfd);
		showmsg_pth(ERROR,"epoll 加入失败 fd==%d.....error...\n", listenfd);		  
		return -1;
	}
	*lis_fd = listenfd;
	return epollfd;
}


int addNewFd(int svr_fd, int lis_fd)
{
    struct sockaddr_in sin;
    socklen_t len= 0;
    int new_fd ;
	struct epoll_event event;
	
    len=sizeof(struct sockaddr_in);
    bzero(&sin,len);

    new_fd=accept(lis_fd, (struct sockaddr *)&sin, &len);
    if(new_fd < 0)
    {
            showmsg_pth(ERROR,"accept     error............\n");                        
            return -1;
    }

    if(set_svr_opt(new_fd))
    {
        close(new_fd);
        showmsg_pth(ERROR,"set_ver_opt failed.....error.....error.......\n");          
        return -1;
    }

	event.data.fd=new_fd;
	event.events=EPOLLIN|EPOLLET|EPOLLRDHUP;
	epoll_ctl(svr_fd, EPOLL_CTL_ADD, new_fd, &event);

    return 0;
}


int RecvData(int fd, unsigned char * buf, int length)  
{  
    int recvLen = 0, needLens=0;  
    unsigned char *rcBuf=NULL;  

    if(buf==NULL || fd<=0)
    {
        showmsg_pth(ERROR,"RecvData  buf==NULL || fd<=0..\n");
        return -1;
    }

    rcBuf = buf;
    needLens = length;
	printf("while之前....needLens==[%d]\n", needLens);

	printf("recv之前....\n");
    if((recvLen = recv(fd, rcBuf, needLens, 0)) < 0)     
    {
		printf("recvLen < 0 \n");
	}
    else if (recvLen == 0)
	{
		printf("recvLen == 0 远端已经关闭 \n");
		return -1;
    }
	return 0;
}  

void getData(unsigned char *ps, unsigned char *buf)
{
	/*unsigned char *pp = buf;
	while(*ps != '"')
	{
		*pp++ = *ps++;
	}*/
}

int parseStr(unsigned char *buf, TMP_STRUCT *stu)
{
	unsigned char *ps;
	if(!buf)
		return -1;

	ps = (unsigned char *)strstr((char*)buf, "MERCHANT_ID");
	if(ps == NULL)
		return  -1;
	ps+=(strlen("MERCHANT_ID")+3);
	getData(ps, stu->MERCHANT_ID);
	printf("MERCHANT_ID===[%s]\n", stu->MERCHANT_ID);
	
	ps = (unsigned char *)strstr((char*)buf, "MSG_CODE");
	if(ps == NULL)
		return  -1;
	ps+=(strlen("MSG_CODE")+3);
	getData(ps, stu->MSG_CODE);
	printf("MSG_CODE===[%s]\n", stu->MSG_CODE);

	return 0;
}





void * processTask(void *args)
{
	int rd_fd=0;
	unsigned char buf[1024], sndBuf[1024];
	TMP_STRUCT stu;

	memset(sndBuf, 0x00, sizeof(sndBuf));		
	memset(buf, 0x00, sizeof(buf));		
	memset(&stu, 0x00, sizeof(TMP_STRUCT));
	pthread_detach(pthread_self());

	///读 数据
	rd_fd = *((int*)args);

	printf("rd_fd===[%d]\n", rd_fd);
	RecvData(rd_fd, buf, sizeof(buf)-1);

	printf("buf===[%s]\n", buf);
	//解析
	if(parseStr(buf, &stu))
	{
		printf("parseStr 失败...\n");
		return NULL;
	}
	//使用
//........................... TODO  ............
	snprintf((char*)sndBuf, sizeof(sndBuf),"%s", "{\"STATUS\":\"OK\"}");


	printf("开始  回写..................\n");
	//回写
	http_send_response(rd_fd, sndBuf, strlen((char*)sndBuf));


	pthread_exit(0);

	return  NULL;
}

int main(int argc, char **argv)
{
    int  srv_fd = 0, lis_fd, i, ret;  /* socket 句柄 */
 	struct epoll_event eventList[MAX_PRE_EVENTS];

	srv_fd = initSerSock(&lis_fd);
	if(srv_fd<0)
	{
			//error
			return -1;
	}
	
    while (1)
    {
    	printf("开始一次......\n");
		ret=epoll_wait(srv_fd, eventList, MAX_PRE_EVENTS, -1);
		if(ret<0)
		{
			if(errno==EINTR)
				continue;
			else
			{
				//sendMobileMessageForAlert("read_proc_prv  epoll_wait error, restart  will  be");
				pthread_exit(NULL);
			}
		}
		
		for(i=0;i<ret;i++)
		{
			if((eventList[i].events & EPOLLERR) || (eventList[i].events & EPOLLHUP))
			{
				showmsg_pth(ERROR,"异常关闭.........\n");
				epoll_ctl(srv_fd, EPOLL_CTL_DEL, eventList[i].data.fd, NULL);
				close(eventList[i].data.fd);
				continue;
			}

			if((eventList[i].events & EPOLLRDHUP) && (eventList[i].events & EPOLLIN))
			{
				showmsg_pth(ERROR,"正常关闭.........\n");
				epoll_ctl(srv_fd, EPOLL_CTL_DEL, eventList[i].data.fd, NULL);
				close(eventList[i].data.fd);				
				continue;
			}

			printf("主链接 有事件eventList[i].data.fd=[%d],lis_fd==[%d]\n", eventList[i].data.fd, lis_fd);
			if(eventList[i].events & EPOLLIN)
				printf("可写...........\n");

			if(eventList[i].data.fd== lis_fd)
			{
				if(addNewFd(srv_fd, lis_fd))
				{
					showmsg_pth(ERROR," AcceptConn	 error...error....\n");
				}
				else
					printf("添加 new_fd 成功==\n");
			}
			else if(eventList[i].events & EPOLLIN) 
			{
				pthread_t pidsd;
				if(pthread_create(&pidsd, NULL, processTask, &(eventList[i].data.fd)))
				{
					showmsg_pth(ERROR,"pthread_create.. workerForRead    error...[%s]\n", strerror(errno));   
					return -1;
				}
			}
		}

    }

    close(srv_fd);
    printf("[Web] The server is stopped.\n");
 
    return 0;
}


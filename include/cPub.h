#ifndef  __CPUB_JIAO__
#define  __CPUB_JIAO__

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

#include "iso8583_cups.h"

#define  ALARM_MSG_QUEUE_KEY       99999


#define  MAYBE_ONLY_ONE 88
#define  INVALID_SIG 99

#define  P_FAILRE       -1
#define  P_SUCCESS      0
#define  P_FD_CLOSED    1

//消息 buf
#define MSG_BUF   1024

//消息缓冲区   节点个数
#define NODE_NUMS   2048

#define KEY_Q_NUMS  2048

//每个key长度
#define KEY_DATA_LENS   128

//key队列中  key超时时间 秒
#define KEY_RETURN_TIMES  30

//KEY status
#define KEY_STATS_INIT  666
#define KEY_STATS_OK    777

//最大连接数
#define MAX_CONN  1000

//最大事件数
#define MAX_PRE_EVENTS  5


//共享内存大小
#define  SHM_SIZES    1024*2

#define  X_CMD_PATH   512
// add queue  type
#define  FD_WRITE              12
#define  FD_READ               13


//事件状态
#define  EV_INIT               00
#define  EV_READ               88
#define  EV_WRITE              99

//连接状态
#define  CONN_FD_INIT             0
#define  CONN_FD_OK               1

//数据链表状态
#define  LINK_INIT                   111
#define  LINK_HAD_DATA               112

//数据 处理状态
#define  PRO_DAT_ING              1991
#define  PRO_DAT_HAD              1992

// 连接 队列节点状态
#define  QUEUE_NODE_INIT        123      ///init

#define  QUEUE_NODE_PRV_MID     124     //此时 prv (lis ok but， conect no ok)
#define  QUEUE_NODE_PRV_OK      125     //double  (lis  and  conect)  ok  

#define  QUEUE_NODE_NXT_OK      126    //double  (lis  and  conect)  ok 
#define  QUEUE_NODE_CONED       127    ///lis accept new  conec ， 连接都ok，开始读写


//读取状态
#define  RD_INIT        0
#define  RD_READING     1
#define  RD_OK          2

#define  RD_READ                0
#define  RD_WRITE               1


//数据节点 状态
#define  NODE_OK                8898
#define  NODE_ERR               8899

//线程 状态
#define  THREAD_STAT_LIVE       9916
#define  THREAD_STAT_DEAD       9917



typedef  int (*FUNC_method)(int fd, unsigned char * buf, int length); 
typedef  void *(*START_THREAD) (void *);

typedef struct 
{
    int  sysId;
    char listen_ip[32];
    char listen_port[16];
    char send_ip[32];
    char send_port[16];
}CFG_CONN_PRV_NXT;


typedef struct
{
    int key_index;
    int lens;
    unsigned char msgBuf[MSG_BUF];
}NODE_T;


typedef struct
{
    NODE_T * nodeBuf;
	pthread_mutex_t mutexVar;
	pthread_cond_t  condVar;
    int currMsgNum; //目前msg  num     
}PR_BUF;

typedef struct 
{
    int sysId;
    NODE_T  dataNode;
}NODE_T_S;

typedef struct
{
    pthread_mutex_t mutex_rd;
    pthread_cond_t cond_rd;
    NODE_T_S * baseNode;//存储 队列 头指针
    int front;  //前 
    int rear;   //后
}QUEQUE_RD;

typedef struct
{
    int sysId;
    pthread_mutex_t mutex_wr;
    pthread_cond_t cond_wr;
    NODE_T_S * baseNode;//存储 队列 头指针
    int front;  //前 
    int rear;   //后     
}QUEQUE_WR;


typedef struct
{
    pthread_mutex_t mutex_dt_rd;
    pthread_cond_t cond_dt_rd;
    NODE_T *baseNodeNxDt_rd;
    int front;  //前 
    int rear;   //后 
}NX_DT_BUF_RD;

typedef struct
{
    pthread_mutex_t mutex_dt_wr;
    pthread_cond_t cond_dt_wr;
    NODE_T *baseNodeNxDt_wr;
    int front;  //前 
    int rear;   //后 
}NX_DT_BUF_WR;


typedef struct
{
    char key_data[KEY_DATA_LENS];
    int sysId;
    short stats;  //KEY_STATS_INIT      KEY_STATS_BACK
    struct  timeval    tv;/*long  tv_sec 秒  long  tv_usec 微妙*/      
}NX_KEY_T;

typedef struct
{
    pthread_mutex_t mutex_key;
    pthread_cond_t cond_key;
    NX_KEY_T * baseKey;//存储 队列 头指针
    int tailer;
}NX_KEY_BUF;


typedef struct
{
    int read_fd;  
    int lisFd;  
    int epFd; 
    short threadStat; 
    struct  sockaddr_in  addr;   //对端发送数据的IP  port
    FUNC_method pro_mth;  //  READ 函数
    pthread_t  read_id;//存储对应线程id   
    NODE_T buf_read;//read缓冲区指针  所有等待处理的任务
}PR_READ;

typedef struct
{
    int write_fd;
    struct  sockaddr_in  addr;   
    FUNC_method pro_mth;  //  WRITE 函数
    pthread_t  write_id;//存储对应线程id
    short threadStat;
}PR_WRITE;

typedef struct
{
    volatile short used;  //  used 已被占用，  init  未被占用
    CFG_CONN_PRV_NXT cfg;   //对应 配置
    PR_READ  pr_read;
    PR_WRITE pr_write;
}SOCK_QUEUE;         // sock_id  队列

typedef struct
{
    int dw_wr_nums;   // prv>nxt? prv-nxt: nxt-prv  TODO
    int dw_wr_id;
    char dw_wr_out_time[16]; 
}DW_WR_DT;


typedef struct
{
    char up_rd_in_time[16]; 
    int  up_rd_in_nums;  // prv>nxt? prv-nxt: nxt-prv   TODO
    char up_rd_out_time[16]; 
    int  up_rd_out_nums; // prv>nxt? prv-nxt: nxt-prv  TODO
}M_UP;

typedef struct
{
    char dw_rd_in_time[16];    // prv>nxt? prv-nxt: nxt-prv  TODO
    DW_WR_DT * dw_wr;
}M_DW;

typedef struct
{
    char key_in_time[16];
    int  key_data_nums; 
}M_KEY;

typedef struct
{
    M_UP up;
    M_DW dw;
    M_KEY key;
}MONITOR_DT;

typedef struct msgbuf {
    long mtype;       
    char mtext[512];
}MSG_ALARM;



int readCfgFileGetNumber(char * filePath, volatile int * eachCfgNumber);
int  readCfgFile(volatile CFG_CONN_PRV_NXT * outPrvBuffer, char * filePath, short eachMax, int i);
int init_queue(volatile SOCK_QUEUE *sok_que);
int  init_listen_sock(volatile CFG_CONN_PRV_NXT  *prv_cfg);
int event_init(short type, int sockfd);
int init_prev_conn(volatile CFG_CONN_PRV_NXT *prv_cfg);
int  SendData(int fd, unsigned char * buf, int length);
int  RecvData(int fd, unsigned char * buf, int length);
int handlRcvPrvData(volatile SOCK_QUEUE * sok_que);
int handlRcvNxtData(volatile SOCK_QUEUE * sok_que);
int set_fd_nonb(int fd);
int start_worker();
int  initShmSpace(int *shmId);
int attchShmSpace(int *shmId);
int shmHandle();
int startListenAndConnect();
int startThreadForWork();
int initAllPrvNxtSockQueue(volatile SOCK_QUEUE *sok_que);
int  createListenSock(volatile CFG_CONN_PRV_NXT  *prv_cfg);
int eventSet(short type, int sockfd);
int createConnect(volatile CFG_CONN_PRV_NXT *prv_cfg);
int set_svr_opt(int sockfd);
int initSockQByCfgAndDataQByCfg();
int initAllDataBufQueue();
int startNxtListenAndConnect();
int startPrvListen();
int checkAndResetSubPthread(volatile SOCK_QUEUE *sok_que, int flags);
void * start_prv_thread(void *args);
void * start_nxt_thread(void *args);
int startPrvNxtThreadAndPrvConn();
int startPrvConnect(volatile SOCK_QUEUE * tmp_sok_que);
int NxtListenAndConnect(volatile SOCK_QUEUE *sok_que, short maxNums, short beforeStat, short afterStat);
void set_fd_send_recv_timeOut(int fd);
void checkThreadAndKeyQueue();
void resetKeyIndex(int key_index);
void check_write_release_read_source(volatile SOCK_QUEUE * tmp_sok_que, int signo);
void check_read_release_write_source(volatile SOCK_QUEUE * tmp_sok_que, int signo);
int reStartPrvListen(volatile SOCK_QUEUE *sok_que);
int reStartNxtListenAndConnect(volatile SOCK_QUEUE *sok_que, short maxNums, short beforeStat, short afterStat);
void sendMobileMessageForAlert(char *strProblem);
void Create_Daemon();
char *GetIniKeyStringProxy(char *title, char *key, char *filename);
int initMsgQueueForAlarm();



#endif


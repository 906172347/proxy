#include "cPub.h"

volatile SOCK_QUEUE *sok_que_base = NULL;
volatile SOCK_QUEUE *sok_que_base_nx = NULL;

//SOCK_QUEUE * volatile  sok_que_base = NULL;  //禁止指针本身优化
//volatile SOCK_QUEUE * sok_que_base = NULL;  //禁止指针指向内存中数据优化

volatile QUEQUE_RD  *q_rd = NULL;
volatile QUEQUE_WR  *q_wr = NULL;
volatile NX_DT_BUF_RD *nx_dt_rd_buf = NULL;
volatile NX_DT_BUF_WR *nx_dt_wr_buf = NULL;
volatile NX_KEY_BUF *nx_key_buf = NULL;
void *pShmAddr = NULL;
static char xBinPath[X_CMD_PATH], prevFileName[X_CMD_PATH], nextFileName[X_CMD_PATH];
volatile int gPrvCfgMax = 0, gNxtCfgMax = 0;
static int alarmMsgQueueId = -1;

int initAllDataBufQueue()
{
    volatile QUEQUE_RD * tmp_q_rd = NULL;
    volatile QUEQUE_WR * tmp_q_wr = NULL;
    volatile NX_KEY_BUF *tp_nx_key_buf = NULL;
    volatile NX_DT_BUF_RD *tp_nx_dt_rd_buf = NULL;
    volatile NX_DT_BUF_WR *tp_nx_dt_wr_buf = NULL;
    int i = 0;

    if(!pShmAddr)
    {
        showmsg_pth(ERROR,"pShmAddr is null\n");    
        return P_FAILRE;
    }

    q_rd = (QUEQUE_RD*)pShmAddr;
    q_wr = (QUEQUE_WR*)(pShmAddr+sizeof(QUEQUE_RD));
    nx_dt_rd_buf = (NX_DT_BUF_RD*)(pShmAddr+sizeof(QUEQUE_RD)+sizeof(QUEQUE_WR)*gPrvCfgMax);
    nx_dt_wr_buf = (NX_DT_BUF_WR*)(pShmAddr+sizeof(QUEQUE_RD)+sizeof(QUEQUE_WR)*gPrvCfgMax +sizeof(NX_DT_BUF_RD));
    nx_key_buf = (NX_KEY_BUF*)(pShmAddr+sizeof(QUEQUE_RD)+sizeof(QUEQUE_WR)*gPrvCfgMax +sizeof(NX_DT_BUF_RD)+sizeof(NX_DT_BUF_WR));

    tmp_q_rd = q_rd;
    tmp_q_wr = q_wr;
    tp_nx_dt_rd_buf = nx_dt_rd_buf;
    tp_nx_dt_wr_buf = nx_dt_wr_buf;
    tp_nx_key_buf = nx_key_buf;

    pthread_mutex_init((pthread_mutex_t *)&(tmp_q_rd->mutex_rd), NULL);
    pthread_cond_init((pthread_cond_t *)&(tmp_q_rd->cond_rd), NULL);
    tmp_q_rd->front=tmp_q_rd->rear=0;
    tmp_q_rd->baseNode = (NODE_T_S*)malloc(sizeof(NODE_T_S)*NODE_NUMS);
    if(!tmp_q_rd->baseNode)
    {
        showmsg_pth(ERROR,"malloc error  QUEQUE_RD is [%s]\n", strerror(errno));         
        tmp_q_rd->baseNode=NULL;
        return P_FAILRE;
    }
    memset(tmp_q_rd->baseNode, 0x00, sizeof(NODE_T_S)*NODE_NUMS);


    for(i=0; i<gPrvCfgMax; i++)
    {
        pthread_mutex_init((pthread_mutex_t *)&(tmp_q_wr[i].mutex_wr), NULL);
        pthread_cond_init((pthread_cond_t *)&(tmp_q_wr[i].cond_wr), NULL);
        tmp_q_wr[i].rear=tmp_q_wr[i].front=0;
        tmp_q_wr[i].baseNode = (NODE_T_S*)malloc(sizeof(NODE_T_S)*NODE_NUMS);
        if(!tmp_q_wr[i].baseNode)
        {
            showmsg_pth(ERROR,"malloc error  QUEQUE_WR is [%s]\n", strerror(errno)); 
            tmp_q_wr[i].baseNode=NULL;
            return P_FAILRE;
        }
        memset(tmp_q_wr[i].baseNode, 0x00, sizeof(NODE_T_S)*NODE_NUMS);
    }

    pthread_mutex_init((pthread_mutex_t *)&(tp_nx_dt_rd_buf->mutex_dt_rd), NULL);
    pthread_cond_init((pthread_cond_t *)&(tp_nx_dt_rd_buf->cond_dt_rd), NULL);
    tp_nx_dt_rd_buf->baseNodeNxDt_rd= (NODE_T*)malloc(sizeof(NODE_T)*NODE_NUMS);
    if(!tp_nx_dt_rd_buf->baseNodeNxDt_rd)
    {
        showmsg_pth(ERROR,"malloc error  NX_DT_RD_BUF is [%s]\n", strerror(errno));
        tp_nx_dt_rd_buf->baseNodeNxDt_rd=NULL;
        return P_FAILRE;
    }
    memset(tp_nx_dt_rd_buf->baseNodeNxDt_rd, 0x00, sizeof(NODE_T)*NODE_NUMS);
    tp_nx_dt_rd_buf->front=tp_nx_dt_rd_buf->rear=0;

    pthread_mutex_init((pthread_mutex_t *)&tp_nx_dt_wr_buf->mutex_dt_wr, NULL);
    pthread_cond_init((pthread_cond_t *)&tp_nx_dt_wr_buf->cond_dt_wr, NULL);
    tp_nx_dt_wr_buf->baseNodeNxDt_wr= (NODE_T*)malloc(sizeof(NODE_T)*NODE_NUMS);
    if(!tp_nx_dt_wr_buf->baseNodeNxDt_wr)
    {
        showmsg_pth(ERROR,"malloc error  NX_DT_WR_BUF is [%s]\n", strerror(errno));
        tp_nx_dt_wr_buf->baseNodeNxDt_wr=NULL;
        return P_FAILRE;
    }
    memset(tp_nx_dt_wr_buf->baseNodeNxDt_wr, 0x00, sizeof(NODE_T)*NODE_NUMS);
    tp_nx_dt_wr_buf->front=tp_nx_dt_wr_buf->rear=0;

    pthread_mutex_init((pthread_mutex_t *)&(tp_nx_key_buf->mutex_key), NULL);
    pthread_cond_init((pthread_cond_t *)&(tp_nx_key_buf->cond_key), NULL);
    tp_nx_key_buf->baseKey = (NX_KEY_T*)malloc(sizeof(NX_KEY_T)*KEY_Q_NUMS);
    if(!tp_nx_key_buf->baseKey)
    {
        showmsg_pth(ERROR,"malloc error  NX_KEY_T is [%s]\n", strerror(errno));
        tp_nx_key_buf->baseKey=NULL;
        return P_FAILRE;
    }

    memset(tp_nx_key_buf->baseKey, 0x00, sizeof(NX_KEY_T)*KEY_Q_NUMS);
    for(i=0; i<KEY_Q_NUMS; i++)
        tp_nx_key_buf->baseKey[i].stats= KEY_STATS_INIT;
    
    tp_nx_key_buf->tailer = 0;

    return P_SUCCESS;
}

static inline void changeKeyQueTailer(volatile NX_KEY_BUF *tp_nx_key_buf)
{
    int  i = 0;

    for(i=tp_nx_key_buf->tailer; (i>=0)&&(tp_nx_key_buf->tailer<KEY_Q_NUMS); i--)
    {
        if(tp_nx_key_buf->baseKey[tp_nx_key_buf->tailer].stats == KEY_STATS_INIT)
        {
            tp_nx_key_buf->tailer--;
        }
        else
            return ;
    }
}

static inline int pushNxKeyQue(char *key_dat, NODE_T_S * node_t_s)
{
    volatile NX_KEY_BUF *tp_nx_key_buf = NULL;
    volatile NX_KEY_T * baseKey = NULL;
    int  i =0;

    if(key_dat==NULL || node_t_s==NULL)
    {
        sendMobileMessageForAlert("pushNxKeyQue key_dat==NULL || node_t_s==NULL  error ");
        showmsg_pth(ERROR,"workerForRead pushNxKeyQue  key_dat==NULL || node_t_s==NULL 信息  错误 ....\n");        
        return P_FAILRE;
    }
    
    tp_nx_key_buf = nx_key_buf;
    baseKey = tp_nx_key_buf->baseKey;
    
    pthread_mutex_lock((pthread_mutex_t *)&tp_nx_key_buf->mutex_key);
    for(i=0; i<KEY_Q_NUMS; i++)
    {
        if(baseKey[i].stats==KEY_STATS_INIT)        
        {
            if(i > tp_nx_key_buf->tailer)
            {
                tp_nx_key_buf->tailer = i;
            }
            memcpy((char*)baseKey[i].key_data, key_dat, strlen(key_dat));
            baseKey[i].stats = KEY_STATS_OK;
            baseKey[i].sysId = node_t_s->sysId;
            node_t_s->dataNode.key_index = i;
            gettimeofday((struct timeval *)&baseKey[i].tv, NULL);         
            pthread_mutex_unlock((pthread_mutex_t *)&tp_nx_key_buf->mutex_key);
            return P_SUCCESS;
        }
    }
    pthread_mutex_unlock((pthread_mutex_t *)&tp_nx_key_buf->mutex_key);   

    return P_FAILRE;
}

static inline int subMethodFindNxtKey(volatile NX_KEY_BUF *tp_nx_key_buf, char *key_dat, int * sysId)
{
    NX_KEY_T * baseKey = NULL;
    int  i = 0;
    
    baseKey = tp_nx_key_buf->baseKey;

    for(i=0; (i<=tp_nx_key_buf->tailer)&&(tp_nx_key_buf->tailer<KEY_Q_NUMS); i++)
    {
       if(memcmp(baseKey[i].key_data, key_dat, strlen(key_dat))==0 && baseKey[i].stats==KEY_STATS_OK)     
       {
           *sysId =  baseKey[i].sysId;
           baseKey[i].sysId = 0;
           memset(baseKey[i].key_data, 0x00, KEY_DATA_LENS);
           baseKey[i].stats = KEY_STATS_INIT;
           changeKeyQueTailer(tp_nx_key_buf);
           return P_SUCCESS;
       }
    }
    return P_FAILRE;
}


static inline int popNxKeyQueForSpecial(char *key_dat, int * sysId)
{
    volatile NX_KEY_BUF *tp_nx_key_buf = NULL;

    if(key_dat==NULL || sysId ==NULL)
    {
        sendMobileMessageForAlert("popNxKeyQueForSpecial key_dat==NULL || sysId==NULL error");
        showmsg_pth(ERROR,"workerForWrite   popNxKeyQue   key_dat==NULL || sysId ==NULL  信息  错误 ....\n");        
        return P_FAILRE;
    }

    tp_nx_key_buf = nx_key_buf;

    pthread_mutex_lock((pthread_mutex_t *)&tp_nx_key_buf->mutex_key);
    if(subMethodFindNxtKey(tp_nx_key_buf, key_dat, sysId))
    {
        pthread_mutex_unlock((pthread_mutex_t *)&tp_nx_key_buf->mutex_key); 
        return P_FAILRE;
    }
    pthread_mutex_unlock((pthread_mutex_t *)&tp_nx_key_buf->mutex_key);

    return P_SUCCESS;
}

static inline int pushNxReadQueue(NODE_T *nod_t)
{
    volatile NX_DT_BUF_RD *tp_nx_dt_rd_buf=nx_dt_rd_buf;

    pthread_mutex_lock((pthread_mutex_t *)&tp_nx_dt_rd_buf->mutex_dt_rd);
    if((tp_nx_dt_rd_buf->rear+1)%NODE_NUMS == tp_nx_dt_rd_buf->front)
    {
        pthread_mutex_unlock((pthread_mutex_t *)&tp_nx_dt_rd_buf->mutex_dt_rd);
        return P_FAILRE;
    }
    memset(&tp_nx_dt_rd_buf->baseNodeNxDt_rd[tp_nx_dt_rd_buf->rear], 0x00, sizeof(NODE_T));
    memcpy(&tp_nx_dt_rd_buf->baseNodeNxDt_rd[tp_nx_dt_rd_buf->rear], nod_t, sizeof(NODE_T));
    tp_nx_dt_rd_buf->rear = (tp_nx_dt_rd_buf->rear+1)%NODE_NUMS;
    pthread_cond_signal((pthread_cond_t *)&tp_nx_dt_rd_buf->cond_dt_rd);    
    pthread_mutex_unlock((pthread_mutex_t *)&tp_nx_dt_rd_buf->mutex_dt_rd);
    
    return P_SUCCESS;
}

static inline void popNxReadQueue(NODE_T *nod_t)
{
    volatile NX_DT_BUF_RD *tp_nx_dt_rd_buf=nx_dt_rd_buf;

    pthread_mutex_lock((pthread_mutex_t *)&tp_nx_dt_rd_buf->mutex_dt_rd);
    while(tp_nx_dt_rd_buf->front == tp_nx_dt_rd_buf->rear)
        pthread_cond_wait((pthread_cond_t *)&tp_nx_dt_rd_buf->cond_dt_rd, (pthread_mutex_t *)&tp_nx_dt_rd_buf->mutex_dt_rd);
    memcpy(nod_t, &(tp_nx_dt_rd_buf->baseNodeNxDt_rd[tp_nx_dt_rd_buf->front]), sizeof(NODE_T));
    memset(&(tp_nx_dt_rd_buf->baseNodeNxDt_rd[tp_nx_dt_rd_buf->front]),0x00, sizeof(NODE_T));
    tp_nx_dt_rd_buf->front = (tp_nx_dt_rd_buf->front+1)%NODE_NUMS;
    pthread_mutex_unlock((pthread_mutex_t *)&tp_nx_dt_rd_buf->mutex_dt_rd);
}

static inline int pushNxWriteQueue(NODE_T *nod_t)
{
    volatile NX_DT_BUF_WR *tp_nx_dt_wr_buf=nx_dt_wr_buf;

    pthread_mutex_lock((pthread_mutex_t *)&tp_nx_dt_wr_buf->mutex_dt_wr);
    if((tp_nx_dt_wr_buf->rear+1)%NODE_NUMS == tp_nx_dt_wr_buf->front)
    {
        pthread_mutex_unlock((pthread_mutex_t *)&tp_nx_dt_wr_buf->mutex_dt_wr);
        return P_FAILRE;
    }
    memset(&tp_nx_dt_wr_buf->baseNodeNxDt_wr[tp_nx_dt_wr_buf->rear], 0x00, sizeof(NODE_T));
    memcpy(&tp_nx_dt_wr_buf->baseNodeNxDt_wr[tp_nx_dt_wr_buf->rear], nod_t, sizeof(NODE_T));
    tp_nx_dt_wr_buf->rear = (tp_nx_dt_wr_buf->rear+1)%NODE_NUMS;
    pthread_cond_signal((pthread_cond_t *)&tp_nx_dt_wr_buf->cond_dt_wr);
    pthread_mutex_unlock((pthread_mutex_t *)&tp_nx_dt_wr_buf->mutex_dt_wr);
    
    return P_SUCCESS;
}

static inline void popNxWriteQueue(NODE_T *nod_t)
{
    volatile NX_DT_BUF_WR *tp_nx_dt_wr_buf=nx_dt_wr_buf;

    pthread_mutex_lock((pthread_mutex_t *)&tp_nx_dt_wr_buf->mutex_dt_wr);
    while(tp_nx_dt_wr_buf->front == tp_nx_dt_wr_buf->rear)
        pthread_cond_wait((pthread_cond_t *)&tp_nx_dt_wr_buf->cond_dt_wr, (pthread_mutex_t *)&tp_nx_dt_wr_buf->mutex_dt_wr);
    memcpy(nod_t, &(tp_nx_dt_wr_buf->baseNodeNxDt_wr[tp_nx_dt_wr_buf->front]), sizeof(NODE_T));
    memset(&(tp_nx_dt_wr_buf->baseNodeNxDt_wr[tp_nx_dt_wr_buf->front]), 0x00, sizeof(NODE_T));
    tp_nx_dt_wr_buf->front = (tp_nx_dt_wr_buf->front+1)%NODE_NUMS;
    pthread_mutex_unlock((pthread_mutex_t *)&tp_nx_dt_wr_buf->mutex_dt_wr);
}

static inline int pushPrvRdQue(NODE_T_S *nod)
{
    volatile QUEQUE_RD * tmp_q_rd = q_rd;

    pthread_mutex_lock((pthread_mutex_t *)&tmp_q_rd->mutex_rd);
    if((tmp_q_rd->rear+1)%NODE_NUMS == tmp_q_rd->front)
    {
        pthread_mutex_unlock((pthread_mutex_t *)&tmp_q_rd->mutex_rd);
        return P_FAILRE;
    }
    memset(&tmp_q_rd->baseNode[tmp_q_rd->rear], 0x00, sizeof(NODE_T_S));
    memcpy(&tmp_q_rd->baseNode[tmp_q_rd->rear], nod, sizeof(NODE_T_S));
    tmp_q_rd->rear = (tmp_q_rd->rear+1)%NODE_NUMS;
    pthread_cond_signal((pthread_cond_t *)&tmp_q_rd->cond_rd);    
    pthread_mutex_unlock((pthread_mutex_t *)&tmp_q_rd->mutex_rd);
    
    return P_SUCCESS;
}

static inline void popPrvRdQue(NODE_T_S *nod)
{
    volatile QUEQUE_RD * tmp_q_rd = q_rd;

    pthread_mutex_lock((pthread_mutex_t *)&tmp_q_rd->mutex_rd);
    while(tmp_q_rd->front == tmp_q_rd->rear)
        pthread_cond_wait((pthread_cond_t *)&tmp_q_rd->cond_rd, (pthread_mutex_t *)&tmp_q_rd->mutex_rd);
    memcpy(nod, &(tmp_q_rd->baseNode[tmp_q_rd->front]), sizeof(NODE_T_S));
    memset(&(tmp_q_rd->baseNode[tmp_q_rd->front]), 0x00, sizeof(NODE_T_S));  
    tmp_q_rd->front = (tmp_q_rd->front+1)%NODE_NUMS;
    pthread_mutex_unlock((pthread_mutex_t *)&tmp_q_rd->mutex_rd);
}

static inline int pushPrvWrQue(NODE_T_S *nod, volatile QUEQUE_WR  *one_q_wr )
{
    volatile QUEQUE_WR * tmp_q_wr = one_q_wr;

    if(!one_q_wr)
        return P_FAILRE;
    
    pthread_mutex_lock((pthread_mutex_t *)&tmp_q_wr->mutex_wr);
    if((tmp_q_wr->rear+1)%NODE_NUMS == tmp_q_wr->front)
    {  
        pthread_mutex_unlock((pthread_mutex_t *)&tmp_q_wr->mutex_wr);
        return P_FAILRE;
    }
    memset(&tmp_q_wr->baseNode[tmp_q_wr->rear], 0x00, sizeof(NODE_T_S));
    memcpy(&tmp_q_wr->baseNode[tmp_q_wr->rear], nod, sizeof(NODE_T_S));
    tmp_q_wr->rear = (tmp_q_wr->rear+1)%NODE_NUMS;
    pthread_cond_signal((pthread_cond_t *)&tmp_q_wr->cond_wr);    
    pthread_mutex_unlock((pthread_mutex_t *)&tmp_q_wr->mutex_wr);
  
    return P_SUCCESS;
}

static inline void popPrvWrQue(NODE_T_S *nod, volatile QUEQUE_WR *one_q_wr)
{
    volatile QUEQUE_WR * tmp_q_wr = one_q_wr;

    pthread_mutex_lock((pthread_mutex_t *)&tmp_q_wr->mutex_wr);
    while(tmp_q_wr->front == tmp_q_wr->rear)   
        pthread_cond_wait((pthread_cond_t *)&tmp_q_wr->cond_wr, (pthread_mutex_t *)&tmp_q_wr->mutex_wr);
    memcpy(nod, &(tmp_q_wr->baseNode[tmp_q_wr->front]), sizeof(NODE_T_S));
    memset(&(tmp_q_wr->baseNode[tmp_q_wr->front]), 0x00, sizeof(NODE_T_S));
    tmp_q_wr->front = (tmp_q_wr->front+1)%NODE_NUMS;
    pthread_mutex_unlock((pthread_mutex_t *)&tmp_q_wr->mutex_wr);
}

int addWrQMapBySysId(volatile SOCK_QUEUE *sok_que_prv)
{
    volatile QUEQUE_WR  *tmp_q_wr = NULL;
    int i = 0;
    
    if(sok_que_prv==NULL || q_wr==NULL)
    {
        showmsg_pth(ERROR,"addWrQMapBySysId sok_que_prv==NULL || q_wr==NULL [%s]\n", strerror(errno));
        sendMobileMessageForAlert("addWrQMapBySysId sok_que_prv==NULL || q_wr==NULL error");
        return P_FAILRE;
    }
    tmp_q_wr = q_wr;
    for(i=0; i < gPrvCfgMax; i++)
    {
        if(sok_que_prv[i].used == QUEUE_NODE_INIT)
            tmp_q_wr[i].sysId = sok_que_prv[i].cfg.sysId;
    }

    return P_SUCCESS;
}

int initSockQueueMem()
{
    sok_que_base = (volatile SOCK_QUEUE*)malloc(sizeof(SOCK_QUEUE)*gPrvCfgMax);
    if(!sok_que_base)
    {
        sendMobileMessageForAlert("initSockQueueMem malloc sok_que_base error ");
        showmsg_pth(ERROR,"malloc sok_que_base error  is [%s]\n", strerror(errno)); 
        sok_que_base=NULL;
        return P_FAILRE;
    }
    memset((void *)sok_que_base, 0x00, sizeof(SOCK_QUEUE)*gPrvCfgMax);

    sok_que_base_nx = (volatile SOCK_QUEUE*)malloc(sizeof(SOCK_QUEUE)*gNxtCfgMax);
    if(!sok_que_base_nx)
    {
        sendMobileMessageForAlert("initSockQueueMem malloc sok_que_base_nx error ");
        showmsg_pth(ERROR,"malloc sok_que_base_nx error  is [%s]\n", strerror(errno)); 
        sok_que_base_nx=NULL;
        return P_FAILRE;
    }
    memset((void *)sok_que_base_nx, 0x00, sizeof(SOCK_QUEUE)*gNxtCfgMax);
    return P_SUCCESS;
}

int readCfgAndInitSock(volatile SOCK_QUEUE *sok_que, char *cfgFile, int max)
{
    int i = 0;

    if(sok_que==NULL || max <= 0 || cfgFile==NULL)
        return P_FAILRE;

    for(i=0; i<max; i++)
    {
        if(readCfgFile(&sok_que[i].cfg, cfgFile, max, i))
        {
            sendMobileMessageForAlert("readCfgAndInitSock readCfgFile error ");
            showmsg_pth(ERROR,"readCfgFile prv cfg....error.... error.......\n");
            return P_FAILRE;
        }
        if(initAllPrvNxtSockQueue(&sok_que[i]))
        {
            sendMobileMessageForAlert("readCfgAndInitSock initAllPrvNxtSockQueue error ");
            showmsg_pth(ERROR,"initAllPrvNxtSockQueue prv..error.... error.......\n");
            return P_FAILRE;
        }     
    }

    return P_SUCCESS;
}

int initSockQByCfgAndDataQByCfg()
{
    volatile SOCK_QUEUE *sok_que=NULL, *sok_que_nx=NULL;
    int i=0;

    if(readCfgFileGetNumber(prevFileName, &gPrvCfgMax) || readCfgFileGetNumber(nextFileName, &gNxtCfgMax))
    {
        sendMobileMessageForAlert("readCfgFileGetNumber. error.");
        showmsg_pth(ERROR,"readCfgFileGetNumber for MAX CFG..error.... error.......\n");
        return P_FAILRE;
    }

    if(gPrvCfgMax<=0 || gNxtCfgMax<=0)
    {
        sendMobileMessageForAlert("gPrvCfgMax<=0 || gNxtCfgMax<=0. error.");
        showmsg_pth(ERROR,"gPrvCfgMax<=0 || gNxtCfgMax<=0 .... error.......\n");
        return P_FAILRE;
    }

    if(initSockQueueMem())
    {
        sendMobileMessageForAlert("initSockQueueMem. error.");
        showmsg_pth(ERROR,"initSockQueueMem 处理 失败[%s]\n", strerror(errno)); 
        return P_FAILRE;
    }

    sok_que = sok_que_base;
    sok_que_nx = sok_que_base_nx;    

    if(readCfgAndInitSock(sok_que, prevFileName, gPrvCfgMax) || readCfgAndInitSock(sok_que_nx, nextFileName, gNxtCfgMax))
    {
        sendMobileMessageForAlert("readCfgAndInitSock. error.");
        showmsg_pth(ERROR,"readCfgAndInitSock 处理 失败[%s]\n", strerror(errno)); 
        return P_FAILRE;
    }

    for(i=0; i<gPrvCfgMax; i++)
        showmsg_pth(NORMAL,"prv_cfg[%d] listen_ip==%s, listen_port==%s, send_ip==%s, send_port==%s\n", i, sok_que[i].cfg.listen_ip, sok_que[i].cfg.listen_port, sok_que[i].cfg.send_ip, sok_que[i].cfg.send_port); 

    for(i=0; i<gNxtCfgMax; i++)
        showmsg_pth(NORMAL,"nex_cfg[%d] listen_ip==%s, listen_port==%s, send_ip==%s, send_port==%s\n", i, sok_que_nx[i].cfg.listen_ip, sok_que_nx[i].cfg.listen_port, sok_que_nx[i].cfg.send_ip, sok_que_nx[i].cfg.send_port);


    if(initAllDataBufQueue())
    {
        sendMobileMessageForAlert("initAllDataBufQueue. error.");
        showmsg_pth(ERROR," 初始化  队列，读，写,  ...失败.......\n");  
    	return P_FAILRE;
    }

    if(addWrQMapBySysId(sok_que))
    {
        sendMobileMessageForAlert("addWrQMapBySysId. error.");
        showmsg_pth(ERROR,"addWrQMapBySysId    FAILED...FAILED.....error.... error.......\n");
        return P_FAILRE;
    }
    
    return P_SUCCESS;
}

inline void checkThreadAndKeyQueue()
{
    volatile SOCK_QUEUE *tmp_sok_prv_que= NULL, *tmp_sok_nxt_que= NULL;
    volatile NX_KEY_BUF *tp_nx_key_buf = NULL;
    struct timeval  tm;
    register int i=0;
    
    tmp_sok_prv_que = sok_que_base;
    tmp_sok_nxt_que = sok_que_base_nx;
    tp_nx_key_buf = nx_key_buf;

    while(1)
    {
        usleep(1000*20);
        for(i=0; (i<=tp_nx_key_buf->tailer) && (tp_nx_key_buf->tailer<KEY_Q_NUMS) &&(tp_nx_key_buf->tailer>0); i++)
        {
            if((tp_nx_key_buf->baseKey[i].stats == KEY_STATS_OK) && ((tm.tv_sec - tp_nx_key_buf->baseKey[i].tv.tv_sec) > KEY_RETURN_TIMES))
            {
                showmsg_pth(ERROR,"++++存在超时节点baseKey[%d].stats==%d,现在时间==%ld,添加时间==%ld,tailer==%d, key_data==%s\n", i, tp_nx_key_buf->baseKey[i].stats, tm.tv_sec, tp_nx_key_buf->baseKey[i].tv.tv_sec, tp_nx_key_buf->tailer, tp_nx_key_buf->baseKey[i].key_data);
                sendMobileMessageForAlert("checkThreadAndKeyQueue timeOut key data node");                
                pthread_mutex_lock((pthread_mutex_t *)&tp_nx_key_buf->mutex_key);
                gettimeofday(&tm, NULL);
                memset(tp_nx_key_buf->baseKey[i].key_data, 0x00, KEY_DATA_LENS);
                tp_nx_key_buf->baseKey[i].sysId = 0;
                tp_nx_key_buf->baseKey[i].stats = KEY_STATS_INIT;
                pthread_mutex_unlock((pthread_mutex_t *)&tp_nx_key_buf->mutex_key);
            }
        }

        for(i=0; (i < gPrvCfgMax)&&(sok_que_base[i].cfg.sysId > 0); i++)
        {   
            if(sok_que_base[i].used==QUEUE_NODE_INIT && (sok_que_base[i].pr_read.threadStat==THREAD_STAT_DEAD || sok_que_base[i].pr_write.threadStat==THREAD_STAT_DEAD))
            {
                if(checkAndResetSubPthread(&sok_que_base[i], 0))
                {
                    sendMobileMessageForAlert("prv  checkAndResetSubPthread error");
                    showmsg_pth(ERROR,"checkAndResetSubPthread....prv..检测..线程..失败...短信报警 .....\n");                    
                }
                else
                {
                    if(!reStartPrvListen(&sok_que_base[i]))
                        start_prv_thread((void*)&sok_que_base[i]);
                    else
                    {
                        sendMobileMessageForAlert("prv  reStartPrvListen error");
                        showmsg_pth(ERROR, "reStartPrvListen....prv....重新启动线程失败..失败...短信报警 .....\n");
                    }
                }
            }
        }

        for(i=0; (i<gNxtCfgMax)&&(sok_que_base_nx[i].cfg.sysId > 0); i++)
        {
            if(sok_que_base_nx[i].used==QUEUE_NODE_INIT && (sok_que_base_nx[i].pr_read.threadStat==THREAD_STAT_DEAD || sok_que_base_nx[i].pr_write.threadStat==THREAD_STAT_DEAD))
            {
                if(checkAndResetSubPthread(&sok_que_base_nx[i], 1))
                {
                    sendMobileMessageForAlert("nxt   checkAndResetSubPthread error");
                    showmsg_pth(ERROR, "checkAndResetSubPthread....nxt..检测..线程..失败...短信报警 .....\n");                                        
                }
                else
                {
                    if(!reStartNxtListenAndConnect(&sok_que_base_nx[i], gNxtCfgMax, QUEUE_NODE_INIT, QUEUE_NODE_NXT_OK))
                        start_nxt_thread((void*)&sok_que_base_nx[i]); 
                    else
                    {
                        sendMobileMessageForAlert("nxt   reStartNxtListenAndConnect error");
                        showmsg_pth(ERROR, "reStartPrvListen....nxt....重新启动线程失败..失败...短信报警 .....\n");
                    }                 
                }
            } 
        }
    }

}



int startThreadForWork()
{
    //pthread_t pid_bl;
/*
    if(pthread_create(&pid_bl, NULL, monitorDataQ, NULL))
    {
        sendMobileMessageForAlert("start monitorDataQ error");
        showmsg_pth(ERROR,"pthread_create.. monitorDataQ    error...[%s]\n", strerror(errno)); 
        return P_FAILRE;
    }
*/
    if(start_worker())
    {
        sendMobileMessageForAlert("start_worker error");
        showmsg_pth(ERROR, "启动start_worker.....error   失败.......\n");                                
        return P_FAILRE;
    }

    if(startPrvNxtThreadAndPrvConn())
    {
        sendMobileMessageForAlert("startPrvNxtThreadAndPrvConn error");
        showmsg_pth(ERROR, "启动startPrvNxtConnect .....error   失败......\n");         
        return P_FAILRE;
    }

    return P_SUCCESS;
}

static inline int getKeyData(char  *key_data, unsigned char* msgBuf)
{
    unsigned char *psStr=NULL;
    struct Pos opPos;
    
    if(key_data==NULL ||  msgBuf==NULL)
    {
        sendMobileMessageForAlert("getKeyData  key_data==NULL ||  msgBuf==NULL error");
        showmsg_pth(ERROR, "getKeyData .....error  key_data==NULL ||  msgBuf==NULL....\n"); 
        return P_FAILRE;
    }
    memset(&opPos, 0x00, sizeof(struct Pos));
/*
      目前msgBuf 是  长度+报文头 + 消息类型 + 位图 +  报文体
*/
     psStr = msgBuf+4+46;  //跳过长度 +46报文头
    Unpack_Iso8583_Package(psStr, &opPos);

    snprintf(key_data, KEY_DATA_LENS, "%s%s%s%s", opPos.sTermSerial, opPos.sTimeTrn, opPos.sTermNumber, opPos.sMerchantID);
    //那些域   7  11  41   42 // 流水  日期  终端   商户 
    
    return P_SUCCESS;
}

void * workerForRead(void *args)
{
    NODE_T_S  node_t_s;
    char key_dat[KEY_DATA_LENS]={0};
    
    pthread_detach(pthread_self());
    
    while(1)
    {
        memset(key_dat, 0x00, KEY_DATA_LENS);
        memset(&node_t_s, 0x00, sizeof(NODE_T_S));

        popPrvRdQue(&node_t_s);
        if(getKeyData(key_dat, node_t_s.dataNode.msgBuf))
        {
            sendMobileMessageForAlert("workerForRead getKeyData  Failure.....give up..");
            showmsg_pth(ERROR,"一次上行  解包.....失败.....失败....\n");
            continue;
        }
        showmsg_pth(NORMAL,"一次上行 getKeyData 解包成功 key_dat=[%s]\n", key_dat);

        if(pushNxKeyQue(key_dat, &node_t_s))
        {
            usleep(1000);
            sendMobileMessageForAlert("workerForRead pushNxKeyQue  Failure.....give up..");
            showmsg_pth(ERROR,"存储 key data  失败....失败....直接丢弃....error.....key_dat===%s\n", key_dat);
            continue;
        }
        showmsg_pth(NORMAL,"一次上行 添加 pushNxKeyQueKEY_DATA 成功 key_dat=[%s]\n", key_dat);
        
        if(pushNxReadQueue(&node_t_s.dataNode)) 
        {
            usleep(1000);
            sendMobileMessageForAlert("workerForRead pushNxReadQueue  Failure.....give up..");
            showmsg_pth(ERROR,"添加......pushNxReadQueue error error  队列已经满了...丢弃...这笔....不用等待.....\n");            
        }
        showmsg_pth(NORMAL,"一次上行 添加 pushNxReadQueue 成功 key_dat=[%s]\n", key_dat);
    }


    return NULL;
}

void * workerForWrite(void *args)
{
    NODE_T  nod_t;
    NODE_T_S nod_t_s;
    char key_dat[KEY_DATA_LENS]={0};
    int sysId=0, i=0;
    volatile QUEQUE_WR  *tmp_q_wr = NULL, *one_q_wr = NULL;

    pthread_detach(pthread_self());
    tmp_q_wr = q_wr;
    
    while(1)
    {
        one_q_wr = NULL;
        memset(key_dat, 0x00, KEY_DATA_LENS);
        memset(&nod_t, 0x00, sizeof(NODE_T));
        memset(&nod_t_s, 0x00, sizeof(NODE_T_S));
        popNxWriteQueue(&nod_t);

        if(getKeyData(key_dat, nod_t.msgBuf))
        {
            sendMobileMessageForAlert("workerForWrite getKeyData Failure.....give up..");        
            showmsg_pth(ERROR,"getKeyData.获取key_data....error...error....error...失败....\n");
            continue;
        }
        showmsg_pth(NORMAL,"二次下行 getKeyData 解包成功 key_dat=[%s]\n", key_dat);
      
        sysId = 0;
        if(popNxKeyQueForSpecial(key_dat, &sysId))
        {
            sendMobileMessageForAlert("workerForWrite popNxKeyQueForSpecial  cann't found back sysId, give up..");
            showmsg_pth(ERROR,"popNxKeyQueForSpecial 没有找到回路的 sysId 失败  失败..., key_dat==[%s]\n", key_dat);
            continue;
        }
        showmsg_pth(NORMAL,"二次下行 添加 popNxKeyQueForSpecial 成功 key_dat=[%s]\n", key_dat);

        nod_t_s.sysId = sysId;
        memcpy(&nod_t_s.dataNode, &nod_t, sizeof(NODE_T));
        for(i=0; i<gPrvCfgMax; i++)
        {
            if(tmp_q_wr[i].sysId == sysId)
            {
                one_q_wr = &tmp_q_wr[i];
                break;
            }
        }
        if(!one_q_wr)
        {
            sendMobileMessageForAlert("workerForWrite  can't find write Queue by sysId, give up...");
            showmsg_pth(ERROR,"popNxKeyQueForSpecial 没有找到写回队列,丢弃 sysId==[%d]\n", sysId);
            continue;
        }
        
        if(pushPrvWrQue(&nod_t_s, one_q_wr))
        {
            usleep(1000);
            sendMobileMessageForAlert("workerForWrite pushPrvWrQue prv write Failure.....give up..");            
            showmsg_pth(ERROR,"pushPrvWrQue 添加 队列 失败.. 已满.....丢弃....丢弃......\n");            
            continue;
        }
        showmsg_pth(NORMAL,"二次下行 添加 pushPrvWrQue 写回数据 成功 sysId=[%d]\n", sysId);        
    }

    return NULL;
}

int getFifoMonitor(int *fdFifo)
{
    char fifoName[256]={0};

    memset(fifoName, 0x00, 256);
    snprintf(fifoName, 256, "%s/proxyMonitorFifo", xBinPath);
    if((mkfifo(fifoName,0777)<0)&&(errno!=EEXIST))
    {
        printf("cannot create fifo...%s\n", strerror(errno));
        sendMobileMessageForAlert("mkfifo error...");
        showmsg_pth(ERROR,"getFifoMonitor.. 命令监控线程,创建交互管道失败  error...[%s]\n", strerror(errno));
        return P_FAILRE;
    }
    
    *fdFifo=open(fifoName, O_RDWR);
    if(*fdFifo==-1)
    {
        printf("open for read error  %s\n", strerror(errno));
        sendMobileMessageForAlert("open fifo error");
        showmsg_pth(ERROR,"打开交互管道失败open error...[%s]\n", strerror(errno));
        return P_FAILRE;
    }

    return P_SUCCESS;
}

void  getDataFromQ(MONITOR_DT *mdt)
{


}
/*
void initMontiorDataStruct(MONITOR_DT * mdt)
{
    if(!mdt)
        exit(-1);
    
    memset(&mdt->up, 0x00, sizeof(PRV_WR_MONITOR_DT)*gPrvCfgMax);
    
    struct
    {
        char up_rd_in_time[16]; 
        int  up_rd_in_nums;  // prv>nxt? prv-nxt: nxt-prv   TODO
        char up_rd_out_time[16]; 
        int  up_rd_out_nums; // prv>nxt? prv-nxt: nxt-prv  TODO
    }up;
    struct
    {
        char dw_rd_in_time[16];    // prv>nxt? prv-nxt: nxt-prv  TODO
        DW_WR_DT * dw_wr;
    }down;

    struct
    {
        char key_in_time[16];
        int  key_data_nums; 
    }key;

    memset(tmpPrvWr, 0x00, sizeof(PRV_WR_MONITOR_DT)*gPrvCfgMax);


}*/
/*
void * monitorDataQ(void *args)
{
    int  fdFifo;
    char flagTag = 0;
    MONITOR_DT  mdt;
    DW_WR_DT *tmpPrvWr = NULL;

    //创建管道
    if(getFifoMonitor(&fdFifo))
    {
        sendMobileMessageForAlert("getFifoMonitor error");
        showmsg_pth(ERROR,"交互管道处理失败error...\n");
        exit(-1);
    }
    
    memset(&mdt, 0x00, sizeof(MONITOR_DT));

    mdt.prv.prv_wr = tmpPrvWr = (DW_WR_DT*)malloc(sizeof(DW_WR_DT)*gPrvCfgMax);
    if(!tmpPrvWr)
    {
        sendMobileMessageForAlert("malloc(sizeof(DW_WR_DT)*gPrvCfgMax) error");
        showmsg_pth(ERROR,"交互数据结构体malloc失败error...\n");
        exit(-1);
    }
    memset(tmpPrvWr, 0x00, sizeof(DW_WR_DT)*gPrvCfgMax);


    while(1)
    {
        read(fdFifo, &flagTag, 1);
        printf("read:%d\n", flagTag);
        
        //initMontiorDataStruct(&mdt);

        memset(tmpPrvWr, 0x00, sizeof(PRV_WR_MONITOR_DT)*gPrvCfgMax);



        //获取队列 数据
        getDataFromQ(&mdt);

        //写入管道
        write(fdFifo, str, 6);

    }

    return NULL;
}
*/

int start_worker()
{
    pthread_t pids_worker;
    int i = 0; 

    for(i=0; i<gPrvCfgMax*3; i++)
    {
        if(i<gPrvCfgMax)
        {
            if(pthread_create(&pids_worker, NULL, workerForRead, NULL))
            {
                sendMobileMessageForAlert("start workerForRead error");
                showmsg_pth(ERROR,"pthread_create.. workerForRead    error...[%s]\n", strerror(errno));   
                return P_FAILRE;
            }
        }
        else
        {
            if(pthread_create(&pids_worker, NULL, workerForWrite, NULL))
            {
                sendMobileMessageForAlert("start workerForWrite error");
                showmsg_pth(ERROR,"pthread_create.. workerForWrite    error...[%s]\n", strerror(errno)); 
                return P_FAILRE;
            }
        }
    }
    
    usleep(1000);
    return P_SUCCESS;    
}

void trim_str(char *pstr)
{
    char tmp[512]={0};
    char *pp = NULL, *ps = NULL;
    if(!pstr)
        exit(-1);
    
    pp = pstr;
    ps = tmp;

    memset(tmp, 0x00, sizeof(tmp));
    while(*pp != '\0')
    {
        if(!isspace(*pp))
            *ps++ = *pp; 
        pp++;
    }
    memcpy(pstr, tmp, strlen(tmp));
    pstr[strlen(tmp)]='\0';
}

int readCfgFileGetNumber(char * filePath, volatile int * eachCfgNumber)
{
    FILE *fp;
    char tmpBuf[512]={0};
    volatile int  index=0;
    
    if(!filePath)
        return P_FAILRE;
    
    fp = fopen(filePath, "r");
    if (!fp) 
    {
        showmsg_pth(ERROR,"打开文件[%s]失败..\n", filePath); 
        return P_FAILRE;
    }

    memset(tmpBuf, 0x00, sizeof(tmpBuf));
    index=0;
    while(fgets(tmpBuf, sizeof(tmpBuf) -1, fp)) 
    {
        trim_str(tmpBuf);
        if (*tmpBuf == 0)
            continue;
        if((strstr(tmpBuf, "PREV_CONN_") || strstr(tmpBuf, "NEXT_CONN_")))
            index++;
        memset(tmpBuf, 0x00, sizeof(tmpBuf));
    }
    fclose(fp);
    *eachCfgNumber = index;

    return P_SUCCESS;
}

int readCfgFile(volatile CFG_CONN_PRV_NXT * outPrvBuffer, char * filePath, short eachMax, int i)
{
    FILE *fp;
    char tmpBuf[512]={0}, *pt=NULL, tag1[32]={0}, tag2[32]={0};
    volatile CFG_CONN_PRV_NXT  * conn_CFG=NULL;
    int  index=0, flag=0;

    if(outPrvBuffer==NULL || filePath==NULL)
    {
        showmsg_pth(ERROR,"outPrvBuffer==NULL || filePath==NULL\n");
        return P_FAILRE;
    }
    
    fp = fopen(filePath, "r");
    if (!fp) 
    {
        showmsg_pth(ERROR,"打开文件[%s]失败..\n", filePath); 
        return P_FAILRE;
    }

    memset(tmpBuf, 0x00, sizeof(tmpBuf));
    conn_CFG  = outPrvBuffer;
    flag=index=0;

    while(fgets(tmpBuf, sizeof(tmpBuf) -1, fp)) 
    {
        trim_str(tmpBuf);
        if (*tmpBuf == 0)
            continue; 
        memset(tag1, 0x00, 32);
        memset(tag2, 0x00, 32);        
        snprintf(tag1, 32, "PREV_CONN_%d", i+1);
        snprintf(tag2, 32, "NEXT_CONN_%d", i+1);        
        if((strstr(tmpBuf, tag1) || strstr(tmpBuf, tag2)))
        {
            memset(tmpBuf, 0x00, sizeof(tmpBuf));
            flag = 1;
            continue;
        }
        else
        {
            if(!flag)
            {
                memset(tmpBuf, 0x00, sizeof(tmpBuf));
                continue;    
            }
            else
            {
                if(strstr(tmpBuf, "SYSID"))
                {
                    pt = strstr(tmpBuf, "=");
                    if(!pt)
                    {
                        memset(tmpBuf, 0x00, sizeof(tmpBuf));
                        continue;
                    }
                    pt++;
                    conn_CFG->sysId = atoi(pt);
                }            
                else if(strstr(tmpBuf, "LISTEN_IP"))
                {
                    pt = strstr(tmpBuf, "=");
                    if(!pt)
                    {
                        memset(tmpBuf, 0x00, sizeof(tmpBuf));
                        continue;
                    }
                    pt++;           
                    memcpy((void*)conn_CFG->listen_ip, pt, strlen(tmpBuf)-(pt-tmpBuf));
                }
                else if(strstr(tmpBuf, "LISTEN_PORT"))
                {
                    pt = strstr(tmpBuf, "=");
                    if(!pt)
                    {
                        memset(tmpBuf, 0x00, sizeof(tmpBuf));
                        continue;
                    }
                    pt++;
                    memcpy((void*)conn_CFG->listen_port, pt, strlen(tmpBuf)-(pt-tmpBuf));
                } 
                else if(strstr(tmpBuf, "SEND_IP"))
                {
                    pt = strstr(tmpBuf, "=");
                    if(!pt)
                    {
                        memset(tmpBuf, 0x00, sizeof(tmpBuf));
                        continue;
                    }
                    pt++;
                    memcpy((void*)conn_CFG->send_ip, pt, strlen(tmpBuf)-(pt-tmpBuf));
                } 
                else if(strstr(tmpBuf, "SEND_PORT"))
                {
                    pt = strstr(tmpBuf, "=");
                    if(!pt)
                    {
                        memset(tmpBuf, 0x00, sizeof(tmpBuf));
                        continue;
                    }
                    pt++;
                    memcpy((void*)conn_CFG->send_port, pt, strlen(tmpBuf)-(pt-tmpBuf));
                } 
                else
                {
                    flag = 0;
                    return P_SUCCESS;
                }
                memset(tmpBuf, 0x00, sizeof(tmpBuf));                
            }
        }

    }
    fclose(fp);

    return P_SUCCESS;
}

int initAllPrvNxtSockQueue(volatile SOCK_QUEUE *sok_que)
{
    if(!sok_que)
        return P_FAILRE;

    sok_que->used = QUEUE_NODE_INIT;
    sok_que->pr_read.read_fd=-1;
    sok_que->pr_read.pro_mth=NULL; 
    sok_que->pr_read.lisFd=-1;
    sok_que->pr_read.epFd=-1;
    sok_que->pr_write.write_fd=-1;
    sok_que->pr_write.pro_mth=NULL;
    return P_SUCCESS;
}


int AcceptConn(volatile SOCK_QUEUE * tmp_sok_que)
{
    struct sockaddr_in sin;
    socklen_t len= 0;
    
    len=sizeof(struct sockaddr_in);
    bzero(&sin,len);

    tmp_sok_que->pr_read.read_fd=accept(tmp_sok_que->pr_read.lisFd,(struct sockaddr *)&sin,&len);
    if(tmp_sok_que->pr_read.read_fd<0)
    {
        if(errno==EAGAIN || errno==EWOULDBLOCK)  
        {
            sendMobileMessageForAlert("AcceptConn errno==EAGAIN || errno==EWOULDBLOCK error");
            showmsg_pth(ERROR,"accept   no pending connections are present on the queue, marked as non-blocking........\n");            
            return P_FAILRE;
        }
        else
        {
            sendMobileMessageForAlert("AcceptConn accept     error...........error");
            showmsg_pth(ERROR,"accept     error............\n");                        
            return P_FAILRE;
        }
    }

    if(set_svr_opt(tmp_sok_que->pr_read.read_fd)) 
    {
        close(tmp_sok_que->pr_read.read_fd);
        sendMobileMessageForAlert("set_svr_opt failed.....error.........error");
        showmsg_pth(ERROR,"set_ver_opt failed.....error.....error.......\n");          
        return P_FAILRE;
    }

    if(set_fd_nonb(tmp_sok_que->pr_read.read_fd))
    {
        close(tmp_sok_que->pr_read.read_fd);
        sendMobileMessageForAlert("set_fd_nonb failed.....error.........error");
        showmsg_pth(ERROR,"set_fd_nonb   error.......error.......\n"); 
        return P_FAILRE;
    }

    return P_SUCCESS;
}

int addEpollQ(volatile SOCK_QUEUE * tmp_sok_que)
{
    struct epoll_event event;

    if(tmp_sok_que->pr_read.read_fd<=2)
    {
        sendMobileMessageForAlert("addEpollQ failed....tmp_sok_que->pr_read.read_fd<=2........error");
        showmsg_pth(ERROR,"addEpollQ    error .... error ........\n");
        return P_FAILRE;
    }
    //default  behavior  is  LT,  可以显示设置ET，不过没必要，默认LT
    event.data.fd=tmp_sok_que->pr_read.read_fd;
    event.events=EPOLLIN|EPOLLRDHUP|EPOLLERR|EPOLLHUP;
    epoll_ctl(tmp_sok_que->pr_read.epFd, EPOLL_CTL_ADD, tmp_sok_que->pr_read.read_fd, &event);
    return P_SUCCESS;
}

void sig_read_proc_nxt_handle (int signum)
{
    int i = 0; 
    struct epoll_event event;

    for(i=0; i<gNxtCfgMax; i++)
    {
        if(sok_que_base_nx[i].pr_read.read_id == pthread_self())
        {
            if(sok_que_base_nx[i].pr_read.read_fd > 0)
            {
                epoll_ctl(sok_que_base_nx[i].pr_read.epFd, EPOLL_CTL_DEL, sok_que_base_nx[i].pr_read.read_fd, &event);                     
                close(sok_que_base_nx[i].pr_read.read_fd);  
            }
            sok_que_base_nx[i].pr_read.read_fd=-1;
            epoll_ctl(sok_que_base_nx[i].pr_read.epFd, EPOLL_CTL_DEL, sok_que_base_nx[i].pr_read.lisFd, &event);
            close(sok_que_base_nx[i].pr_read.lisFd);
            close(sok_que_base_nx[i].pr_read.epFd);
            sok_que_base_nx[i].pr_read.threadStat=THREAD_STAT_DEAD;
            sok_que_base_nx[i].used = QUEUE_NODE_INIT; 
            pthread_exit(NULL);
        }
    }
}



void sig_read_proc_prv_handle (int signum)
{
    int i = 0; 
    struct epoll_event event;

    for(i=0; i<gPrvCfgMax; i++)
    {
        if(sok_que_base[i].pr_read.read_id == pthread_self())
        {
           if(sok_que_base[i].pr_read.read_fd > 0)
           {
                epoll_ctl(sok_que_base[i].pr_read.epFd, EPOLL_CTL_DEL, sok_que_base[i].pr_read.read_fd, &event);
                close(sok_que_base[i].pr_read.read_fd);   
           }
           sok_que_base[i].pr_read.read_fd=-1;
           epoll_ctl(sok_que_base[i].pr_read.epFd, EPOLL_CTL_DEL, sok_que_base[i].pr_read.lisFd, &event);
           close(sok_que_base[i].pr_read.lisFd);
           close(sok_que_base[i].pr_read.epFd);
           sok_que_base[i].pr_read.threadStat=THREAD_STAT_DEAD;
           sok_que_base[i].used = QUEUE_NODE_INIT;
           pthread_exit(NULL);
        }
    }
}


void * read_proc_nxt(void *args)
{
    int timeout=0, ret=0, i=0, retStat=0;
    struct epoll_event eventList[MAX_PRE_EVENTS];
    volatile SOCK_QUEUE * tmp_sok_que = NULL;

    pthread_detach(pthread_self());

    if(args==NULL)
    {
        showmsg_pth(ERROR,"线程[%ld]退出...error.....error......\n", (long)syscall(SYS_gettid));
        sendMobileMessageForAlert("start read_proc_nxt...args==NULL.error");
        pthread_exit(NULL);
    }

    sigset_t set;
    sigfillset(&set);
    sigdelset(&set, SIGRTMIN+4);
    pthread_sigmask(SIG_SETMASK, &set, 0);
    signal(SIGRTMIN+4, sig_read_proc_nxt_handle);
    
    tmp_sok_que = (SOCK_QUEUE*)args;
    tmp_sok_que->pr_read.read_id = pthread_self();
    tmp_sok_que->pr_read.threadStat=THREAD_STAT_LIVE;

    while(1)
    {
        timeout=1000*2;  
        ret=epoll_wait(tmp_sok_que->pr_read.epFd, eventList, MAX_PRE_EVENTS, timeout);
        if(ret<0)
        {
            if(errno==EINTR)
                continue;              
            else
            {
                sendMobileMessageForAlert("read_proc_nxt...epoll_wait...error...");
                check_write_release_read_source(tmp_sok_que, SIGRTMIN+3);
                pthread_exit(NULL);
            }
        }
        else if(ret==0)
            continue;

        for(i=0;i<ret;i++)
        {/*
        POLLRDHUP :
                      Stream  socket  peer  closed  connection,  or shut down writing half of connection.  (This flag is
                      especially useful for writing simple code to detect peer shutdown when using Edge Triggered  moni-
                      toring.)
        */
            if((eventList[i].events & EPOLLERR) || (eventList[i].events & EPOLLHUP) || (eventList[i].events & EPOLLRDHUP))
            {
                showmsg_pth(ERROR,"EPOLLERR  被监测的描述符，出问题啦..[%ld]....errno=%s\n", tmp_sok_que->pr_read.read_id, strerror(errno));
                check_write_release_read_source(tmp_sok_que, SIGRTMIN+3);
                sendMobileMessageForAlert("read_proc_nxt EPOLLERR|EPOLLHUP|EPOLLRDHUP  error...");
                pthread_exit(NULL);//结束  当前 读 线程
            }
            if(eventList[i].data.fd== tmp_sok_que->pr_read.lisFd)
            {
                if(AcceptConn(tmp_sok_que))
                {
                    sendMobileMessageForAlert("read_proc_nxt...AcceptConn, need connnect me..");
                    showmsg_pth(ERROR," AcceptConn   error...error....\n");
                }
                else
                {
                    if(!addEpollQ(tmp_sok_que))
                    {
                        showmsg_pth(NORMAL, "nxt lisnten   新连接 成功 ， connect 也成功...........\n");
                        tmp_sok_que->used = QUEUE_NODE_CONED;
                    }
                    else
                    {
                        //NOTE 这种情况很少出现,如果出现重启就可以了20180125 jll
                        sendMobileMessageForAlert("read_proc_nxt  addEpollQ......error..");                            
                        showmsg_pth(ERROR, "read_proc_nxt  addEpollQ......失败......\n");                        
                        tmp_sok_que->pr_read.read_fd=-1;
                    }
                }
            }
            else if(eventList[i].events & EPOLLIN) 
            {
                /*
                20170306  jll note:
                在高并发 且 数据块  比较大的 时候，处理本次事件会影响其他事件，让其他事件等待。
                所以，这里可以做线程池。 deliver req to thread pool
                这样可以把并发量提高到3万，应该没问题。 
                使用线程池之后，可以改成ET模式,以提高事件处理速度
                */
                retStat=handlRcvNxtData(tmp_sok_que);
                if(retStat==P_FD_CLOSED)
                {
                    check_write_release_read_source(tmp_sok_que, SIGRTMIN+3);
                    sendMobileMessageForAlert("read_proc_nxt...handlRcvNxtData...error, restart will be..");
                    pthread_exit(NULL);
                }
                else if(retStat==P_FAILRE)
                {
                    sendMobileMessageForAlert("handlRcvPrvData give up this, continue next...error..");
                    showmsg_pth(ERROR,"handlRcvPrvData 丢弃这笔交易, 继续下一笔....error....error......\n");
                }
            }
        }
    }

    return NULL;
}

void * read_proc_prv(void *args)
{
    int timeout=0, ret=0, i=0, retStat=0;
    struct epoll_event eventList[MAX_PRE_EVENTS];
    volatile SOCK_QUEUE * tmp_sok_que = NULL;


    pthread_detach(pthread_self());

    if(args==NULL)
    {
        showmsg_pth(ERROR,"线程[%ld]退出...error.....error......\n", (long)syscall(SYS_gettid));
        sendMobileMessageForAlert("read_proc_prv   args==NULL exit, restart will be..");
        pthread_exit(NULL);
    }

    sigset_t set;
    sigfillset(&set);
    sigdelset(&set, SIGRTMIN+2);
    pthread_sigmask(SIG_SETMASK, &set, 0);
    signal(SIGRTMIN+2, sig_read_proc_prv_handle);
    
    tmp_sok_que = (SOCK_QUEUE*)args;
    tmp_sok_que->pr_read.read_id = pthread_self(); 
    tmp_sok_que->pr_read.threadStat=THREAD_STAT_LIVE;
    
    while(1)
    {
        timeout=1000*2;
        ret=epoll_wait(tmp_sok_que->pr_read.epFd, eventList, MAX_PRE_EVENTS, timeout);
        if(ret<0)
        {
            if(errno==EINTR)
                continue;
            else
            {
                sendMobileMessageForAlert("read_proc_prv  epoll_wait error, restart  will  be");            
                check_write_release_read_source(tmp_sok_que, SIGRTMIN+1);
                pthread_exit(NULL);
            }
        }
        else if(ret==0)         
            continue;

        for(i=0;i<ret;i++)
        {
            if((eventList[i].events & EPOLLERR) || (eventList[i].events & EPOLLHUP) || (eventList[i].events & EPOLLRDHUP))
            {
                showmsg_pth(ERROR,"EPOLLERR  被监测的描述符，出问题啦..[%ld]....errno=%s\n", tmp_sok_que->pr_read.read_id, strerror(errno));                
                check_write_release_read_source(tmp_sok_que, SIGRTMIN+1);
                sendMobileMessageForAlert("read_proc_prv EPOLLERR|EPOLLHUP|EPOLLRDHUP, restart will  be");                
                pthread_exit(NULL);
            }
            if(eventList[i].data.fd== tmp_sok_que->pr_read.lisFd)
            {
                if(AcceptConn(tmp_sok_que))
                {
                    sendMobileMessageForAlert("read_proc_prv..AcceptConn error , need reconnec me..");
                    showmsg_pth(ERROR," AcceptConn   error...error....\n");
                }
                else
                {
                    while(tmp_sok_que->used==QUEUE_NODE_PRV_MID)
                        usleep(1000);
                    if(tmp_sok_que->used==QUEUE_NODE_PRV_OK)
                    {
                        if(!addEpollQ(tmp_sok_que))
                        {                     
                            showmsg_pth(NORMAL, "prv lisnten   新连接 成功 ， connect 也成功...........\n");
                            tmp_sok_que->used = QUEUE_NODE_CONED;
                        }
                        else
                        {   //NOTE 这种情况很少出现,如果出现重启就可以了20180125 jll
                            sendMobileMessageForAlert("read_proc_prv addEpollQ......error");                            
                            showmsg_pth(ERROR, "read_proc_prv addEpollQ......失败......\n");                        
                            tmp_sok_que->pr_read.read_fd=-1;
                        }
                    }
                }
            }
            else if(eventList[i].events & EPOLLIN) 
            {
                retStat=handlRcvPrvData(tmp_sok_que);
                if(retStat==P_FD_CLOSED)
                {
                    sendMobileMessageForAlert("read_proc_prv...handlRcvPrvData..P_FD_CLOSED... restart will be");
                    check_write_release_read_source(tmp_sok_que, SIGRTMIN+1);
                    pthread_exit(NULL);
                }
                else if(retStat==P_FAILRE)
                {
                    sendMobileMessageForAlert("read_proc_prv...handlRcvPrvData..P_FAILRE... restart will be"); 
                    showmsg_pth(ERROR,"handlRcvPrvData 丢弃这笔交易, 继续下一笔....error....error......\n");
                }            
            }
        }
    }

    return NULL;
}

void check_write_release_read_source(volatile SOCK_QUEUE * tmp_sok_que, int signo)
{
    int retNums=0;
    struct epoll_event event;

    if(tmp_sok_que==NULL || signo < 0)
    {
        showmsg_pth(ERROR,"tmp_sok_que==NULL || signo < 0...\n");
        return ;
    }
    retNums = pthread_kill(tmp_sok_que->pr_write.write_id, 0);
    if(retNums!=ESRCH && retNums!=EINVAL)
        pthread_kill(tmp_sok_que->pr_write.write_id, signo);
    else if(retNums==EINVAL)
        showmsg_pth(DEBUG,"发送信号非法...非法...非法....非法...\n");
    else if(retNums==ESRCH)
    {
        if(tmp_sok_que->pr_write.write_fd > 0)
            close(tmp_sok_que->pr_write.write_fd);
        tmp_sok_que->pr_write.threadStat=THREAD_STAT_DEAD;
    }
    if(tmp_sok_que->pr_read.read_fd > 0)
    {
        epoll_ctl(tmp_sok_que->pr_read.epFd, EPOLL_CTL_DEL, tmp_sok_que->pr_read.read_fd, &event); 
        close(tmp_sok_que->pr_read.read_fd);
    }
    epoll_ctl(tmp_sok_que->pr_read.epFd, EPOLL_CTL_DEL, tmp_sok_que->pr_read.lisFd, &event);  

    close(tmp_sok_que->pr_read.epFd);
    close(tmp_sok_que->pr_read.lisFd);
    tmp_sok_que->pr_read.threadStat=THREAD_STAT_DEAD;
    tmp_sok_que->used= QUEUE_NODE_INIT;
}


int handlRcvNxtData(volatile SOCK_QUEUE * sok_que)
{
    volatile SOCK_QUEUE * tmp_sok_que=NULL;
    int lengths = 0, retStat=0;//, ret = 0;
    char tmp[8]={0};
    NODE_T *dtNode =  NULL;
    NODE_T_S nod_s ;
    
    if(sok_que==NULL)
    {
        sendMobileMessageForAlert("handlRcvNxtData sok_que==NULL...error");     
        showmsg_pth(ERROR,"sok_que==NULL........error......error....\n");
        return P_FAILRE;
    }

    tmp_sok_que = sok_que;
    memset(&nod_s, 0x00, sizeof(NODE_T_S));
    dtNode = (NODE_T *)&(tmp_sok_que->pr_read.buf_read);
    memset(dtNode, 0x00, sizeof(NODE_T));

    retStat=tmp_sok_que->pr_read.pro_mth(tmp_sok_que->pr_read.read_fd, dtNode->msgBuf, 4);
    if(retStat==P_FAILRE)
    {
        sendMobileMessageForAlert("handlRcvNxtData ..P_FAILRE..error...error");
        showmsg_pth(ERROR,"读取数据失败.......失败....失败.......error......error....\n");
        return P_FAILRE;
    }
    else if(retStat==P_FD_CLOSED)
    {
        sendMobileMessageForAlert("handlRcvNxtData ..P_FD_CLOSED..error...error");
        showmsg_pth(ERROR,"描述符出错, 需要关闭连接......error.....error.......\n");
        return P_FD_CLOSED;
    }

    if(memcmp(dtNode->msgBuf, "0000", 4)==0)
    {
        showmsg_pth(NORMAL,"这是心跳包........ok.......ok......ok....\n");
        return P_SUCCESS;
    }
    
    memset(tmp, 0x00, sizeof(tmp));
    memcpy(tmp, dtNode->msgBuf, 4);
    lengths = atoi(tmp);

    retStat =tmp_sok_que->pr_read.pro_mth(tmp_sok_que->pr_read.read_fd, dtNode->msgBuf+4, lengths);
    if(retStat==P_FAILRE)
    {
        showmsg_pth(ERROR,"读取数据失败.......失败....失败.......error......error....\n");
        return P_FAILRE;
    }    
    if(retStat==P_FD_CLOSED)
    {
        showmsg_pth(ERROR,"描述符出错, 需要关闭连接......error.....error.......\n");
        return P_FD_CLOSED;
    }
    dtNode->lens = lengths+4;

    //showmsg_pth(DEBUG,"数据长度 lens====%d......\n", dtNode->lens);    
    //showhex_pth(NORMAL,"数据 data====\n", (char*)dtNode->msgBuf, dtNode->lens);

    if(pushNxWriteQueue(dtNode))
    {
        sendMobileMessageForAlert("handlRcvNxtData pushNxWriteQueue error, give up this, continue next");
        showmsg_pth(ERROR,"pushNxWriteQueue 添加队列...  失败  ..失败 ..直接...丢弃......error.....error.......\n");
        return P_FAILRE;
    }
    
    return P_SUCCESS;
}


int handlRcvPrvData(volatile SOCK_QUEUE * sok_que)
{
    volatile SOCK_QUEUE * tmp_sok_que=NULL;
    int lengths = 0, retStat = 0;
    char tmp[8]={0};
    NODE_T *dtNode =  NULL;
    NODE_T_S nod_s ;
    
    if(sok_que==NULL)
    {
        sendMobileMessageForAlert("handlRcvPrvData sok_que==NULL  error");            
        showmsg_pth(ERROR,"sok_que==NULL........error......error....\n");
        return P_FAILRE;
    }

    tmp_sok_que = sok_que;
    memset(&nod_s, 0x00, sizeof(NODE_T_S));
    dtNode = (NODE_T *)&tmp_sok_que->pr_read.buf_read;
    memset(dtNode, 0x00, sizeof(NODE_T));
    
    retStat =tmp_sok_que->pr_read.pro_mth(tmp_sok_que->pr_read.read_fd, dtNode->msgBuf, 4);
    if(retStat==P_FAILRE)
    {
        sendMobileMessageForAlert("handlRcvPrvData length retStat==P_FAILRE  error");  
        showmsg_pth(ERROR,"读取数据失败.......失败....失败.......error......error....\n");
        return P_FAILRE;
    }
    else if(retStat==P_FD_CLOSED)
    {
        sendMobileMessageForAlert("handlRcvPrvData length retStat==P_FD_CLOSED  error");
        showmsg_pth(ERROR,"描述符出错, 需要关闭连接......error.....error.......\n");
        return P_FD_CLOSED;
    }

    if(memcmp(dtNode->msgBuf, "0000", 4)==0)
    {
        showmsg_pth(NORMAL,"这是心跳包........ok.......ok......ok....\n");
        return P_SUCCESS;
    }

    memset(tmp, 0x00, 8);
    memcpy(tmp, dtNode->msgBuf, 4);
    lengths = atoi(tmp);
    retStat =tmp_sok_que->pr_read.pro_mth(tmp_sok_que->pr_read.read_fd, dtNode->msgBuf+4, lengths);
    if(retStat==P_FAILRE)
    {
        sendMobileMessageForAlert("handlRcvPrvData data retStat==P_FAILRE  error"); 
        showmsg_pth(ERROR,"读取数据失败.......失败....失败.......error......error....\n");
        return P_FAILRE;
    }
    else if(retStat==P_FD_CLOSED)
    {
        sendMobileMessageForAlert("handlRcvPrvData data retStat==P_FD_CLOSED  error");
        showmsg_pth(ERROR,"描述符出错, 需要关闭连接......error.....error.......\n");
        return P_FD_CLOSED;
    }
    dtNode->lens = lengths+4;

    showmsg_pth(NORMAL,"dtNode->lens====%d......\n", dtNode->lens);
    showhex_pth(NORMAL,"读取CLIENT 长度+数据成功..ok\n", (char*)dtNode->msgBuf, dtNode->lens);

    memcpy(&nod_s.dataNode, dtNode, sizeof(NODE_T));
    nod_s.sysId = tmp_sok_que->cfg.sysId;

    if(pushPrvRdQue(&nod_s))
    {
        sendMobileMessageForAlert("handlRcvPrvData pushPrvRdQue  error , give up  , continue next");
        showmsg_pth(ERROR,"pushPrvRdQue 添加队列...  失败  ..失败 ..直接...丢弃......error.....error.......\n");
        return P_FAILRE;
    }

    return P_SUCCESS;
}



void sig_write_proc_nxt_handle (int signum)
{
    int i = 0; 

    for(i=0; i<gNxtCfgMax; i++)
    {
        if(sok_que_base_nx[i].pr_write.write_id == pthread_self())
        {
           close(sok_que_base_nx[i].pr_write.write_fd);
           sok_que_base_nx[i].pr_write.write_fd=-1;
           sok_que_base_nx[i].pr_write.threadStat=THREAD_STAT_DEAD;
           sok_que_base_nx[i].used = QUEUE_NODE_INIT;
           pthread_exit(NULL);
        }
    }
}

void sig_write_proc_prv_handle (int signum)
{
    int i = 0; 

    for(i=0; i<gPrvCfgMax; i++)
    {
        if(sok_que_base[i].pr_write.write_id == pthread_self())
        {
            if(sok_que_base[i].pr_write.write_fd>0)
            close(sok_que_base[i].pr_write.write_fd);
            sok_que_base[i].pr_write.write_fd=-1;
            sok_que_base[i].pr_write.threadStat=THREAD_STAT_DEAD;
            sok_que_base[i].used = QUEUE_NODE_INIT;
            pthread_exit(NULL);
        }
    }
}

void * write_proc_nxt(void *args)
{
    SOCK_QUEUE * tmp_sok_que = NULL;
    NODE_T nod_t ;
    int retStat=0;

    pthread_detach(pthread_self());

    if(args==NULL)
    {
        sendMobileMessageForAlert("start write_proc_nxt...args==NULL.error");
        showmsg_pth(ERROR,"线程[%ld]退出...error......error.......\n", pthread_self());
        pthread_exit(NULL);
    }

    sigset_t set;
    sigfillset(&set);
    sigdelset(&set, SIGRTMIN+3);
    pthread_sigmask(SIG_SETMASK, &set, 0);
    signal(SIGRTMIN+3, sig_write_proc_nxt_handle);
    
    tmp_sok_que = (SOCK_QUEUE*)args;
    tmp_sok_que->pr_write.write_id= pthread_self(); 
    tmp_sok_que->pr_write.threadStat=THREAD_STAT_LIVE;

    do{
        usleep(100);
    }
    while(tmp_sok_que->used != QUEUE_NODE_CONED);
    
    while(1)
    {
        memset(&nod_t, 0x00, sizeof(NODE_T));
        popNxReadQueue(&nod_t);

        retStat=tmp_sok_que->pr_write.pro_mth(tmp_sok_que->pr_write.write_fd, nod_t.msgBuf, nod_t.lens);
        if(retStat==P_FD_CLOSED)
        {
            showmsg_pth(ERROR,"发送到 银联 失败.....retStat==P_FD_CLOSED .....\n");
            resetKeyIndex(nod_t.key_index);
            check_read_release_write_source(tmp_sok_que, SIGRTMIN+4);
            sendMobileMessageForAlert("write_proc_nxt P_FD_CLOSED send to CUPS error, restart will be..");
            pthread_exit(NULL);
        }
        else if(retStat==P_FAILRE)
        {
            sendMobileMessageForAlert("write_proc_nxt retStat==P_FAILRE  give up this, continue next");
            showmsg_pth(ERROR,"发送到 银联 失败......error.....error ....丢弃.\n");
        }
    }

    return NULL;
}

void resetKeyIndex(int key_index)
{
    volatile NX_KEY_BUF *tp_nx_key_buf = NULL;
    
    tp_nx_key_buf = nx_key_buf;

    if(!pthread_mutex_trylock((pthread_mutex_t *)&tp_nx_key_buf->mutex_key))
    {
        if(tp_nx_key_buf->baseKey[key_index].stats == KEY_STATS_OK)
        {
            memset(tp_nx_key_buf->baseKey[key_index].key_data, 0x00, KEY_DATA_LENS);
            tp_nx_key_buf->baseKey[key_index].sysId = 0;
            tp_nx_key_buf->baseKey[key_index].stats = KEY_STATS_INIT;
        }        
        pthread_mutex_unlock((pthread_mutex_t *)&tp_nx_key_buf->mutex_key);
    }
}


void * write_proc_prv(void *args)
{
    int i=0, retStat=0;
    volatile SOCK_QUEUE * tmp_sok_que = NULL;
    NODE_T_S nod ;
    volatile QUEQUE_WR  *one_q_wr=NULL, *tmp_q_wr=NULL; 

    pthread_detach(pthread_self());

    if(args==NULL)
    {
        showmsg_pth(ERROR,"线程[%ld]退出...error......error.......\n", pthread_self());
        sendMobileMessageForAlert("write_proc_prv   args==NULL restart will be..");        
        pthread_exit(NULL);
    }

    sigset_t set;
    sigfillset(&set);
    sigdelset(&set, SIGRTMIN+1);
    pthread_sigmask(SIG_SETMASK, &set, 0);
    
    signal(SIGRTMIN+1, sig_write_proc_prv_handle);
    
    tmp_sok_que = (SOCK_QUEUE*)args;
    tmp_sok_que->pr_write.write_id= pthread_self(); 
    tmp_sok_que->pr_write.threadStat=THREAD_STAT_LIVE;
    tmp_q_wr = q_wr;

    while(tmp_sok_que->used == QUEUE_NODE_PRV_MID)
    {
        usleep(1000);
        startPrvConnect(tmp_sok_que);
    }

    do{
        usleep(1000);
    }
    while(tmp_sok_que->used != QUEUE_NODE_CONED);

    memset(&nod, 0x00, sizeof(NODE_T_S));
    nod.sysId = tmp_sok_que->cfg.sysId;
    for(i=0; i < gPrvCfgMax; i++)
    {
        if(tmp_q_wr[i].sysId == nod.sysId)
        {
            one_q_wr = &tmp_q_wr[i];
            break;
        }
    }
    if(!one_q_wr)
    {
        sendMobileMessageForAlert("write_proc_prv  no compare write  data Queue, thread exit..");    
        check_read_release_write_source(tmp_sok_que, SIGRTMIN+2);
        pthread_exit(NULL);
    }

    while(1)
    {
        memset(&nod.dataNode, 0x00, sizeof(NODE_T));
        popPrvWrQue(&nod, one_q_wr);
        
        retStat=tmp_sok_que->pr_write.pro_mth(tmp_sok_que->pr_write.write_fd, nod.dataNode.msgBuf, nod.dataNode.lens);
        if(retStat==P_FD_CLOSED)
        {
            showmsg_pth(ERROR,"发送到 前端 prv 失败.....retStat==P_FD_CLOSED .....\n");
            check_read_release_write_source(tmp_sok_que, SIGRTMIN+2);
            sendMobileMessageForAlert("write_proc_prv retStat==P_FD_CLOSED, restart  will be..");            
            pthread_exit(NULL);
        }
        else if(retStat==P_FAILRE)
        {
            sendMobileMessageForAlert("write_proc_prvP_FAILRE give up this, continue next");
            showmsg_pth(ERROR,"发送到 前端 prv P_FAILRE  发送数据  失败...error.......\n");
        }
        else
            showmsg_pth(NORMAL,"发送到 前端 prv P_SUCCESS 发送数据 到  client   client  成功 .ok...ok.....\n");
    }

    return NULL;
}

void check_read_release_write_source(volatile SOCK_QUEUE * tmp_sok_que, int signo)
{
    int retNums = 0;
    struct epoll_event event;
    volatile int mux_flg = 0;

    if(tmp_sok_que==NULL || signo < 0)
    {
        showmsg_pth(ERROR,"tmp_sok_que==NULL || signo < 0...");
        return ;
    }
    
    retNums = pthread_kill(tmp_sok_que->pr_read.read_id, 0);
    if(retNums==ESRCH)
    {     
        if(tmp_sok_que->pr_read.read_fd > 0)
        {
            epoll_ctl(tmp_sok_que->pr_read.epFd, EPOLL_CTL_DEL, tmp_sok_que->pr_read.read_fd, &event);  //剔除read_fd  
            close(tmp_sok_que->pr_read.read_fd);
        }
        epoll_ctl(tmp_sok_que->pr_read.epFd, EPOLL_CTL_DEL, tmp_sok_que->pr_read.lisFd, &event);  //剔除lis_fd  
        close(tmp_sok_que->pr_read.lisFd);
        close(tmp_sok_que->pr_read.epFd); 
        tmp_sok_que->pr_read.threadStat=THREAD_STAT_DEAD;
        mux_flg = 1;
    }
    else if(retNums==EINVAL)
        showmsg_pth(DEBUG,"发送信号非法.......");

    if(!mux_flg)
        pthread_kill(tmp_sok_que->pr_read.read_id, signo);

    if(tmp_sok_que->pr_write.write_fd > 0)
    {
        close(tmp_sok_que->pr_write.write_fd);
        tmp_sok_que->pr_write.write_fd = -1;
    }
    tmp_sok_que->pr_write.threadStat=THREAD_STAT_DEAD;
    tmp_sok_que->used= QUEUE_NODE_INIT;
}


void * start_prv_thread(void *args)
{
    SOCK_QUEUE * tmp_sok_que = NULL;

    pthread_detach(pthread_self());

    if(args==NULL)
    {
        showmsg_pth(ERROR,"线程[%ld]退出...error.....error......\n", (long)syscall(SYS_gettid));
        sendMobileMessageForAlert("start_prv_thread...error......error.");
        pthread_exit(NULL);
    }

    tmp_sok_que = (SOCK_QUEUE*)args;

    if(pthread_create((pthread_t*)&tmp_sok_que->pr_read.read_id, NULL, read_proc_prv, tmp_sok_que))
    {
        sendMobileMessageForAlert("start read_proc_prv...error..");
        showmsg_pth(ERROR,"read_proc_prv ....创建 失败   error ..直接报警......直接报警.............\n");
        return NULL;        
    }    

    if(pthread_create((pthread_t*)&tmp_sok_que->pr_write.write_id, NULL, write_proc_prv, tmp_sok_que))
    {
        sendMobileMessageForAlert("start write_proc_prv...error..");
        showmsg_pth(ERROR,"write_proc_prv ....创建 失败   error ..直接报警......直接报警.............\n");        
        return NULL;
    }
    
    usleep(1000);

    return NULL;
}


void * start_nxt_thread(void *args)
{
    SOCK_QUEUE * tmp_sok_que = NULL;

    pthread_detach(pthread_self());

    if(args==NULL)
    {
        sendMobileMessageForAlert("start_nxt_thread....args==NULL");
        showmsg_pth(ERROR,"线程[%ld]退出...error.....error......\n", (long)syscall(SYS_gettid));
        pthread_exit(NULL);
    }
    
    tmp_sok_que = (SOCK_QUEUE*)args;

    if(pthread_create((pthread_t*)&tmp_sok_que->pr_read.read_id, NULL, read_proc_nxt, tmp_sok_que))
    {
        sendMobileMessageForAlert("start read_proc_nxt...error");
        showmsg_pth(ERROR,"read_proc_nxt ....创建 失败   error ..直接报警......直接报警.............\n");        
        return NULL;
    }

    if(pthread_create((pthread_t*)&tmp_sok_que->pr_write.write_id, NULL, write_proc_nxt, tmp_sok_que))
    {
        sendMobileMessageForAlert("start write_proc_nxt...error");
        showmsg_pth(ERROR,"write_proc_nxt ....创建 失败   error ..直接报警......直接报警.............\n");                
        return NULL;
    }

    usleep(1000);

    return NULL;
}

int reStartNxtListenAndConnect(volatile SOCK_QUEUE *sok_que, short maxNums, short beforeStat, short afterStat)
{
    int sockfdRD=0, sockfdWR=0, epollfdRD=0;

    if(!sok_que)
    {
        sendMobileMessageForAlert("reStartNxtListenAndConnect sok_que is null");
        showmsg_pth(ERROR,"reStartNxtListenAndConnect   sok_que is null......\n");
        return P_FAILRE;
    }   
  
    sockfdRD =createListenSock(&sok_que->cfg);    
    if(sockfdRD == P_FAILRE)
    {
        sendMobileMessageForAlert("reStartNxtListenAndConnect createListenSock error");
        showmsg_pth(ERROR,"NxtListenAndConnect    FAILED   FAILED ......\n");
        return P_FAILRE;
    }
    epollfdRD=eventSet(RD_READ, sockfdRD);
    if(epollfdRD==P_FAILRE)
    {
        sendMobileMessageForAlert("reStartNxtListenAndConnect eventSet error");
        showmsg_pth(ERROR,"NxtListenAndConnect  eventSet    FAILED   FAILED ......\n");
        return P_FAILRE;
    }

    do{
        sockfdWR=createConnect(&(sok_que->cfg));
        usleep(100*1000);
    }while(sockfdWR==P_FAILRE);

    sok_que->pr_read.pro_mth=RecvData;
    sok_que->pr_read.read_fd=-1;
    sok_que->pr_read.lisFd=sockfdRD;
    sok_que->pr_read.epFd=epollfdRD;

    sok_que->pr_write.pro_mth=SendData;
    sok_que->pr_write.write_fd=sockfdWR;
    sok_que->used = afterStat;
    
    return P_SUCCESS;
}


int NxtListenAndConnect(volatile SOCK_QUEUE *sok_que, short maxNums, short beforeStat, short afterStat)
{
    int i = 0;

    if(!sok_que)
    {
        sendMobileMessageForAlert("NxtListenAndConnect sok_que is null");
        showmsg_pth(ERROR,"NxtListenAndConnect   sok_que is null......\n");
        return P_FAILRE;
    } 


    for(i=0; i<maxNums && sok_que[i].cfg.listen_ip[0]!='\0' && sok_que[i].used==beforeStat; i++)
    {
        if(reStartNxtListenAndConnect(&sok_que[i], maxNums, beforeStat, afterStat))
            return P_FAILRE;
    }
    
    return P_SUCCESS;
}

int checkAndResetSubPthread(volatile SOCK_QUEUE *sok_que, int flags)
{
    int retStat=0;

    if(!sok_que)
    {
        sendMobileMessageForAlert("checkAndResetSubPthread sok_que is null");
        showmsg_pth(ERROR,"checkAndResetSubPthread   sok_que is null......\n");
        return P_FAILRE;
    }
    
    if(sok_que->pr_read.threadStat==THREAD_STAT_LIVE)
    {
        retStat = pthread_kill(sok_que->pr_read.read_id, 0);   
        if(ESRCH==retStat)
            sok_que->pr_read.threadStat=THREAD_STAT_DEAD;
        else if(retStat==EINVAL)
            return P_FAILRE;
        else
        {
        	if(flags)
			    pthread_kill(sok_que->pr_read.read_id, SIGRTMIN+4);
    		else
    			pthread_kill(sok_que->pr_read.read_id, SIGRTMIN+2);
        }
    }
    
    if(sok_que->pr_write.threadStat==THREAD_STAT_LIVE)
    {      
        retStat = pthread_kill(sok_que->pr_write.write_id, 0);   
        if(ESRCH==retStat)
            sok_que->pr_write.threadStat=THREAD_STAT_DEAD;
        else if(retStat==EINVAL)
            return P_FAILRE;
        else
        {
            if(flags)
                pthread_kill(sok_que->pr_write.write_id, SIGRTMIN+3);
            else
                pthread_kill(sok_que->pr_write.write_id, SIGRTMIN+1);
        }
    }
    
    return P_SUCCESS;
}

int setSignal()
{
    sigset_t bset, oset;
    sigemptyset(&bset);
    sigaddset(&bset, SIGRTMIN+1);
    sigaddset(&bset, SIGRTMIN+2);
    sigaddset(&bset, SIGRTMIN+3);
    sigaddset(&bset, SIGRTMIN+4);

    if (pthread_sigmask(SIG_BLOCK, &bset, &oset) != 0)
        return P_FAILRE;
    return P_SUCCESS;
}


int startPrvNxtThreadAndPrvConn()
{
    SOCK_QUEUE *tmp_sok_prv_que= NULL, *tmp_sok_nxt_que= NULL;
    volatile NX_KEY_BUF *tp_nx_key_buf = NULL;
    int i=0;
    pthread_t startThreadPid=0;
    
    tmp_sok_prv_que = (SOCK_QUEUE *)sok_que_base;
    tmp_sok_nxt_que = (SOCK_QUEUE *)sok_que_base_nx;
    tp_nx_key_buf = nx_key_buf;

    if(setSignal())
        return P_FAILRE;
    
    for(i=0; i<gNxtCfgMax; i++)
    {
        if(tmp_sok_nxt_que[i].used==QUEUE_NODE_NXT_OK)
        {
            if(pthread_create(&startThreadPid, NULL, start_nxt_thread, &tmp_sok_nxt_que[i]))
            {
                sendMobileMessageForAlert("start start_nxt_thread error");
                showmsg_pth(ERROR,"pthread_create..start_nxt_thread   error...  FAILED ....[%s].\n", strerror(errno));
                return P_FAILRE;
            }
        }
    }

    for(i=0; i<gPrvCfgMax; i++)
    {
        if(tmp_sok_prv_que[i].used==QUEUE_NODE_PRV_MID)
        {
            if(pthread_create(&startThreadPid, NULL, start_prv_thread, &tmp_sok_prv_que[i]))
            {   
                sendMobileMessageForAlert("start start_prv_thread error");
                showmsg_pth(ERROR,"pthread_create..start_prv_thread   error...  FAILED .....[%s].\n", strerror(errno));
                return P_FAILRE;
            }
        }
    }
    usleep(3*1000);
    
    return P_SUCCESS;
}

int SendData(int fd, unsigned char * buf, int length)
{
    int bytes_left;
    int written_bytes;
    unsigned char *ptr=NULL;

    if(buf==NULL || fd<=0 || length<0)
    {
        sendMobileMessageForAlert("SendData buf==NULL || fd<0 || length<0");
        showmsg_pth(ERROR,"SendData  buf==NULL || fd<0 || length<0..\n");
        return P_FAILRE;
    }

    ptr=buf;
    bytes_left=length;
    while(bytes_left>0)
    {                              
        written_bytes=send(fd, (const char *)ptr, bytes_left, 0);
        if(written_bytes<0)
        {       
            if(errno==EAGAIN || errno==EWOULDBLOCK||errno==ENOBUFS)
            {
                usleep(10);
                continue;
            }
            else if(errno==EINTR)
                continue;
            else if(errno==EPIPE||errno==ECONNRESET||errno==EBADF||errno==EACCES||errno==EDESTADDRREQ||errno==EFAULT||errno==EINVAL||errno==ENOTCONN||errno==ENOTSOCK||errno==ENOMEM)
                return P_FD_CLOSED;            
            else
            {
                showmsg_pth(ERROR,"丢弃报文, 继续处理下一笔 error is...[%s].\n", strerror(errno));
                return P_FAILRE;
            }
        }
        bytes_left-=written_bytes;
        ptr+=written_bytes;     
    }
    return P_SUCCESS;
}

int RecvData(int fd, unsigned char * buf, int length)  
{  
    int recvLen = 0, needLens=0;  
    unsigned char *rcBuf=NULL;  

    if(buf==NULL || fd<=0)
    {
        sendMobileMessageForAlert("RecvData buf==NULL || fd<=0");
        showmsg_pth(ERROR,"RecvData  buf==NULL || fd<=0..\n");
        return P_FAILRE;
    }

    rcBuf = buf;
    needLens = length;
	while (needLens > 0) 
    {
        if((recvLen = recv(fd, rcBuf, needLens, 0)) < 0)     
        {
			if(errno == EINTR || errno==EAGAIN || errno==EWOULDBLOCK)
			{
			    usleep(10);
                continue;
            }
			else
			    return P_FD_CLOSED;
		}
        else if (recvLen == 0)
			return P_FD_CLOSED;

		needLens -= recvLen;
		rcBuf   += recvLen;
	}
	return P_SUCCESS;
}  


int eventSet(short type, int sockfd)
{
    int epollfd;
    epollfd=epoll_create(MAX_PRE_EVENTS);
    struct epoll_event event;
    switch(type)
    {//default  behavior  is  LT,  可以显示设置ET，不过没必要，默认LT
        case RD_READ:
            event.events=EPOLLIN|EPOLLRDHUP|EPOLLERR|EPOLLHUP;
            break;
        case RD_WRITE:
            event.events=EPOLLOUT|EPOLLRDHUP|EPOLLERR|EPOLLHUP;
            break;
        default:
            return P_FAILRE;
    }
    event.data.fd=sockfd;

    if(epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&event)<0)
    {
        showmsg_pth(ERROR,"epoll 加入失败 fd==%d.....error...\n", sockfd);        
        return P_FAILRE;
    }

    return epollfd;
}

int createConnect(volatile CFG_CONN_PRV_NXT *prv_cfg)
{
    int fdSock=-1, selres=0, tmp_error=0;
    struct sockaddr_in srvaddr;
    fd_set rfds, wfds;    
    struct timeval tv;    

    if(!prv_cfg)
    {
        sendMobileMessageForAlert("createConnect prv_cfg is null");
        showmsg_pth(ERROR,"createConnect prv_cfg is null.\n");
        return P_FAILRE;
    }

    memset(&srvaddr, 0, sizeof(struct sockaddr_in));
    srvaddr.sin_addr.s_addr = inet_addr((const char *)prv_cfg->send_ip);
    srvaddr.sin_port = htons(atoi((const char *)prv_cfg->send_port));
    srvaddr.sin_family = AF_INET;
    
    do{
        fdSock = socket(AF_INET, SOCK_STREAM, 0);
        usleep(1000*100);
    }while(fdSock < 0);

    if(set_svr_opt(fdSock)) 
    {
        close(fdSock);
        showmsg_pth(ERROR,"set_ver_opt failed.....error...\n");
        return P_FAILRE;
    }

    if(set_fd_nonb(fdSock))
    {
        close(fdSock);
        showmsg_pth(ERROR," set_fd_nonb failed......error...\n");        
        return P_FAILRE;
    }

    if(connect(fdSock, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0)
    {   
        if(errno==EINPROGRESS || errno==EALREADY || errno==EWOULDBLOCK)
        {   
EINPROGRESS_TAG:        
            FD_ZERO(&rfds);    
            FD_ZERO(&wfds);    
            FD_SET(fdSock, &rfds);    
            FD_SET(fdSock, &wfds);    

            tv.tv_sec = 1;
            tv.tv_usec = 0;    
            selres = select(fdSock + 1, &rfds, &wfds, NULL, &tv);    
            if(selres<0)
            {
                close(fdSock);
                return P_FAILRE;
            }
            else if(selres==0)
            {
                close(fdSock);
                return P_FAILRE;
            }
            
            if(FD_ISSET(fdSock, &rfds) || FD_ISSET(fdSock, &wfds))
            {         
                connect(fdSock, (struct sockaddr *)&srvaddr, sizeof(srvaddr));    
				tmp_error = errno;    
				if(tmp_error == EISCONN)
					return fdSock;  
                else if(tmp_error == EINPROGRESS || errno==EALREADY || errno==EWOULDBLOCK)
                {   
                    usleep(1000*10);
                    goto EINPROGRESS_TAG;
                }
				else
				{
                    showmsg_pth(ERROR,"connect=======失败======.ip=[%s]..port=[%s].....[%s]\n", prv_cfg->send_ip, prv_cfg->send_port, strerror(errno));
                    close(fdSock);
                    return P_FAILRE;
				}
            }
        }      
        else if(errno==EISCONN)
            return fdSock;
        else 
        {
            showmsg_pth(ERROR,"connect 失败  ..ip=[%s]..port=[%s].....[%s]\n", prv_cfg->send_ip, prv_cfg->send_port, strerror(errno));
            close(fdSock);
            return P_FAILRE;
        }
    }

    return fdSock;
}

int startNxtListenAndConnect()
{   
    volatile SOCK_QUEUE *sok_que_nx=NULL;

    sok_que_nx = sok_que_base_nx;
    if(NxtListenAndConnect(sok_que_nx, gNxtCfgMax, QUEUE_NODE_INIT, QUEUE_NODE_NXT_OK))
    {
        sendMobileMessageForAlert("startNxtListenAndConnect NxtListenAndConnect error");
        showmsg_pth(ERROR,"startNxtListenAndConnect    NxtListenAndConnect    FAILED   FAILED .....");        
        return P_FAILRE;
    }

    return  P_SUCCESS;
}

int startPrvConnect(volatile SOCK_QUEUE * tmp_sok_que)
{   
    int sockfdWR=0;

    if(!tmp_sok_que)
    {
        sendMobileMessageForAlert("startPrvConnect tmp_sok_que is null");
        showmsg_pth(ERROR,"startPrvConnect tmp_sok_que is null.\n");
        return P_FAILRE;
    }

    do{
        sockfdWR=createConnect(&(tmp_sok_que->cfg));
        usleep(100*1000);
    }while(sockfdWR==P_FAILRE);         

    tmp_sok_que->pr_write.pro_mth=SendData;
    tmp_sok_que->pr_write.write_fd=sockfdWR;
    tmp_sok_que->used = QUEUE_NODE_PRV_OK;

    return  P_SUCCESS;
}


int reStartPrvListen(volatile SOCK_QUEUE *sok_que)
{   
    int sockfdRD=0, epollfdRD=0;

    if(!sok_que)
    {
        sendMobileMessageForAlert("reStartPrvListen sok_que is null");
        showmsg_pth(ERROR,"reStartPrvListen sok_que is null.\n");
        return P_FAILRE;
    }

    sockfdRD =createListenSock(&(sok_que->cfg));    
    if(sockfdRD == P_FAILRE)
    {
        sendMobileMessageForAlert("reStartPrvListen createListenSock    FAILED   FAILED");
        showmsg_pth(ERROR,"createListenSock    FAILED   FAILED ... FAILED .....");
        return P_FAILRE;
    }
    epollfdRD=eventSet(RD_READ, sockfdRD);
    if(epollfdRD==P_FAILRE)
    {
        sendMobileMessageForAlert("reStartPrvListen eventSet    FAILED   FAILED");
        showmsg_pth(ERROR,"eventSet    FAILED   FAILED ... FAILED .....");        
        return P_FAILRE;
    }

    sok_que->pr_read.pro_mth=RecvData;
    sok_que->pr_read.read_fd=-1;
    sok_que->pr_read.lisFd=sockfdRD;
    sok_que->pr_read.epFd=epollfdRD;
    sok_que->used = QUEUE_NODE_PRV_MID;
    
    return  P_SUCCESS;
}


int startPrvListen()
{   
    SOCK_QUEUE *sok_que=NULL;
    int sockfdRD=0, epollfdRD=0, i=0;
  
    sok_que = (SOCK_QUEUE *)sok_que_base;
    for(i=0; i<gPrvCfgMax && sok_que[i].cfg.listen_ip[0]!='\0' && sok_que[i].used==QUEUE_NODE_INIT; i++)
    {
        sockfdRD =createListenSock(&(sok_que[i].cfg));    
        if(sockfdRD == P_FAILRE)
        {
            sendMobileMessageForAlert("startPrvListen createListenSock error");
            showmsg_pth(ERROR,"createListenSock    FAILED   FAILED ... FAILED .....");
            return P_FAILRE;
        }
        epollfdRD=eventSet(RD_READ, sockfdRD);
        if(epollfdRD==P_FAILRE)
        {
            sendMobileMessageForAlert("startPrvListen eventSet error");
            showmsg_pth(ERROR,"eventSet    FAILED   FAILED ... FAILED .....");
            return P_FAILRE;
        }
       sok_que[i].pr_read.pro_mth=RecvData;
       sok_que[i].pr_read.read_fd=-1;
       sok_que[i].pr_read.lisFd=sockfdRD;
       sok_que[i].pr_read.epFd=epollfdRD;
       sok_que[i].used = QUEUE_NODE_PRV_MID;
    }

    return  P_SUCCESS;
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
        return P_FAILRE;
    if(setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle))) 
        return P_FAILRE;
    if(setsockopt(sockfd, IPPROTO_TCP,TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval))) 
        return P_FAILRE;
    if(setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount))) 
        return P_FAILRE;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) 
        return P_FAILRE;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &ru, sizeof(ru))) 
        return P_FAILRE;
    
    if(setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &lg, sizeof(lg))) 
        return P_FAILRE;
    
    if(setsockopt(sockfd, SOL_SOCKET, SO_RCVLOWAT, (char *)(&n), sizeof(n))) 
        return P_FAILRE;
    return P_SUCCESS;
}


int set_fd_nonb(int fd)
{
	int	flags;

	if ((flags = fcntl(fd, F_GETFL, 0)) < 0)
		return P_FAILRE;

	if ( fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		return P_FAILRE;

	return P_SUCCESS;
}

int  createListenSock(volatile CFG_CONN_PRV_NXT  *prv_cfg)
{
    int listenfd;
    struct sockaddr_in servaddr;       

    if(!prv_cfg)
    {
        sendMobileMessageForAlert("createListenSock prv_cfg is null");
        showmsg_pth(ERROR,"createListenSock prv_cfgis null.\n");
        return P_FAILRE;
    }

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
        return P_FAILRE;
    }

    if(set_fd_nonb(listenfd))
    {
        showmsg_pth(ERROR," set_fd_nonb failed...... errno:%d-%s...\n", errno, strerror(errno));    
        close(listenfd);
        return P_FAILRE;
    }
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_addr.s_addr = inet_addr((void*)prv_cfg->listen_ip);  
    servaddr.sin_port = htons(atoi((void*)prv_cfg->listen_port));    
    servaddr.sin_family = AF_INET;
    
    if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)))
    {
        showmsg_pth(ERROR," bind failed...... errno:%d-%s...\n", errno, strerror(errno));     
        close(listenfd);
        return P_FAILRE;
    }

    if(listen(listenfd, MAX_CONN)==-1)
    {
        showmsg_pth(ERROR," listen failed...... errno:%d-%s...\n", errno, strerror(errno));
        close(listenfd);
        return P_FAILRE;
    }   
    
    return listenfd;
}

int initMsgQueueForAlarm()
{
    alarmMsgQueueId = msgget(ALARM_MSG_QUEUE_KEY, 0666 | IPC_CREAT); 
    if(alarmMsgQueueId == -1)  
    {  
        showmsg_pth(ERROR,"initMsgQueueForAlarm... errno:%d-%s...\n", errno, strerror(errno));
        return P_FAILRE;  
    } 
    return P_SUCCESS;
}

void Create_Daemon() 
{ 
    int pid; 
    char *ptr_home=NULL, tmp_home[128];

    pid = fork();
    if( pid != 0 ) 
        exit(0); 
    else if(pid< 0) 
        exit(1);
    
    setsid();
    
    pid=fork();
    if(pid !=0 ) 
        exit(0); 
    else if(pid< 0)
        exit(1);
    
    ptr_home = getenv("HOME");
    if(!ptr_home)
    {
        perror("$HOME is  null.....");
        exit(-1);
    }
    memset(tmp_home, 0x00, 128);
    snprintf(tmp_home, 128, "%s/proxy/bin/", ptr_home);

    memset(xBinPath, 0x00, X_CMD_PATH);
    memset(prevFileName, 0x00, X_CMD_PATH);
    memset(nextFileName, 0x00, X_CMD_PATH);

    memcpy(xBinPath, tmp_home, strlen(tmp_home));
    snprintf(prevFileName, X_CMD_PATH, "%s/proxy/etc/prev_listen_cfg", ptr_home);
    snprintf(nextFileName, X_CMD_PATH,  "%s/proxy/etc/next_cpus_conn_cfg", ptr_home);

    chdir(tmp_home); 
    /* 重设文件创建掩模  */
    umask(0);
    
    return; 
} 


int  createShmSpace(int *shmId)
{
    int shm_id = 0;
    key_t  shm_key = 0;

    if(!shmId)
    {
        sendMobileMessageForAlert("createShmSpace shmId is null");
        showmsg_pth(ERROR,"createShmSpace shmId is null.\n");
        return P_FAILRE;
    }

    shm_key = ftok(xBinPath, atoi("jll"));
    if(shm_key<0)
    {
        showmsg_pth(ERROR, "ftok  error  error ...shm_key==%d.%s\n", shm_key, strerror(errno));
        return P_FAILRE;
    }

    shm_id = shmget(shm_key, SHM_SIZES, IPC_CREAT|IPC_EXCL|0666);
    if(shm_id < 0)
    {
        if(errno==EEXIST)
        {
            shm_id = shmget(shm_key, SHM_SIZES, 0666);
            if(shm_id < 0)
            {
                showmsg_pth(ERROR, "获取已经存在的shm_id错误,,,errstr=%s\n", strerror(errno));
                return P_FAILRE;
            }
            *shmId = shm_id;

            if(shmctl(shm_id, IPC_RMID, 0) < 0)
            {
                showmsg_pth(ERROR, "删除已经存在的共享内存...失败...失败....\n");
                return P_FAILRE;
            }
            shm_id = shmget(shm_key, SHM_SIZES, IPC_CREAT|IPC_EXCL|0666);
            if(shm_id < 0)
            {
                showmsg_pth(ERROR, "创建新的共享内存失败....error====%s\n", strerror(errno));
                return P_FAILRE;                
            }
            *shmId = shm_id;
            showmsg_pth(ERROR, "创建共享内存.. 成功..shm_id==%d\n", shm_id);
            return P_SUCCESS;
        }
    }
    
    *shmId = shm_id;

    return P_SUCCESS;
}

int shmHandle()
{
    int shm_id = 0;
    
    if(createShmSpace(&shm_id))
    {
        showmsg_pth(ERROR, "初始化共享内存 失败....error.....error.....\n");
        return P_FAILRE;
    }

    if(attchShmSpace(&shm_id))
    {
        showmsg_pth(ERROR, "attchShmSpace  连接共享内存 失败......error.....error.....\n");
        return P_FAILRE;
    }
    return P_SUCCESS;
}

int attchShmSpace(int *shmId)
{
    void *tmpShmAddr = NULL;

    if(!shmId)
    {
        sendMobileMessageForAlert("attchShmSpace shmId is null");
        showmsg_pth(ERROR,"attchShmSpace shmId is null.\n");
        return P_FAILRE;
    }
    
    tmpShmAddr = shmat(*shmId, NULL, 0);
    if(tmpShmAddr < 0)
    {
        showmsg_pth(ERROR, "attchShmSpace  shmat  error  error  %s\n", strerror(errno));        
        return P_FAILRE;
    }
    memset(tmpShmAddr, 0x00, SHM_SIZES);
    pShmAddr = tmpShmAddr;
    return P_SUCCESS;
}

void sendMobileMessageForAlert(char *strProblem)
{ 
    int iRetCode = -1;
    MSG_ALARM  msg ; 

    if(strProblem==NULL ||  alarmMsgQueueId<0)
    {
        showmsg_pth(ERROR, "sendMobileMessageForAlert strProblem  is null \n");    
        return ;
    }
    msg.mtype=9;
    memset(msg.mtext, 0x00, 512);    
    snprintf(msg.mtext,  512, "%s", strProblem);
    iRetCode =  msgsnd(alarmMsgQueueId, &msg, strlen(msg.mtext), IPC_NOWAIT);
    if (iRetCode == -1)
    {
        showmsg(ERROR,"写入数据到消息队列失败....丢弃....errno===%s\n", strerror(errno));
        return ;
    }
}


#include "cPub.h"
#include "log.h"


int main()
{

	InitLog("proxy");        
    Create_Daemon();

	//初始化 报警 消息队列
    if(initMsgQueueForAlarm())
    {
        showmsg_pth(ERROR,"initMsgQueueForAlarm ....error.....\n");            
        return P_FAILRE;
    }
    
    sendMobileMessageForAlert("proxy..start.");

    showmsg_pth(DEBUG,"proxy..........start..........\n");

	//创建共享内存， 存储队列 基地址
    if(shmHandle())
    {
        showmsg_pth(ERROR,"shmHandle ....error.....\n");            
        return P_FAILRE;
    }

	//根据配置  初始化  链接队列  数据队列, 以及绑定
    if(initSockQByCfgAndDataQByCfg())
    {
        showmsg_pth(ERROR,"initSockQByCfgAndDataQByCfg  FAILED...error......\n");             
        return P_FAILRE;
    }

///-------------------------------------------------------------------
	// 红黑树 做 数据池 线程池     20190628 新增 jll

	//利用 rb tree 的深度遍历 pick 线程   20190718  jll  改变现在广度pick算法

	/*  20190718 思考 
	
		从新 扩展  现有 pick : 1, pick thread, pick  data block

		是 准确pick thread from tree 好点呢，这样tree好维护,但要维护两套tree， 也离不开lock
		还是 thread 随机注册，随机signal唤醒，让系统去选择 呢 ??????????????????? 纠结
		
	*/

	//获取 REQ 内存节点
	//getTreeNodeFromMemPool(M_REQ_MEM_POOL, &p_tmp_get_mem);

	//获取 RSP 内存节点
	//getTreeNodeFromMemPool(M_RSP_MEM_POOL, &p_tmp_get_mem);

	//释放 REQ 内存节点
	//putTreeNodeToMemPool(M_REQ_MEM_POOL, p_tmp_get_mem);

	//释放 RSP 内存节点
	//putTreeNodeToMemPool(M_REQ_MEM_POOL, p_tmp_get_mem);

	//初始化 内存池
	if(initMemPool())
	{
		showmsg_pth(ERROR,"initMemPool 失败...[%s]\n", strerror(errno));
		return X_FAILRE;
	}

	//初始化 数据池
	initDataPool();

	//初始化 线程池
	initThreadPool();







//----------------------------------------------------------

	//和后端的关系，启动本地服务，主动链接后端
    if(startNxtListenAndConnect())
    {
        showmsg_pth(ERROR,"startNxtListenAndConnect nxt listen and connect FAILED...error........\n");    
        return P_FAILRE;
    }

	//启动前端  监听
    if(startPrvListen())
    {
        showmsg_pth(ERROR,"startPrvListen prv listen FAILED...error..........\n");          
        return P_FAILRE;
    }

	//启动业务处理线程, 启动前端的主动链接线程和读写的工作线程,启动后端读写工作线程
    if(startThreadForWork())
    {
        showmsg_pth(ERROR,"startThreadForWork  FAILED...error..........\n"); 
        return P_FAILRE;        
    }

	//启动数据队列维护 和 链接队列维护线程
    checkThreadAndKeyQueue();

    return P_SUCCESS;
}

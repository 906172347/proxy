#include "cPub.h"
#include "log.h"


int main()
{

	InitLog("proxy");        
    Create_Daemon();

	//��ʼ�� ���� ��Ϣ����
    if(initMsgQueueForAlarm())
    {
        showmsg_pth(ERROR,"initMsgQueueForAlarm ....error.....\n");            
        return P_FAILRE;
    }
    
    sendMobileMessageForAlert("proxy..start.");

    showmsg_pth(DEBUG,"proxy..........start..........\n");

	//���������ڴ棬 �洢���� ����ַ
    if(shmHandle())
    {
        showmsg_pth(ERROR,"shmHandle ....error.....\n");            
        return P_FAILRE;
    }

	//��������  ��ʼ��  ���Ӷ���  ���ݶ���, �Լ���
    if(initSockQByCfgAndDataQByCfg())
    {
        showmsg_pth(ERROR,"initSockQByCfgAndDataQByCfg  FAILED...error......\n");             
        return P_FAILRE;
    }

///-------------------------------------------------------------------
	// ����� �� ���ݳ� �̳߳�     20190628 ���� jll

	//���� rb tree ����ȱ��� pick �߳�   20190718  jll  �ı����ڹ��pick�㷨

	/*  20190718 ˼�� 
	
		���� ��չ  ���� pick : 1, pick thread, pick  data block

		�� ׼ȷpick thread from tree �õ��أ�����tree��ά��,��Ҫά������tree�� Ҳ�벻��lock
		���� thread ���ע�ᣬ���signal���ѣ���ϵͳȥѡ�� �� ??????????????????? ����
		
	*/

	//��ȡ REQ �ڴ�ڵ�
	//getTreeNodeFromMemPool(M_REQ_MEM_POOL, &p_tmp_get_mem);

	//��ȡ RSP �ڴ�ڵ�
	//getTreeNodeFromMemPool(M_RSP_MEM_POOL, &p_tmp_get_mem);

	//�ͷ� REQ �ڴ�ڵ�
	//putTreeNodeToMemPool(M_REQ_MEM_POOL, p_tmp_get_mem);

	//�ͷ� RSP �ڴ�ڵ�
	//putTreeNodeToMemPool(M_REQ_MEM_POOL, p_tmp_get_mem);

	//��ʼ�� �ڴ��
	if(initMemPool())
	{
		showmsg_pth(ERROR,"initMemPool ʧ��...[%s]\n", strerror(errno));
		return X_FAILRE;
	}

	//��ʼ�� ���ݳ�
	initDataPool();

	//��ʼ�� �̳߳�
	initThreadPool();







//----------------------------------------------------------

	//�ͺ�˵Ĺ�ϵ���������ط����������Ӻ��
    if(startNxtListenAndConnect())
    {
        showmsg_pth(ERROR,"startNxtListenAndConnect nxt listen and connect FAILED...error........\n");    
        return P_FAILRE;
    }

	//����ǰ��  ����
    if(startPrvListen())
    {
        showmsg_pth(ERROR,"startPrvListen prv listen FAILED...error..........\n");          
        return P_FAILRE;
    }

	//����ҵ�����߳�, ����ǰ�˵����������̺߳Ͷ�д�Ĺ����߳�,������˶�д�����߳�
    if(startThreadForWork())
    {
        showmsg_pth(ERROR,"startThreadForWork  FAILED...error..........\n"); 
        return P_FAILRE;        
    }

	//�������ݶ���ά�� �� ���Ӷ���ά���߳�
    checkThreadAndKeyQueue();

    return P_SUCCESS;
}

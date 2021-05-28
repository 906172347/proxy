#include "cPub.h"
#include "log.h"


int main(int argc, char*argv[])
{
	char prograName[32];

	printf("argc==%d, argv[0]==%s, argv[1]\n", argc, argv[0], argv[1]);
	memset(prograName,0x00, sizeof(prograName));
	memcpy(prograName, argv[0]+2, strlen(argv[0])-2);
	InitLog("prograName");        
//    Create_Daemon();

    if(initMsgQueueForAlarm())
    {
        showmsg_pth(ERROR,"initMsgQueueForAlarm ....error.....\n");            
        return P_FAILRE;
    }
    
    sendMobileMessageForAlert("proxy..start.");

    showmsg_pth(DEBUG,"proxy..........start..........\n");

    if(shmHandle())
    {
        showmsg_pth(ERROR,"shmHandle ....error.....\n");            
        return P_FAILRE;
    }

    if(initSockQByCfgAndDataQByCfg())
    {
        showmsg_pth(ERROR,"initSockQByCfgAndDataQByCfg  FAILED...error......\n");             
        return P_FAILRE;
    }
    
    if(startNxtListenAndConnect())
    {
        showmsg_pth(ERROR,"startNxtListenAndConnect nxt listen and connect FAILED...error........\n");    
        return P_FAILRE;
    }
    
    if(startPrvListen())
    {
        showmsg_pth(ERROR,"startPrvListen prv listen FAILED...error..........\n");          
        return P_FAILRE;
    }

    if(startThreadForWork())
    {
        showmsg_pth(ERROR,"startThreadForWork  FAILED...error..........\n"); 
        return P_FAILRE;        
    }

    checkThreadAndKeyQueue();

    return P_SUCCESS;
}

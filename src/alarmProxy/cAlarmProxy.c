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

typedef struct msgbuf {
    long mtype;       
    char mtext[512];
}MSG_ALARM;

#define  P_FAILRE       -1
#define  P_SUCCESS      0

static char monitorMsgUrl[512];
static int alarmMsgQueueId = -1;


char *GetIniKeyStringProxy(char *title, char *key, char *filename)  
{  
    FILE *fp;  
    int  flag = 0;  
    char sTitle[32], *wTmp;
    static char sLine[1024];  

    memset(sTitle, 0x00, 32);
    memset(sLine, 0x00, 1024);    
    snprintf(sTitle, 32, "[%s]", title);  
    if(NULL == (fp = fopen(filename, "r"))) {
        perror("fopen");  
        return NULL;  
    }  
  
    while (NULL != fgets(sLine, 1024, fp)) {  
        // 这是注释行  
        if (0 == strncmp("//", sLine, 2)) continue;  
        if ('#' == sLine[0])              continue;  
  
        wTmp = strchr(sLine, '=');  
        if ((NULL != wTmp) && (1 == flag)) {  
            if (0 == strncmp(key, sLine, wTmp-sLine)) { // 长度依文件读取的为准  
                sLine[strlen(sLine) - 1] = '\0';  
                fclose(fp);  
                return wTmp + 1;  
            }  
        } else {  
            if (0 == strncmp(sTitle, sLine, strlen(sLine) - 1)) { // 长度依文件读取的为准  
                flag = 1; // 找到标题位置  
            }  
        }  
    }  
    fclose(fp);  
    return NULL;  
}  

void sendMobileMessageForAlert_bak(char *strProblem)
{ 
    CURL *curl;
    
    if(strProblem==NULL || monitorMsgUrl==NULL)
    { 
        perror("strProblem  is null ");
        return ;
    }
    memset(monitorMsgUrl+strlen(monitorMsgUrl), 0x00, 512-strlen(monitorMsgUrl));
    snprintf(monitorMsgUrl+strlen(monitorMsgUrl), 512-strlen(monitorMsgUrl), "%s", strProblem);
    curl = curl_easy_init();  
    if(curl)
    {  
        curl_easy_setopt(curl, CURLOPT_URL, monitorMsgUrl);   
        curl_easy_perform(curl);  
        curl_easy_cleanup(curl);  
    }
}


int initMsgQueueForAlarm()
{
    alarmMsgQueueId = msgget(99999, 0666 | IPC_CREAT); 
    if(alarmMsgQueueId == -1)  
    {  
        perror("alarmMsgQueueId == -1");
        return P_FAILRE;  
    } 
    return P_SUCCESS;
}

void Create_Daemon() 
{ 
    int pid; 
    char *ptr_home=NULL, *pstr=NULL, tmp_home[128], mobileCfgFile[512], sMobileNo[512], sPath[512];

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

    memset(mobileCfgFile, 0x00, 512);
    snprintf(mobileCfgFile, 512,  "%s/proxy/etc/msg.ini", ptr_home);

    pstr=GetIniKeyStringProxy("PROXY_ALERT", "path", mobileCfgFile);
    if(!pstr)
    {
        perror("GetIniKeyStringProxy PROXY_ALERT path"); 
        exit(-1);
    }
    memset(sPath, 0x00, 512);
    snprintf(sPath, 512, "%s", pstr);
    pstr=GetIniKeyStringProxy("PROXY_ALERT", "mobile", mobileCfgFile);
    if(!pstr)
    {
        perror("GetIniKeyStringProxy PROXY_ALERT mobile");
        exit(-1);
    }
    memset(sMobileNo, 0x00, 512);
    snprintf(sMobileNo, 512, "%s", pstr);

    memset(monitorMsgUrl, 0x00, 512);
    snprintf(monitorMsgUrl, 512, "%smobile=%s&msg=proxyAlarm:", sPath, sMobileNo);

    memset(sPath, 0x00, 512);
    snprintf(sPath, 512, "%s/proxy/bin/", tmp_home);
    
    chdir(sPath);
    umask(0);
    return; 
} 



int main()
{
    MSG_ALARM msg;
    int iRetCode=-1;
    
    Create_Daemon();

    if(initMsgQueueForAlarm())
    {
        perror("initMsgQueueForAlarm error");
        exit(-1) ;
    }

    msg.mtype=9;

    while(1)
    {
        if(alarmMsgQueueId<0)
        {
            sendMobileMessageForAlert_bak("alarmMsgQueueId < 0 error");
            return -1;
        }
        memset(&msg, 0x00, sizeof(MSG_ALARM)); 
        iRetCode =  msgrcv(alarmMsgQueueId, &msg, 512, 9, 0);
        if(iRetCode == -1)
        {
            perror("msgrcv");
            sleep(1);
            sendMobileMessageForAlert_bak("msgrcv error");
            continue;
        }
        sendMobileMessageForAlert_bak(msg.mtext);
    }




	return 0;
}

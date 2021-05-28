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
#include <time.h>
#include <sys/times.h>
#include <stdarg.h>

#include "log.h"

volatile FILE *fp = NULL;

#define uchar unsigned char
#define MAXBUFLEN				8192	//最大缓冲池大小


int rtrim( char *Pts )
{
	int  i;
	
	i = strlen(Pts);
	/*for(;i>=0;i--) 1999.9.21**/
	for(;i>0;i--)
	{
		if(( Pts[i-1] == 0x20)||( Pts[i-1] == 0x09))
			Pts[i-1] = 0;
		else
			break;
	}
	return(i);
}

int ltrim( char *Pts )
{
	int  i,j;
	char s[250];

	memset(s,0,sizeof(s));
	j = strlen(Pts);
	for(i=0;i<j;i++)
	{
		if(( Pts[i] == 0x20)||( Pts[i] == 0x09)) continue;
		else{
			if(i==0)return 0;
			memcpy(s,Pts+i,j-i);
			memcpy(Pts,s,j-i);
			Pts[j-i]=0;
			break;
		}
	}
	return(strlen(s));
}

int trim(char *Pts)
{
	int iLen;
	char *p;

	p=Pts;
	iLen=ltrim(p);

	p=Pts;
	iLen=rtrim(p);

	return iLen;
}

int MemCmp( char *psStr1, char *psStr2 )
{
	char str1[1024],*ptr1;
	char str2[1024],*ptr2;

	memset(str1,0,sizeof(str1));
	memset(str2,0,sizeof(str2));
	sprintf(str1,"%s",psStr1);
	sprintf(str2,"%s",psStr2);

	ptr1=str1;ptr2=str2;

	trim(str1);
	trim(str2);
   if( strlen(str1) != strlen(str2) )   return(-1);

   while( toupper(*ptr1) == toupper(*ptr2) )
   {
       if( *ptr1 == '\0' )   break;
       ptr1++;
       ptr2++;
   }
   if( *ptr1 == 0 ) return(0);
   return(-1);
}

int ReadLine(int iFd,char *psLine,int *iLineLen,int *iOffset)
{
	int n,iSet;
	char sLine[MAXBUFLEN];
	char *pLine;

	memset(sLine,0,sizeof(sLine));
	pLine=sLine;
	iSet=*iOffset;
	lseek(iFd,iSet,SEEK_SET);
	while(1)
	{
		n=read(iFd,pLine,1);
		if(n<=0)break;
		iSet++;
		if(*pLine!=0x0a)
		{
			pLine++;
		}
		else break;
	}
	if(n<0)
	{
		perror("read");
		return -1;
	}
	else
	{
		*iLineLen=pLine-sLine;
		*iOffset=iSet;
		memcpy(psLine,sLine,*iLineLen);
		return n;
	}
}

int ReadEntry(int iFd,char *psEntry,char *psValue,int *piOffSet)
{
	int iRet,n,iLen=0,iLineLen;
	char sLine[MAXBUFLEN],*ptr,*pFind;
	char sEntry[255];

	n=1;
	while(n>0)
	{
		memset(sLine,0,sizeof(sLine));
		ptr=sLine;
		n=ReadLine(iFd,sLine,&iLineLen,piOffSet);
		/*
		fprintf(stderr,"line=[%s]\n",sLine);
		fprintf(stderr,"n=[%d],linelen=[%d],set=[%d]\n",n,iLineLen,*piOffSet);*/
		if(n<0)
		{
			fprintf(stderr,"[%s][%d]n=[%d]\n",__FILE__,__LINE__,n);
			return -1;
		}
		else
		{
			/*行预处理，去除行首TAB和空格*/
			while( *ptr==0x20 || *ptr==0x09 ) ptr++;
			/*去除注释行*/
			if(*ptr=='#')continue;
			/*去除空行*/
			if( *ptr=='\0' || *ptr==0x0d || *ptr==0x0a )continue;
         	/*发现标题符号，结束*/
         	if(*ptr=='[')
        	{
				while( *ptr!=']' && *ptr!='\0' )ptr++;
				/*发现']'*/
				if(*ptr==']')break;
         	}
         	/*查找entry*/
         	pFind=ptr;
         	while( *ptr!='=' && *ptr!='\0' )ptr++;
         	if(*ptr=='=')
         	{
         		memset(sEntry,0,sizeof(sEntry));
				memcpy(sEntry,pFind,ptr-pFind);
				iRet=MemCmp(sEntry,psEntry);
				if(iRet<0)
				{
					continue;
				}
				else{
					iLen=ptr-sLine;
					if(iLen>0)
					memcpy(psValue,ptr+1,strlen(sLine)-iLen-1);

					return *piOffSet-iLineLen;/*返回行首指针*/
				}
         	}

		}
	}
	fprintf(stderr,"[%s][%d]No Found Entry=[%s]\n",__FILE__,__LINE__,psEntry);
	return -1;
}


int ReadString(char *psFileName,char *psSection,char *psEntry,char *psValue)
{
	int iRet,n=1,iFd,iLineLen=0,iOffSet=0;
	char sLine[MAXBUFLEN],*ptr,*pFind;
	char sSection[255];

	iFd=open(psFileName,O_RDONLY);
	if(iFd<0)
	{
		fprintf(stderr,"[%s][%d]open=[%s]\n",__FILE__,__LINE__,strerror(errno));
		return -1;
	}
	while(n>0)
	{
		memset(sLine,0,sizeof(sLine));
		ptr=sLine;
		n=ReadLine(iFd,sLine,&iLineLen,&iOffSet);
		/*
		fprintf(stderr,"line=[%s]\n",sLine);
		fprintf(stderr,"n=[%d],linelen=[%d],set=[%d]\n",n,iLineLen,iOffSet);*/
		if(n<0)
		{
			fprintf(stderr,"[%s][%d]n=[%d]\n",__FILE__,__LINE__,n);
			close(iFd);
			return -1;
		}
		else
		{
			/*行预处理，去除行首TAB和空格*/
			while( *ptr==0x20 || *ptr==0x09 ) ptr++;
			/*去除注释行*/
			if(*ptr=='#')continue;
			/*去除空行*/
			if( *ptr=='\0' || *ptr==0x0d || *ptr==0x0a )continue;
         	/*取得标题*/
         	if(*ptr!='[') continue;
         	else
        	{
				pFind = ptr;
				while( *ptr!=']' && *ptr!='\0' )ptr++;
				if(*ptr=='\0')continue;
				/*发现']'*/
				if(*ptr==']')
				{

					memset(sSection,0,sizeof(sSection));
					memcpy(sSection,pFind+1,ptr-pFind-1);
					iRet=MemCmp(sSection,psSection);
					if(iRet<0)continue;
					else{
						iRet=ReadEntry(iFd,psEntry,psValue,&iOffSet);
						if(iRet<0)
						{
							close(iFd);
							return -1;
						}
						else
						{
							close(iFd);
							return iRet;
						}
					}
				}
         	}
		}
	}
	close(iFd);
	fprintf(stderr,"[%s][%d]No Found Section=[%s]\n",__FILE__,__LINE__,psSection);
	return -1;

}

int ReadInteger(char *psFileName,char *psSection,char *psEntry,int *piValue)
{
	char sValue[1024];
	int iRet;

	memset(sValue,0,sizeof(sValue));
	iRet=ReadString(psFileName,psSection,psEntry,sValue);
	if(iRet<0)
	{
		fprintf(stderr,"[%s][%d]No Found [%s][%s]\n",__FILE__,__LINE__,psSection,psEntry);
		return -1;
	}
	iRet=trim(sValue);
	if(iRet>8)
	{
		fprintf(stderr,"[%s][%d]Out Length[%s]\n",__FILE__,__LINE__,sValue);
		return -1;
	}
	*piValue=atoi(sValue);
	return 0;
}


/*读取日志配置，日志初始化*/
int	InitLog(char *sProgramName)
{
	char sIniName[256];
	char sTitle[100];
	int iLevel;
	memset(&goLogConfig,0,sizeof(goLogConfig));
	memset(sIniName,0,sizeof(sIniName));
	snprintf(sIniName, 256, "%s", (char *)getenv("LOG_CONFIG_NAME"));


	sprintf(goLogConfig.sLogFilePath,"%s",getenv("LOG_FILE_PATH"));
	sprintf(goLogConfig.sLogFileName,"%s",sProgramName);
	sprintf(sTitle,"loglevel");
	ReadInteger(sIniName,sProgramName,sTitle,&iLevel);
	goLogConfig.iLogLevel=iLevel;
	ReadInteger(sIniName,sProgramName,"logsize",&(goLogConfig.iLogSize));
	ReadInteger(sIniName,sProgramName,"logswitchmode",&(goLogConfig.iLogSwitchMode));

	fprintf(stderr,"[%s]\n",sIniName);
	fprintf(stderr,"InitLog goLogConfig.sLogFilePath[%s]\n",goLogConfig.sLogFilePath);
	fprintf(stderr,"InitLog goLogConfig.sLogFileName[%s]\n",goLogConfig.sLogFileName);
	fprintf(stderr,"InitLog goLogConfig.iLogLevel[%d]\n",goLogConfig.iLogLevel);
	fprintf(stderr,"InitLog goLogConfig.iLogSize[%d]\n",goLogConfig.iLogSize);
	fprintf(stderr,"InitLog goLogConfig.iLogSwitchMode[%d]\n",goLogConfig.iLogSwitchMode);
	return 0;
}

void Write_Log_Hex_pth(char *sData,int iLen, char*output, int *lens)
{

   	unsigned char *pStr;
    int i,j,k;
    int iRow3;
    char sTr1[108];
    char sTmp[1024];
    int iNum, outLen=0;
    char *tmp_out = NULL;

    tmp_out = output;

    iRow3 = ROW1 * ROW2;
    iNum = 1;
    memset(sTr1,0,sizeof(sTr1));
    memset(sTr1,'-',3*iRow3+4+4+2*ROW2);
    for(i = 0 ;i < ROW2;i++)
    {
    	sTr1[4 + (2*ROW1 + 1)*i + ROW1] = 0x30 + i + 1;
    	sTr1[4 + 2*iRow3+ROW2+4 +(ROW1+1)*i + ROW1/2] = 0x30 + i + 1;
    }
    snprintf(tmp_out, 2048, "%s\n%03d ",sTr1,iNum);	
    outLen += strlen(tmp_out);
    iNum ++;

    memset(sTmp, 0x00, 1024);
    
    if(iLen == 0)
    {
   		strcat(tmp_out, "No Data" );
        outLen += 7;
   	}
   	else
   	{
	   	pStr = (uchar *)sData;
	    for( i = 0; i < iLen; i ++,pStr ++ )
	    {
	        memset(sTmp, 0x00, 1024);
	      	snprintf(sTmp, 1024, "%02X", (*pStr)&0xff );
            strcat(tmp_out, sTmp);
            outLen += strlen(sTmp);
	        if((i + 1)%ROW1 == 0 )
	        {
               strcat(tmp_out, " ");
               outLen += 1;
	        }

	       if((i+1)%iRow3 == 0 )
	       {
	      	   pStr -= (iRow3 - 1);
               strcat(tmp_out, " -> ");
               outLen += 4;
	      	   for(j = 0;j < iRow3;j ++,pStr ++)
	      	   {
	      	   	  if((*pStr) < 32 || (*pStr) > 127)
                  {
                      strcat(tmp_out, "*");
                      outLen += 1;
                  }
	   	  		  else
                  {
                      memset(sTmp, 0x00, 1024);
                      snprintf(sTmp, 1024, "%c",*pStr);
                      strcat(tmp_out, sTmp);
                      outLen += 1;
                  }                    
	      	   	      
	      	   	  if((j+1)%ROW1 == 0 ) {
                      strcat(tmp_out, " ");
                      outLen += 1;
	      		  }
	       	   }
	       	   pStr--;
               memset(sTmp, 0x00, 1024);
		  	   snprintf( sTmp, 1024, "\n%03d ",iNum);	
               strcat(tmp_out, sTmp);
               outLen += strlen(sTmp);

               iNum ++;
	      }
	   }



	   k = iLen % iRow3;
	   pStr -= k;
	   if((i % iRow3) != 0)
	   {
	   		do{
                strcat(tmp_out, "  ");
                outLen += 2;
	      		if((i+1)%ROW1 == 0 )
	      		{
                    strcat(tmp_out, " ");
                    outLen += 1;
	      		}
	      		i ++;
	   		}while(i%iRow3!=0);
            strcat(tmp_out, " -> ");
            outLen += 4;
	   		for(j = 0;j < k;j ++,pStr ++)
	   		{
	   		      memset(sTmp, 0x00, 1024);
	      	   	  snprintf(sTmp, 1024, "%c",*pStr);	                  
                  strcat(tmp_out, sTmp);
                  outLen += strlen(sTmp);
	      	   	  if((j+1)%ROW1 == 0 ) {
                      strcat(tmp_out, "  ");
                      outLen += 1;
	      		  }
	       	}

	   }
	}

    strcat(tmp_out, "\n\n");
    outLen += 2;

   *lens = outLen;
}

int GetHostTime( char *pszTime )
{
	time_t t;
	struct tm otm;
	struct tms	otms;

	t = time( NULL );
	memcpy( &otm, localtime(&t), sizeof(struct tm) );
	snprintf( pszTime, 16, "%04d%02d%02d%02d%02d%02d",
		otm.tm_year+1900, otm.tm_mon+1, otm.tm_mday,
		otm.tm_hour, otm.tm_min, otm.tm_sec );

	return times(&otms);
}

int OpenLogFile_pthread (char *sDateTime)
{
	char		sFullLogName[256];
	char		sFullBakLogName[256];
    char        dateTmp[16];
	int			iRet;
    volatile int fd = -1;

	struct stat	oStatBuf;

	/* set log file name */
	memset(sFullLogName,0,sizeof(sFullLogName));
	snprintf (sFullLogName, 256, "%s/%s.log", goLogConfig.sLogFilePath,goLogConfig.sLogFileName);

    memset(dateTmp, 0x00, 16);
    memcpy(dateTmp, sDateTime, 8);
	if (goLogConfig.iLogSwitchMode == LOG_SWITCH_MODE_DATE)
	{
		/* append date in log file name */
		snprintf (sFullLogName+strlen(sFullLogName), 256-strlen(sFullLogName), ".%s", dateTmp);
	}
	else if (goLogConfig.iLogSwitchMode == LOG_SWITCH_MODE_SIZE)
	{
		/* this is LOG_SWITCH_MODE_SIZE */
		/* check file size */
		memset (&oStatBuf, 0x00, sizeof(oStatBuf));
		iRet = stat (sFullLogName, &oStatBuf);
		/*printf("[%ld][%ld]\n",oStatBuf.st_size,LOG_SIZE_UNIT * goLogConfig.iLogSize);*/
		if ( iRet == 0 && oStatBuf.st_size >= LOG_SIZE_UNIT * goLogConfig.iLogSize )
		{
			memset (sFullBakLogName, 0x00, sizeof(sFullBakLogName));
			snprintf (sFullBakLogName, 256, "%s.%s", sFullLogName, sDateTime);
			rename (sFullLogName, sFullBakLogName);
		}
	}
	else if (goLogConfig.iLogSwitchMode == LOG_SWITCH_MODE_DATE_SIZE)
	{
		snprintf (sFullLogName+strlen(sFullLogName), 256-strlen(sFullLogName), ".%s", dateTmp);
		/* check file size */
		memset (&oStatBuf, 0x00, sizeof(oStatBuf));
		iRet = stat (sFullLogName, &oStatBuf);
		if ( iRet == 0 && oStatBuf.st_size >= LOG_SIZE_UNIT * goLogConfig.iLogSize )
		{
			memset (sFullBakLogName, 0x00, sizeof(sFullBakLogName));
			snprintf (sFullBakLogName, 256, "%s.%s", sFullLogName, sDateTime);
			rename (sFullLogName, sFullBakLogName);
		}
	}

	/* open log file */
	fd = open (sFullLogName, O_RDWR|O_APPEND|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}
	return fd;
}

void writeData(int fd, char * buf, int length)
{
    int bytes_left;
    int written_bytes;
    char *ptr;

    if(fd < 0 || buf==NULL)
        return ;

    ptr=buf;
    bytes_left=length;
    while(bytes_left>0)
    {                                                            
        written_bytes=write(fd, (const char *)ptr, bytes_left);
        if(written_bytes<0)
        {
            if(errno==EAGAIN)
            {
                usleep(100);
                continue;
            }
            else if(errno==EINTR)
                continue; 
            else
                return ;
        }
        bytes_left-=written_bytes;
        ptr+=written_bytes;     
    }
}


/*****************************************************************************/
/* FUNC:   int showmsg_pthread( char *sProgramName,									 */
/*                    int iLine, int iLogLevel,char *sFmt, ...);             */
/* INPUT:                                                                    */
/*         iLogLevel: 日志级别,                                              */
/*                   LOG_LEVEL_ERROR,LOG_LEVEL_NORMAL,LOG_LEVEL_DEBUG        */
/*         sProgramName: 报错的源程序名                                      */
/*         iLine: 报错的源程序的行号                                         */
/*         sFmt: 出错信息                                                    */
/* OUTPUT: 无                                                                */
/* RETURN: 0: 成功, 其它: 失败                                               */
/* DESC:   根据LOG_MODE, 将该级别之下的日志记录到日志文件中,                 */
/*****************************************************************************/
int showmsg_pth(char *sProgramName, const char *sFunctionName, int iLine, int iLogLevel,char *sFmt, ...)
{

	int		lTickTime;
	char	sDateTime[16];
	va_list	ap;
	char	sLogType[32];
	char 	sBuf[1024];
  	char 	ttsBuf[2048];
	char 	sTitle[512];
    volatile int fd = -1;

	if (iLogLevel>goLogConfig.iLogLevel)
		return 0;

	memset(sDateTime,0,sizeof(sDateTime));
	lTickTime=GetHostTime(sDateTime);

	memset(sLogType,0,sizeof(sLogType));
	switch(iLogLevel)
	{
		case LOG_LEVEL_ERROR:
		 	strcpy(sLogType,"ERROR");
			break;
		case LOG_LEVEL_NORMAL:
			strcpy(sLogType,"NORMAL");
			break;
		case LOG_LEVEL_DEBUG:
			strcpy(sLogType,"DEBUG");
			break;
		case LOG_LEVEL_STDERR:
			strcpy(sLogType,"STDERR");
			break;
		case LOG_LEVEL_STDOUT:
			strcpy(sLogType,"STDOUT");
			break;
		default:
			snprintf(sLogType, 32, "DEBUG_%d",iLogLevel);
			break;
	}

	memset(sTitle,0,sizeof(sTitle));
	snprintf(sTitle, 512, "[%s][%s][%s][%s][%d][%ld]:",sLogType,sDateTime,sProgramName,sFunctionName,iLine,(long)syscall(SYS_gettid));

    memset(sBuf,0,sizeof(sBuf));
	va_start(ap, sFmt);
	vsprintf(sBuf, sFmt, ap);
	va_end(ap);
	if(iLogLevel==LOG_LEVEL_STDERR)
		fprintf(stderr, "%s%s\n",sTitle,sBuf);
	else if(iLogLevel==LOG_LEVEL_STDOUT)
		fprintf(stdout, "%s%s\n",sTitle,sBuf);
	else
	{
        if((fd = OpenLogFile_pthread (sDateTime))<0)
        {
            perror("OpenLogFile_pthread:==");
            return -1;
        }
	    memset(ttsBuf, 0x00, 2048);
		snprintf(ttsBuf, 2048,"%s%s\n", sTitle, sBuf);       
        writeData(fd, ttsBuf, strlen(ttsBuf));
		//fsync(fd);
		close(fd);
        fd = -1;
	}

	return (0);
}

int showhex_pth(char *sProgramName, const char *sFunctionName, int iLine, int iLogLevel,char *sTitle,char *sData,int iLen)
{

	int		lTickTime, bufLens=0;
	char	sDateTime[16];
	char	sLogType[10];
  	char 	ttsBuf[2048];
    char 	ttstmp[512];
    char    outbuf[2048];
    volatile int fd = -1;
    
	if (iLogLevel>goLogConfig.iLogLevel)
		return 0;

	memset(sDateTime,0,sizeof(sDateTime));
	lTickTime=GetHostTime(sDateTime);
    
	memset(sLogType, 0, sizeof(sLogType));
	switch(iLogLevel)
	{
		case 0x01:
            strncpy(sLogType, "ERROR", strlen("ERROR"));
			break;
		case 0x02:
            strncpy(sLogType, "NORMAL", strlen("NORMAL"));            
			break;
		default:
            strncpy(sLogType, "DEBUG", strlen("DEBUG"));            
			break;
	}

    if((fd = OpenLogFile_pthread (sDateTime))<0)    
    {
        perror("OpenLogFile_pthread:==");
        return -1;
    }

    memset(ttsBuf, 0x00, 2048);
    memset(outbuf, 0x00, 2048);
    memset(ttstmp, 0x00, 512);
    snprintf(ttstmp, 512, "[%s][%s][%s][%s][%d][%ld][%s] HEX SHOW\n",sLogType,sDateTime,sProgramName,sFunctionName,iLine,(long)syscall(SYS_gettid),sTitle);
    bufLens = 0;
    Write_Log_Hex_pth(sData,iLen, outbuf, &bufLens);
    snprintf(ttsBuf, 2048, "%s%s\n", ttstmp, outbuf);
    writeData(fd, ttsBuf, bufLens+strlen(ttstmp));
    //fsync(fd); jll 先写入filesys， sync to phyical block by kernel
    close (fd);

	return 0;

}


/*
int main(int argc,char **argv)
{
	int a=1;
	char s[]="111111";

	InitLog("test");

	showmsg(DEBUG,"a[%d]s[%s]sdsdfsdf",a,s);
	showmsg(NORMAL,"a[%d]s[%s]sdsdfsdf",a,s);
	showmsg(STDERR,"a[%d]s[%s]sdsdfsdf",a,s);
	showmsg(STDOUT,"a[%d]s[%s]sdsdfsdf",a,s);
	showhex(ERROR,"testhex","1234567890",10);
}
*/

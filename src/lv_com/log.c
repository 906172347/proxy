#include "head.h"
#include "log.h"
#include "inifile.h"
volatile FILE *fp = NULL;

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

/**
 *@brief  功能:记录日志信息
 *@param  char *sTitle 标题
 *@param  char *sData  日志数据
 *@param  int  iLen    日志数据长度
 *@return 无
 */
void Write_Log_Hex(char *sData,int iLen)
{

   	unsigned char *pStr;
    int i,j,k;
    int iRow3;
    char sTr1[108];
    int iNum;


    iRow3 = ROW1 * ROW2;
    iNum = 1;
    memset(sTr1,0,sizeof(sTr1));
    memset(sTr1,'-',3*iRow3+4+4+2*ROW2);
    for(i = 0 ;i < ROW2;i++)
    {
    	sTr1[4 + (2*ROW1 + 1)*i + ROW1] = 0x30 + i + 1;
    	sTr1[4 + 2*iRow3+ROW2+4 +(ROW1+1)*i + ROW1/2] = 0x30 + i + 1;
    }
    fprintf(fp,"%s\n%03d ",sTr1,iNum);	iNum ++;

    if(iLen == 0)
    {
   		fprintf( fp, "No Data" );
   	}
   	else
   	{
	   	pStr = (uchar *)sData;
	    for( i = 0; i < iLen; i ++,pStr ++ )
	    {
	      	fprintf( fp, "%02X", (*pStr)&0xff );
	        if((i + 1)%ROW1 == 0 )
	        {
		  	   fprintf( fp, " " );
	        }

	       if((i+1)%iRow3 == 0 )
	       {
	      	   pStr -= (iRow3 - 1);
	      	   fprintf(fp," -> ");
	      	   for(j = 0;j < iRow3;j ++,pStr ++)
	      	   {
	      	   	  if((*pStr) < 32 || (*pStr) > 127)
	   	  			  fprintf(fp,"*");
	   	  		  else
	      	   	      fprintf(fp,"%c",*pStr);
	      	   	  if((j+1)%ROW1 == 0 ) {
		  	  		  fprintf( fp, " " );
	      		  }
	       	   }
	       	   pStr--;
		  	   fprintf( fp, "\n%03d ",iNum);	iNum ++;
	      }
	   }



	   k = iLen % iRow3;
	   pStr -= k;
	   if((i % iRow3) != 0)
	   {
	   		do{
	   	  		fprintf(fp,"  ");
	      		if((i+1)%ROW1 == 0 )
	      		{
		  	   		fprintf( fp, " " );
	      		}
	      		i ++;
	   		}while(i%iRow3!=0);
	   		fprintf(fp," -> ");
	   		for(j = 0;j < k;j ++,pStr ++)
	   		{
	      	   	  fprintf(fp,"%c",*pStr);
	      	   	  if((j+1)%ROW1 == 0 ) {
		  	  		 fprintf( fp, " " );
	      		  }
	       	}

	   }
	}

	fprintf( fp, "\n\n" );
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

	return times( &otms);
}

/*****************************************************************************/
/* FUNC:   int OpenLogFile (char *sDateTime)       			                 */
/* INPUT:                                                                    */
/*         sDateTime: 当前时间, YYYYMMDDhhmmss                               */
/* OUTPUT: fp: 打开的日志文件的指针                                          */
/* RETURN: 0: 成功, 其它: 失败                                               */
/* DESC:   根据nLogSwitchMode, 打开日志文件                                  */
/*         LOG_SWITCH_MODE_SIZE: 当文件大小(M)达到iLogSize, 切换到新文件,    */
/*                               原文件改名为文件名中带有时间                */
/*								 新文件 xx.log                               */
/*                               原文件 xx.log.YYYYMMDDhhmmss                */
/*         LOG_SWITCH_MODE_DATE: 日志文件名带有日期, xx.log.YYYYMMDD         */
/*         LOG_SWITCH_MODE_DATE_SIZE: 当文件大小(M)达到iLogSize, 切换到新文件*/
/*                               原文件改名为文件名中带有时间                */
/*								 新文件 xx.log.YYYYMMDD                      */
/*                               原文件 xx.log.YYYYMMDDhhmmss                */
/*****************************************************************************/
int OpenLogFile (char *sDateTime)
{
	char		sFullLogName[256];
	char		sFullBakLogName[256];
	int			iRet;
    char        dateTmp[16];

	struct stat	oStatBuf;

	/* set log file name */
	memset(sFullLogName,0,sizeof(sFullLogName));
	snprintf (sFullLogName, 256, "%s/%s.log", goLogConfig.sLogFilePath,goLogConfig.sLogFileName);

    memset(dateTmp, 0x00, 16);
    memcpy(dateTmp, sDateTime, 8);


	if (goLogConfig.iLogSwitchMode == LOG_SWITCH_MODE_DATE)
	{
		/* append date in log file name */
        //sprintf (sFullLogName, "%s.%8.8s",sFullLogName,sDateTime);        
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
        //sprintf (sFullLogName, "%s.%8.8s",sFullLogName,sDateTime);
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
	fp = fopen (sFullLogName, "a+");
	if (fp == NULL )
	{
		perror("fopen");
		return -1;
	}
	return 0;
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

void writeData(int fd, unsigned char * buf, int length)
{
    int bytes_left;
    int written_bytes;
    unsigned char *ptr;

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
	int		iRet;
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
	int		iRet;
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
    snprintf(ttsBuf, 2048,"%s%s\n",ttstmp,outbuf);
    writeData(fd, ttsBuf, bufLens+strlen(ttstmp));
    //fsync(fd); jll 先写入filesys， sync to phyical block by kernel
    close (fd);

	return 0;

}



/*****************************************************************************/
/* FUNC:   int showmsg( char *sProgramName,									 */
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
int showmsg(char *sProgramName, const char *sFunctionName, int iLine, int iLogLevel,char *sFmt, ...)
{

	int		lTickTime;
	char	sDateTime[16];
	va_list	ap;
	int		iRet;
	char	sLogType[20];
	char 	sBuf[1024];
	char 	sTitle[255];
	__pid_t	pid;
	/*check log level*/

	if (iLogLevel>goLogConfig.iLogLevel)
		return 0;

	/*get host time*/
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
			sprintf(sLogType,"DEBUG_%d",iLogLevel);
			break;
	}

	/* save log msg in file */
	memset(sTitle,0,sizeof(sTitle));
	pid=getpid();
	sprintf(sTitle, "[%s][%s][%s][%s][%d][%d]:",sLogType,sDateTime,sProgramName,sFunctionName,iLine,pid);

    memset(sBuf,0,sizeof(sBuf));
	va_start(ap, sFmt);
	vsprintf(sBuf, sFmt, ap);
	va_end(ap);
	if(iLogLevel==LOG_LEVEL_STDERR)
	{
		fprintf(stderr, "%s%s\n",sTitle,sBuf);
	}
	else if(iLogLevel==LOG_LEVEL_STDOUT)
	{
		fprintf(stdout, "%s%s\n",sTitle,sBuf);
	}
	else
	{
		iRet = OpenLogFile (sDateTime);
		if (iRet)
		{
			return iRet;
		}
		if(fp)
		{
			fprintf(fp, "%s%s\n",sTitle,sBuf);
			fflush(fp);
			/* close file */
			fclose (fp);
			fp = NULL;
		}
	}

	return (0);
}

/**
 *@brief  功能:记录日志信息
 *@param  char *iLogType 日志类型  ERROR 错误日志 DEBUG 调试日志 RECODE 记录数据日志
 *@param  char *sTitle 标题
 *@param  char *sData  日志数据
 *@param  int  iLen    日志数据长度
 *@return 无
 */
int showhex(char *sProgramName, const char *sFunctionName, int iLine, int iLogLevel,char *sTitle,char *sData,int iLen)
{

	int		lTickTime;
	char	sDateTime[16];
	int		iRet;
	char	sLogType[10];

	/*check log level*/
	if (iLogLevel>goLogConfig.iLogLevel)
	{
		return 0;
	}
	/*get host time*/
	memset(sDateTime,0,sizeof(sDateTime));
	lTickTime=GetHostTime(sDateTime);


	memset(sLogType,0,sizeof(sLogType));
	switch(iLogLevel)
	{
		case 0x01:
		 	strcpy(sLogType,"ERROR");
			break;
		case 0x02:
			strcpy(sLogType,"NORMAL");
			break;
		default:
			strcpy(sLogType,"DEBUG");
			break;
	}
	/* open log file */

	iRet = OpenLogFile (sDateTime);
	if (iRet)
	{
		return iRet;
	}

	if(fp)
	{
		/* save log msg in file */
		fprintf(fp, "[%s][%s][%s][%s][%d][%d][%s] HEX SHOW\n",sLogType,sDateTime,sProgramName,sFunctionName,iLine,getpid(),sTitle);
		Write_Log_Hex(sData,iLen);
		fprintf(fp, "\n");
		fflush(fp);
    
		fclose (fp);
		fp = NULL;
    	}


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

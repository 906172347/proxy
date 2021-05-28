/* %B% %E% %I% %L% %M% %R% %S% %T% */
#ifndef _LOG_H
#define _LOG_H

#include<errno.h>
#include<sys/types.h> 
#include<sys/stat.h>
#include<fcntl.h>
#include <sys/syscall.h>

/*LOG_LEVEL ��ʾ����*/
#define LOG_LEVEL_ERROR		0x01		/*����*/
#define LOG_LEVEL_NORMAL	0x02		/*��ʾ*/
#define LOG_LEVEL_DEBUG		0x03		/*����*/
#define LOG_LEVEL_STDERR	0x04		/*��׼�������*/
#define LOG_LEVEL_STDOUT	0x05		/*��׼���*/


/* ��־�ļ�·���ĳ��ȵ����ֵ */
#define LOG_PATH_LEN_MAX		255

/* ��־�л�ģʽ, ���ļ���С������ */
#define LOG_SWITCH_MODE_DATE		1	/*����*/
#define LOG_SWITCH_MODE_SIZE		2	/*��С*/
#define LOG_SWITCH_MODE_DATE_SIZE	3	/*��С������*/
#define LOG_DEFAULT_NAME		"app.log"
#define SINGLE_LINE				"--------------------------------------------------------------------------------\n"
#define LOG_SIZE_UNIT			1024000

#define ROW1 5
#define ROW2 4

/*��־��������*/
struct TLogConfig
{
	char sLogFilePath[255];	/*��־Ŀ¼*/
	char sLogFileName[255];	/*��־�ļ���*/
	int iLogLevel;			/*��־����*/
	int iLogSize;			/*��־��С,��MΪ��λ*/
	int iLogSwitchMode;		/*��־�л�ģʽ*/
};
struct TLogConfig goLogConfig;


#define ERROR		__FILE__, __func__, __LINE__, 0x01		/*����*/
#define NORMAL		__FILE__, __func__, __LINE__, 0x02		/*��ʾ*/
#define DEBUG		__FILE__, __func__, __LINE__, 0x03		/*����*/
#define STDERR		__FILE__, __func__, __LINE__, 0x04		/*��ע�������*/
#define STDOUT		__FILE__, __func__, __LINE__, 0x05		/*��׼���*/

int showmsg(char *sProgramName, const char *sFunctionName, int iLine, int iLogLevel,char *sFmt, ...);
int showhex(char *sProgramName, const char *sFunctionName, int iLine, int iLogLevel,char *sTitle,char *sData,int iLen);
int	InitLog(char *sProgramName);


//��־  ��� 
int showmsg_pth(char *sProgramName, const char *sFunctionName, int iLine, int iLogLevel,char *sFmt, ...);
int showhex_pth(char *sProgramName, const char *sFunctionName, int iLine, int iLogLevel,char *sTitle,char *sData,int iLen);

#endif

/* %B% %E% %I% %L% %M% %R% %S% %T% */
#ifndef _LOG_H
#define _LOG_H

#include<errno.h>
#include<sys/types.h> 
#include<sys/stat.h>
#include<fcntl.h>
#include <sys/syscall.h>

/*LOG_LEVEL 提示类型*/
#define LOG_LEVEL_ERROR		0x01		/*错误*/
#define LOG_LEVEL_NORMAL	0x02		/*提示*/
#define LOG_LEVEL_DEBUG		0x03		/*调试*/
#define LOG_LEVEL_STDERR	0x04		/*标准错误输出*/
#define LOG_LEVEL_STDOUT	0x05		/*标准输出*/


/* 日志文件路径的长度的最大值 */
#define LOG_PATH_LEN_MAX		255

/* 日志切换模式, 按文件大小或按日期 */
#define LOG_SWITCH_MODE_DATE		1	/*日期*/
#define LOG_SWITCH_MODE_SIZE		2	/*大小*/
#define LOG_SWITCH_MODE_DATE_SIZE	3	/*大小和日期*/
#define LOG_DEFAULT_NAME		"app.log"
#define SINGLE_LINE				"--------------------------------------------------------------------------------\n"
#define LOG_SIZE_UNIT			1024000

#define ROW1 5
#define ROW2 4

/*日志环境配置*/
struct TLogConfig
{
	char sLogFilePath[255];	/*日志目录*/
	char sLogFileName[255];	/*日志文件名*/
	int iLogLevel;			/*日志级别*/
	int iLogSize;			/*日志大小,以M为单位*/
	int iLogSwitchMode;		/*日志切换模式*/
};
struct TLogConfig goLogConfig;


#define ERROR		__FILE__, __func__, __LINE__, 0x01		/*错误*/
#define NORMAL		__FILE__, __func__, __LINE__, 0x02		/*提示*/
#define DEBUG		__FILE__, __func__, __LINE__, 0x03		/*调试*/
#define STDERR		__FILE__, __func__, __LINE__, 0x04		/*标注错误输出*/
#define STDOUT		__FILE__, __func__, __LINE__, 0x05		/*标准输出*/

int showmsg(char *sProgramName, const char *sFunctionName, int iLine, int iLogLevel,char *sFmt, ...);
int showhex(char *sProgramName, const char *sFunctionName, int iLine, int iLogLevel,char *sTitle,char *sData,int iLen);
int	InitLog(char *sProgramName);


//日志  输出 
int showmsg_pth(char *sProgramName, const char *sFunctionName, int iLine, int iLogLevel,char *sFmt, ...);
int showhex_pth(char *sProgramName, const char *sFunctionName, int iLine, int iLogLevel,char *sTitle,char *sData,int iLen);

#endif

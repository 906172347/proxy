#ifndef _HEAD_H
#define _HEAD_H
/*
#define HPN4000_UX
#define SUN_OS
#define SUN_SOLARIS
#define IBM_AIX
#define DEC_DGUNIX
#define SCO_UNIX
*/
#define REDHAT
#ifdef  REDHAT

#include "log.h"

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <termio.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <memory.h>
#include <signal.h>
#include <netdb.h>
#include <dirent.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/sem.h>
#include <setjmp.h>
#include <sys/wait.h>
#include <curses.h>
#include <regex.h>
#endif

#ifndef CHAR
#define CHAR char
#endif

#ifndef UCHAR
#define UCHAR unsigned char
#endif

#ifndef SHORT
#define SHORT short
#endif

#ifndef USHORT
#define USHORT unsigned short
#endif

#ifndef INT
#define INT int
#endif

#ifndef UINT
#define UINT unsigned int
#endif

#ifndef LONG
#define LONG long
#endif

#ifndef ULONG
#define ULONG unsigned long
#endif

#ifndef BOOL
#define BOOL int
#endif

#ifndef FLOAT
#define FLOAT float
#endif

#ifndef DOUBLE
#define DOUBLE double
#endif

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif

#ifndef ulong
#define ulong unsigned long
#endif

#ifndef bool
#define bool unsigned char
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* 返回码 */
#ifndef OK
#define OK	0		/* 操作成功 */
#endif

#ifndef CANCEL
#define CANCEL	-1		/* 取消操作 */
#endif

#ifndef FAIL
#define FAIL    -1		/* 失败 */
#endif

#ifndef FILE_MODE
#define FILE_MODE (S_IRUSR | S_IWUSR |S_IRGRP |S_IROTH)
#endif

#define FILE_EOF				0
#define MAXBUFLEN				8192	//最大缓冲池大小
#define FILENAMELEN             150
#define MAXFILENAMELEN			255

#define FLOAT_PRECISION			0.004	/*浮点数精度*/

#define SUCCESS					0		/*成功*/
#define FAILURE					-1		/*失败*/

#define GOON					1		/*继续*/
#define TRANSEND				999		/*结束*/

#define TEST 					1		/*测试标志*/
#define PSWDMACHINE				1		/*加密机检查*/
#endif

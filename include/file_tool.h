#ifndef _FILE_TOOL_H
#define _FILE_TOOL_H

#include "head.h"
#define MsgMaxLen 1024
#define MaxBufLen 1024

#define SYNC 0
#define ASYNC 1

#define GETCHAR 0
#define GETINT 1

#define iniOpenFail 		-1401
#define iniNotFoundSect 	-1402
#define iniNotFoundEntry 	-1403

union INFOCFG
{	int getintinfo;			
	char getcharinfo[100];
	double getdoubleinfo;
	char *getcharpinfo;
};

char gl_szProcLog[MaxBufLen];/*通讯日支*/
char gl_szDebugLog[MaxBufLen];/*调试日志*/

 void vTrimStr( char *pszBuf );
 void vGetHostTime( char *pszTime );
 void vGetHome( char *pszHome );
 void vGettyName( char *pszName );


 void vPrtLogHexMsg(char *pszTitle, unsigned char *pusMsg, unsigned int uiLength );
 void vPrtDebugMsg(const char *fmt,...);
 void vInitPubParm(char* sLogPath, char *pszProcName );

 int iIsEmptyFile( char *pszFName );
 int iExistFile( char *pszFName );

 void vAddYY( char *pOriginal, char *pTarget );
 void vAddYYYY( char *pOriginal, char *pTarget );

 int iniGetString( char *pszSection, char *pszEntry,char *pszRetBuf, char *pszFileName );
 int iniPutString( char *pszSection, char *pszEntry, char *pszVal,char *pszFileName );
 int iniGetInit( char *pszSection, char *pszEntry,union INFOCFG *piVal, int rtrType,char *pszFileName );
 
#endif

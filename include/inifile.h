#ifndef _INIFILE_H
#define _INIFILE_H

/***************************************
函数：ReadLine
输入参数：
	int iFd,文件描述符
输出参数：
	char *psLine,行数据
	int *iLineLen,行长度
	int *iOffset：行结束偏移量
功能：读取文件某一行
返回值：
	0:文件结束
	-0:失败
	1:成功
*****************************************/
int ReadLine(int iFd,char *psLine,int *iLineLen,int *iOffset);

/***************************************
函数：UpdateLine
输入参数：

	int iOffset,行起始偏移量
	char *psLine,行数据
	char *psFileName文件名
功能：更新文件指定行
返回值：
	<0:失败
	=0:成功
*****************************************/

int UpdateLine(int iOffSet,char *psLine,char *psFileName);
/***************************************
函数：ReadEntry
输入参数：
	int iFd,文件描述符
输出参数：
	char *psEntry,参数名
	char *psValue,参数值
	int *iOffset：行结束偏移量
功能：读取配置文件制定参数的值
返回值：
	n=FILE_EOF文件结束
	<0:失败
	>0:行起始偏移量
*****************************************/
int ReadEntry(int iFd,char *psEntry,char *psValue,int *piOffSet);
/***************************************
函数：ReadString
输入参数：
	char *psFileName,配置文件名
	char *psSection,标题名称
	char *psEntry,参数名称
输出参数：
	char *psValue参数值
功能：读取配置文件制定参数的值
返回值：
	n=FILE_EOF文件结束
	<0:失败
	>0:行起始偏移量
*****************************************/
int ReadString(char *psFileName,char *psSection,char *psEntry,char *psValue);

/***************************************
函数：ReadInteger
输入参数：
	char *psFileName,配置文件名
	char *psSection,标题名称
	char *psEntry,参数名称
输出参数：
	char *psValue参数值
功能：读取配置文件制定参数的值
返回值：
	n=FILE_EOF文件结束
	<0:失败
	>0:行起始偏移量
*****************************************/
int ReadInteger(char *psFileName,char *psSection,char *psEntry,int *piValue);
/***************************************
函数：WriteString
输入参数：
	char *psFileName,配置文件名
	char *psSection,标题名称
	char *psEntry,参数名称
	char *psValue参数值

功能：更新配置文件制定参数的值
返回值：
	<0:失败
	=0:成功
*****************************************/
int WriteString(char *psFileName,char *psSection,char *psEntry,char *psValue);
#endif

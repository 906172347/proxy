#ifndef _DLLLIB_H
#define _DLLLIB_H

/**
 *@brief 前置系统基本静态库函数设计--libbase.a
 *@brief (1)动态库访问操作
 *@file dlllib.h
 */
#include "head.h"
#include <dlfcn.h>
 
/**
 *@brief	功能:打开动态库
 *@param char *sDllPath   动态库的路径
 *@return void *   动态库句柄
*@see 调用函数：dlopen
 */
void * DllOpenLib(char *sDllPath);

/**
 *@brief	功能:得到动态库中指定函数
 *@param void *pHandle    动态库句柄
 *@param char *sFuncName 函数名
 *@return void *   函数指针
 *@see  调用函数：dlsym
 */
void * DllOpenFunc(void *pHandle,char *sFuncName);

/**
 *@brief	功能:关闭动态库
 *@param void * pHandle    动态库句柄
 *@param void *  函数指针
 *@see 	  调用函数：dlclose
 */ 
int DllCloseLib(void *pHandle);

#endif

#ifndef _DLLLIB_H
#define _DLLLIB_H

/**
 *@brief ǰ��ϵͳ������̬�⺯�����--libbase.a
 *@brief (1)��̬����ʲ���
 *@file dlllib.h
 */
#include "head.h"
#include <dlfcn.h>
 
/**
 *@brief	����:�򿪶�̬��
 *@param char *sDllPath   ��̬���·��
 *@return void *   ��̬����
*@see ���ú�����dlopen
 */
void * DllOpenLib(char *sDllPath);

/**
 *@brief	����:�õ���̬����ָ������
 *@param void *pHandle    ��̬����
 *@param char *sFuncName ������
 *@return void *   ����ָ��
 *@see  ���ú�����dlsym
 */
void * DllOpenFunc(void *pHandle,char *sFuncName);

/**
 *@brief	����:�رն�̬��
 *@param void * pHandle    ��̬����
 *@param void *  ����ָ��
 *@see 	  ���ú�����dlclose
 */ 
int DllCloseLib(void *pHandle);

#endif

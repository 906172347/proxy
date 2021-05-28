/**
 *@brief     共享内存类头文件
 *@file      SharedMemory.h
 *@version   1.0
 *@author    dongsupan
 *@since     2008-10-21
*/

#ifndef __SHAREDMEMORY_H_INCL__
#define __SHAREDMEMORY_H_INCL__

#include "head.h"

//#define SVSHM_MODE    (SHM_R | SHM_W | SHM_R>>3 | SHM_R>>6)
#define SVSHM_MODE  (S_IRUSR | S_IWUSR | S_IRUSR>>3 | S_IRUSR>>6)

uchar * shmCreateArea(int _iAreaSize,int _iAreaKey);
uchar * shmOpenArea(int _iAreaKey);
void shmDeleteArea(int _iAreaKey);
void shmUnsetArea(uchar * _SmbArea);

#endif // __SHAREDMEMORY_H_INCL__

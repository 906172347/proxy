#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include "head.h"

typedef struct {
	int sem_semid; /*the System V semaphore ID*/
	int sem_magic; /*magic number if open */
	int sem_num;/*ÐÅºÅ¸öÊý*/
} sem_t;

union semun {
	int val;
	struct semid_ds *buf;
	ushort* array;
};

#define SEM_MAGIC 0x45678923
//for unix
//#define SVSEM_MODE  (SEM_R|SEM_A|SEM_R>>3|SEM_R>>6)
//for linux
#define SVSEM_MODE  (S_IRUSR|S_IWUSR|S_IRUSR>>3|S_IRUSR>>6)

#define va_mode_t mode_t

#ifdef SEM_FAILED
#undef SEM_FAILED
#endif

#define SEM_FAILED ((sem_t*)(-1))

#ifndef SEMVMX
#define SEMVMX 32767
#endif

int sem_create(const char* pathname,int semnum);
sem_t* sem_open(const char* pathname);
int sem_setvalue(sem_t* sem,int semno,int value);
int sem_close(sem_t* sem);
int sem_unlink(const char* pathname);
int sem_post(sem_t* sem,int semno);
int sem_wait(sem_t* sem,int semno);
int sem_trywait(sem_t* sem,int semno);
int sem_getvalue(sem_t* sem,int semno,int* pvalue);
#endif

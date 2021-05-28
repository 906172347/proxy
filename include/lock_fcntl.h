#ifndef _LOCK_FCNTL_H
#define _LOCK_FCNTL_H
#include "head.h"
#define ReadLock(fd) lock_req(fd, F_SETLK,F_RDLCK,0,SEEK_SET,0)
#define ReadWLock(fd) lock_req(fd, F_SETLKW,F_RDLCK,0,SEEK_SET,0)
#define UNLock(fd)	lock_req(fd, F_SETLK,F_UNLCK,0,SEEK_SET,0)


#define WRLock(fd) 	lock_req(fd, F_SETLKW,F_WRLCK,0,SEEK_SET,0)
#define WRUnLock(fd) lock_req(fd, F_SETLKW,F_UNLCK,0,SEEK_SET,0)


int LockWait();
int LockRelease();
int LockInit(char *pathname);
int lock_req(int fd,int cmd,int type,off_t offset,int whence,off_t len);


#endif

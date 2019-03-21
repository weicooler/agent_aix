/*
 *  Copyright 2006, Shanghai Huateng Software Systems Co., Ltd.
 *  All right reserved.
 *
 *  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF SHANGHAI HUATENG
 *  SOFTWARE SYSTEMS CO., LTD.  THE CONTENTS OF THIS FILE MAY NOT
 *  BE DISCLOSED TO THIRD PARTIES, COPIED OR DUPLICATED IN ANY FORM,
 *  IN WHOLE OR IN PART, WITHOUT THE PRIOR WRITTEN PERMISSION OF
 *  SHANGHAI HUATENG SOFTWARE SYSTEMS CO., LTD.
 *
 *  $Id$
 *
 *  功能:信号函数库
 *
 *  Edit History:
 *
 */
#include "ipcopr.h"

#ifdef LINUX
 typedef unsigned short int ushort;
#endif

/*******************************************************
**函数名称: makekey                                   **
**功能描述: 生成指定名称的键值                        **
**作    者:                                           **
**输入参数:                                           **
**          name：名称                                **
**输出参数:                                           **
**返回值:  键值                                       **
*******************************************************/
static key_t makekey(const char *name)
{
    key_t   key, base_key;
    int     i;
    char    *stop;
    char    buf[128 + 1];
        
    memset(buf, 0, sizeof(buf));
    for (i = 0; i < strlen(name); i++) {
        sprintf(buf + strlen(buf), "%02x", name[i]);
    }

    key = strtol(buf, &stop, 16);
    if (getenv("IPCREGION") == NULL)
        base_key = 0;
    else
        base_key = atoi(getenv("IPCREGION"));
    if (base_key < 0 || base_key > 255) base_key = 0;
    
    key = key + IPC_KEY_BASE * base_key;
    return key;
}

/*******************************************************
**函数名称: shm_shmget_internal                       **
**功能描述: 获取共享内存id                            **
**作    者:                                           **
**输入参数:                                           **
**          shm_key：共享内存键值                     **
**          name：共享内存大小                        **
**          flags：标志                                **
**输出参数:                                           **
**返回值:  共享内存ID                                 **
*******************************************************/
int shm_shmget_internal(key_t shm_key, int shm_size, int flags) 
{
    int       shmid;
    
    if ((shmid = shmget(shm_key, shm_size, flags)) == -1) {
        return -1;
    }

    return shmid;
}

/*******************************************************
**函数名称: shm_attach_internal                       **
**功能描述: 连接共享内存                              **
**作    者:                                           **
**输入参数:                                           **
**          shmid：共享内存ID                         **
**输出参数:                                           **
**返回值:  shm                                        **
*******************************************************/
void *shm_attach_internal(int shmid) 
{
    char   *shm;

    shm = shmat(shmid, (void*)0, 0);
    if (shm == (void*)-1) {
        return NULL;
    }
    
    return shm;
}

/*******************************************************
**函数名称: shm_delete_internal                       **
**功能描述: 删除共享内存                              **
**作    者:                                           **
**输入参数:                                           **
**          shmid：共享内存ID                         **
**输出参数:                                           **
**返回值:  0/-1                                       **
*******************************************************/
int shm_delete_internal(int shmid)
{
    struct shmid_ds shmds;

    if (shmctl(shmid, IPC_RMID, &shmds) < 0) {
        return -1;
    }

    return 0;
}

/*******************************************************
**函数名称: sem_semget_internal                       **
**功能描述: 获取信号队列                              **
**作    者:                                           **
**输入参数:                                           **
**          sem_key: 信号键值                         **
**          sem_size：信号大小                        **
**          flags：标志                               **
**输出参数:                                           **
**返回值:  信号ID                                     **
*******************************************************/
int sem_semget_internal(key_t sem_key, int sem_size, int flags)
{
    int   sem_id;

    /*  create the semaphore    */
    if ((sem_id = semget(sem_key, sem_size, flags)) == -1) {
        return -1;
    }

    return sem_id;
}

/*******************************************************
**函数名称: sem_attach_internal                       **
**功能描述: 连接信号                                  **
**作    者:                                           **
**输入参数:                                           **
**          sem_id: 信号ID                         **
**          sem_size：信号大小                        **
**输出参数:                                           **
**返回值:  信号ID                                     **
*******************************************************/
int sem_attach_internal(int sem_id, int sem_size)
{
    int   i;
    union semum {
        int val;
        struct semid_ds *buf;
        ushort *array;
    } arg;

    /*  set the semaphore initial value(s)      */
    arg.val = 1;
    for (i = 0; i < sem_size; i++) {
        if (semctl(sem_id, i, SETVAL, arg) == -1) {
            return -1;
        }  /* end if */
    } /* end for */

    return sem_id;
}

/*******************************************************
**函数名称: sem_delete_internal                       **
**功能描述: 删除共享内存                              **
**作    者:                                           **
**输入参数:                                           **
**         sem_id：共享内存ID                         **
**输出参数:                                           **
**返回值:  0/-1                                           **
*******************************************************/
int sem_delete_internal(int sem_id)
{
    /*  delete the semaphore */
    if (semctl(sem_id, 0, IPC_RMID) == -1) {
        return (-1);
    }

    return (0);
}

/*******************************************************
**函数名称: msq_msqget_internal                       **
**功能描述: 建立信息队列                              **
**作    者:                                           **
**输入参数:                                           **
**          key：键值                                 **
**          flags：标志                               **
**输出参数:                                           **
**返回值:   msq_id                                   **
*******************************************************/
int msq_msqget_internal(int key, int flags)
{
    int   msq_id;

    if ((msq_id = msgget(key, flags)) == -1) {
        return (-1);
    }

    return (msq_id);
}

/*******************************************************
**函数名称: msq_delete_internal                       **
**功能描述: 删除信息队列                              **
**作    者:                                           **
**输入参数:                                           **
**          msq_id：队列ID                            **
**输出参数:                                           **
**返回值:   0/-1                                      **
*******************************************************/
int msq_delete_internal(int msq_id)
{
    if (msgctl(msq_id, IPC_RMID, NULL) == -1) {
        return (-1);
    }
   
    return (0);    
}

/*******************************************************
**函数名称: sem_getid                                 **
**功能描述: 获取信号ID                                **
**作    者:                                           **
**输入参数:                                           **
**          semname：信号名字                         **
**          semsize：信号大小                         **
**          createflg：创建标志                       **
**输出参数:                                           **
**返回值:   id值                                      **
*******************************************************/
int sem_getid(const char *semname, int semsize, int createflg)
{
    int    flags;
    key_t  semkey;

    if (semname != NULL)
        semkey = makekey(semname);
    else
        semkey = IPC_PRIVATE;

    if (createflg == 1) {
        flags = SEM_PERM|IPC_CREAT;
    } else {
        flags = SEM_PERM;
    }

    return sem_semget_internal(semkey, semsize, flags);
}


/*******************************************************
**函数名称: sem_getid                                 **
**功能描述: 信号处理                                  **
**作    者:                                           **
**输入参数:                                           **
**          sem_num：信号编号                         **
**          sem_id：信号ID                            **
**          op：处理号                                **
**输出参数:                                           **
**返回值:   0/-1                                      **
*******************************************************/
static int sem_call(int sem_num, int sem_id, int op)
{
    struct sembuf SemBuf;

    SemBuf.sem_num = sem_num;
    SemBuf.sem_op = op;
    SemBuf.sem_flg = SEM_UNDO;

    if (semop(sem_id, &SemBuf, 1) < 0) {
        return(-1);
    }

    return (0);
}

/*******************************************************
**函数名称: sem_create                                **
**功能描述: 创建信号                                  **
**作    者:                                           **
**输入参数:                                           **
**          semname：信号名                           **
**          semsize：信号大小                         **
**输出参数:                                           **
**返回值: id值                                         **
*******************************************************/
int sem_create(const char *semname, int semsize)
{
    int semid;

    semid = sem_getid(semname, semsize, 1);
    if (semid < 0) {
        return -1;
    }

    return sem_attach_internal(semid, semsize);
}

/*******************************************************
**函数名称: sem_delete                                **
**功能描述: 删除信号                                  **
**作    者:                                           **
**输入参数:                                           **
**          semname：信号名                           **
**          semsize：信号大小                         **
**输出参数:                                           **
**返回值:   0/-1                                      **
*******************************************************/
int sem_delete(const char *semname, int semsize)
{
    int semid;

    semid = sem_getid(semname, semsize, 0);
    if (semid < 0) {
        return -1;
    }
    
    return sem_delete_internal(semid);
}

/*******************************************************
**函数名称: sem_connect                               **
**功能描述: 信号连接                                  **
**作    者:                                           **
**输入参数:                                           **
**          semname：信号名                           **
**          semsize：信号大小                         **
**输出参数:                                           **
**返回值: id值                                        **
*******************************************************/
int sem_connect(const char *semname, int semsize)
{
    int semid;

    semid = sem_getid(semname, semsize, 0);
    if (semid < 0) {
        return -1;
    }

    return sem_attach_internal(semid, semsize);
}

/*******************************************************
**函数名称: sem_lock                                  **
**功能描述: 信号锁定                                  **
**作    者:                                           **
**输入参数:                                           **
**          sem_num：信号编号                         **
**          sem_id：信号ID                            **
**输出参数:                                           **
**返回值:   0/-1                                      **
*******************************************************/
int sem_lock(int sem_num, int sem_id)
{
    /*  lock the specified semaphore */
    if (sem_call(sem_num, sem_id, -1) == -1) {
        return (-1);
    }

    return (0);
}

/*******************************************************
**函数名称: sem_unlock                                **
**功能描述: 信号解锁                                  **
**作    者:                                           **
**输入参数:                                           **
**          sem_num：信号编号                         **
**          sem_id：信号ID                            **
**输出参数:                                           **
**返回值:   0/-1                                      **
*******************************************************/
int sem_unlock(int sem_num, int sem_id)
{
    /*  operate the specified semaphore */
    if (sem_call(sem_num, sem_id, 1) == -1) {
        return (-1);
    }

    return (0);
}

/*******************************************************
**函数名称: shm_getid                                 **
**功能描述: 获取共享内存ID                            **
**作    者:                                           **
**输入参数:                                           **
**          shmname：共享内存名                       **
**          shmsize：共享内存大小                     **
**          createflg：创建标志                       **
**输出参数:                                           **
**返回值:  shmid                                      **
*******************************************************/
int shm_getid(const char *shmname, int shmsize, int createflg)
{
    int    flags;
    key_t  shmkey;

    if (shmname != NULL)
        shmkey = makekey(shmname);
    else
        shmkey = IPC_PRIVATE;

    if (createflg == 1) {
        flags = SHM_PERM|IPC_CREAT;
    } else {
        flags = SHM_PERM;
    }

    return shm_shmget_internal(shmkey, shmsize, flags);
}

/*******************************************************
**函数名称: shm_create                                **
**功能描述: 创建共享内存                              **
**作    者:                                           **
**输入参数:                                           **
**          shmname：共享内存名                       **
**          shmsize：共享内存大小                     **
**输出参数:                                           **
**返回值:  shm                                        **
*******************************************************/
void *shm_create(const char *shmname, int shmsize)
{
    int shmid;

    shmid = shm_getid(shmname, shmsize, 1);
    if (shmid < 0) {
        return NULL;
    }

    return (void *)shm_attach_internal(shmid);
}

/*******************************************************
**函数名称: shm_connect                               **
**功能描述: 连接共享内存                              **
**作    者:                                           **
**输入参数:                                           **
**          shmname：共享内存名                       **
**输出参数:                                           **
**返回值:  shm                                        **
*******************************************************/
void *shm_connect(const char *shmname)
{
    int shmid;

    shmid = shm_getid(shmname, 0, 0);
    if (shmid < 0) {
        return NULL;
    }

    return (void *)shm_attach_internal(shmid);
}


shm_disconnect(const char *shmname)
{
     if (shmname == NULL)
		 return -1;
	 return shmdt(shmname);
}

/*******************************************************
**函数名称: shm_delete                                **
**功能描述: 删除共享内存                              **
**作    者:                                           **
**输入参数:                                           **
**          shmname：共享内存名                       **
**          mem：共享内存地址                         **
**输出参数:                                           **
**返回值:  0/-1                                       **
*******************************************************/
int shm_delete(const char *shmname, const char *mem)
{
    struct shmid_ds shmds;
    int    memid;

    if (mem != NULL && shmdt(mem) == -1) {
        return -1;
    }

    memid = shm_getid(shmname, 0, 0);
    if (memid < 0) 
        return -1;

    return shm_delete_internal(memid);
}

/*******************************************************
**函数名称: msq_getid                                 **
**功能描述: 获取消息队列ID                            **
**作    者:                                           **
**输入参数:                                           **
**          quename：队列名                           **
**          createflg：标志                           **
**输出参数:                                           **
**返回值:  消息队列ID                                 **
*******************************************************/
int msq_getid(const char *quename, int createflg)
{
    int    flags;
    key_t  quekey;

    if (quename != NULL)
        quekey = makekey(quename);
    else
        quekey = IPC_PRIVATE;

    if (createflg == 1) {
        flags = MSQ_PERM | IPC_CREAT;
    } else {
        flags = MSQ_PERM;
    }

    return msq_msqget_internal(quekey, flags);
}

/*******************************************************
**函数名称: msq_create                                **
**功能描述: 创建消息队列                              **
**作    者:                                           **
**输入参数:                                           **
**          quename：队列名                           **
**输出参数:                                           **
**返回值:  消息队列ID                                 **
*******************************************************/
int msq_create(const char *quename)
{
    return msq_getid(quename, 1);
}

/*******************************************************
**函数名称: msq_create_key                            **
**功能描述: 创建消息队列                         **
**作    者:                                           **
**输入参数:                                           **
**          quekey：队列键值                          **
**输出参数:                                           **
**返回值:  消息队列ID                                 **
*******************************************************/
int msq_create_key(long quekey)
{
    int flags = MSQ_PERM | IPC_CREAT;
    return msq_msqget_internal(quekey, flags);
}

/*******************************************************
**函数名称: msq_connect                               **
**功能描述: 连接消息队列                              **
**作    者:                                           **
**输入参数:                                           **
**          quename：队列名                           **
**输出参数:                                           **
**返回值:  消息队列ID                                 **
*******************************************************/
int msq_connect(const char *quename)
{
    return msq_getid(quename, 0);
}


/*******************************************************
**函数名称: msq_connect_key                           **
**功能描述: 连接消息队列                              **
**作    者:                                           **
**输入参数:                                           **
**          quekey：队列键值                          **
**输出参数:                                           **
**返回值:  消息队列ID                                 **
*******************************************************/
int msq_connect_key(long quekey)
{
    int flags = MSQ_PERM;
    return msq_msqget_internal(quekey, flags);
}

/*******************************************************
**函数名称: msq_delete                                **
**功能描述: 删除消息队列                              **
**作    者:                                           **
**输入参数:                                           **
**          quename：队列名                           **
**输出参数:                                           **
**返回值:  0/-1                                       **
*******************************************************/
int msq_delete(const char *quename)
{
    int  msq_id;

    msq_id = msq_getid(quename, 0);
    if (msq_id == -1) return -1;
    return msq_delete_internal(msq_id);
}

/*******************************************************
**函数名称: msq_write                                 **
**功能描述: 发送消息                                  **
**作    者:                                           **
**输入参数:                                           **
**          msq_id：消息ID                            **
**          msg_buf：消息                             **
**          msg_len：消息长度                         **
**          msg_type：消息类型                        **
**          flags：标志                               **
**输出参数:                                           **
**返回值:  0/-1                                       **
*******************************************************/
int msq_write(int msq_id, char *msg_buf, int msg_len, 
              long msg_type, int flags)
{
    if (msgsnd(msq_id, msg_buf, msg_len - sizeof(long), flags) == -1) {
        return (-1);
    }

    return (0);
}

/*******************************************************
**函数名称: msq_read                                 **
**功能描述: 发送消息                                  **
**作    者:                                           **
**输入参数:                                           **
**          msq_id：消息ID                            **
**          msg_len：消息长度                         **
**          msg_type：消息类型                        **
**          flags：标志                               **
**输出参数:                                           **
**          msg_buf：消息                             **
**返回值:  0/-1                                       **
*******************************************************/
int msq_read(int msq_id, char *msg_buf, int msg_len, 
             long msg_type, int flags)
{

    if (msgrcv(msq_id, msg_buf, msg_len - sizeof(long), 
               msg_type, flags) == -1) {
        return (-1);
    }
    
    return (0);
}

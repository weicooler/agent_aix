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
 *  ����:�źź�����
 *
 *  Edit History:
 *
 */
#include "ipcopr.h"

#ifdef LINUX
 typedef unsigned short int ushort;
#endif

/*******************************************************
**��������: makekey                                   **
**��������: ����ָ�����Ƶļ�ֵ                        **
**��    ��:                                           **
**�������:                                           **
**          name������                                **
**�������:                                           **
**����ֵ:  ��ֵ                                       **
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
**��������: shm_shmget_internal                       **
**��������: ��ȡ�����ڴ�id                            **
**��    ��:                                           **
**�������:                                           **
**          shm_key�������ڴ��ֵ                     **
**          name�������ڴ��С                        **
**          flags����־                                **
**�������:                                           **
**����ֵ:  �����ڴ�ID                                 **
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
**��������: shm_attach_internal                       **
**��������: ���ӹ����ڴ�                              **
**��    ��:                                           **
**�������:                                           **
**          shmid�������ڴ�ID                         **
**�������:                                           **
**����ֵ:  shm                                        **
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
**��������: shm_delete_internal                       **
**��������: ɾ�������ڴ�                              **
**��    ��:                                           **
**�������:                                           **
**          shmid�������ڴ�ID                         **
**�������:                                           **
**����ֵ:  0/-1                                       **
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
**��������: sem_semget_internal                       **
**��������: ��ȡ�źŶ���                              **
**��    ��:                                           **
**�������:                                           **
**          sem_key: �źż�ֵ                         **
**          sem_size���źŴ�С                        **
**          flags����־                               **
**�������:                                           **
**����ֵ:  �ź�ID                                     **
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
**��������: sem_attach_internal                       **
**��������: �����ź�                                  **
**��    ��:                                           **
**�������:                                           **
**          sem_id: �ź�ID                         **
**          sem_size���źŴ�С                        **
**�������:                                           **
**����ֵ:  �ź�ID                                     **
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
**��������: sem_delete_internal                       **
**��������: ɾ�������ڴ�                              **
**��    ��:                                           **
**�������:                                           **
**         sem_id�������ڴ�ID                         **
**�������:                                           **
**����ֵ:  0/-1                                           **
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
**��������: msq_msqget_internal                       **
**��������: ������Ϣ����                              **
**��    ��:                                           **
**�������:                                           **
**          key����ֵ                                 **
**          flags����־                               **
**�������:                                           **
**����ֵ:   msq_id                                   **
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
**��������: msq_delete_internal                       **
**��������: ɾ����Ϣ����                              **
**��    ��:                                           **
**�������:                                           **
**          msq_id������ID                            **
**�������:                                           **
**����ֵ:   0/-1                                      **
*******************************************************/
int msq_delete_internal(int msq_id)
{
    if (msgctl(msq_id, IPC_RMID, NULL) == -1) {
        return (-1);
    }
   
    return (0);    
}

/*******************************************************
**��������: sem_getid                                 **
**��������: ��ȡ�ź�ID                                **
**��    ��:                                           **
**�������:                                           **
**          semname���ź�����                         **
**          semsize���źŴ�С                         **
**          createflg��������־                       **
**�������:                                           **
**����ֵ:   idֵ                                      **
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
**��������: sem_getid                                 **
**��������: �źŴ���                                  **
**��    ��:                                           **
**�������:                                           **
**          sem_num���źű��                         **
**          sem_id���ź�ID                            **
**          op�������                                **
**�������:                                           **
**����ֵ:   0/-1                                      **
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
**��������: sem_create                                **
**��������: �����ź�                                  **
**��    ��:                                           **
**�������:                                           **
**          semname���ź���                           **
**          semsize���źŴ�С                         **
**�������:                                           **
**����ֵ: idֵ                                         **
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
**��������: sem_delete                                **
**��������: ɾ���ź�                                  **
**��    ��:                                           **
**�������:                                           **
**          semname���ź���                           **
**          semsize���źŴ�С                         **
**�������:                                           **
**����ֵ:   0/-1                                      **
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
**��������: sem_connect                               **
**��������: �ź�����                                  **
**��    ��:                                           **
**�������:                                           **
**          semname���ź���                           **
**          semsize���źŴ�С                         **
**�������:                                           **
**����ֵ: idֵ                                        **
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
**��������: sem_lock                                  **
**��������: �ź�����                                  **
**��    ��:                                           **
**�������:                                           **
**          sem_num���źű��                         **
**          sem_id���ź�ID                            **
**�������:                                           **
**����ֵ:   0/-1                                      **
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
**��������: sem_unlock                                **
**��������: �źŽ���                                  **
**��    ��:                                           **
**�������:                                           **
**          sem_num���źű��                         **
**          sem_id���ź�ID                            **
**�������:                                           **
**����ֵ:   0/-1                                      **
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
**��������: shm_getid                                 **
**��������: ��ȡ�����ڴ�ID                            **
**��    ��:                                           **
**�������:                                           **
**          shmname�������ڴ���                       **
**          shmsize�������ڴ��С                     **
**          createflg��������־                       **
**�������:                                           **
**����ֵ:  shmid                                      **
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
**��������: shm_create                                **
**��������: ���������ڴ�                              **
**��    ��:                                           **
**�������:                                           **
**          shmname�������ڴ���                       **
**          shmsize�������ڴ��С                     **
**�������:                                           **
**����ֵ:  shm                                        **
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
**��������: shm_connect                               **
**��������: ���ӹ����ڴ�                              **
**��    ��:                                           **
**�������:                                           **
**          shmname�������ڴ���                       **
**�������:                                           **
**����ֵ:  shm                                        **
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
**��������: shm_delete                                **
**��������: ɾ�������ڴ�                              **
**��    ��:                                           **
**�������:                                           **
**          shmname�������ڴ���                       **
**          mem�������ڴ��ַ                         **
**�������:                                           **
**����ֵ:  0/-1                                       **
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
**��������: msq_getid                                 **
**��������: ��ȡ��Ϣ����ID                            **
**��    ��:                                           **
**�������:                                           **
**          quename��������                           **
**          createflg����־                           **
**�������:                                           **
**����ֵ:  ��Ϣ����ID                                 **
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
**��������: msq_create                                **
**��������: ������Ϣ����                              **
**��    ��:                                           **
**�������:                                           **
**          quename��������                           **
**�������:                                           **
**����ֵ:  ��Ϣ����ID                                 **
*******************************************************/
int msq_create(const char *quename)
{
    return msq_getid(quename, 1);
}

/*******************************************************
**��������: msq_create_key                            **
**��������: ������Ϣ����                         **
**��    ��:                                           **
**�������:                                           **
**          quekey�����м�ֵ                          **
**�������:                                           **
**����ֵ:  ��Ϣ����ID                                 **
*******************************************************/
int msq_create_key(long quekey)
{
    int flags = MSQ_PERM | IPC_CREAT;
    return msq_msqget_internal(quekey, flags);
}

/*******************************************************
**��������: msq_connect                               **
**��������: ������Ϣ����                              **
**��    ��:                                           **
**�������:                                           **
**          quename��������                           **
**�������:                                           **
**����ֵ:  ��Ϣ����ID                                 **
*******************************************************/
int msq_connect(const char *quename)
{
    return msq_getid(quename, 0);
}


/*******************************************************
**��������: msq_connect_key                           **
**��������: ������Ϣ����                              **
**��    ��:                                           **
**�������:                                           **
**          quekey�����м�ֵ                          **
**�������:                                           **
**����ֵ:  ��Ϣ����ID                                 **
*******************************************************/
int msq_connect_key(long quekey)
{
    int flags = MSQ_PERM;
    return msq_msqget_internal(quekey, flags);
}

/*******************************************************
**��������: msq_delete                                **
**��������: ɾ����Ϣ����                              **
**��    ��:                                           **
**�������:                                           **
**          quename��������                           **
**�������:                                           **
**����ֵ:  0/-1                                       **
*******************************************************/
int msq_delete(const char *quename)
{
    int  msq_id;

    msq_id = msq_getid(quename, 0);
    if (msq_id == -1) return -1;
    return msq_delete_internal(msq_id);
}

/*******************************************************
**��������: msq_write                                 **
**��������: ������Ϣ                                  **
**��    ��:                                           **
**�������:                                           **
**          msq_id����ϢID                            **
**          msg_buf����Ϣ                             **
**          msg_len����Ϣ����                         **
**          msg_type����Ϣ����                        **
**          flags����־                               **
**�������:                                           **
**����ֵ:  0/-1                                       **
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
**��������: msq_read                                 **
**��������: ������Ϣ                                  **
**��    ��:                                           **
**�������:                                           **
**          msq_id����ϢID                            **
**          msg_len����Ϣ����                         **
**          msg_type����Ϣ����                        **
**          flags����־                               **
**�������:                                           **
**          msg_buf����Ϣ                             **
**����ֵ:  0/-1                                       **
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

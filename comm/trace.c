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
 *  ����: ���ٺ�����
 *
 *  Edit History:
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/times.h>
#include <sys/timeb.h>
#include "trace.h"

static const char *g_LevelDsp[] = {
    "NONE", "FATAL", "ERROR", "WARNING", "INFO", "DEBUG"
};
static int nLogSize;
static trace_ctrl_t *ptTraceCtrl;
static trace_item_t *ptTraceItem;
static struct loginfo stLogInfo;

int  *TraceLevel;
char TraceName[TRACE_LEN_MAX + 1];

/*******************************************************
**��������: LogMsg                                    **
**��������: ��¼��Ϣ                                  **
**��    ��:                                           **
**�������:                                           **
**          level������                               **
**          filename������                            **
**          line������                                **
**          fmt����ʽ������                           **
**�������:                                           **
**����ֵ:                                             **
*******************************************************/
int LogMsg(int level, const char *filename,
           int line, const char *fmt, ...)
{
    va_list      ap;
    FILE         *fp;
    char         sLogFile[128 + 1];
    char         sCurrTime[6 + 1];
    struct timeb tTimeB;
    char         sMilliTM[4];

    memset(sLogFile, 0, sizeof(sLogFile));
    LogFile(sLogFile);
    GetTime_HHMMSS(sCurrTime);
    memset(&tTimeB, 0, sizeof(tTimeB));    
    ftime(&tTimeB);
    snprintf(sMilliTM, sizeof(sMilliTM), "%03d", tTimeB.millitm);
   
    fp = fopen(sLogFile, "a+");
    if (fp != (FILE*)NULL) {
        fprintf(fp, "[%08d][%.6s:%.3s][%16s][%04d][%7s]",
                getpid(), sCurrTime, sMilliTM, filename, line, g_LevelDsp[level]);
        va_start(ap, fmt);
        vfprintf(fp, fmt, ap);
        va_end(ap);
        fprintf(fp, "\n");
        fflush(fp);
        fclose(fp);
    }

    return 0;
}

/*******************************************************
**��������: LogFile                                   **
**��������: ��ȡ��־�ļ��ľ���·��                    **
**��    ��:                                           **
**�������:                                           **
**          sFullLogName��                            **
**�������:                                           **
**����ֵ:  0/-1                                       **
*******************************************************/
int LogFile(char *sFullLogName)
{
    char        sDate[8 + 1];
    char        sDateTime[14 + 1];
    struct stat statbuf;
    int         rc;
    char        sFullBakLogName[TRACE_LEN_MAX + 14 + 2]; 
    char        sTmpDir[32];
 
    memset(sDate, 0x00, sizeof(sDate));
    /*modify by liangww for ֱ���ڵ�ǰĿ¼�� ����Ŀ¼ */
    /* chdir(getenv("TRACEHOME")); */
    
    /* mkdir date directory */
    CommonGetCurrentDate(sDate);
    mkdir(sDate, S_IRWXU|S_IRWXG|S_IROTH);
    /* ��������Ŀ¼ */
    memset(sTmpDir, 0x00, sizeof(sTmpDir));
  /*  sprintf(sTmpDir, "%s/%s",sDate,"offlbat"); */
    sprintf(sTmpDir, "%s","mario_log"); 
    mkdir(sTmpDir, S_IRWXU|S_IRWXG|S_IROTH);
    /*strcat(sFullLogName, sDate);*/

    /* set log file name */
    if (TraceName[0] == '\0') 
        sprintf(sFullLogName, "%s", LOG_DEFAULT_NAME);
    else
        sprintf(sFullLogName, "%s",  TraceName);

    /* this is LOG_SWITCH_MODE_SIZE */
    /* check file size */
    memset(&statbuf, 0x00, sizeof(statbuf));
    rc = stat(sFullLogName, &statbuf);
    if (rc == 0 && statbuf.st_size >= nLogSize) {
        memset(sFullBakLogName, 0x00, sizeof(sFullBakLogName));
        memset(sDateTime, 0x00, sizeof(sDateTime));
        CommonGetCurrentTime(sDateTime);
        snprintf(sFullBakLogName, sizeof(sFullBakLogName), 
                 "%s.%s", sFullLogName, sDateTime);
        rename(sFullLogName, sFullBakLogName);
    }
    return 0;
}

/*******************************************************
**��������: TraceMsg                                  **
**��������: ��¼��Ϣ                                  **
**��    ��:                                           **
**�������:                                           **
**          level������                               **
**          filename���ļ���                          **
**          line���к�                                **
**          sBuf����Ϣ                                **
**          nLen����Ϣ����                            **
**�������:                                           **
**����ֵ:  0/-1                                       **
*******************************************************/
int TraceMsg(int level, const char *filename,
             int line, const char *sBuf, int nLen)
{
    char      sLogFile[128 + 1];
    char      sCurrTime[6 + 1];
    int       i, j = 0;
    char      s[100], Temp[5];
    FILE      *fp = NULL;
    struct timeb tTimeB;
    char         sMilliTM[4];

    memset(sLogFile, 0, sizeof(sLogFile));
    LogFile(sLogFile);
    GetTime_HHMMSS(sCurrTime);
    memset(&tTimeB, 0, sizeof(tTimeB));    
    ftime(&tTimeB);
    snprintf(sMilliTM, sizeof(sMilliTM), "%03d", tTimeB.millitm);

    if ((fp = fopen(sLogFile, "a+") ) == NULL) return 0;

    fprintf(fp, "[%08d][%.6s:%.3s][%16s][%04d][%7s] debug buffer length: %d.........\n",
            getpid(), sCurrTime, sMilliTM, filename, line, g_LevelDsp[level], nLen);
    
    memset(s, 0, sizeof(s));
    memset(s, '-', 76);
    fprintf(fp, "%s\n", s);
    for (i = 0; i < nLen; i++){
       if (j == 0){
           memset(s, ' ', 84);
           sprintf(Temp, " %03d:", i);
           memcpy(s, Temp, 5);
           sprintf(Temp, ":%03d", i + 15);
           memcpy(&s[72], Temp, 4);
       }

       sprintf(Temp, "%02X ", (unsigned char)sBuf[i]);
       memcpy(&s[j * 3 + 5 + (j > 7)], Temp, 3);
       if (isprint((unsigned char)sBuf[i])){
           s[j + 55 + (j > 7)] = sBuf[i];
       }else{
           if (sBuf[i] == 0)
               s[j + 55 + (j > 7)] = ' ';
           else
               s[j + 55 + (j > 7)] = '.';
      }

      j++;
      if (j == 16){
          s[76] = 0;
          fprintf(fp, "%s\n", s);
          j = 0;
      }
    }

    if (j){
        s[76] = 0;
        fprintf(fp, "%s\n", s);
    }
    memset(s, 0, sizeof(s));
    memset(s, '-', 76);
    fprintf(fp, "%s\n", s);

    fclose(fp);
    return 0;
}

#if 1
/*******************************************************
**��������: trace_on                                  **
**��������: ���ø��������ļ�����                      **
**��    ��:                                           **
**�������:                                           **
**          buf��������                               **
**�������:                                           **
**����ֵ:   0/-1                                      **
*******************************************************/
int trace_on(const char *fname)
{
    int   trace_level;
    char  buf[128 + 1];
    char  buf1[128 + 1];
    char  trace_name[32 + 1];
    FILE  *fp;

    if (trace_init() < 0) 
        return -1;

    fp = (FILE *)cf_open(SYS_PARAM_CFG);
    if (fp == NULL) {
        fprintf(stderr, "trace_on: cfg_open %s error %d %s\n",
                SYS_PARAM_CFG, errno, strerror(errno));
        return -1;
    }

    memset(buf, 0, sizeof(buf));
    memset(buf1, 0, sizeof(buf1));
    memset(trace_name, 0, sizeof(trace_name));
    strncpy(trace_name, fname, 32);
    sprintf(buf, "tracelevel.%s", trace_name);
  
    cf_locate(fp, buf, buf1);
    if (buf1[0] == '\0') 
        cf_locate(fp, "trace.defaultlevel", buf1);
    if (buf1[0] == '\0') /* default debug level */ 
        strcpy(buf1, "DEBUG");

    if (cf_locatenum(fp, "trace.logsize", &nLogSize) < 0 || nLogSize <= 0)
        nLogSize = 5;
    nLogSize *= LOG_SIZE_UNIT;     
 
    cf_close(fp);

    CommonRTrim(buf1);
    getTraceLevel(buf1, &trace_level);
    trace_set(trace_name, trace_level); 
    return 0;
}

/*******************************************************
**��������: getTraceLevel                             **
**��������: ��ȡָ�������ļ����                      **
**��    ��:                                           **
**�������:                                           **
**          buf��������                               **
**�������:                                           **
**          trace_level������                         **
**����ֵ:   ���ټ���/0                                **
*******************************************************/
int getTraceLevel(char *buf, int *trace_level)
{
    if (!strcasecmp(buf, "none")) {
        *trace_level = 0;
    } else if (!strcasecmp(buf, "fatal")) {
        *trace_level = 1;
    } else if (!strcasecmp(buf, "error")) {
        *trace_level = 2;
    } else if (!strcasecmp(buf, "warning")) {
        *trace_level = 3;
    } else if (!strcasecmp(buf, "info")) {
        *trace_level = 4;
    } else if (!strcasecmp(buf, "debug")) {
        *trace_level = 5;
    } else {
        *trace_level = 0;
    }
    return 0;
}

/*******************************************************
**��������: trace_init                                **
**��������: ���ٳ�ʼ��                                **
**��    ��:                                           **
**�������:                                           **
**�������:                                           **
**����ֵ:   0/-1                                      **
*******************************************************/
int trace_init()
{
    int  i, value, shmsize, semid, max_trace;
    FILE *fp;

    semid = sem_create(TRC_SEM_NAME, 1);
    if (semid < 0) {
        fprintf(stderr, "trace_init: sem_create %s error %d %s\n",
                TRC_SEM_NAME, errno, strerror(errno));
        return -1;
    }

    trace_lock(semid);
    ptTraceCtrl = (trace_ctrl_t *)shm_connect(TRC_MEM_NAME);
    if (ptTraceCtrl == NULL) {
        if ((fp = (FILE *)cf_open(SYS_PARAM_CFG)) == NULL) {
            fprintf(stderr, "trace_init: cfg_open %s error %d %s\n",
                    SYS_PARAM_CFG, errno, strerror(errno)); 
            return -1;
        }
        if (cf_locatenum(fp, "trace.max", &value) < 0 || value < 0)
            max_trace = 200;
        else 
            max_trace = value;
        if (max_trace < 50) 
            max_trace = 200;
        
        cf_close(fp);

        shmsize = sizeof(trace_ctrl_t) + sizeof(trace_item_t) * max_trace;
        ptTraceCtrl = (trace_ctrl_t *)shm_create(TRC_MEM_NAME, shmsize);
        if (ptTraceCtrl == NULL) {
            fprintf(stderr, "trace_init: shm_create %s error %d %s\n",
                    TRC_MEM_NAME, errno, strerror(errno));
            return -1;
        }
 
        ptTraceCtrl->semid = semid;
        ptTraceCtrl->maxnum = max_trace;
        ptTraceCtrl->usenum = 0;
        ptTraceItem = (trace_item_t*)&ptTraceCtrl[1];
        for (i = 0; i < ptTraceCtrl->maxnum; i++) {
            ptTraceItem[i].trace_flag = 0;
            ptTraceItem[i].trace_level = 0;
            memset(ptTraceItem[i].trace_name, 0, sizeof(ptTraceItem[i].trace_name));
        }
    } else {
        ptTraceItem = (trace_item_t *)&ptTraceCtrl[1];
    }

    trace_unlock(semid);

    return 0;
}

#endif 

/*******************************************************
**��������: trace_set                                **
**��������: ���ø�����Ϣ                             **
**��    ��:                                          **
**�������:                                          **
**          fname������                              **
**          level������                              **
**�������:                                          **
**����ֵ:   0/-1                                     **
*******************************************************/
int trace_set(const char *fname, int level)
{
    int  i, pos, trace;

    if (ptTraceCtrl == NULL || ptTraceItem == NULL) return -1;

    pos = -1;
    trace_lock(ptTraceCtrl->semid);
    for (i = 0; i < ptTraceCtrl->maxnum; i++) {
        if (ptTraceItem[i].trace_flag == 1 && 
            !strcasecmp(ptTraceItem[i].trace_name, fname))
            break;
        if (pos == -1 && ptTraceItem[i].trace_flag == 0) 
            pos = i;
    }

    if (i < ptTraceCtrl->maxnum) {
        pos = i;
        ptTraceItem[pos].trace_level = level;
    } else if (pos != -1) {
        ptTraceItem[pos].trace_flag = 1;
        ptTraceItem[pos].trace_level = level;
        strncpy(ptTraceItem[pos].trace_name, fname, 32);
        ptTraceCtrl->usenum++;
    }

    if (pos != -1)
        TraceLevel = &ptTraceItem[pos].trace_level;

    trace_unlock(ptTraceCtrl->semid);
    return 0;
}
/*******************************************************
**��������: trace_set_level                           **
**��������: ���ø��ټ���                              **
**��    ��:                                           **
**�������:                                           **
**          level������                               **
**�������:                                           **
**����ֵ:   0/-1                                      **
*******************************************************/
int trace_set_level(int level)
{
    if (TraceLevel != NULL) 
        *TraceLevel = level;
    return 0;
}

/*******************************************************
**��������: trace_get_level                           **
**��������: ��ȡ���ټ���                              **
**��    ��:                                           **
**�������:                                           **
**�������:                                           **
**����ֵ:   ���ټ���/-1                               **
*******************************************************/
int trace_get_level(void)
{
    if (TraceLevel != NULL) 
        return *TraceLevel;
    else 
        return -1;
}

/*******************************************************
**��������: trace_lock                                **
**��������: ����ָ���ĸ���                            **
**��    ��:                                           **
**�������:                                           **
**          semid���ź�ID                             **
**�������:                                           **
**����ֵ:   ���ټ���/-1                               **
*******************************************************/
int trace_lock(int semid)
{
    return sem_lock(0, semid);
}

/*******************************************************
**��������: trace_unlock                              **
**��������: Ϊָ���ĸ��ٽ���                          **
**��    ��:                                           **
**�������:                                           **
**          semid���ź�ID                             **
**�������:                                           **
**����ֵ:   ���ټ���/-1                               **
*******************************************************/
int trace_unlock(int semid)
{
    return sem_unlock(0, semid);
}

/*******************************************************
**��������: trace_set_name                            **
**��������: ������־�ļ���                            **
**��    ��:                                           **
**�������:                                           **
**          name������                                **
**�������:                                           **
**����ֵ:                                             **
*******************************************************/
void trace_set_name(const char *name)
{
    strncpy(TraceName, name, TRACE_LEN_MAX);
}


/*******************************************************
**��������: trace_get_name                            **
**��������: ��ȡ��־�ļ���                            **
**��    ��:                                           **
**�������:                                           **
**�������:                                           **
**          name������                                **
**����ֵ:                                             **
*******************************************************/
void trace_get_name(char *name)
{
    strncpy(name, TraceName, TRACE_LEN_MAX);
}


void TimeLog(char *sFuncName, int n)
{
    static long lBeginTime;
    long lEndTime;
    struct tms  tagTMS;

    if (n == 1) {
        lBeginTime = times(&tagTMS);
        return;
    } else if (n == 2) {
        lEndTime = times(&tagTMS);
        LogInfo("%s Handle Times: %ld ticks", sFuncName, lEndTime - lBeginTime);
        return;
    }
    return;
}

/*******************************************************
**��������: trace_init_file                           **
**��������: �ļ�������ٳ�ʼ��                        **
**��    ��:                                           **
**�������:                                           **
**�������:                                           **
**����ֵ:   0/-1                                      **
*******************************************************/
int trace_init_file()
{
    int  i, value, shmsize, semid, max_trace;
    FILE *fp;

    semid = sem_create(FS_TRC_SEM_NAME, 1);
    if (semid < 0) {
        fprintf(stderr, "trace_init: sem_create %s error %d %s\n",
                FS_TRC_SEM_NAME, errno, strerror(errno));
        return -1;
    }

    trace_lock(semid);
    ptTraceCtrl = (trace_ctrl_t *)shm_connect(FS_TRC_MEM_NAME);
    if (ptTraceCtrl == NULL) {
        max_trace = stLogInfo.nLogFileNum;

        shmsize = sizeof(trace_ctrl_t) + sizeof(trace_item_t) * max_trace;
        ptTraceCtrl = (trace_ctrl_t *)shm_create(FS_TRC_MEM_NAME, shmsize);
        if (ptTraceCtrl == NULL) {
            fprintf(stderr, "trace_init: shm_create %s error %d %s\n",
                    FS_TRC_MEM_NAME, errno, strerror(errno));
            return -1;
        }

        ptTraceCtrl->semid = semid;
        ptTraceCtrl->maxnum = max_trace;
        ptTraceCtrl->usenum = 0;
        ptTraceItem = (trace_item_t*)&ptTraceCtrl[1];
        for (i = 0; i < ptTraceCtrl->maxnum; i++) {
            ptTraceItem[i].trace_flag = 0;
            ptTraceItem[i].trace_level = 0;
            memset(ptTraceItem[i].trace_name, 0, sizeof(ptTraceItem[i].trace_name));
        }
    }
    else {
        ptTraceItem = (trace_item_t *)&ptTraceCtrl[1];
    }

    trace_unlock(semid);

    return 0;
}

/*******************************************************
**��������: trace_on_file                             **
**��������: ���ø��������ļ�����                      **
**��    ��:                                           **
**�������:                                           **
**          buf��������                               **
**�������:                                           **
**����ֵ:   0/-1                                      **
*******************************************************/
int trace_on_file(const char *fname )                 
{
    int   trace_level;                                
    char  buf[128 + 1];                               
    char  buf1[128 + 1];                              
    char  trace_name[32 + 1];                         
    FILE  *fp;
    char sIniPath[256 + 1];

    memset(sIniPath, 0, sizeof(sIniPath));
    sprintf(sIniPath, "%s/etc/", getenv("PRODUCTHOME") );
        
    memset(&stLogInfo, 0, sizeof(stLogInfo));
    if ( getLogCfg(sIniPath, &stLogInfo) != 0) {
        return -1;
    }

    if (trace_init_file() < 0)
        return -1;

    nLogSize = stLogInfo.nLogFileSize;

    memset(trace_name, 0, sizeof(trace_name));
    strncpy(trace_name, fname, 32);

    trace_set(trace_name, stLogInfo.nLogLevel);
    return 0;
}

/**********************************************************
 * ������getLogCfg()
 * ����������
 *     ��ȡfilehosts.ini�����ļ���LOG�����ݵ��ṹ��
 * ���������
 *    1)  char *chIniFilePath : filehosts.ini���ڵ�Ŀ¼��
 * ���������
 *    1)  struct hostsCommon *stHostsCommon: Common�����ݽṹ��
 * ����ֵ��
 *    int     : ������
 *           0: �ɹ�
 *        1001: filehosts.iniֻ����ʽ��ʧ��
 *        1015: Server_PortС��1024
 *        ����: ���checkParamIsNULL()��checkValue()
 ***********************************************************/
int getLogCfg(char *chIniFilePath, char *sHostIniFileName, struct loginfo *stLogInfo)
{   
    /* ������� */
    char chTmpVal[256+1];
    long num1=0, num2=0, num3=0;
    FILE *fp;
    
    FILE *fpTemp;
    char chTmpFile[512+1];
        
    memset(chTmpFile,'\0',sizeof(chTmpFile));
    sprintf(chTmpFile,"%s%s", chIniFilePath, sHostIniFileName);
    
    fpTemp = fopen(chTmpFile,"r");
    if(fpTemp == NULL)
    {
        printf("�����ļ�%s������", chTmpFile);
        return 1001;
    }
    fclose(fpTemp);

    fp = (FILE *)openConfigFile(chIniFilePath, sHostIniFileName);
    if(fp == NULL)
    {
        return 1002;
    }

     memset(stLogInfo,'\0',sizeof(struct loginfo));

    memset(chTmpVal,'\0',sizeof(chTmpVal));
    ReadCfgFile(fp, "LOG", "LOGFILE_NUM", "30",chTmpVal);
    stLogInfo->nLogFileNum = atoi(chTmpVal);

    memset(chTmpVal,'\0',sizeof(chTmpVal));
    ReadCfgFile(fp, "LOG", "LOGFILE_SIZE", "2000000",chTmpVal);
    stLogInfo->nLogFileSize = atol(chTmpVal);
    if(stLogInfo->nLogFileSize < 512 * 1024)
    {
        stLogInfo->nLogFileSize = 512 * 1024;
    }

    memset(chTmpVal,'\0',sizeof(chTmpVal));
    ReadCfgFile(fp, "LOG", "LOG_LEVEL", "INFO",chTmpVal);
    if(strcmp(chTmpVal, "ERROR") == 0)
    {
        stLogInfo->nLogLevel = TRACE_ERROR;
    }
    else if(strcmp(chTmpVal, "DEBUG") == 0)
    {
        stLogInfo->nLogLevel = TRACE_DEBUG;
    }
    else
    {
        stLogInfo->nLogLevel = TRACE_NORMAL;
    }

    fclose(fp);
    return 0;
}


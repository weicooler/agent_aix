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
 *  ����: �����ļ�����
 *
 *  Edit History:
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include "trace.h"


char * AllTrim(char *str)
{
    int i, j, k, h;

    for (i = 0; str[i] == ' ' && str[i] != '\0'; i++);
    for (j = strlen(str) - 1; str[j] == ' ' && j >= 0 ; j--);

    for (k = 0; k <= j - i; k++) 
        str[k] = str[k + i]; 
    str[k] = '\0';

    while(str[k] != 0){ 
        str[k] = 0;  
        k++; 
    }    

    return str; 
}

char * GetTime_HHMMSS( char * buff )
{
    time_t time(), now; 
    struct tm * tTime, * localtime();

    time( &now );
    tTime = localtime( &now );

    sprintf( buff, "%.2d%.2d%.2d", tTime->tm_hour,
        tTime->tm_min, tTime->tm_sec );
    return buff;
}


int IsDigitBuf(char *sBuf, int nLen)
{
    int    i;  

    if (nLen == 0)
        return 1;

    for (i = 0; i < nLen; i++)
        if (!isdigit(sBuf[i]))
            return 0;

    return 1;
}

/*******************************************************
**��������: CommonRTrim                               **
**��������: ���ҿո�                                  **
**��    ��:                                           **
**�������:                                           **
**          caDest Դ����                             **
**�������:                                           **
**          caDest Ŀ������                           **
**����ֵ:                                             **
*******************************************************/
void CommonRTrim(char *caDest)
{
    int i;
    for (i = strlen(caDest)-1 ; i>=0 ; i-- ) {
        if (caDest[i] !=' ' && caDest[i] !='\t' && caDest[i] !='\r' && caDest[i] !='\n')
            break;
    }
    caDest[i+1] = 0;
}

/*******************************************************
**��������: CommonRTrim                               **
**��������: ����ո�                                  **
**��    ��:                                           **
**�������:                                           **
**          caDest Դ����                             **
**�������:                                           **
**          caDest Ŀ������                           **
**����ֵ:                                             **
*******************************************************/
void CommonLTrim(char *caDest)
{
    char *s = caDest;

    while ( *s )
        if ( (*s == ' ') || (*s == '\t') || (*s == '\r') || (*s == '\n') )
          ++s;
        else
            break;

    if ( s > caDest )
        strcpy ( (char *)caDest, (char *)s );
}

/*******************************************************
**��������: CommonGetCurrentDate                      **
**��������: ��ȡ��ǰ����                              **
**��    ��:                                           **
**�������:                                           **
**�������:                                           **
**          sCurrentDate���õ�������                  **
**����ֵ:                                             **
*******************************************************/
void  CommonGetCurrentDate(char *sCurrentDate)
{
   time_t current;
   struct tm *tmCurrentTime;

   time(&current);
   tmCurrentTime = localtime(&current);
   sprintf(sCurrentDate, "%4d%02d%02d", tmCurrentTime->tm_year + 1900,
           tmCurrentTime->tm_mon + 1, tmCurrentTime->tm_mday);
}

/*******************************************************
**��������: CommonGetCurrentTime                      **
**��������: ��ȡ��ǰʱ��                              **
**��    ��:                                           **
**�������:                                           **
**�������:                                           **
**          sCurrentTime���õ���ʱ��                  **
**����ֵ:                                             **
*******************************************************/
void CommonGetCurrentTime(char *sCurrentTime)
{
   time_t current;
   struct tm *tmCurrentTime;

   tzset();
   time(&current);
   tmCurrentTime = localtime(&current);
   sprintf(sCurrentTime, "%4d%02d%02d%02d%02d%02d",
           tmCurrentTime->tm_year + 1900, tmCurrentTime->tm_mon + 1,
           tmCurrentTime->tm_mday, tmCurrentTime->tm_hour,
           tmCurrentTime->tm_min, tmCurrentTime->tm_sec);
}

/*******************************************************
**��������: cf_open                                   **
**��������: ��ֻ����ʽ���ļ�                        **
**��    ��:                                           **
**�������:                                           **
**          file���ļ���                              **
**�������:                                           **
**����ֵ:  �ļ�������                                 **
*******************************************************/
FILE* cf_open(const char *file)
{
    FILE *fpcfg = NULL; 

    fpcfg = fopen(file, "r");
    if (fpcfg  == NULL) {
        printf("fopen: [%s] error: [%d]\n", file, errno);
        return NULL;
    }

    return fpcfg;
}

/*******************************************************
**��������: cf_close                                  **
**��������: �ر�ָ�����ļ�������                      **
**��    ��:                                           **
**�������:                                           **
**          fpcfg���ļ�������                         **
**�������:                                           **
**����ֵ:  0/1                                        **
*******************************************************/
int cf_close(FILE* fpcfg)
{
    if (fpcfg != NULL) 
        fclose(fpcfg);
    
    return 0;
}

/*******************************************************
**��������: cf_nextline                               **
**��������: ���ļ��ж�ȡ���ַ���                      **
**��    ��:                                           **
**�������:                                           **
**          fpcfg���ļ�������                         **
**          size�� ��ȡ�ַ����Ĵ�С                   **
**�������:                                           **
**          bp����ȡ���ַ���                          **
**����ֵ:  0/-1                                       **
*******************************************************/
int cf_nextline(FILE* fpcfg, char *bp, int size)
{
    if (fpcfg == NULL)  return -1;
    fgets(bp, size, fpcfg);
    CommonRTrim(bp);
    CommonLTrim(bp);
    return 0;
}

/*******************************************************
**��������: cf_rewind                                 **
**��������: �����ļ����Ķ�дλ��Ϊ�ļ���ͷ            **
**��    ��:                                           **
**�������:                                           **
**          fpcfg���ļ�������                         **
**�������:                                           **
**����ֵ:  0/-1                                       **
*******************************************************/
int cf_rewind(FILE* fpcfg)
{
    if (fpcfg == NULL)  return -1;
    fseek(fpcfg, 0L, SEEK_SET);
    return 0;
}

/*******************************************************
**��������: cf_locate                                 **
**��������: ��ȡ������ֵ                              **
**��    ��:                                           **
**�������:                                           **
**          fpcfg���ļ�������                         **
**          pname��������                             **
**          value������ֵ                             **
**�������:                                           **
**����ֵ:  0/-1                                       **
*******************************************************/
int cf_locate(FILE* fpcfg, const char *pname, char *value)
{
    int   len;
    char  line[512 + 1];

    if (fpcfg == NULL) return -1;

    cf_rewind(fpcfg);
    while (!feof(fpcfg)) {
        memset(line, 0, sizeof(line));
        cf_nextline(fpcfg, line, sizeof(line));
        if (line[0] == '#') continue;
        if ((line[strlen(pname)] == 0x00 || line[strlen(pname)] == 0x20) &&
            !memcmp(line, pname, strlen(pname))) {
            break;
        }
    }

    if (feof(fpcfg)) return -1;

    strcpy(value, &line[strlen(pname)]);
    CommonRTrim(value);
    CommonLTrim(value);
    return 0;
}

/*******************************************************
**��������: cf_locatenum                              **
**��������: ��ȡ������ֵ��������                      **
**��    ��:                                           **
**�������:                                           **
**          fpcfg���ļ�������                         **
**          pname��������                             **
**          value������ֵ                             **
**�������:                                           **
**����ֵ:  0/-1                                       **
*******************************************************/
int cf_locatenum(FILE* fpcfg, const char *pname, int *value)
{
    char    buf[24 + 1];

    if (fpcfg == NULL) return -1;

    memset(buf, 0, sizeof(buf));
    if (cf_locate(fpcfg, pname, buf) < 0) return -1;
    if (!IsDigitBuf(buf, strlen(buf))) return -1;

    *value = atoi(buf);
    return 0;
}

/*******************************************************
**��������: cf_nextparm                               **
**��������: ��ȡ������ֵ                              **
**��    ��:                                           **
**�������:                                           **
**          fpcfg���ļ�������                         **
**          pname��������                             **
**          value������ֵ                             **
**�������:                                           **
**����ֵ:  0/-1                                       **
*******************************************************/
int cf_nextparm(FILE* fpcfg, const char *pname, char *value)
{
    char  line[512 + 1];

    if (fpcfg == NULL) return -1;

    while (!feof(fpcfg)) {
        memset(line, 0, sizeof(line));
        cf_nextline(fpcfg, line, sizeof(line));
        if (line[0] == '#') continue;
        if ((line[strlen(pname)] == 0x00 || line[strlen(pname)] == 0x20) &&
            !memcmp(line, pname, strlen(pname))) {
            break;
        }
    }

    if (feof(fpcfg)) 
        return -1;
    
    CommonLTrim(&line[strlen(pname)]);
    CommonRTrim(line); 
    strcpy(value, &line[strlen(pname)]);
    return 0;
}

/*******************************************************
**��������: cf_locate_group                           **
**��������: ��ȡ�����ֵ                              **
**��    ��:                                           **
**�������:                                           **
**          fpcfg���ļ�������                         **
**          pname��������                             **
**�������:                                           **
**����ֵ:  0/-1                                       **
*******************************************************/
int cf_locate_group(FILE* fpcfg, const char *pname)
{
    char  line[512 + 1];

    if (fpcfg == NULL) return -1;

    while (!feof(fpcfg)) {
        memset(line, 0, sizeof(line));
        cf_nextline(fpcfg, line, sizeof(line));
        if (line[0] == '#') continue;
        if ((line[5] == 0x00 || line[5] == 0x20) &&
            (memcmp(line, "GROUP", 5)))
            continue;

        CommonLTrim(&line[5]);
        if ((line[5 + strlen(pname)] == 0x00 || line[5 + strlen(pname)] == 0x20) &&
            (!memcmp(&line[5], pname, strlen(pname)))) 
            break;
    }

    if (feof(fpcfg))
        return -1;

    return 0;
}

/*******************************************************
**��������: cf_group_param                            **
**��������: ��ȡ�����ֵ                              **
**��    ��:                                           **
**�������:                                           **
**          fpcfg���ļ�������                         **
**          size����С                                **
**�������:                                           **
**          bp:����ֵ                                 **
**����ֵ:  0/-1                                       **
*******************************************************/
int cf_group_param(FILE* fpcfg, char *bp, int size)
{
    char  line[512 + 1];
    
    if (fpcfg == NULL)  return -1;
    while (!feof(fpcfg)) {
        memset(line, 0, sizeof(line));
        cf_nextline(fpcfg, line, sizeof(line));
        if (line[0] == '#') continue;
        if ((line[9] == 0x00 || line[9] == 0x20) &&
            (!memcmp(line, "END_GROUP", 9)))
            return -1;
        memcpy(bp, line, (size > 512 ? 512 : size));
        return 0;
    }
}
/*******************************************************
**��������: cf_env_parse                              **
**��������: ����env                                   **
**��    ��:                                           **
**�������:                                           **
**          name������                                **
**          buf��Դ����                               **
**�������:                                           **
**          value:ֵ                                  **
**����ֵ:  0/-1                                       **
*******************************************************/
int cf_env_parse(char *buf, char* name, char *value)
{
    char *p;

    CommonRTrim(buf);
    CommonLTrim(buf);

    if (*buf == '#')
        return -1;

    p = strchr(buf, '=');
    if (p == NULL)
        return -1;
 
    memcpy(name, buf, p - buf);
    name[p-buf] = '\0';
    CommonRTrim(name);
    strcpy(value, p + 1);
    CommonRTrim(value);
    CommonLTrim(value);
    return 0;
}

int ConvertEnv(char *str)
{
    char envbuf[512];
    char buf[512];
    char *ptr = NULL, *ptr1 = NULL;
    int  len, choice=0;
    
    memset(buf, 0, sizeof(buf));
    ptr = (char *)strpbrk((char *)str, "./");
    if (ptr != NULL) {
        choice = 1;
        memset(envbuf, 0, sizeof(envbuf));
        memcpy(envbuf, &str[1], ptr-str-1);
    } else {
        choice = 2;
        memset(envbuf, 0, sizeof(envbuf));
        strcpy(envbuf, &str[1]);
    }

    ptr1 = (char *)getenv(envbuf);
    if (ptr1 == (char *)NULL) {
        return -1;
    } else {
        if (choice == 1) {
            strcat(buf, ptr1);
            strcat(buf, ptr);
        } else if (choice == 2)
            strcat(buf, ptr1);
    }

    len = strlen(buf);
    memset(str, 0, sizeof(str));
    strcpy(str, buf);
    return 0;
}

/*****************************************************************************/
/* FUNC:   int ReplaceEnvVar(char *str)                                      */
/* INPUT:  str: �ַ���, ���ܰ�����������                                     */
/*              ��: $(HOME)/src/$TERM                                        */
/* OUTPUT: str: �ַ���, ���������Ѿ��ö�Ӧ��ֵ�滻                           */
/* RETURN: 0: �ɹ�, ����: ʧ��                                               */
/* DESC:   ��������ַ���, �����еĻ��������ö�Ӧ��ֵ�滻                    */
/*         ��: $(HOME)/src/$TERM, ���е�$(HOME)��$TERM�ᱻ�滻               */
/*         ����ʱ�뱣֤str��Ӧ��buffer�㹻��, �������滻���Խ��             */
/*****************************************************************************/
int ReplaceEnvVar(char *str)
{
    char *ptr = NULL;
    char buf[512];
    char field[512];
    int  len = 0, nRet, flag;
    int  count = 1, i, j;

    /********************************************************************/
    /*         delete     characters ' ','(',')','\t' from string "str" */
    /********************************************************************/
    len = strlen(str);
    memset(buf, 0, sizeof(buf));

    for(i = 0, j = 0; i < len; i++)
        if (str[i] != ' ' && str[i] != '\t' &&
            str[i] != '(' && str[i] != ')')
        buf[j++] = str[i];

    memset(str, 0, len);
    strcpy(str, buf);

    /********************************************************/
    /*         distinguish if first character is '$' or not */
    /********************************************************/
    if (str[0] == '$')
        flag = 1;
    else
        flag = 2;

    memset(buf, 0, sizeof(buf));
    if (flag == 1) {
        ptr = (char *)strtok((char *)str,"$");
        while (ptr != NULL) {
            memset(field, 0, sizeof(field));
            sprintf(field, "%c%s", '$', ptr);
            nRet = ConvertEnv(field);
            if (nRet != 0)
                return -1;
    
            strcat(buf, field);
            ptr = (char *)strtok((char *)NULL,"$");
        }
    } else if (flag == 2) {
        ptr = (char *)strtok((char *)str, "$");
        while (ptr != NULL) {
            count++;
            if (count != 2) {
                memset(field, 0, sizeof(field));
                sprintf(field, "%c%s", '$', ptr);
                nRet = ConvertEnv(field);
                if (nRet != 0) 
                    return -1;
            } else {
                memset(field, 0, sizeof(field));
                sprintf(field, "%s", ptr);
            }

            strcat(buf, field);
            ptr = (char *)strtok((char *)NULL, "$");
        }
    }

    len = strlen(buf);
    memcpy(str, buf, len+1);
    return 0;
}   

/**********************************************************
 * ������openConfigFile()
 * ����������
 *     ��ֻ����ʽ�������ļ�
 * ���������
 *    2)  char *chPath : �����ļ�����Ŀ¼��
 *    3)  char *chFileName: �����ļ���
 * ���������
 *    ��
 * ����ֵ��
 *    FILE *  : �ļ����
 ***********************************************************/
FILE *openConfigFile(char *chPath,char *chFileName)
{
    FILE *fpTemp;
    char chTmpFile[512+1];

    memset(chTmpFile,'\0',sizeof(chTmpFile));
    if(chPath != NULL)
    {
        sprintf(chTmpFile,"%s/%s",chPath,chFileName);
    }
    else
    {
        sprintf(chTmpFile,"%s",chFileName);
    }
    fpTemp = fopen(chTmpFile,"r");
    /* ���ļ��򿪴�����д�������־*/
    if(fpTemp == NULL)
    {
        printf("%s file opened error.",chTmpFile);
        return NULL;
    }
    return fpTemp;
}

/**********************************************************
 * ������ReadCfgFile()
 * ����������
 *     ��ָ�������ļ��и���ָ������ͼ�����ȡ��ֵ,�����ؼ�ֵ���ڱ�־
 * ���������
 *    1)  FILE *fpConfig : �����ļ����
 *    2)  const char *pchSection : ��������
 *    3)  const char *pchKey : ���õļ���
 *    4)  char  *pchDftVal: ���ص�ȱʡֵ
 * ���������
 *    1)  char  *pchVal: ���õļ�ֵ
 * ����ֵ��
 *    int     : ���ڱ�־�������
 *           0: ���õļ�ֵ������
 *           1: ���õļ�ֵ����
 *        1009: ���������ַΪ�ջ������ͼ���Ϊ��
 ***********************************************************/
int   ReadCfgFile(FILE       *fpConfig,
                  const char *pchSection,
                  const char *pchKey,
                  char         *pchDftVal,
                  char         *pchVal)
{
    char strLine[512];
    char *strSearchRC;
    char strSection[256+1];
    char strKey[64];
    char KeyVal[256];

    if (!fpConfig || !pchSection || !pchKey )
    {
        return 1009;
    }

    if( ( strlen(pchSection) == 0) ||
        ( strlen(pchKey) == 0)  )
    {
        return 1009;
    }
    /* ��ʼ������ */
    memset(strSection, '\0', sizeof(strSection));
    sprintf(strSection, "[%s]", pchSection);
    memset(strKey, '\0', sizeof(strKey));
    sprintf(strKey, "%s=", pchKey);
    memset(KeyVal, '\0', sizeof(KeyVal));

    fseek(fpConfig, 0, SEEK_SET);
    while(!feof(fpConfig))
    {
        memset(strLine, '\0', sizeof(strLine));
        if(!fgets(strLine, sizeof(strLine), fpConfig))
        {
            sprintf(pchVal,pchDftVal);
            return 0;
        }
        /* ������ */
        if(strstr(strLine,strSection))
        {
            while(1)
            {
                memset(strLine, '\0', sizeof(strLine));
                if(!fgets(strLine, sizeof(strLine), fpConfig))
                {
                    sprintf(pchVal,pchDftVal);
                    return 0;
                }
                if((strstr(strLine,"[")) || (strstr(strLine,"]")))
                {
                    sprintf(pchVal,pchDftVal);
                    return 0;
                }
                strSearchRC = strstr(strLine,strKey);
                if(strSearchRC)
                {
                    memcpy(KeyVal,strSearchRC+strlen(strKey),strlen(strSearchRC)-strlen(strKey)-1);
                    /* ɾ��ǰ��ո� */
                    AllTrim(KeyVal);
                    sprintf(pchVal,KeyVal);
                    return 1;
                }
            }
        }
    }
    return 0;
}   
/**************************************
*��������: GetStrItem
*��������: ȡ�ַ����б��ָ����ָ�����Ŀ,
           �������ַ���"12|345|6789" ��ȡƫ��Ϊ1����,���Ϊ"345" (��'|'Ϊ�ָ���)
*�������: src   - Դ��
           ch    - �ָ���
           index - ��Ŀ��ƫ��ֵ(��0��ʼ)
*�������: dest  - Ŀ�괮(Ӧ��֤�������㹻��)
*�� �� ֵ: NULL  - δ�ҵ�ָ��ƫ��ֵ����
          ��NULL - Ŀ�괮
***************************************/
char * GetStrItem(char * dest, const char *src, char ch, unsigned int index)
{          
    unsigned int i;
    int len;
    const char * p, * q;
          
    for (i = 0, p = src; i < index; i ++, p++)
    {
        if ((p = strchr(p, ch)) == NULL)
            return NULL;
    }
    
    q = strchr(p, ch);
    
    len = ((q == NULL) ? strlen(p) : q - p);
    memset(dest, 0,  len + 1);
    memcpy(dest, p, len);
    
    return dest;
}



/**************************************
*��������: Sys_Command
*��������: ��ȡϵͳ�����ֵ
*�������: pCommand- Դ��
*�������: pRstData- Ŀ�괮(Ӧ��֤�������㹻��1024)
*�� �� ֵ: 1  - �쳣
           0  - ��ȷ
***************************************/
int Sys_Command(char *pCommand,char *pRstData) 
{
    FILE *fp_cmd=NULL; 
    char  data[512+1];
    char  sCommand_tmp[512+1];


    memset(data,0x00, sizeof(data));
    memset(sCommand_tmp,0x00, sizeof(sCommand_tmp));
    strcpy(sCommand_tmp,pCommand);
    fp_cmd= popen(sCommand_tmp, "r");
    if (fp_cmd == NULL)
    {   
      printf("popen error!\n");
      return 1;
    }   
    while (fgets(data, sizeof(data)-1, fp_cmd) != NULL)
    {   
#if 1
       if(strlen(pRstData)+strlen(data)>1024*1024){
           printf("ALLData[%s]+data[%s]",pRstData,data);
           printf("outofmemory\n");
           return   1;   
       }
       strcat(pRstData,data);
#endif
    }   

    pclose(fp_cmd);
    return 0;
}


/**************************************
*��������: checkProcessRun
*��������: �жϳ����Ƿ�������
*�������: pProcess- ������
*�������:  ��
*�� �� ֵ: 1 /-1 - �쳣
           0  - ��ȷ
***************************************/
int checkProcessRun(char *pProcess)
{
    int iRet=0;
    FILE *fstream=NULL;
    char  sCommand_tmp[256];
    char buff[1024] = {0};
    char sPid[16] = {0};    
    
    sprintf(sPid, "%d",getpid());
    /* ִ��ϵͳ����-�жϼ���Ƿ������� */
    sprintf("process[%s]",pProcess);
    memset(sCommand_tmp, 0x00, sizeof(sCommand_tmp));
    sprintf(sCommand_tmp,"ps -ef  | grep  -w  %s | grep -v grep | awk '{print $2}'", pProcess);

    if(NULL==(fstream=popen(sCommand_tmp, "r")))    
    {   
        LogError("execute command failed:errno[%d],[%s] ", errno,strerror(errno));    
        return -1;    
    }   
    while(NULL!=fgets(buff, sizeof(buff), fstream)) {
        /* ȥ�����з� */
        if (buff[strlen(buff) - 1] == '\n') {
            buff[strlen(buff) - 1] = '\0';
        }
        if (strlen(buff) > 0) {
            LogDebug("buff[%s],pid[%s]",buff,sPid);
            if (strcmp(buff, sPid) !=0) {
                /* �г��������� */
                return 1; 
            }
        }
    }
    pclose(fstream);
    return 0;
}




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
 *  功能: 配置文件操作
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
**函数名称: CommonRTrim                               **
**功能描述: 清右空格                                  **
**作    者:                                           **
**输入参数:                                           **
**          caDest 源数据                             **
**输出参数:                                           **
**          caDest 目的数据                           **
**返回值:                                             **
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
**函数名称: CommonRTrim                               **
**功能描述: 清左空格                                  **
**作    者:                                           **
**输入参数:                                           **
**          caDest 源数据                             **
**输出参数:                                           **
**          caDest 目的数据                           **
**返回值:                                             **
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
**函数名称: CommonGetCurrentDate                      **
**功能描述: 获取当前日期                              **
**作    者:                                           **
**输入参数:                                           **
**输出参数:                                           **
**          sCurrentDate：得到的日期                  **
**返回值:                                             **
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
**函数名称: CommonGetCurrentTime                      **
**功能描述: 获取当前时间                              **
**作    者:                                           **
**输入参数:                                           **
**输出参数:                                           **
**          sCurrentTime：得到的时间                  **
**返回值:                                             **
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
**函数名称: cf_open                                   **
**功能描述: 以只读方式打开文件                        **
**作    者:                                           **
**输入参数:                                           **
**          file：文件名                              **
**输出参数:                                           **
**返回值:  文件描述符                                 **
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
**函数名称: cf_close                                  **
**功能描述: 关闭指定的文件描述符                      **
**作    者:                                           **
**输入参数:                                           **
**          fpcfg：文件描述符                         **
**输出参数:                                           **
**返回值:  0/1                                        **
*******************************************************/
int cf_close(FILE* fpcfg)
{
    if (fpcfg != NULL) 
        fclose(fpcfg);
    
    return 0;
}

/*******************************************************
**函数名称: cf_nextline                               **
**功能描述: 从文件中读取以字符串                      **
**作    者:                                           **
**输入参数:                                           **
**          fpcfg：文件描述符                         **
**          size： 获取字符串的大小                   **
**输出参数:                                           **
**          bp：获取的字符串                          **
**返回值:  0/-1                                       **
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
**函数名称: cf_rewind                                 **
**功能描述: 重设文件流的读写位置为文件开头            **
**作    者:                                           **
**输入参数:                                           **
**          fpcfg：文件描述符                         **
**输出参数:                                           **
**返回值:  0/-1                                       **
*******************************************************/
int cf_rewind(FILE* fpcfg)
{
    if (fpcfg == NULL)  return -1;
    fseek(fpcfg, 0L, SEEK_SET);
    return 0;
}

/*******************************************************
**函数名称: cf_locate                                 **
**功能描述: 获取变量的值                              **
**作    者:                                           **
**输入参数:                                           **
**          fpcfg：文件描述符                         **
**          pname：变量名                             **
**          value：变量值                             **
**输出参数:                                           **
**返回值:  0/-1                                       **
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
**函数名称: cf_locatenum                              **
**功能描述: 获取变量的值的整型数                      **
**作    者:                                           **
**输入参数:                                           **
**          fpcfg：文件描述符                         **
**          pname：变量名                             **
**          value：变量值                             **
**输出参数:                                           **
**返回值:  0/-1                                       **
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
**函数名称: cf_nextparm                               **
**功能描述: 获取变量的值                              **
**作    者:                                           **
**输入参数:                                           **
**          fpcfg：文件描述符                         **
**          pname：变量名                             **
**          value：变量值                             **
**输出参数:                                           **
**返回值:  0/-1                                       **
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
**函数名称: cf_locate_group                           **
**功能描述: 获取组变量值                              **
**作    者:                                           **
**输入参数:                                           **
**          fpcfg：文件描述符                         **
**          pname：变量名                             **
**输出参数:                                           **
**返回值:  0/-1                                       **
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
**函数名称: cf_group_param                            **
**功能描述: 获取组变量值                              **
**作    者:                                           **
**输入参数:                                           **
**          fpcfg：文件描述符                         **
**          size：大小                                **
**输出参数:                                           **
**          bp:参数值                                 **
**返回值:  0/-1                                       **
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
**函数名称: cf_env_parse                              **
**功能描述: 解析env                                   **
**作    者:                                           **
**输入参数:                                           **
**          name：名字                                **
**          buf：源数据                               **
**输出参数:                                           **
**          value:值                                  **
**返回值:  0/-1                                       **
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
/* INPUT:  str: 字符串, 可能包含环境变量                                     */
/*              如: $(HOME)/src/$TERM                                        */
/* OUTPUT: str: 字符串, 环境变量已经用对应的值替换                           */
/* RETURN: 0: 成功, 其它: 失败                                               */
/* DESC:   对输入的字符串, 将其中的环境变量用对应的值替换                    */
/*         如: $(HOME)/src/$TERM, 其中的$(HOME)和$TERM会被替换               */
/*         调用时须保证str对应的buffer足够大, 不会因替换造成越界             */
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
 * 函数：openConfigFile()
 * 功能描述：
 *     以只读方式打开配置文件
 * 输入参数：
 *    2)  char *chPath : 配置文件所在目录名
 *    3)  char *chFileName: 配置文件名
 * 输出参数：
 *    无
 * 返回值：
 *    FILE *  : 文件句柄
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
    /* 如文件打开错误，则写入错误日志*/
    if(fpTemp == NULL)
    {
        printf("%s file opened error.",chTmpFile);
        return NULL;
    }
    return fpTemp;
}

/**********************************************************
 * 函数：ReadCfgFile()
 * 功能描述：
 *     在指定配置文件中根据指定的域和键名读取键值,并返回键值存在标志
 * 输入参数：
 *    1)  FILE *fpConfig : 配置文件句柄
 *    2)  const char *pchSection : 配置域名
 *    3)  const char *pchKey : 配置的键名
 *    4)  char  *pchDftVal: 返回的缺省值
 * 输出参数：
 *    1)  char  *pchVal: 配置的键值
 * 返回值：
 *    int     : 存在标志或错误码
 *           0: 配置的键值不存在
 *           1: 配置的键值存在
 *        1009: 输入参数地址为空或域名和键名为空
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
    /* 初始化变量 */
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
        /* 查找区 */
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
                    /* 删除前后空格 */
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
*函数名称: GetStrItem
*函数功能: 取字符串中被分隔符分隔的项目,
           例如在字符串"12|345|6789" 中取偏移为1的项,结果为"345" (以'|'为分隔符)
*输入参数: src   - 源串
           ch    - 分隔符
           index - 项目的偏移值(从0开始)
*输出参数: dest  - 目标串(应保证缓冲区足够大)
*返 回 值: NULL  - 未找到指定偏移值的项
          非NULL - 目标串
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
*函数名称: Sys_Command
*函数功能: 获取系统命令返回值
*输入参数: pCommand- 源串
*输出参数: pRstData- 目标串(应保证缓冲区足够大1024)
*返 回 值: 1  - 异常
           0  - 正确
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
*函数名称: checkProcessRun
*函数功能: 判断程序是否已运行
*输入参数: pProcess- 程序名
*输出参数:  无
*返 回 值: 1 /-1 - 异常
           0  - 正确
***************************************/
int checkProcessRun(char *pProcess)
{
    int iRet=0;
    FILE *fstream=NULL;
    char  sCommand_tmp[256];
    char buff[1024] = {0};
    char sPid[16] = {0};    
    
    sprintf(sPid, "%d",getpid());
    /* 执行系统命令-判断监控是否已运行 */
    sprintf("process[%s]",pProcess);
    memset(sCommand_tmp, 0x00, sizeof(sCommand_tmp));
    sprintf(sCommand_tmp,"ps -ef  | grep  -w  %s | grep -v grep | awk '{print $2}'", pProcess);

    if(NULL==(fstream=popen(sCommand_tmp, "r")))    
    {   
        LogError("execute command failed:errno[%d],[%s] ", errno,strerror(errno));    
        return -1;    
    }   
    while(NULL!=fgets(buff, sizeof(buff), fstream)) {
        /* 去除换行符 */
        if (buff[strlen(buff) - 1] == '\n') {
            buff[strlen(buff) - 1] = '\0';
        }
        if (strlen(buff) > 0) {
            LogDebug("buff[%s],pid[%s]",buff,sPid);
            if (strcmp(buff, sPid) !=0) {
                /* 有程序已运行 */
                return 1; 
            }
        }
    }
    pclose(fstream);
    return 0;
}




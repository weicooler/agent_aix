#include <stdio.h> 
#include <time.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include "comm/mario.h"


void init_daemon(void);
int Sys_Command(char *pCommand, char *pRstData);
static char sVersion[8];

main(int argc, char *argv[]) 
{ 
   FILE *fp=NULL; 
   char  sCommand[256];
   char  sCommand_tmp[256];
   char  data[1024];
   char  Ip[64];
   char  port[8];
   char  Http_page[500];
   char  file[256];
   char  process_buf[1024];
   char  sRltData[1024*1024];
   char  process[64];
   char  sVersion[8+1];
   char sLogName[128];
   char        sDate[8 + 1];

   int iRet = 0;
   time_t t; 
    /*init_daemon(); */
           
    memset(file,0x00, sizeof(file));
    memset(Ip,0x00,sizeof(Ip));
    memset(port,0x00,sizeof(port));
    memset(Http_page,0x00,sizeof(Http_page));
    memset(sCommand,0x00,sizeof(sCommand));
    memset(process_buf,0x00,sizeof(process_buf));
    memset(process,0x00,sizeof(process));
    memset(sRltData,0x00,sizeof(sRltData));

    memset(sLogName, 0, sizeof(sLogName));
    CommonGetCurrentDate(sDate);
    trace_on("AIX_AGENT");
    memset(sLogName,0x00,sizeof(sLogName));
    sprintf(sLogName, "mario_log/Mario_aix.log", sDate);
    trace_set_name(sLogName);


    /*读取参数*/
    strcpy(file,"conf/ini.cfg");
    fp = fopen(file, "r");
    if (fp == NULL) {
         LogError("fopen: [%s] error: [%d]\n", file, errno);
         return NULL;
     } 

    cf_locate(fp, "IP", Ip);
    cf_locate(fp, "PORT", port);
    cf_locate(fp, "HTTP_PAGE", Http_page);
    cf_locate(fp, "process_cmd", sCommand);
    fclose(fp);

    /*获取版本信息 */
    if(argc >1)
    {
         LogDebug("argc=[%d]",argc);
         LogDebug("argv[0][%s],argv[1][%s],argv[2][%s]",argv[0],argv[1],argv[2]);
         if(strcmp(argv[1],"version")== 0) {
             strcpy(sVersion, MARIO_AIX_VERSION);
             printf("version[%s]\n",sVersion);
             LogInfo("ERROR:macchiato is running\n");
             return 0;
         } else if(strcmp(argv[1],"update")== 0) {
             /* 获取最新版本信息并下载 */
             iRet=getAgentVersion(Ip,atoi(port),Http_page);
             if(iRet != 0) {
                LogError("getAgentVersion is error\n");
                return -1;
             }

             return 0;
         }
    } else {
       /* 执行系统命令-判断监控是否已运行 */
       iRet = checkProcessRun(argv[0]);
       if( iRet > 0 ) {
          LogDebug("Macchiato is running\n");
          printf("ERROR:macchiato is running\n");
          return  0;
       } else if(iRet <0 ) {
          LogError("Func(checkProcessRun) is error\n");
          return -1;
       }
   }
    
   sleep(1);

   /* 接收命令信息-服务 */
   socket_srv(atoi(port));


   return 0;
} 

#ifndef _MARIO_H
#define _MARIO_H

#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>

#include "trace.h"
#include "ipcopr.h"

typedef struct {
    char sSystem[33];
    char sInstance[33]; 
    char sArgs1[33];
    char sArgs2[33];
    char sArgs3[33];
    char sArgs4[33];
    char sArgs5[33];
} st_mario_tag_def;


typedef struct {
    char sMetric[65];
    char sEndpoint[33];
    int  timestamp ;
    int  step;
    int  value;
    int  valuecount;
    char sCounterType[33];
    st_mario_tag_def pTag;
} st_mario_sendmsg_def;


#define MAXLINE 1024
#define FILE_SIZE 500 

/* °æ±¾ËµÃ÷ */
#define MARIO_AIX_VERSION  "1.0.0"

#endif

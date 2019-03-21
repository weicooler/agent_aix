#ifndef _TRACE_H
#define _TRACE_H

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_DEBUG   5,__FILE__,__LINE__        /* 跟踪级别 5 -- 测试 */
#define LOG_NORMAL  4,__FILE__,__LINE__        /* 跟踪级别 4 -- 正常 */
#define LOG_ERROR   2,__FILE__,__LINE__        /* 跟踪级别 2 -- 错误 */
#define LOG_SYS     1,__FILE__,__LINE__        /* 跟踪级别 1 -- 系统 */

/* 记录内部XML报文 */
int WriteXml(int iLevel,char * psSrcFile,int nSrcLine,char * psAction );

/* 记录XML数据流 */
int TraceXml(int iLevel,char * psSrcFile,int nSrcLine,char * psXmlBuff,char * psAction );

/* 记录XML链表 */
int TraceNode(int iLevel,char * psSrcFile,int nSrcLine,void * pNode,char * psAction );

typedef struct _trace_item_t {
    int trace_flag;
    int trace_level;
    char trace_name[32 + 1];
} trace_item_t;

typedef struct _trace_ctrl_t {
    int        semid;
    int        maxnum;
    int        usenum;
} trace_ctrl_t;

struct loginfo
{
    int nLogFileNum;
    long nLogFileSize;
    int nLogLevel;
};

#define TRC_SEM_NAME            "STRC"
#define TRC_MEM_NAME            "STRC"
#define FS_TRC_SEM_NAME         "FSTC"
#define FS_TRC_MEM_NAME         "FSTC"
#define LOG_SIZE_UNIT           1000000

#define TRACE_ACTION_ADD        1
#define TRACE_ACTION_SET        2
#define TRACE_ACTION_REMOVE     3

#define TRACE_NONE      0
#define TRACE_FATAL     1
#define TRACE_ERROR     2
#define TRACE_WARNING   3
#define TRACE_INFO      4
#define TRACE_DEBUG     5
#define TRACE_NORMAL    4

#define TRACE_LEN_MAX    64
#define LOG_DEFAULT_NAME "ht.log"

extern int  *TraceLevel;
extern char TraceName[TRACE_LEN_MAX + 1];

#define Log(A, format,args...) \
    ((TraceLevel == NULL || TraceName == NULL || *TraceLevel < (A)) ? 0 : LogMsg(A, __FILE__, __LINE__, format, ##args))

#define LogFatal(format,args...) \
    Log(TRACE_FATAL, format, ##args)
#define LogError(format,args...) \
    Log(TRACE_ERROR, format, ##args)
#define LogWarning(format,args...) \
    Log(TRACE_WARNING, format, ##args)
#define LogInfo(format,args...) \
    Log(TRACE_INFO, format, ##args)
#define LogDebug(format,args...) \
    Log(TRACE_DEBUG, format, ##args)

#define Trace(A, buf, len) \
    ((TraceLevel == NULL || TraceName == NULL || *TraceLevel < (A)) ? 0 : TraceMsg(A, __FILE__, __LINE__, buf, len))

#define TraceFatal(buf, len) \
    Trace(TRACE_FATAL, buf, len)
#define TraceError(buf, len) \
    Trace(TRACE_ERROR, buf, len)
#define TraceWarning(buf, len) \
    Trace(TRACE_WARNING, buf, len)
#define TraceInfo(buf, len) \
    Trace(TRACE_INFO, buf, len)
#define TraceDebug(buf, len) \
    Trace(TRACE_DEBUG, buf, len)

/* #define SYS_PARAM_CFG   getenv("SYS_PARAM_CFG") */
#define SYS_PARAM_CFG   "./conf/ini.cfg" 

extern int  trace_on(const char *filename);
extern int  trace_set_level(int level);
extern int  trace_get_level(void);
extern int  trace_init(void);
extern int  trace_display(const char *fname);
extern int  trace_set(const char *fname, int level);
extern void trace_set_name(const char*);
extern void trace_get_name(char*);

#ifdef __cplusplus
}
#endif


#endif

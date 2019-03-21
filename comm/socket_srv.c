/*
 * multi_thread_socket_server.c
 *
 *  Created on: Mar 14, 2014
 *      Author: nerohwang
 */
#include<stdlib.h>
#include<pthread.h>
#include<sys/socket.h>
#include<sys/types.h>       /* pthread_t , pthread_attr_t and so on. */
#include<stdio.h>
#include <netdb.h>
#include<netinet/in.h>      /* structure sockaddr_in */
#include<arpa/inet.h>       /* Func : htonl; htons; ntohl; ntohs */
#include<assert.h>          
#include<string.h>         
#include<unistd.h>          
#include<errno.h>
#include<fcntl.h>
#include<stdbool.h>
#include "mario.h"



#define SOCK_PORT 21007
#define BUFFER_LENGTH 1024
#define MAX_CONN_LIMIT 3     /* MAX connection limit */

static void *Data_handle(void * sock_fd);   /* Only can be seen in the file */

int socket_srv(int iPort)
{
    int sockfd_server;
    int sockfd;
    int fd_temp;
    struct sockaddr_in s_addr_in;
    struct sockaddr_in s_addr_client;
    int client_length;

    sockfd_server = socket(AF_INET,SOCK_STREAM,0);  /*ipv4,TCP */
    assert(sockfd_server != -1);

    /*before bind(), set the attr of structure sockaddr.*/
    memset(&s_addr_in,0,sizeof(s_addr_in));
    s_addr_in.sin_family = AF_INET;
    s_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);  /*trans addr from uint32_t host byte order to network byte order.*/
    s_addr_in.sin_port = htons(SOCK_PORT);          /*trans port from uint16_t host byte order to network byte order.*/
    fd_temp = bind(sockfd_server,(const struct sockaddr *)(&s_addr_in),sizeof(s_addr_in));
    if(fd_temp == -1)
    {
        fprintf(stderr,"bind error!\n");
        exit(1);
    }

    fd_temp = listen(sockfd_server,MAX_CONN_LIMIT);
    if(fd_temp == -1)
    {
        fprintf(stderr,"listen error!\n");
        exit(1);
    }

    while(1)
    {
        printf("waiting for new connection...\n");
        pthread_t thread_id;
        client_length = sizeof(s_addr_client);

        /*Block here. Until server accpets a new connection.*/
        sockfd = accept(sockfd_server,(struct sockaddr*)(&s_addr_client),(socklen_t *)(&client_length));
        if(sockfd == -1)
        {
            fprintf(stderr,"Accept error!\n");
            continue;                               /*ignore current socket ,continue while loop.*/
        }
        printf("A new connection occurs!\n");
        if(pthread_create(&thread_id,NULL, Data_handle,(void *)(&sockfd)) == -1)
        {
            fprintf(stderr,"pthread_create error!\n");
            break;                                  /*break while loop*/
        }
    }

    /*Clear*/
    int ret = shutdown(sockfd_server,SHUT_WR); /*shut down the all or part of a full-duplex connection.*/
    assert(ret != -1);

    printf("Server shuts down\n");
    return 0;
}

void *Data_handle(void * sock_fd)
{
    int fd = *((int *)sock_fd);
    FILE *fstream=NULL;
    int i_recvBytes;
    char data_recv[BUFFER_LENGTH];
    char *data_send = "Server has received your request!\n";
    /* 该子线程的状态设置为detached,则该线程运行结束后会自动释放所有资源 */
    pthread_detach(pthread_self()); /*线程分离*/

    printf("waiting for request...\n");
    /*Reset data.*/
    memset(data_recv,0,BUFFER_LENGTH);

    i_recvBytes = read(fd,data_recv,BUFFER_LENGTH);
    if(i_recvBytes == 0)
    {
         printf("Maybe the client has closed\n");
         return;
    } 

    if(i_recvBytes == -1)
    {
        fprintf(stderr,"read error!\n");
        return;
    }
     
    if(NULL==(fstream=popen(data_recv, "r")))    
    {   
        printf("execute command failed:errno[%d],[%s] ", errno,strerror(errno));    
        return ;    
    }   
    printf("read from client : %s\n",data_recv);
    printf("send to client : \n");
    while(NULL!=fgets(data_send, sizeof(data_send), fstream)) {
        if (strlen(data_send) > 0)  {
           printf("%s",data_send);
           if(write(fd,data_send,strlen(data_send)) == -1)
           {
               break;
           }
       }
     }
    
    /*Clear */
    printf("terminating current client_connection...\n");
    close(fd);            /*close a file descriptor.*/
    pthread_exit(NULL);   /*terminate calling thread!*/
}



/* 获取版本--并下载 */
int getAgentVersion(char *ip,int port,char *page)
{
    int sockfd;
    int iRet=0;
    int j = 0;

    while(1){
       /* 建立连接 */
       iRet = http_conn(ip,port,page,&sockfd);
       if ( iRet != 0) {
           LogError("Func(http_conn) is error\n");
           close(sockfd);
           sleep(60);
           j++;
           /* 连续连接失败，退出 */
           if(j>5){
              break;
           }   
       } else{
          j=0;
          break;
       }   
    } 

    /* 发送请求--并下载 */ 
    msg_post(ip,port,page,sockfd);
    LogInfo("----------LogContext:[%s] end---------");
    close(sockfd);
    return 0;
}


int http_conn(char *ip,int port,char *page,int *sockfd)
{
    int n,ret;
    char recvline[MAXLINE];
    struct sockaddr_in servaddr;
    char content[4096];
    char content_page[500];
    sprintf(content_page,"POST %s HTTP/1.1\r\n",page);
    char content_host[50];
    memset(content_host,0x00,sizeof(content_host));
    sprintf(content_host,"HOST: %s:%d\r\n",ip,port);
    /*char content_type[] = "Content-Type: application/x-www-form-urlencoded\r\n"; */
    char content_type[] = "Content-Type: application/json\r\n";
    char content_len[50];
    fd_set   t_set;

    if((*sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        printf("socket error\n");
        return -1;
    }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if(inet_pton(AF_INET,ip,&servaddr.sin_addr) <= 0) {
        printf("inet_pton error\n");
        return -1;
    }
    if(connect(*sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0) {
        printf("connect error erron[%d],[%s]\n", errno,strerror(errno));
        return -1;
    }

    return  0;
}




msg_post(char *ip,int port,char *page,int sockfd)
{
    int file_fp;
    char file_name[FILE_SIZE];
    char msg[1024];
    char recvline[MAXLINE+1];
    int  nRet;
    time_t t;
    t = time(NULL);
    struct tm *lt;
    char content[4096];
    char content_page[500];
  /*  sprintf(content_page,"POST %s HTTP/1.1\r\n",page);*/
    sprintf(content_page,"GET %s HTTP/1.1\r\n",page);
    char content_host[50];
    memset(content_host,0x00,sizeof(content_host));
    /*sprintf(content_host,"HOST: %s:%d\r\n",ip,port);*/
    sprintf(content_host,"HOST: %s\r\n",ip);
   /* char content_type[] = "Content-Type: application/x-www-form-urlencoded\r\n";*
    char content_type[] = "Content-Type: application/json\r\n"; */
    char content_type[] = "Content-Type: text/plain\r\n"; 
    char content_len[50];
    fd_set   t_set;


    int ii = time(&t);
    ii=(ii-60*14*60)/60*60;


    sprintf(content_len,"Content-Length: %d\r\n\r\n",strlen(msg));
    sprintf(content,"%s%s%s%s%s",content_page,content_host,content_type,content_len,msg); 
   /* sprintf(content,"%s%s",content_page,content_host);*/
    /* printf("content[%s]\r\n",content); */

    nRet= write(sockfd,content,strlen(content));
    if (nRet < 0) {
       printf("send failed!,errcode=[%d] errmsg='%s'\n",errno, strerror(errno));
       exit(0);
    }else{
        printf("nRet[%d],send succ! send_msg_size=[%d] \n",nRet, strlen(content));
        printf("%s\n",content);
    }   

#if 1
    /*FD_ZERO(&t_set);
    FD_SET(sockfd, &t_set);*/
    nRet= read(sockfd,recvline,MAXLINE);
    printf("read ret [%d] errcode=[%d] errmsg='%s'\n", nRet,errno, strerror(errno));
    printf("first----recv_msg=[%s]\n", recvline);

#endif 
    
    
    
    memset(content_page,0x00, sizeof(content_page));
    memset(content,0x00, sizeof(content));
    /* sprintf(content_page,"GET %s HTTP/1.1\r\n","http://10.4.146.9/upgrade/Macchiato_aix.bak"); */
    sprintf(content_page,"GET %s HTTP/1.1\r\n","http://10.4.146.9/upgrade/Macchiato_aix");
    sprintf(content,"%s%s%s%s%s",content_page,content_host,content_type,content_len,msg); 
    nRet= write(sockfd,content,strlen(content));
    if (nRet < 0) {
       printf("send failed!,errcode=[%d] errmsg='%s'\n",errno, strerror(errno));
       exit(0);
    }else{
        printf("nRet[%d],send succ! send_msg_size=[%d] \n",nRet, strlen(content));
        printf("%s\n",content);
    }   

    /* 每接收一段数据，便将其写入文件中，循环直到文件接收完并写完为止 */
    int length = 0;  
    int writeLength = 0;

    memset( recvline,0, sizeof(recvline) );
#if 0 
    nRet= read(sockfd,recvline,321);
    printf("read ret [%d] errcode=[%d] errmsg='%s'\n", nRet,errno, strerror(errno));
    printf("second recv_msg=[%s]\n", recvline);
#endif
    char response;
    bool isGetContent = false;
    int index=0;
    while(read(sockfd, &response, 1)>0){
       recvline[index++] = response;
       if(response == '\r'){
         if(read(sockfd, &response, 1)>0){
           recvline[index++] = response;
           if(response == '\n'){
             if(read(sockfd, &response, 1)>0){
               recvline[index++] = response;
               if(response == '\r'){
                 if(read(sockfd, &response, 1)>0){
                    recvline[index++] = response;
                    if(response == '\n'){
                      isGetContent = true;
                      printf("\n\nSUCCESS GET HEAD\n\n");
                      break;
                    }
                 }
               }
             }
          }
        }
      }
    }

    if(isGetContent == true){
        int status_code;
        char content_type[1024]; 
        long content_length;
        get_resp_header(recvline, &status_code, content_type, &content_length);
        printf("%d, %s, %ld\n",  status_code, content_type, content_length);
#if 1
        /* 打开文件，准备写入 */
        strcpy(file_name,"Macchiato_new");
        file_fp = open("Macchiato_new",O_CREAT|O_RDWR|O_TRUNC,0777); 
        if( file_fp<0 ) 
        {   
            printf("File:\t%s Can Not Open To Write\n", file_name); 
            exit(1); 
        }   
   

        memset(recvline,0x00, sizeof(recvline) );
        while((length = read(sockfd, recvline, sizeof(recvline))) > 0)  
        {   
            if( write( file_fp, recvline, length ) < length) 
            {   
                printf("File:\t%s Write Failed\n", file_name); 
                break; 
            }   
            writeLength+=length;
            if(writeLength >= content_length){ 
                printf("writeLength[%d]\n",writeLength);
                printf("write is over\n"); 
                break;                         
            }                                  

            memset(recvline,0x00, sizeof(recvline) );
        }   
    }
#endif
    /* 接收成功后，关闭文件，关闭socket */
    close(file_fp);
    return;

}

/*
# status_code : 200, 503, .... 状态码
# content_type : image/jpeg 内容类型
# content_length : 560437 内容长度(字节)
*/
get_resp_header(const char *response, int *status_code, char*content_type, long* content_length){
    char *pos = (char*)strstr(response, "HTTP/");
    if (pos)
        sscanf(pos, "%*s %d", status_code);

    pos = (char*)strstr(response, "Content-Type:");
    if (pos)
        sscanf(pos, "%*s %s", content_type);

    pos = (char*)strstr(response, "Content-Length:");
    if (pos)
        sscanf(pos, "%*s %ld",content_length);

    return;
}

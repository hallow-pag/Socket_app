#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/sysinfo.h>
#include <sys/epoll.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include"Device.h"
#include "Responses.h"
#include"SqlManage.h"

#define DEFAULT_PORT 8080
#define MAX_EVENT_NUM 1024
#define INFTIM -1


/*  screen   */
void process(int);
int char_int(char str[]);
void handle_subprocess_exit(int);

int main(int argc, char *argv[])  
{
    struct sockaddr_in server_addr;
    int listen_fd;
    int cpu_core_num;
    int on = 1;
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(listen_fd, F_SETFL, O_NONBLOCK);
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(DEFAULT_PORT);

    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind error, message: ");
        exit(1);
    }

    if (listen(listen_fd, 5) == -1) {
        perror("listen error, message: ");
        exit(1);
    }

    printf("listening 8080\n");
     
    signal(SIGCHLD, handle_subprocess_exit);
    process(listen_fd);
    // cpu_core_num = get_nprocs();
    // printf("cpu core num: %d\n", cpu_core_num);
    // for (int i = 0; i < cpu_core_num * 2; i++) {
    //     pid_t pid = fork();
    //     if (pid == 0) {
    //         process(listen_fd);
    //         exit(0);
    //     }
    // }

    // while (1) {
    //     sleep(1);
    // }

    return 0;
}

void process(int listen_fd) 
{
    int conn_fd;
    int ready_fd_num;
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    char buf[128];
    Responses Res;


    struct epoll_event ev, events[MAX_EVENT_NUM];
    int epoll_fd = epoll_create(MAX_EVENT_NUM);
    ev.data.fd = listen_fd;
    ev.events = EPOLLIN;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) == -1) {
        perror("epoll_ctl error, message: ");
        exit(1);
    }

    while(1) {
        ready_fd_num = epoll_wait(epoll_fd, events, MAX_EVENT_NUM, INFTIM);
        
        if (ready_fd_num == -1) {
            perror("epoll_wait error, message: ");
            continue;
        }
        for(int i = 0; i < ready_fd_num; i++) {
            if (events[i].data.fd == listen_fd) {
                conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_size);
                if (conn_fd == -1) {
                    sprintf(buf, "[pid %d] ❌ accept 出错了: ", getpid());
                    perror(buf);
                    continue;
                }

                if (fcntl(conn_fd, F_SETFL, fcntl(conn_fd, F_GETFD, 0) | O_NONBLOCK) == -1) {
                    continue;
                }

                ev.data.fd = conn_fd;
                ev.events = EPOLLIN;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev) == -1) {
                    perror("epoll_ctl error, message: ");
                    close(conn_fd);
                }
               
            } else if (events[i].events & EPOLLIN) {
                printf("[pid %d] ✅ 处理来自 %s:%d 的请求\n", getpid(), inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
                conn_fd = events[i].data.fd;
            
                Res.responses_init(conn_fd,inet_ntoa(client_addr.sin_addr));
              
                Res.accept_request(&client_addr);
                close(conn_fd);
            } else if (events[i].events & EPOLLERR) {
                fprintf(stderr, "epoll error\n");
                close(conn_fd);
            }
        }
    }
}

void handle_subprocess_exit(int signo)
{
    printf("clean subprocess.\n");
    int status;  
    while(waitpid(-1, &status, WNOHANG) > 0);
}

/*************************************************
Function: // inforinti
Description: // 初始化用户信息
Input: // strint
Output: // void
*************************************************/
void infointi(){
    sqlite3 * db = NULL;
    char *zErrMsg =NULL;
    int rc;
    rc = sqlite3_open("../Date/hello.db", &db); 
    if( rc ) 
        { 

        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db)); 
        sqlite3_close(db); 
    }
    char **azResult=NULL; //二维数组存放结果
    int nrow=0;
    int ncolumn = 0;
    
    const char * sql = "select * from User";
    //onst char * sql="select pwd from User where Id='2'";
        sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn , &zErrMsg );
        printf("nrow=%d ncolumn=%d\n",nrow,ncolumn);
        printf("the result is:\n");
        int id;
        for(int i = ncolumn;i<=(nrow*ncolumn);i+=ncolumn){
            id = char_int(azResult[i]);
            
            if(strlen(azResult[i+5])>6) strcpy( minfo[id].addrip,azResult[i+5]);
           
            //strcpy( minfo[id].phone,azResult[i+6]);
          strcpy( minfo[id].power,"-1");
            //strcpy( minfo[id].user,azResult[i+1]);
           if(strlen(azResult[i+4])>0) strcpy( minfo[id].timer,azResult[i+4]);
           
         
        }

    sqlite3_close(db); 
}

/*************************************************
Function: // char_int
Description: // 字符串转int
Input: // strint
Output: // int
*************************************************/
int char_int(char str[]){
    int sum=0;
    int l = strlen(str);
    for(int i=0;i<l;i++){
        sum*=10;
        sum += (str[i]-'0');
    }
return sum;
 }

//**************************************//
void test(){
  /*
    void getMemoryinf();
    void getOsInfo();
    void getCpuInfo();
    void getNodeAvge();
    void getNetInfo();
    void datapackge();
    char* getresponse();
  */
  Device dic;
  dic.getNetInfo();
  dic.getCpuInfo();
  dic.getMemoryinf();
  dic.getOsInfo();
  dic.getNodeAvge();
  dic.datapackge();
  char sp[1024];
  strcpy(sp,dic.getresponse());
}


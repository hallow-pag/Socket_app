
#ifndef RESPONSES_S
#define RESPONSES_S
/* 相应格式 */
/*
http://39.97.110.254:8080/login  登陆地址
参数: username  pwd
请求方式 GET 
例如:
http://39.97.110.254:8080/login?username=111&pwd=111 
账号密码正确返回 true
错误返回false

http://39.97.110.254:8080/cpuinfo  设备硬件信息
请求方式: GET 
返回硬件信息

/registered
参数: username 用户名; pwd 密码; power权限
*/


#include<iostream>
#include<stdio.h>
#include<string.h>
#include<map>
#include"SqlManage.h"
#include"Device.h"
#include"log.h"

class Responses
{
private:
    /* data */
    size_t numchars;
    /* GET请求数据 */
    std::map<std::string,std::string> map; 
    Device De;
    SqlManage *Sq;
    log *lg;
    char ipaddr[16];
    int client;  //link number
    int islogin; //-1 未登陆; 0普通权限; 1高级权限; 2root权限;
    char userid[5]; 
    char cookie[22];//保存cookie信息
    bool reqisok;  //访问是否异常


public:
    
    Responses();

    void accept_request(struct sockaddr_in *client_addr);
    void dispose_GET( struct sockaddr_in *client_addr);
    void dispose_POST(struct sockaddr_in *client_addr);

    /* 路由管理 */
    void management(char url[]);
    void screen(char Request[],int len);
    ~Responses();

    /* init */
    void responses_init(int clit,char addr[]);

    char* strtochar(std::string str, char an[]);
    void getCookie(char buf[]);
    void randstr(char str[]);
};


#endif
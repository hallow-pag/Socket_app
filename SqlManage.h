#ifndef SQLMANAGE_H
#define SQLMANAGE_H

#include<sqlite3.h>
#include<iostream>
#include<string.h>
#include"md5.h"

static struct meninfo
{
    /* data */
    char user[10];
    char power[3];
    char addrip[17];
    char timer[38];

}minfo[1000];




class SqlManage
{
private:
    /* data */
    sqlite3 * db=NULL;
    char *zErrMsg =NULL;
    int rc;
    SqlManage(const SqlManage * sql);
    char cookie[24];
    int id;
    char idstr[50];
    

public:
    SqlManage(/* args */);
    ~SqlManage();

    /* Log */
    void set_log(int num,char inf[]);
    void login_log(int num,char inf[]);
    

    /* user-password */
    /* 新增用户 */
    int adduser( char user[], char pwd[], char power[],char addr[]);

    void setadder();

    /* 登陆 */
    int login(char user[], char pwd[],char randte[],char addr[]);
    char * loghist(char userid[]);
    
    /* 登出 */
    bool log_out();
    /* 检验是否登陆 */
    int islogin();

    /* user list */
    void userlist(char req[],int vim);
    /* Update user */
    int updateuser(char userid[], char user[],char pwd[], char power[]);
    /* delete user*/
    int deluser(char userid[]);

    /*正常访问*/
    void req_ok(char url[],char addr[]);

    /*异常访问*/
    void req_dager(char url[],char addr[]);

    void gettime(char time[]);
    meninfo getinfo();

    
    /* */
    void sqlinit(char cooki[]);


    int chartoint(char str[]);
    void linkinit();
};



#endif
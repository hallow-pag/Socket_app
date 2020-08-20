#ifndef LOG_H
#define LOG_H
#include<sqlite3.h>
#include<iostream>


class log
{
private:
    sqlite3 * db=NULL;
    char *zErrMsg =NULL;

public:
    /*---- 用户 ---*/
    void login_log(char userid[],const char * info,char addr[]);
    void logout_log(char userid[],char user[]);
    void adduser_log(char user[],char username[],char power[]);
    void upuser_log(char user[],char upuser[],const char * info);
    void deluser_log(char userid[]);
    /*---- 网络设置 ---*/


    /* ---- 查看用户 ---*/
    char * show_log_user(char * req,int n);

    /* --- 查看设置 ---*/
   

    log(/* args */);
    ~log();
};





#endif
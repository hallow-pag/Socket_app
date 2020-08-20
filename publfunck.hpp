
#define System_Err 0    // 系统错误 0
#define No_Log -1   // 未登录 -1
#define NO_POWER -2 // 权限不足 -2
#define Lack_Of_Parameter -3    // 缺少参数 -3
#define Paramter_Err -4 // 参数错误 -4
#define Err -5  // 未知错误 -5
#define Cookie_Err -6   // cookie失效 -6
#define Url_Err -7  // 路径错误 
#define Pwd_Err -8 //密码错误
#define Username_Err -9 //用户名错误
#define Name_Repeat -10 //用户名重复
#define Name_To_long -11 //用户名不规范
#define pwd_No_Specification -12 //密码不规范



#include<time.h>
#include<stdio.h>
#include<string.h>

struct loginhistory
{
    char timer[28];
    char addrip[16];
};

void gettime(char psTime[]) {
    time_t nSeconds;
    struct tm * pTM;
    
    time(&nSeconds);
    pTM = localtime(&nSeconds);
    

    /*
    sprintf(psDate,"%04d-%02d-%02d", 
            pTM->tm_year + 1900, pTM->tm_mon + 1, pTM->tm_mday);
    */
    /* 系统时间，格式: HHMMSS */
    sprintf(psTime, "%02d-%02d-%02d %02d:%02d:%02d",
            pTM->tm_year + 1900, pTM->tm_mon + 1, pTM->tm_mday,
            pTM->tm_hour, pTM->tm_min, pTM->tm_sec);
           
    return ;       
}

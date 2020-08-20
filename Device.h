
/*****************************************************************************
Copyright: 2020
File name: Device.h
Description: CPU 内存 网络信息
Author: YK
Version: 1
Date: 2020.7
History: --
*****************************************************************************/

#ifndef DEVICE_H
#define DEVICE_H

#include<string>

class Device
{
private:
    /* Linux版本号 */
    char os_version[20];

    /* CPU */
    char vendor[10]; //供应商
    char cpu_family[10];//核心数
    char cpu_model[10];//型号
    char cpu_name[60];//名字
    
    char loadavg_1m[8]; //1min 负载
    char loadavg_5m[8]; //5min 负载
    char loadavg_15m[8]; //15min 负载

    /* 内存 */
    char  memoryinf[7][60];
    char free_total[10]; //总量
    char free_used[10];// 已使用
    char free_free[10];//可用
    char free_shared[10];//进程共享的
    char free_buffers[10];//磁盘缓存
    char free_cached[10];//缓存

    //网络信息
    char ensname[10];   //网卡名
    char inetv4[18];    //ipv4
    char netmask[18];   //mask
    char broadcast[18]; //broadcast
    char inetv6[30];    //ipv6
    char mac[30];        //MAC
    char prefixlen[10]; //连接速度
    long RX;         //下载
    long TX;         //上传

    /* response */
    char response[1024];


public:
    Device(/* args */);
    ~Device();
    /* 获取信息 */
    void getMemoryinf();
    void getOsInfo();
    void getCpuInfo();
    void getNodeAvge();
    void getNetInfo();
    
    /* 设置*参数 */
    //bool setNetwork();
    
    /* 数据*打包 */
    void datapackge();
    char* getresponse();

    /*** 命令 ***/
    char * command(char cmd[],char req[],int n);
    

    
};







#endif
#include"Device.h"
#include<stdio.h>
#include<string.h>
#include<iostream>

using std::cout;
using std::endl;

Device::Device(/* args */)
{
    getCpuInfo();
    getOsInfo();

  getNetInfo();
  getMemoryinf();
  getNodeAvge();
  datapackge();
 std::cout<<"Device.h to BUILD"<<std::endl;
}

/*************************************************
Function: // getMemoryinf
Description: // 获取内存信息
Input: // void
Output: // void
*************************************************/
void Device::getMemoryinf(){
    FILE * fp = fopen("/proc/meminfo","r");
    int n=0;
 
        while(n<6){
            feof(fp);
            fgets(memoryinf[n],sizeof(memoryinf[n])-1,fp);
            n++;
        }
    sscanf(memoryinf[0],"%*s %[0-9] %*s",free_total);
    sscanf(memoryinf[1],"%*s %[0-9] %*s",free_used);
    sscanf(memoryinf[2],"%*s %[0-9] %*s",free_free);
    sscanf(memoryinf[3],"%*s %[0-9] %*s",free_shared);
    sscanf(memoryinf[4],"%*s %[0-9] %*s",free_buffers);
    sscanf(memoryinf[5],"%*s %[0-9] %*s",free_cached);
    
        fclose(fp);
}

/*************************************************
Function: // getOsInfo
Description: // 获取Linux版本s信息
Input: // void
Output: // void
*************************************************/
void Device::getOsInfo()
{
     FILE *fp = fopen("/proc/version", "r");
    if(NULL == fp)
        printf("failed to open version\n");
    char szTest[1000] = {0};
    feof(fp);
    memset(szTest, 0, sizeof(szTest));
    fgets(szTest, sizeof(szTest) - 1, fp); // leave out \n
    sscanf(szTest,"%*s %*s %[0-9,a-z,.,-]",os_version);
    fclose(fp);
}

/*************************************************
Function: // getCpuInfo
Description: // 获取CPU信息
Input: // void
Output: // void
*************************************************/
void Device::getCpuInfo()
{
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if(NULL == fp)
        printf("failed to open cpuinfo\n");
    char szTest[10][40];
    int n=0;
    while(n<6)
    {
        // read file line by line
        feof(fp);
        fgets(szTest[n], sizeof(szTest[n]) - 1, fp); // leave out \n
        n++;

    }
    sscanf(szTest[1],"%*s : %[^\n]",vendor);
    sscanf(szTest[2],"%*s %*s : %[^\n]",cpu_family);
    sscanf(szTest[3],"%*s %*s : %[^\n]",cpu_model);
    sscanf(szTest[4],"%*s %*s : %[^\n]",cpu_name);
    fclose(fp);

}

/*************************************************
Function: // getNodeAvge
Description: // 获取CPU负载信息
Input: // void
Output: // void
*************************************************/
 void Device::getNodeAvge(){
    FILE *fp = fopen("/proc/loadavg", "r");
    if(NULL == fp)
        printf("failed to open cpuinfo\n");
    char szTest[40];
    int n=0;
    feof(fp);
    fgets(szTest, sizeof(szTest) - 1, fp); // leave out \n
    /*
    char loadavg_1m[8]; //1min 负载
    char loadavg_5m[8]; //5min 负载
    char loadavg_15m[8]; //15min 负载
    */
    sscanf(szTest,"%[0-9,.] %[0-9,.] %[0-9,.]",loadavg_1m,loadavg_5m,loadavg_15m);

    fclose(fp);

 }

/*************************************************
Function: // getNetInfo
Description: // 获取网络信息
Input: // void
Output: // void
*************************************************/
 void Device::getNetInfo(){
    // system("ifconfig > ifconfig");
    FILE * fp = NULL;
    char szTest[8][100];
    fp = popen("ifconfig","r");
    int n =0;
    while(n<7){
        feof(fp);
        fgets(szTest[n], sizeof(szTest[n]) - 1, fp); 
        n++;
    }
    
    int ip4[4];
    int net[4];  //net[0],net[1],net[2],net[3]
    int bro[4]; //bro[0],bro[1],bro[2],bro[3]
   
    sscanf(szTest[0],"%[^:]",ensname);
  
    sscanf(szTest[1]," %*s %d.%d.%d.%d %*s %d.%d.%d.%d %*s %d.%d.%d.%d",
            &ip4[0],&ip4[1],&ip4[2],&ip4[3],&net[0],&net[1],&net[2],&net[3],&bro[0],&bro[1],&bro[2],&bro[3]); 
    sscanf(szTest[2],"%*s %[0-9,a-z,:] ",inetv6);
    sscanf(szTest[3],"%*s %[0-9a-z,:] %*s %[0-9]",mac,prefixlen);
    sscanf(szTest[4],"%*s %*s %*d %*s %ld ",&RX);
    sscanf(szTest[6],"%*s %*s %*d %*s %ld ",&TX);
     
    sprintf(inetv4,"%d.%d.%d.%d",ip4[0],ip4[1],ip4[2],ip4[3]);
    sprintf(broadcast,"%d.%d.%d.%d",bro[0],bro[1],bro[2],bro[3]);
    sprintf(netmask,"%d.%d.%d.%d",net[0],net[1],net[2],net[3]);

    pclose(fp);
 }

/*************************************************
Function: // datapackge
Description: // 数据打包
Input: // void
Output: // void
*************************************************/
void Device::datapackge(){


    int vis;
    /* data{"cpu":{ "1":1,"2":2 }, "free":{ "1":1},"2":2} }*/
    vis = sprintf(response,"HTTP/1.1 200 OK \r\nContent-Type: text/plain; charset=utf-8\r\n\r\n{\"data\":{"); 
    vis +=sprintf(response+vis,"\"os_version\" : \"%s\",",os_version);
    vis +=sprintf(response+vis,"\"cpu\" :{ \"vendor\":\"%s\", \"cpu_family\":\"%s\", \"cpu_model\":\"%s\", \"cpu_name\":\"%s\"},"
                                ,vendor,cpu_family,cpu_model,cpu_name);
    vis +=sprintf(response+vis,"\"loadavg\" :{ \"loadavg_1m\":\"%s\", \"loadavg_5m\":\"%s\", \"loadavg_15m\":\"%s\"},"
                                ,loadavg_1m,loadavg_5m,loadavg_15m);
    vis +=sprintf(response+vis,"\"memoryinf\" :{ \"free_total\":\"%s\", \"free_used\":\"%s\", \"free_free\":\"%s\", \"free_shared\":\"%s\", \"free_buffers\":\"%s\", \"free_cached\":\"%s\"},"
                                ,free_total,free_used,free_free,free_shared,free_buffers,free_cached);  
    vis +=sprintf(response+vis,"\"network\" :{ \"ensname\":\"%s\", \"inetv4\":\"%s\", \"netmask\":\"%s\", \"broadcast\":\"%s\", \"inetv6\":\"%s\", \"mac\":\"%s\", \"prefixlen\":\"%s\", \"RX\":%ld, \"TX\":%ld}"
                                ,ensname,inetv4,netmask,broadcast,inetv6,mac,prefixlen,RX,TX);  
    sprintf(response+vis,"}}");
}

char* Device::getresponse(){

    return response;
}


Device::~Device()
{
     std::cout<<"~Device.h to BUILD"<<std::endl;
}

char * Device::command(char cmd[],char req[],int n){
    FILE * fp = NULL;
    char buff[1280] = {0};
    fp = popen(cmd,"r");
    fread(buff,1,1279,fp);
    pclose(fp);
    sprintf(req+n,"{\"Date\":\"%s\n\",\"return\":1}",buff);
    return req;

}
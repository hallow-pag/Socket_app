

#include <sys/socket.h>
#include"Responses.h"
#include <unistd.h>
#include <fcntl.h>

using std::cout;
using std::endl;

#define STR_LEN 20//定义随机输出的字符串长度。
#define CHAR_MIN 'a'
#define CHAR_MAX 'z' //定义输出随机字符串每个字符的最大最小值。
/*************************************************
Function: // get_line
Description: // 读取文件第一行
Input: // sock(文件标识符),buf(存文件信息的字符串),size(大小)
Output: // 返回结果的字符串长度 int
*************************************************/

 char filename[20];
 char re[50000];


int get_line(int sock, char *buf, int size)
{
    int i = 0;
    char c = '\0';
    int n;

    while ((i < size - 1) && (c != '\n')&&(i<300))
    {
        n = recv(sock, &c, 1, 0);
        /* DEBUG printf("%02X\n", c); */
        if (n > 0)
        {
            if (c == '\r')
            {
                n = recv(sock, &c, 1, MSG_PEEK);
                /* DEBUG printf("%02X\n", c); */
                if ((n > 0) && (c == '\n'))
                    recv(sock, &c, 1, 0);
                else
                    c = '\n';
            }
            buf[i] = c;
            i++;
        }
        else
            c = '\n';
    }
    buf[i] = '\0';

    return(i);
}

Responses::Responses()
{
    Sq = new SqlManage;
    Sq->linkinit();
    lg = new log;
    std::cout<<"Responese.h to BUILD"<<std::endl;
}

void Responses::accept_request( struct sockaddr_in *client_addr){

char Request[300];
char othre[1000];
char funck[5];
char Url[2000];
numchars = get_line(client,Request,300);
int len = strlen(Request);
read(client,othre,sizeof(othre));
try
{
    /* code */
    sscanf(Request,"%s %[a-z,/,] ",funck,Url);
}
catch(const std::exception& e)
{
    std::cerr << "url err" << '\n';
    strcpy(Url,"/0");
}



getCookie(othre); 

    if(strcasecmp("GET",funck)==0){
        screen(Request,len);
       management(Url);
    }
    else if(strcasecmp("POST",funck)==0){
        cout<<"Other: "<<othre<<endl;

    }
}

void Responses::dispose_GET( struct sockaddr_in *client_addr){
    


}
void Responses::dispose_POST( struct sockaddr_in *client_addr){

}


/**********************************************************************/
/* URL 管理*/
/**********************************************************************/
void Responses::management(char url[]){
    reqisok = true;
    meninfo myinfo;
    Sq->sqlinit(cookie);
    islogin = Sq->islogin(); //power

   if(islogin)  meninfo myinfo = Sq->getinfo();
    
    char response[3200];
    int vism;
    
    vism = sprintf(response,"HTTP/1.1 200 OK \r\nContent-Type: text/plain; charset=utf-8;\r\n\r\n");
    /* cpuinf  */ 
    if(strcasecmp(url,"/cpuinfo")==0){      
        if(islogin>=0){
        reqisok = false;
       
        strcpy(response,De.getresponse());
        }
        else{
            sprintf(response+vism,"{\"date\":{\"return\":%d,\"text\":\"%s\"}}",-1,"未登陆");
        }
        //检测访问是否安全
      if(reqisok) Sq->req_dager(url,ipaddr);
        
    }
    /* cpuset  */ 
    else if(strcasecmp(url,"/cpuset")==0){
        if(islogin>=1){
             sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",1,"修改成功"); 
        }
        else if(islogin==0){
            sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",-2,"权限不足"); 
        }
        else{
            sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",-1,"未登陆"); 
        } 

         //检测访问是否安全
      if(reqisok) Sq->req_dager(url,ipaddr);
    }

    else if(strcasecmp(url,"/command")==0){
        if(islogin>1){
                std::map<std::string,std::string> ::iterator cmd = map.find("cmd");
                char an[21];
                if(cmd!=map.end()){
                    De.command(strtochar((*cmd).second,an),response,vism);
                }
                else{
                    sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",-4,"缺少参数"); 
                }
        }
        else{
            sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",-2,"权限不足"); 
        }
    }

    /* login  */ //--------------------------------------------
    else if( strcasecmp(url,"/login")==0){ 
        /* 登陆验证 */
        std::map<std::string,std::string> ::iterator username = map.find("username");
        std::map<std::string,std::string> ::iterator pwd = map.find("pwd"); 
        
        if(username != map.end()&& pwd!=map.end()){
            char an[22];
            char bn[40];
    
            //Set-Cookie:asdasdasd;\n
            /*生成随机数 - 设置Cookie*/
            char randst[22];
            randstr(randst);
            Sq->sqlinit(randst);
           int num =  Sq->login(strtochar((*username).second,an),strtochar((*pwd).second,bn),randst,ipaddr);
            if(num==1){
            myinfo = Sq->getinfo();
            sprintf(response,"HTTP/1.1 200 OK \r\nSet-Cookie:%s;\nContent-Type: text/plain; charset=utf-8;\r\n\r\n{\"data\":{\"return\":%d,\"text\":\"%s\",\"on_time\":\"%s\",\"on_addr\":\"%s\"}}"
                                    ,randst,1,"登陆",myinfo.timer,myinfo.addrip); 
            //UPDATE "main"."User" SET "cookie" = 'qwqw' WHERE rowid = 3;
            Sq->req_ok(url,ipaddr);
            lg->login_log(myinfo.user,"登陆成功",ipaddr);
            }
            
            else if(num==0)
            sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",-8,"密码错误"); 
            else
            sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",-9,"没有此账号"); 
        }
        else{
        sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",-4,"缺少参数"); 
        }
         //检测访问是否安全
      if(reqisok) Sq->req_dager(url,ipaddr);
    }


    /* logout  */ 
    else if(strcasecmp(url,"/logout")==0){
        
       if(Sq->log_out()){
           sprintf(response,"HTTP/1.1 200 OK \r\nSet-Cookie:%s;\nContent-Type: text/plain; charset=utf-8;\r\n\r\n{\"data\":{\"return\":%d,\"text\":\"%s\"}}","log_out",1,"注销成功");
            lg->login_log(myinfo.user,"登出",ipaddr);
       } 
       
       else sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",-1,"未登陆"); 
    if(reqisok) Sq->req_dager(url,ipaddr);
    }



    /* registered */
    else if(strcasecmp(url,"/registered")==0){
        std::map<std::string,std::string> ::iterator username = map.find("username");
        std::map<std::string,std::string> ::iterator pwd = map.find("pwd"); 
        std::map<std::string,std::string> ::iterator power = map.find("power"); 
        if(username!=map.end()&&pwd!=map.end()&&power!=map.end()){
            char an[20],bn[40],cn[20];
            int regist; 
            regist= Sq->adduser(strtochar((*username).second,an),strtochar((*pwd).second,bn),strtochar((*power).second,cn),ipaddr);
            if(regist == 1) { 
                sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",1,"添加成功"); 
                lg->adduser_log(myinfo.user,an,cn);
            }
            else if(regist == -1) sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",0,"系统错误"); 
            else if(regist == -10) sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",regist,"用户名存在"); 
           else if(regist == -12) sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",regist,"密码不规范"); 

            else sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",-2,"权限不足"); 
        }
        else{
            sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",-3,"缺少参数"); 
        }

     //检测访问是否安全
      if(reqisok) Sq->req_dager(url,ipaddr);
    }



    /* user list */
    else if(strcasecmp(url,"/userlist")==0){
        Sq->userlist(response,vism);
    }


    /* updaate user*//*returrn -2 权限不足; -1 id错误; 0 系统错误; 1 OK */
    else if(strcasecmp(url,"/upuser")==0){
        std::map<std::string,std::string> ::iterator uptoid = map.find("id"); 
        if(uptoid == map.end()){
            sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",-2,"缺少参数"); 

        }else{
            std::map<std::string,std::string> ::iterator username = map.find("username");
            std::map<std::string,std::string> ::iterator pwd = map.find("pwd"); 
            std::map<std::string,std::string> ::iterator power = map.find("power"); 
            char upid[10];
            int conts=0;
            strtochar((*uptoid).second,upid);

            char upname[10] = {0};
            if(username!= map.end()){
                conts+=100;
            strtochar((*username).second,upname);
            }
            char uppwd[40] = {0};
            if(pwd!= map.end()){
                conts+=10;
            strtochar((*pwd).second,uppwd);
            }
            char uppro[10] = {0};
            if(power!= map.end()){
                conts+=1;
            strtochar((*power).second,uppro);
            }
           int up_req =  Sq->updateuser(upid,upname,uppwd,uppro);
           if(up_req==-2)  sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",up_req,"权限不足"); 
           else if(up_req == -1) sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",up_req,"id错误"); 
           else if(up_req == -10) sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",up_req,"用户名存在"); 
           else if(up_req == -12) sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",up_req,"密码不规范"); 

           else if(up_req == 1){
                sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",up_req,"操作成功"); 
                switch (conts)
                {
                case 111:
                    lg->upuser_log(myinfo.user,upid,"修改了用户名,密码,权限");
                    break;
                case 100:
                    lg->upuser_log(myinfo.user,upid,"修改了用户名");
                    break;
                case 110:
                    lg->upuser_log(myinfo.user,upid,"修改了用户名,密码");
                    break;
                case 101:
                    lg->upuser_log(myinfo.user,upid,"修改了用户名,权限");
                    break;
                case 11:
                    lg->upuser_log(myinfo.user,upid,"密码,权限");
                    break;
                case 10:
                    lg->upuser_log(myinfo.user,upid,"密码");
                    break;
                case 1:
                    lg->upuser_log(myinfo.user,upid,"权限");
                    break;
                
                default:
                    break;
                }
                }
           else sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",0,"系统错误"); 

        }

        //检测访问是否安全
      if(reqisok) Sq->req_dager(url,ipaddr);
    }


    else if(strcasecmp(url,"/deluser")==0){
        if(islogin<=1){
            sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",-2,"权限不足"); 
        }
        else{
             std::map<std::string,std::string> ::iterator uptoid = map.find("id"); 
            if(uptoid == map.end()) sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",-3,"缺少参数"); 
            else{
                char ids[5];
                int del_re;
                del_re = Sq->deluser(strtochar((*uptoid).second,ids));
                if(del_re==1) {
                    sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",1,"操作成功");
                    lg->upuser_log(myinfo.user,ids,"删除用户");
                }
                else  sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",0,"系统错误"); 
            }
        }
       //检测访问是否安全
      if(reqisok) Sq->req_dager(url,ipaddr);
    }


    else if(strcasecmp(url,"/showlog")==0){
        if(islogin>=0)
        lg->show_log_user(response,vism);
        else
        {
            sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",-1,"未登陆"); 
        }
        
         //检测访问是否安全
      if(reqisok) Sq->req_dager(url,ipaddr);
    }
//-----------------------------------------------------------------------------------------------
    else if(strcasecmp(url,"/")==0){
        int headlen = strlen(filename);
        sprintf(re,"HTTP/1.1 200 OK \r\nContent-Type: text/html\r\n\r\n");
        int filefd = open("../src/home.html",O_RDONLY);
        int filen = read(filefd,re+headlen,sizeof(re)-headlen);
        write(client,re,headlen+filen);
        close(filefd);
        return ;
    }
    else if(strcasecmp(url,"/update")==0){
        int headlen = strlen(filename);
        sprintf(re,"HTTP/1.1 200 OK \r\nContent-Type: text/html\r\n\r\n");
        int filefd = open("../src/post.html",O_RDONLY);
        int filen = read(filefd,re+headlen,sizeof(re)-headlen);
        write(client,re,headlen+filen);
        close(filefd);
        return ;
    }

//----------------------------------------------------------------------------------------------
    else if(strcasecmp(url,"/favicon")==0)  return ;
    }
    /* ------Erry------ */
    else{
        sprintf(response+vism,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",-7,"路径err"); 

         //检测访问是否安全
      if(reqisok) Sq->req_dager(url,ipaddr);
    }


    write(client,response,strlen(response));
}

/*************************************************
Function: // screen
Description: // 获取GET参数并保存在MAP中
Input: // char Request[] 需要被解析的数据 ,int len 长度
Output: // void
*************************************************/
void Responses::screen(char Request[],int len){
    map.clear();
        int i=0;int n=0;
        for(i;i<len;i++){
            if(Request[i]=='?')
            break;
        }
        char key[20] = {0};
        char value[20] = {0};

        for(i++;i<len;i++,n++){
            if(Request[i]=='='){
                key[n] = '\0';
                n=0;
            for(i++;i<=len;i++,n++){
                if(Request[i]=='&'||Request[i]==' '){
                    value[n]='\0';
                    std::string k(key);
                    std::string y(value);
                    map.insert(make_pair(k,y));
                    k.clear();
                    y.clear();
                    n=0;i++;
                    break;
                }
                 if(n>19) n--;
                value[n] = Request[i];
            }

            }
        if(n>19) n--;
        key[n]=Request[i];
        }   
        std::map<std::string,std::string> ::iterator it; 
        for(it = map.begin();it!=map.end();it++)
        std::cout<<(*it).first<<" "<<(*it).second<<std::endl;
}

void Responses::responses_init(int clit,char addr[]){
    strcpy(ipaddr,addr);
    client = clit;
}

char* Responses::strtochar(std::string str, char an[]){

    int i=0;
    for(i;i<str.length()&&i<20;i++)
    an[i] = str[i];
    an[i] = '\0';
   
return an;
}

void Responses::getCookie(char buf[]){
    int lentocookie = strlen(buf);
    int ncook = 0;
    for(int i=400;i<lentocookie;i++){
        if (buf[i] == 'C' && buf[i + 1] == 'o' && buf[i + 2] == 'o') {
            i = i + 8;
            while (buf[i] != '\n')  cookie[ncook++] = buf[i++];
            cookie[ncook] = '\0';
            return;
        }
    }
    strcpy(cookie,"000000000000000000000");
    return ;


}

void Responses::randstr(char strss[]){
    int i;
    srand(time(NULL));//通过时间函数设置随机数种子，使得每次运行结果随机。
    for(i = 0; i < STR_LEN; i ++)
    {
        strss[i] = rand()%(CHAR_MAX-CHAR_MIN + 1) + CHAR_MIN; //生成要求范围内的随机数。
    }
    strss[i] = '\0';
    return ;
}

Responses::~Responses()
{
     std::cout<<"~Responses.h to BUILD"<<std::endl;
     delete lg;
     lg=NULL;
}

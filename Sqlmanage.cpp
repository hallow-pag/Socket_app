#include"SqlManage.h"

#include<iostream>
#include <math.h>
#include<time.h>
using std::cout;
using std::endl;


/*************************************************
Function: // 函数名称
Description: // 函数功能、性能等的描述
Calls: // 被本函数调用的函数清单
Called By: // 调用本函数的函数清单
Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）
Table Updated: // 被修改的表（此项仅对于牵扯到数据库操作的程序）
Input: // 输入参数说明，包括每个参数的作
// 用、取值说明及参数间关系。
Output: // 对输出参数的说明。
Return: // 函数返回值的说明
Others: // 其它说明
*************************************************/
SqlManage::SqlManage(/* args */)
{
    std::cout<<"SqlManage.h to BUILD"<<std::endl;
    
}

/*************************************************
Function: // 析构函数   关闭数据库
*************************************************/
SqlManage::~SqlManage()
{
    sqlite3_close(db); 
    std::cout<<"Colse SQL OK "<<std::endl;
    std::cout<<"~SqlManage.h to BUILD"<<std::endl;

}

/*************************************************
Function: // adduser
Description: // 添加用户
Input: // 用户名;密码;电话;权限等级;
Output: // 1 成功; 0 权限不足 -1 系统错误
*************************************************/
int SqlManage::adduser( char user[], char pwd[],char power[],char addr[]){
    if(islogin()>=1){
    static MD5 md5(pwd);
    char time[20];
    gettime(time);
    char sql[200];
    /*------用户名 密码检查------*/
    char **azResult=NULL; //二维数组存放结果
    int nrow=0;
    int ncolumn = 0;
    int usernamelen = strlen(user);
    int pwdlen = strlen(pwd);
    if(usernamelen>10||usernamelen<1){
        cout<<"命名不不规范"<<endl;
        return -11;
    }
    if(pwdlen<6){
        cout<<"密码太短"<<endl;
        return -12;
    }
    sprintf(sql,"select id from User where username = '%s'",user);
    sqlite3_get_table( db , sql, &azResult , &nrow , &ncolumn , &zErrMsg );
        if(nrow!=0){
        cout<<"名字重复"<<endl;
        return -10;
    }
    /*--------------------------*/
     sprintf(sql,"INSERT INTO \"main\".\"User\"(\"username\", \"pwd\", \"power\", \"birthday\",\"addr\") VALUES ('%s', '%s','%s', '%s', '%s')"
                                ,user,md5.toStr().c_str(),power,time,addr);
    if(0 == sqlite3_exec(db,sql,NULL,NULL,&zErrMsg)){
        sprintf(sql,"select id from User where username = '%s'",user);
        cout<<"将信息添加到内存,\nsql:"<<sql<<endl;
        azResult =NULL;
        sqlite3_get_table( db , sql, &azResult , &nrow , &ncolumn , &zErrMsg );
        if(nrow!=0){
            int n = chartoint(azResult[1]);
            strcpy(minfo[n].addrip ,addr);
            strcpy(minfo[n].power ,"-1");
            strcpy(minfo[n].timer,time);
            strcpy(minfo[n].user,user);
            return 1;
        }
        std::cout<<"数据插入错误"<<std::endl;
        return 0;
    }
       
    }
    else return 0;
    return -1;
 }


/*************************************************
Function: // login
Description: // 用户登陆
Input: // 用户名;密码;
Output: // 1(登陆成功),-1(未找到用户),0(密码错误);
*************************************************/
int SqlManage::login(char user[], char pwd[],char randte[],char addr[]){
    
    char **azResult=NULL; //二维数组存放结果
    int nrow=0;
    int ncolumn = 0;
    char sql[200];
    char time[20];
    gettime(time);
    sprintf(sql,"select id,pwd,birthday,power,addr from User where username='%s'",user);
    //onst char * sql="select pwd from User where Id='2'";
        int p = sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn , &zErrMsg );
        if(nrow==0){
            return -1; //没有此用户
            }
        else{
            MD5 *md = new MD5(pwd);
            //static MD5 md5(pwd);
            std::cout<<"pwd:"<<md->toStr()<<" sql:"<<azResult[6]<<std::endl;
            if(strcmp(md->toStr().c_str(),azResult[6])==0){
                /* 更新数据 */
                id = chartoint(azResult[5]);
                strcpy(idstr,azResult[5]);
               if(azResult[9]!=NULL) strcpy(minfo[id].addrip,azResult[9]);
                strcpy(minfo[id].timer,azResult[7]);
                strcpy(minfo[id].power,azResult[8]);



                char updatesql[400];
                sprintf(updatesql,"UPDATE \"main\".\"User\" SET \"birthday\" = '%s', \"addr\" = '%s',\"cookie\" = '%s' WHERE username='%s';",time,addr,randte,user);

                sqlite3_exec(db,updatesql,0,0,&zErrMsg);
                delete md;
                
                return 1; //登陆成功
            }
            delete md;
            return 0;//密码错误
        }
        
}

/*************************************************
Function: // log_out
Description: // 用户注销
Input: // void
Output: // false注销失败(未登陆); true(注销成功)
*************************************************/
 bool SqlManage::log_out(){
      char **azResult=NULL; //二维数组存放结果
    int nrow=0;
    int ncolumn = 0;
    char sql[70];
    sprintf(sql,"select id from User where cookie='%.20s'",cookie);
        int p = sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn , &zErrMsg );
        if(nrow==0)
            return false;
        else
        {
            return chartoint(minfo[chartoint(azResult[1])].power) ==-1 ? false:true;
            
        }
        return false;
 }


/*************************************************
Function: // islogin
Description: // 检测用户是否登陆
Input: // void
Output: // -1 未登陆; 0普通权限; 1高级权限; 2root权限;
*************************************************/
int SqlManage::islogin(){
    //linkinit();
   char **azResult=NULL; //二维数组存放结果
    int nrow=0;
    int ncolumn = 0;
    id = 0;
    char sql[156];
    sprintf(sql,"select id from User where cookie='%.20s'",cookie);
   
    //onst char * sql="select pwd from User where Id='2'";
        sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn , &zErrMsg );
  
        if(nrow == 0) return -1;
        
        else{
                id = chartoint(azResult[1]);
                int s = chartoint(minfo[chartoint(azResult[1])].power);
               return s;
        }
            return  -1;
}



/*************************************************
Function: // chartoint
Description: // 字符串转int
Input: // string
Output: // int
*************************************************/
int SqlManage::chartoint(char str[]){
    if(str[0]=='-')
    return -1;
    int sum=0;
    int l = strlen(str);
    for(int i=0;i<l;i++){
        sum*=10;
        sum += (str[i]-'0');
    }
return sum;
}


/*************************************************
Function: // linkinit
Description: // 初始化数据库-连接诶数据库 - 将用户信息载入内存
*************************************************/
void SqlManage::linkinit(){
  
    char *zErrMsg =NULL;
    int rc;
    rc = sqlite3_open("../Date/hello.db", &db); 
    if( rc ) 
        { 
        std::cout<<" Can't open database "<<std::endl;
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db)); 
        sqlite3_close(db); 
    }
    char **azResult=NULL; //二维数组存放结果
    int nrow=0;
    int ncolumn = 0;
    const char * sql = "select * from User";
    //onst char * sql="select pwd from User where Id='2'";
        sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn , &zErrMsg );
        
        int ids;
        for(int i = ncolumn;i<=(nrow*ncolumn);i+=ncolumn){
            ids = chartoint(azResult[i]);
            if(strlen(azResult[i+5])>4) strcpy( minfo[ids].addrip,azResult[i+5]);
                strcpy( minfo[ids].power,"-1");
           if(strlen(azResult[i+4])>0) strcpy( minfo[ids].timer,azResult[i+4]);
           if(strlen(azResult[i+1])>0) strcpy(minfo[ids].user,azResult[i+1]);
        }
        
    
    std::cout<<" Open Sql Succulb "<<std::endl;
}



/*************************************************
Function: // gettime
Description: // 获取时间 格式:YY:mm:dd HH:MM:SS
Input: // string
Output: // 
*************************************************/
void SqlManage::gettime(char psTime[]) {
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



/*************************************************
Function: // char_int
Description: // 字符串转int
Input: // strint
Output: // int
*************************************************/
void SqlManage::sqlinit(char cooki[]){
    strcpy(cookie,cooki);
    
}

meninfo SqlManage::getinfo(){
    return minfo[id];
}

/* user list */
void SqlManage::userlist(char req[],int vim){
    if(islogin()>1){
    char **azResult=NULL; //二维数组存放结果
    int nrow=0;
    int ncolumn = 0;
    vim+= sprintf(req+vim,"{\"Date\":[");
    const char * sql = "select * from User";
        sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn , &zErrMsg );
        
        int ids,i;
        for(i = ncolumn;i<(nrow*ncolumn);i+=ncolumn){
            ids = chartoint(azResult[i]);
            vim += sprintf(req+vim,"{\"id\":%d,\"username\":\"%s\",\"power\":\"%s\",\"login_before\":\"%s\",\"ip\":\"%s\"},"
                                    ,ids,azResult[i+1],azResult[i+3],azResult[i+4],azResult[i+5]);
        }
        ids = chartoint(azResult[i]);
        vim += sprintf(req+vim,"{\"id\":%d,\"username\":\"%s\",\"power\":\"%s\",\"login_before\":\"%s\",\"ip\":\"%s\"}]}"
                                ,ids,azResult[i+1],azResult[i+3],azResult[i+4],azResult[i+5]);
    }
    else sprintf(req+vim,"{\"data\":{\"return\":%d,\"text\":\"%s\"}}",-1,"权限不足");
    
   }  

/* Update user */ 
/*returrn -2 权限不足; -1 id错误; 0 系统错误; 1 OK */
int SqlManage::updateuser(char userid[],char user[],char pwd[], char power[]){
    if(islogin()>1){
    int user_len = strlen(user);
    int pwd_len = strlen(pwd);
    int power_len = strlen(power);
    
    char **azResult=NULL; //二维数组存放结果
    char **azResul=NULL;
    int nrow=0;
    int ncolumn = 0;
    char sql[100];
    sprintf(sql,"select username,pwd,power from User where id=%s",userid);
    sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn , &zErrMsg );
        if(nrow == 0) return -1;
        
        else{
            
            char updatesql[200];
            if(user_len==0)
            {
                strcpy(user,azResult[3]);
            } else{
                sprintf(sql,"select id from User where username = '%s'",user);
                sqlite3_get_table( db , sql, &azResul , &nrow , &ncolumn , &zErrMsg );
                if(nrow!=0){
                    return -10;
            } }
            if(power_len==0) strcpy(power,azResult[5]);
            if(pwd_len!=0&&pwd_len<6) return -12;
            if(pwd_len==0) { 
                sprintf(updatesql,"UPDATE \"main\".\"User\" SET \"username\" = '%s', \"pwd\" = '%s',\"power\" = '%s' WHERE id=%s",user,azResult[4],power,userid);
            }
       
            else{ 
                printf("--%s\n",pwd);
                static MD5 md5(pwd); 
                printf("--%s--\n",md5.toStr().c_str());
                sprintf(updatesql,"UPDATE \"main\".\"User\" SET \"username\" = '%s', \"pwd\" = '%s',\"power\" = '%s' WHERE id=%s",user,md5.toStr().c_str(),power,userid);
                
                }
            if(sqlite3_exec(db,updatesql,0,0,&zErrMsg)==0) return 1;
            return 0;
        }
        return  0;
    }
    else return -2;
       
}

int SqlManage::deluser(char userid[]){
    char sql[50];
    sprintf(sql,"DELETE FROM \"main\".\"User\" WHERE id =%s",userid);
    int res;
    res = sqlite3_exec( db , sql  ,NULL  ,NULL ,&zErrMsg );
    if(res == 0) return 1;
    return 0;
}

char * SqlManage::loghist( char userid[]){
    strcpy(userid,idstr);
    return userid;
}

/*正常访问*/
void SqlManage::req_ok(char url[], char addr[]){
    char sql[120];
    char time[20];
    gettime(time);
    sprintf(sql,"INSERT INTO \"main\".\"req_abnormal\"(\"addr\", \"time\") VALUES ('%s','%s')",addr,time);
    cout<<sql<<endl;
    sqlite3_exec(db,sql,NULL,NULL,&zErrMsg);
}

/*异常访问*/
void SqlManage::req_dager(char url[],char addr[]){
    int n = strlen(url);
    if(n>20)
    strcpy(url,"aaa");
     char sql[220];
    char time[20];
    gettime(time);
    sprintf(sql,"INSERT INTO \"main\".\"req_normal\"(\"url\",\"addr\", \"time\") VALUES ('%s','%s','%s')",url,addr,time);
    cout<<sql<<endl;
    sqlite3_exec(db,sql,NULL,NULL,&zErrMsg);
}

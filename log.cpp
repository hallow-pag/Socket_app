

#include"log.h"
using std::cout;
using std::endl;
#include"publfunck.hpp"

log::log(/* args */)
{
    int rc;
    rc = sqlite3_open("../Date/hello.db", &db); 
    if( rc ) 
        { 
        std::cout<<" Can't open database "<<std::endl;
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db)); 
        sqlite3_close(db); 
    }
    else
    std::cout<<" Open Sql Succulb log"<<std::endl;
}

log::~log()
{
    sqlite3_close(db); 
    std::cout<<"Colse SQL OK "<<std::endl;
    std::cout<<"~SqlManage.h to BUILD"<<std::endl;
}

void log::login_log(char userid[],const char * info,char addr[]){
    cout<<"log: login_log "<<info<<" "<<addr<<endl;
    char sql[150];
    char time[20];
    gettime(time);
    sprintf(sql,"INSERT INTO \"main\".\"log_login\"(\"info\", \"addrs\", \"time\", \"user\") VALUES ('%s', '%s', '%s', '%s')",info,addr,time,userid);
    sqlite3_exec(db,sql,NULL,NULL,&zErrMsg);
}

void log::logout_log(char userid[],char id[]){

}
void log::adduser_log(char userid[],char username[],char power[]){
    cout<<"log: adduser_log name("<<username<<") power("<<power<<")"<<endl;
    char sql[150];
    char time[20];
    gettime(time);
    sprintf(sql,"INSERT INTO \"main\".\"log_adduser\"(\"username\",\"userid\", \"time\", \"power\") VALUES ('%s','%s', '%s', '%s')",username,userid,time,power);

    sqlite3_exec(db,sql,NULL,NULL,&zErrMsg);

}

/* 删除修改 */
void log::upuser_log(char user[],char upuserid[],const char * info){

    //INSERT INTO "main"."log_upuser"("userid", "info", "time") VALUES ('12', '12', '1221')
    cout<<"log: upuser_log "<<info<<endl;
    char sql[150];
    char time[20];
    gettime(time);
    sprintf(sql,"INSERT INTO \"main\".\"log_upuser\"(\"userid\",\"upuserid\",\"info\", \"time\") VALUES ('%s', '%s', '%s', '%s')",user,upuserid,info,time);
  
    sqlite3_exec(db,sql,NULL,NULL,&zErrMsg);
}


char * log::show_log_user(char req[],int n){

    char sql1[60] = {"select * from log_login"};
    char sql2[60] = {"select * from log_adduser"};
    char sql3[60] = {"select * from log_upuser"};
    n += sprintf(req+n,"{\"Date\":{\"log_user\":[");
    char **azResult=NULL; //二维数组存放结果
    int nrow=0;
    int ncolumn = 0;
    int i;
    sqlite3_get_table( db , sql1 , &azResult , &nrow , &ncolumn , &zErrMsg );
    for(i = ncolumn;i<(nrow*ncolumn);i+=ncolumn){
        n += sprintf(n+req,"{\"user\":\"%s\",\"info\":\"%s\",\"addr\":\"%s\",\"time\":\"%s\"},"
        ,azResult[i+4],azResult[i+1],azResult[i+2],azResult[i+3]);
        }
         n += sprintf(n+req,"{\"user\":\"%s\",\"info\":\"%s\",\"addr\":\"%s\",\"time\":\"%s\"}],\"log_adduser\":["
         ,azResult[i+4],azResult[i+1],azResult[i+2],azResult[i+3]);
    
    //INSERT INTO \"main\".\"log_adduser\"(\"username\",\"userid\", \"time\", \"power\") 
    sqlite3_get_table( db , sql2 , &azResult , &nrow , &ncolumn , &zErrMsg );
    for(i = ncolumn;i<(nrow*ncolumn);i+=ncolumn){
        n += sprintf(n+req,"{\"usering\":\"%s\",\"user\":\"%s\",\"time\":\"%s\",\"power\":\"%s\"},"
        ,azResult[i+1],azResult[i+2],azResult[i+3],azResult[i+4]);
        }
         n += sprintf(n+req,"{\"usering\":\"%s\",\"user\":\"%s\",\"time\":\"%s\",\"power\":\"%s\"}],\"log_upuser\":["
        ,azResult[i+1],azResult[i+2],azResult[i+3],azResult[i+4]);

    //INSERT INTO \"main\".\"log_upuser\"(\"userid\",\"upuserid\",\"info\", \"time\"
    sqlite3_get_table( db , sql2 , &azResult , &nrow , &ncolumn , &zErrMsg );
    for(i = ncolumn;i<(nrow*ncolumn);i+=ncolumn){
    n += sprintf(n+req,"{\"usering\":\"%s\",\"usered\":\"%s\",\"info\":\"%s\",\"time\":\"%s\"},"
    ,azResult[i+1],azResult[i+2],azResult[i+3],azResult[i+4]);
    }
        n += sprintf(n+req,"{\"usering\":\"%s\",\"user\":\"%s\",\"time\":\"%s\",\"power\":\"%s\"}]}}"
    ,azResult[i+1],azResult[i+2],azResult[i+3],azResult[i+4]);
    return req;

}

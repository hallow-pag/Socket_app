[TOC]

 <img src="https://github.com/hallow-pag/Socket_app/raw/master/src/7.png" style="zoom:80%;" />

# 环境

```
系统 Ubuntu 18
环境 g++ 7.5
数据库 sqlite3 
编译工具 GNU Make 4.1 cmake version 3.10.2
```

## sqlite3 安装

```
下载地址：https://www.sqlite.org/
下载格式：sqlite-autoconf-xxxxxx.tar.gz
安装
$ tar xvzf sqlite-autoconf-3071502.tar.gz
$ cd sqlite-autoconf-3071502
$ ./configure --prefix=/usr/local
$ make
$ make install
```

运行

```
$ cd Server/builed
$ cmake ..
$ make
$ ./socket_app
系统初始密码 username:2 pwd:22
```

# 技术说明

MD5加密

```
用户密码采用md5加密
MD5的全称是Message-Digest Algorithm，是Hash算法中的一种重要算法，具有单项加密、加密结果唯一、安全性能好等特点。MD5以512位分组来处理输入的信息，且每一分组又被划分为16个 32位子分组，经过了一系列的处理后 ，算法的输出由四个32位分组组成，将这四个32位分组级联后将生成一个128位散列值。
```

Epoll

```
epoll是Linux内核为处理大批量文件描述符而作了改进的poll，是Linux下多路复用IO接口select/poll的增强版本，它能显著提高程序在大量并发连接中只有少量活跃的情况下的系统CPU利用率。另一点原因就是获取事件的时候，它无须遍历整个被侦听的描述符集，只要遍历那些被内核IO事件异步唤醒而加入Ready队列的描述符集合就行了。epoll除了提供select/poll那种IO事件的水平触发（Level Triggered）外，还提供了边缘触发（Edge Triggered），这就使得用户空间程序有可能缓存IO状态，减少epoll_wait/epoll_pwait的调用，提高应用程序效率。
```

多线程

```
使用多线程使CPU利用率最大化。
```

# 异常的返回值

```c
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
```

```
参数过长
请求头过长

```

# 权限说明

| 权限标识符 | 说明                                     |      |
| ---------- | ---------------------------------------- | ---- |
| -1         | 无权限账号、账号注销、无法登录。         |      |
| 0          | 普通权限账号、可以登录、查看设备信息。   |      |
| 1          | 管理员权限、可以对设备进行配置。         |      |
| 2          | 最高权限、可以添加用户、对用户进行修改。 |      |

# 接口说明

## 用户模块

### 登录 

```
url地址:/login
权限：0
请求方式:GET
参数: username(必填), pwd(必填)
Exp:/login?username=1&pwd=1
返回:
1、是否成功（成功返回1，失败返回-1）， 2、中文信息 3、上次登录时间 4、上次登录地址
{"return":1,"text":"登陆成功","on_time":"2020-08-09 09:32:05","on_addr":"120.229.60.56"}

url地址:/lg
权限：0
请求方式:GET
参数: username(必填), pwd(必填)
Exp:/login?username=1&pwd=1
返回:
1、是否成功（成功返回1，失败返回-1）， 2、中文信息 3、上次登录时间 4、上次登录地址
{"return":1,"text":"登陆成功","on_time":"2020-08-09 09:32:05","on_addr":"120.229.60.56"}
```

### 登出 

```
url地址:/logout
权限：0
请求方式:GET
参数: 无
Exp:/logout
返回:
{"data":{"return":1,"text":"注销成功"}}
```

### 添加用户 

（权限：2）

```
url地址:/registered
权限：2
请求方式:GET
参数: username(必填), pwd(必填), power(必填)
Exp:/registered?username=1&pwd=1&power=-1
返回:
{"data":{"return":1,"text":"添加成功"}}
```

### 查看所有用户信息 

```
url地址:/userlist
权限：2
请求方式:GET
参数: 无
Exp:/userlist
返回:
```

<img src="https://github.com/hallow-pag/Socket_app/raw/master/src/1.PNG" style="zoom: 80%;" />

### 修改用户信息 

```
url地址:/upuser
权限：2
请求方式:GET
参数: id(必填), username(选填), pwd(选填), power(选填)
Exp:/upuser?id=1
返回:
{"data":{"return":1,"text":"操作成功"}}
```

### 删除用户

```
url地址:/deluser
权限：2
请求方式:GET
参数: id(必填)
Exp:/deluser?id=1
返回:
{"data":{"return":1,"text":"操作成功"}}
```

## 系统模块

### 系统信息

```
url地址:/deluser
权限：2
请求方式:GET
参数: id(必填)
Exp:/deluser?id=1
返回:
```

<img src="https://github.com/hallow-pag/Socket_app/raw/master/src/2.PNG" style="zoom: 80%;" />

```c
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
```

## 网络设置

关闭&启动指定网卡

```shell
# ifconfig eth0 down
# ifconfig eth0 up
```

设置IPv6地址

```shell
# ifconfig eth0 add 33ffe:3240:800:1005::2/ 64 //为网卡设置IPv6地址
```

删除IPv6地址

```shell
# ifconfig eth0 del 33ffe:3240:800:1005::2/ 64 //为网卡删除IPv6地址
```

修改MAC地址

```shell
# ifconfig eth0 down //关闭网卡
# ifconfig eth0 hw ether 00:AA:BB:CC:DD:EE //修改MAC地址
# ifconfig eth0 up //启动网卡
# ifconfig eth1 hw ether 00:1D:1C:1D:1E //关闭网卡并修改MAC地址 
# ifconfig eth1 up //启动网卡
```

配置IP地址

```shell
# ifconfig eth0 192.168.1.56 
//给eth0网卡配置IP地址
# ifconfig eth0 192.168.1.56 netmask 255.255.255.0 
// 给eth0网卡配置IP地址,并加上子掩码
# ifconfig eth0 192.168.1.56 netmask 255.255.255.0 broadcast 192.168.1.255
// 给eth0网卡配置IP地址,加上子掩码,加上个广播地址
```

启用&关闭ARP协议

```shell
# ifconfig eth0 arp  //开启
# ifconfig eth0 -arp  //关闭
```

设置最大传输单元

```shell
# ifconfig eth0 mtu 1500 
//设置能通过的最大数据包大小为 1500 bytes
```



## 其他命令

```
url地址:/
权限：2
请求方式:GET
参数:
Exp:/
返回:
```

# 日志

```
url地址:/showlog
权限：1
请求方式:GET
参数: id(必填)
Exp:/showlog
返回:
```

<img src="https://github.com/hallow-pag/Socket_app/raw/master/src/4.png" style="zoom:80%;" />

```
参数过长
请求头过长

```

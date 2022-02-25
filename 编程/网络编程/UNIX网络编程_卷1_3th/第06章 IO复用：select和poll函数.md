

[TOC]

## 1.概述

在第05章的`边界条件>服务端进程终止`的场景的`步骤5`和`步骤6`中出现这样的一个过程：

> 由于客户端阻塞在`fget()`函数，当服务端断开连接，客户端无法及时处理该响应，直到收控制台输入内容

本章介绍IO复用模型并使用I/O复用模型改造前面的例子。



## 2.五种I/O模型

### 2.1 阻塞式I/O

![image-20220223100200920](%E7%AC%AC06%E7%AB%A0%20IO%E5%A4%8D%E7%94%A8%EF%BC%9Aselect%E5%92%8Cpoll%E5%87%BD%E6%95%B0.assets/image-20220223100200920.png)

> 阻塞式I/O是容易实现也是低效的，如图6-1所示，以`recvfrom()`的调用为例，系统调用会导致进程一直阻塞直到数据到达且被复制到应用进程的缓冲区或发生错误才返回。

### 2.2 非阻塞式I/O

![image-20220223101212163](%E7%AC%AC06%E7%AB%A0%20IO%E5%A4%8D%E7%94%A8%EF%BC%9Aselect%E5%92%8Cpoll%E5%87%BD%E6%95%B0.assets/image-20220223101212163.png)

> 非阻塞IO中很明显的不同是：如果数据没有准备好，函数调用不会一直阻塞而是立即返回一个`EWOULDBLOCK`错误告知进程数据未达到。非阻塞IO可以通过不断询问数据，但缺点是消耗大量CPU

### 2.3 I/O复用（select和poll）

![image-20220223101908090](%E7%AC%AC06%E7%AB%A0%20IO%E5%A4%8D%E7%94%A8%EF%BC%9Aselect%E5%92%8Cpoll%E5%87%BD%E6%95%B0.assets/image-20220223101908090.png)

> IO复用模型中重要的两个函数`select()`、`poll()`。图6-1和图6-3看到的IO复用模型和阻塞式IO在形式上是类似的，实际上IO复用模型很大的一个优势是：可以同时等待多个描述符。

### 2.4信号驱动式I/O（SIGIO）

![image-20220223103238825](%E7%AC%AC06%E7%AB%A0%20IO%E5%A4%8D%E7%94%A8%EF%BC%9Aselect%E5%92%8Cpoll%E5%87%BD%E6%95%B0.assets/image-20220223103238825.png)

> 信号驱动IO通过`sigaction()`函数安装信号处理函数，当等待的IO事件发生后，内核为进程产生一个SIGIO信号，并自动触发信号处理函数。

### 2.5 异步I/O（POSIX的aio\_系列函数）

![image-20220223104522480](%E7%AC%AC06%E7%AB%A0%20IO%E5%A4%8D%E7%94%A8%EF%BC%9Aselect%E5%92%8Cpoll%E5%87%BD%E6%95%B0.assets/image-20220223104522480.png)

> 异步IO工作机制是：告知内核启动某个动作，并让内核在完成整个操作后通知进程。
>
> 异步IO和信号驱动IO的区别在于：信号驱动IO由内核通知何时可以启动一个IO操作，异步IO由内核通知IO操作何时完成。（其实，，还是不大懂）

### 6.6 各IO模型对比

![image-20220223105413152](%E7%AC%AC06%E7%AB%A0%20IO%E5%A4%8D%E7%94%A8%EF%BC%9Aselect%E5%92%8Cpoll%E5%87%BD%E6%95%B0.assets/image-20220223105413152.png)

> 图6-6对比上述5中IO模型，可以看出，前4中模型的主要区别在于第一阶段，因为它们的第二阶段是一样的：在数据从内核复制到调用者的缓冲区期间，进程阻塞于`recvfrom()`调用。相反，异步IO模型在这两个阶段都要处理，从而不同于其他4中模型。

> 同步IO：导致请求进程阻塞，直到IO操作完成
>
> 异步IO：步导致请求进程阻塞

## 3. IO函数

### 3.1 select函数

```cpp
#include <sys/select.h>
#include <sys/time.h>
int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, struct timeval* timeout);                    // 若由就绪描述符返回数目，超时返回0，出错返回-1
```

其中：`readfds`、`writefds`、`exceptfds`分别指定关注的读、写、异常条件的描述符集（空指针表示不关心该类型事件）。描述符集通常设计成一个证书数组，数组中每个整数中的每一位对应一个描述符。（例如，使用32为整数的系统中，数组中第一个整数对于描述符0~31，第二个整数对应描述符32~63，以此类推）。描述符集的设置主要通过以下四个宏函数：

```cpp
void FD_ZERO(fd_set *fdset);                // 将描述符集fdset置为零
void FD_SET(int fd, fd_set *fdset);         // 打开描述符集中的fd位
void FD_CLR(int fd, fd_set *fdset);         // 关闭描述符集中的fd位
int  FD_ISSET(int fd, fd_set *fdset);       // 查看fd位是否打开
```



`nfds`参数指定描述符空间，等于最大描述符+1（描述符从0开始，如果最大描述符为5，实际上有6个描述符）。

> `readfds`、`writefds`、`exceptfds`是`值-结果`参数，调用`select()`函数前需要将关心的描述符位设为1，`select()`函数返回时不为零的描述符位表示已就绪的描述符，再次调用`select()`函数需要再次设置关心的描述符集。



超时参数`timeout`的结构定义如下

```cpp
struct timeval{
    long tv_sec;         // seconds
    long tv_usec;        // microseconds
}
```

> 若timeout为空指针，则永远等待下去；若timeout中等待时间为0，表示立即返回；否则等待指定时间后超时
>
> 此外，虽然我们设置的超时精确到了微秒，实际上，内核不一定支持这么细的粒度。



#### 3.1.1 描述符就绪条件

![image-20220223145738487](%E7%AC%AC06%E7%AB%A0%20IO%E5%A4%8D%E7%94%A8%EF%BC%9Aselect%E5%92%8Cpoll%E5%87%BD%E6%95%B0.assets/image-20220223145738487.png)

> 详细参考《UNIX网络编程 卷1 第3版》第6.3.1节



### 3.2 shutdown函数

引入`shutdown()`函数的原因是：`close()`函数关闭套接字需要满足两个限制条件：

1. 调用`close()`会将套接字的引用计数减1，如果计数变成0，套接字才能被关闭
2. `close()`函数会同时终止套接字的读操作和写操作，而TCP连接时全双工，无法满足只关闭读/写的场景

```cpp
#include <sys/socket.h>
int shutdown(int fd, int how);   // 成功返回0，出错返回-1
```

`fd`参数表示要关闭的描述符，`how`值为下列的选项之一：

- `SHUT_RD`: 关闭套接字读一半，不再接收数据并且丢弃缓冲区中现有数据，进程不可对该套接字进行读操作
- `SHUT_WR`: 关闭套接字写一半，发送缓冲区现有数据后发送`FIN`，套接字将变成`半关闭`。
- `SHUT_RDWR`: 关闭套接字读写，等价于先调用`SHUT_RD`，再调用`SHUT_WR`。



### 3.3 pselect函数

```cpp
#include <sys/select.h>
#include <signal.h>
#include <time.h>

int pselect(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, const struct timespec *timeout, const sigset_t *sigmask); // 返回：正整数表示就绪描述符数量，0表示超时，-1表示出错

struct timespec {
    __time_t tv_sec;		    /* Seconds.  */
    __syscall_slong_t tv_nsec;	/* Nanoseconds.  */
};
```

`pselect()`函数和`select()`函数的区别在于：

- `pselect()`使用`timespec`结构，而不是`timeval`结构，新的结构中`tv_nsec`使用的时纳秒。并且，`timeout`结构使用`const`限定符，表示该`timeout`参数不会被改变。
- `pselect()`函数增加第六个参数：指向信号掩码的指针。该参数允许程序先禁止`sigmask`指定的信号，再测试由`sigmask`的处理函数设置的全局变量，然后调用`pselect()`重新设置信号掩码。

关于第二点不是太懂，之后再看。。



### 3.4 poll函数

```cpp
#include <poll.h>

typedef unsigned long int nfds_t;
struct pollfd {
    int fd;         /* 检查的描述符 */
    short events;   /* 该描述符等待的事件 */
    short revents;  /* 该描述符发生的事件 */
};

int poll(struct pollfd *fds, nfds_t nfds, int timeout); // 返回：正整数表示就绪描述符数量，0表示超时，-1表示出错
```

`fds`表示要检测的描述符数组指针，`nfds`表示数组的长度，`timeout`表示等待的超时时间。

其中 `pollfd`数据结构的中的`event`、`revent`的可取值如图6-23 所示：

![image-20220223175220099](%E7%AC%AC06%E7%AB%A0%20IO%E5%A4%8D%E7%94%A8%EF%BC%9Aselect%E5%92%8Cpoll%E5%87%BD%E6%95%B0.assets/image-20220223175220099.png)

`timeout`参数可能的取值有三种：

- `INFTIM`：被定义为符数，表示无限等待
- `0`：立即返回而不阻塞
- `>0`：等待指定时间超时



### 3.5 同时处理多个事件——使用select实现客户端（修订版1）

在本章的概述中说到的`由于客户端阻塞在fget()函数，当服务端断开连接，客户端无法及时处理该响应，直到收控制台输入内容`。代码`tcpcli06_select.c`是在`tcpcli01.c`代码基础上，使用`select()`函数改造`str_cli()`函数后解决该问题。

```cpp
// tcpcli06_select.c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#define SA          struct sockaddr
#define SERV_PORT   8888
#define MAXLINE     4096   /* max text line length */
using namespace std;

void err_quit(const char *fmt, ...) {
    std::cout << fmt << endl;
    _exit(1);
}

void str_cli(FILE *fp, int sockfd) {
    int     nfds;
    fd_set  rset;
    char    sendline[MAXLINE], recvline[MAXLINE];

    FD_ZERO(&rset);
    for( ; ; ){
        FD_SET(sockfd, &rset);
        FD_SET(fileno(fp), &rset);
        nfds = max(sockfd, fileno(fp)) + 1;
        select(nfds, &rset, nullptr, nullptr, nullptr);

        if(FD_ISSET(sockfd, &rset)){
            if(read(sockfd, recvline, MAXLINE) == 0)
                err_quit("str_cli: server terminated prematurely");
    
            fputs(recvline, stdout);
            memset(recvline, 0, sizeof(recvline));
        }
        if(FD_ISSET(fileno(fp), &rset)){
            if(fgets(sendline, MAXLINE, fp) == NULL)
                return;

            write(sockfd, sendline, MAXLINE);
        }
    }
}

int main(int argc, char** argv){
    int sockfd;
    struct sockaddr_in servaddr;

    if(argc != 2){
        err_quit("usage: tcpcli <IP ADDRESS>");
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
    str_cli(stdin, sockfd);
    _exit(0);
}
```

> `tcpcli06_select.c`代码在`tcpcli01.c`代码基础上，修改`str_cli()`函数为`select()`的使用。



### 3.6 批量输入——使用shutdown实现客户端（修订版2）

即便我们的程序功能很简单了，但在客户端的`修订版1`的`str_cli()`中仍然存在一些问题，尝试以下测试：

```shell
# 生成input文件
[root@QingYun chapter06]# for i in {1..100}; do printf "$i %.0s" {1..10};echo ; done > input

# 多次运行修订版本1，查看结果
[root@QingYun chapter06]# cat input | ./tcpcli06_select.o 127.0.0.1
```



可以看到，客户端得到的输出内容总是少于input中的输入！！这是因为：

- 在客户端使用批量输入，批量输入在很短的时间内完成输入，而客户完成处理一条信息明显需要更长的时间。
- 并且在`str_cli()`函数中，一旦接收到客户端的`EOF`，客户端立即return返回main函数随后终止进程。

因而导致客户端未完成处理和接收所有信息，客户端程序已经先退出了。

解决这个问题我们只需要在客户端完成所有输入之后，关闭客户端socket的读一半连接，而保持socket可写的一半连接。代码`tcpcli06_select_shutdown.c`为使用`shutdown()`改造后的客户端`修改版2`。



```cpp
// tcpcli06_select_shutdown.c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#define SA          struct sockaddr
#define SERV_PORT   8888
#define MAXLINE     4096   /* max text line length */
using namespace std;

void err_quit(const char *fmt, ...) {
    std::cout << fmt << endl;
    _exit(1);
}

void str_cli(FILE *fp, int sockfd) {
    fd_set  rset;
    int     nfds, stdinEof = 0;
    char    sendline[MAXLINE], recvline[MAXLINE];

    FD_ZERO(&rset);
    for( ; ; ){
        FD_SET(sockfd, &rset);
        if(stdinEof==0) FD_SET(fileno(fp), &rset);

        nfds = max(sockfd, fileno(fp)) + 1;
        select(nfds, &rset, nullptr, nullptr, nullptr);

        if(FD_ISSET(sockfd, &rset)){
            if(read(sockfd, recvline, MAXLINE) == 0){
                if(stdinEof == 0) return;
                else err_quit("str_cli: server terminated prematurely");
            }

            fputs(recvline, stdout);
            memset(recvline, 0, sizeof(recvline));
        }
        if(FD_ISSET(fileno(fp), &rset)){
            if(fgets(sendline, MAXLINE, fp) == NULL){
                stdinEof = 1;
                shutdown(sockfd, SHUT_WR);
                FD_CLR(sockfd, &rset);
            }
            else write(sockfd, sendline, MAXLINE);
        }
    }
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        write(sockfd, sendline, strlen(sendline));
        if (read(sockfd, recvline, MAXLINE) == 0)
            err_quit("str_cli: server terminated prematurely");

        fputs(recvline, stdout);
        memset(recvline, 0, sizeof(recvline));
    }
}

int main(int argc, char** argv){
    int sockfd;
    struct sockaddr_in servaddr;

    if(argc != 2){
        err_quit("usage: tcpcli <IP ADDRESS>");
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
    str_cli(stdin, sockfd);
    _exit(0);
}
```

> 执行`cat input | ./tcpcli06_select.o 127.0.0.1`，测试修改版2的客户端代码



### 3.8 处理多个客户端——使用select实现服务端（修订版1）



#### 拒绝服务型攻击





### 3.9 缓冲区

## 原理

红黑树





## 常用函数

### 创建epoll树

```cpp
int eopll_create(int size)  // 成功返回新创建的新红黑数的 根节点epfd，失败就-1
```

`size`：创建的红黑树的监听节点数量。（仅供内核参考）



### 操作epoll树

```cpp
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);  // 成功0，失败-1
```

`epfd`：红黑树的根节点，epoll_create的返回值
`op`：对该监听所做的操作，节点添加、修改、摘除。

- EPOLL_CTL_ADD： 添加fd到红黑树
- EPOLL_CTL_MOD： 修改fd在红黑树上的监听事件
- EPOLL_CTL_DEL： 将fd从红黑树上摘除（取消监听）

`fd`：待监听的fd
`event`：一个**结构体**，不是数组，注意和poll区分





### 等待事件

等待事件的产生，返回须要处理的事件的数量，并将需处理事件的套接字集合于參数events内，能够遍历events来处理事件。

```cpp
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
```

`epfd`：红黑树根节点
`events`：注意和`creat`函数中的不一样，这里面有s，所以这个是一个**数组**，典型的传出参数，传出满足监听条件的fd结构体
`maxevents`：数组 元素的总个数 不是实际监听的

- struct epoll_event events[1024]；传1024

`timeout`：-1阻塞，0是非阻塞，>0超时时间
`返回值`：

- 大于0：满足监听的总个数，**可以用作数组下标**，**用作循环上限**
- 0：没有满足监听事件
- 小于0：错误



## 实例

服务端

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

#define PORT        8888
#define MAX_SIZE    1024
#define LISTENQ     1024
#define LINE_SIZE   1024

void epoll_add(int epollfd, int fd, int flag)
{
    struct epoll_event event;

    event.data.fd = fd;
    event.events = EPOLLIN;

    if (flag) {
        event.events |= EPOLLET;
    }

    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);

    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

char buf[LINE_SIZE];
void process_event(struct epoll_event* event, int epollfd, int listenfd){
    int ret, socketfd = event->data.fd;

    if(socketfd == listenfd){
        struct sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);
        int connfd = accept(socketfd, (struct sockaddr*)&addr, &addrlen);

        epoll_add(epollfd, connfd, true);
    }
    else if(event->events & EPOLLIN){
        while(1){
            memset(buf, 0, sizeof(buf));
            ret = read(socketfd, buf, sizeof(buf)-1);

            if(ret <= 0){
                if ((errno != EAGAIN) && (errno != EWOULDBLOCK) && (errno != EINTR)) {
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, socketfd, NULL);
                    close(socketfd);
                }
                break;
            }
            else{
                cout << "recv msg from: " << socketfd << " : " << buf << endl;
                write(socketfd, buf, strlen(buf));
            }
        }
    }
    else {
        cout << "unknown: " << event->data.fd << ", "  << event->events << endl;
    }

}

int main(int argc, char **argv)
{
    int ret, listenfd;
    struct sockaddr_in addr;
    struct epoll_event events[MAX_SIZE];
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(listenfd >= 0);

    ret = bind(listenfd, (struct sockaddr *)&addr, sizeof(addr));
    assert(ret == 0);

    ret = listen(listenfd, LISTENQ);
    assert(ret == 0);

    int epollfd = epoll_create(MAX_SIZE);
    assert(epollfd >= 0);

    epoll_add(epollfd, listenfd, true);

    for (;;) {
        ret = epoll_wait(epollfd, events, MAX_SIZE, -1);
        assert(ret >= 0);

        for(int i = 0; i< ret; i++){
            process_event(&events[i], epollfd, listenfd);
        }
    }
    close(listenfd);
    return 0;
}
```



客户端

```cpp
#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
using namespace std;

#define PORT        8888
#define MAX_SIZE    1024
#define LISTENQ     1024
#define LINE_SIZE   1024


int client_init(string ip, string port){
    int ret, socketfd;
    struct sockaddr_in addr;
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port.c_str()));
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(socket > 0);

    ret = connect(socketfd, (struct sockaddr *)&addr, sizeof(addr));
    assert(ret == 0);

    return socketfd;
}

bool epoll_add(int epollfd, int fd, int flags) {
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;

    if(flags)
        event.events |= EPOLLET;
    
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void epoll_del(int epollfd, int fd){
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
}

void process(int fd){
    int ret, epollfd;
    char sendbuf[LINE_SIZE], recvbuf[LINE_SIZE];

    epollfd = epoll_create(5);
    assert(epollfd > 0);

    epoll_add(epollfd, fd, true);
    epoll_add(epollfd, fileno(stdin), true);

    struct epoll_event events[5];

    for(;;){
        ret = epoll_wait(epollfd, events, 5, -1);
        
        for(int i = 0; i < ret; i++){
            int socketfd = events[i].data.fd;

            if(socketfd == fd && (events[i].events & EPOLLIN)){
                read(fd, recvbuf, LINE_SIZE);
                 cout << string("recv from server: ") + recvbuf << endl;
            }
            else if(socketfd == fileno(stdin) && (events[i].events & EPOLLIN)){
                memset(sendbuf, 0, sizeof(sendbuf));
                if(fgets(sendbuf, LINE_SIZE, (stdin)) == NULL)
                    return;
                
                write(fd, sendbuf, strlen(sendbuf)+1);
            }
        }
    }
}

int main(int argc, char **argv){
    if(argc != 3){
        cout << "Usage: ./client $SERVER_IP $SERVER_PORT" << endl;
        return 1;
    }

    int socketfd = client_init(argv[1], argv[2]);
    assert(socketfd > 0);

    process(socketfd);

    return 0;
}
```








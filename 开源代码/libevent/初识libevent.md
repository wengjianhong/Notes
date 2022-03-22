



## 安装配置

### hello_world

```shell
# 安装目录的sample路径下，启动服务端
[root@QingYun sample]# ./hello-world
flushed answer

# 启动客户端
[root@QingYun ~]# nc 127.0.0.1 9995
Hello, World!

```



## 常用函数

### 创建event

```cpp
struct event* event_new	(
    struct event_base *  base,         // the event base to which the event should be attached.
    evutil_socket_t      fd,           // the file descriptor or signal to be monitored, or -1.
    short                events,       // esired events to monitor: bitfield of EV_READ, EV_WRITE, EV_SIGNAL, EV_PERSIST, EV_ET.
    event_callback_fn    callback,     // callback function to be invoked when the event occurs
    void*                callback_arg  // an argument to be passed to the callback function
);
```



### 添加事件

```cpp
int event_add(struct event *ev, const struct timeval *tv);  //对已经初始化的事件调用 event_del()将使其成为非未决和非激活的。如果事件不是未决的或者激活的,调用将没有效果。
```



### 移除事件

```cpp
int event_del(struct event *ev);     //成功时函数返回 0,失败时返回-1。
```



### 释放event

```cpp
void event_free(struct event *event);   
```



### 创建event_base

```cpp
struct event_base* event_base_new(void);// a new event_base on success, or NULL on failure.
```



### 创建带定制化event_base

```cpp
struct event_config* event_config_new(void);

struct event_base* event_base_new_with_config(const struct event_config *cfg);

void event_config_free(struct event_config *cfg);
```



### 事件循环

```cpp
int event_base_loop(struct event_base *base, int flags);  // 成功返回0,失败返回-1,没有事件退出返回1
//flags
    #define EVLOOP_ONCE               0x01         // 事件只会被触发一次, 没有被触发, 阻塞等
    #define EVLOOP_NONBLOCK           0x02         // 非阻塞 等方式去做事件检测
    #define EVLOOP_NO_EXIT_ON_EMPTY   0x04         // 没有事件的时候, 也不退出轮询检测

// 简化版event_base_loop, 等同于没有设置标志的 event_base_loop()
int event_base_dispatch(struct event_base* base);  // 将一直运行,直到没有已经注册的事件了,或者调用了event_base_loopbreak()或者 event_base_loopexit()为止
```



### 停止循环

```cpp
int event_base_loopexit(struct event_base *base, const struct timeval *tv); // 指定时间停止循环
int event_base_loopbreak(struct event_base *base);                          // 指定时间停止循环

// 获取停止循环的原因
int event_base_got_exit(struct event_base *base);  // 因调用event_base_loopexit()停止循环返回true，否则false
int event_base_got_break(struct event_base *base); // 因调用event_base_loopexit()停止循环返回true，否则false
```



### 释放event_base

```cpp
void event_base_free(struct event_base *eb);
```

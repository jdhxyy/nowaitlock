# nowaitlock

## 介绍
C语言实现的基于Lamport面包店算法的无等待互斥锁。可用于没有提供锁的系统，比如无RTOS的单片机中。

## 开源
- [github上的项目地址](https://github.com/jdhxyy/nowaitlock)
- [gitee上的项目地址](https://gitee.com/jdhxyy/nowaitlock)

## API
```c
// NWLCreate 创建锁
// total: 使用此锁的最大用户数量
// 返回锁的句柄.如果是0表示创建失败
intptr_t NWLCreate(int total);

// NWLDelete 删除锁
void NWLDelete(intptr_t handle);

// NWLLock 上锁.如果返回false表示上锁失败.上锁成功后必须要解锁
bool NWLLock(intptr_t handle, int id);

// NWLUnlock 解锁.必须上锁成功后才能解锁
void NWLUnlock(intptr_t handle, int id);
```

## 测试用例
windows下创建若干线程，按顺序打印数据。

```c
#include <stdio.h>
#include <winsock2.h>
#include <process.h>
#include <windows.h>
#include <time.h>

#include "nowaitlock.h"

#pragma comment(lib,"ws2_32.lib")

#define MAX_THREAD_NUM 5

// 锁及保护的资源
static intptr_t lock;
// 顺序控制字
static int seq = 0;

static void case1(void);

static void case2(void);
static DWORD WINAPI thread(LPVOID lpParam);

int main() {
    case2();
    return 0;
}

static void case1(void) {
    intptr_t lock = NWLCreate(3);

    // 本轮应该0获取锁成功
    printf("---------->0\n");
    printf("0:%d\n", NWLLock(lock, 0));
    printf("1:%d\n", NWLLock(lock, 1));
    printf("2:%d\n", NWLLock(lock, 2));
    NWLUnlock(lock, 0);

    // 本轮应该0获取锁成功
    printf("---------->0\n");
    printf("0:%d\n", NWLLock(lock, 0));
    printf("1:%d\n", NWLLock(lock, 1));
    printf("2:%d\n", NWLLock(lock, 2));
    NWLUnlock(lock, 0);

    // 本轮应该1获取锁成功
    printf("---------->1\n");
    printf("1:%d\n", NWLLock(lock, 1));
    printf("0:%d\n", NWLLock(lock, 0));
    printf("2:%d\n", NWLLock(lock, 2));
    NWLUnlock(lock, 1);

    // 本轮应该2获取锁成功
    printf("---------->2\n");
    printf("2:%d\n", NWLLock(lock, 2));
    printf("0:%d\n", NWLLock(lock, 0));
    printf("1:%d\n", NWLLock(lock, 1));
    NWLUnlock(lock, 2);

    NWLDelete(lock);
}

static void case2(void) {
    lock = NWLCreate(MAX_THREAD_NUM);

    int a[MAX_THREAD_NUM] = {0};
    for (int i = 0; i < MAX_THREAD_NUM; i++) {
        a[i] = i;
        CreateThread(NULL, 0, thread, &a[i], 0, NULL);
    }
    while(1) {}
}

static DWORD WINAPI thread(LPVOID lpParam) {
    int id = *(int*)lpParam;

    printf("id:%d thread run\n", id);
    Sleep(1000);

    while (1) {
        if (NWLLock(lock, id) == false) {
            Sleep(0);
            continue;
        }

        printf("id:%d start\n", id);
        if (seq % MAX_THREAD_NUM == id) {
            seq++;
            printf("----->id:%d seq:%d\n", id, seq);
        }
        if (seq >= 20) {
            printf("id:%d exit\n", id);
            NWLUnlock(lock, id);
            break;
        }
        printf("id:%d stop\n", id);

        NWLUnlock(lock, id);
        Sleep(0);
    }
    return 0;
}
```

输出：
```text
id:0 thread run
id:1 thread run
id:2 thread run
id:3 thread run
id:4 thread run
id:4 start
id:4 stop
id:0 start
----->id:0 seq:1
id:0 stop
id:2 start
id:2 stop
id:0 start
id:0 stop
id:1 start
----->id:1 seq:2
id:1 stop
id:2 start
----->id:2 seq:3
id:2 stop
id:1 start
id:1 stop
id:3 start
----->id:3 seq:4
id:3 stop
id:2 start
id:2 stop
id:4 start
----->id:4 seq:5
id:4 stop
id:0 start
----->id:0 seq:6
id:0 stop
id:1 start
----->id:1 seq:7
id:1 stop
id:2 start
----->id:2 seq:8
id:2 stop
id:0 start
id:0 stop
id:3 start
----->id:3 seq:9
id:3 stop
id:2 start
id:2 stop
id:3 start
id:3 stop
id:1 start
id:1 stop
id:4 start
----->id:4 seq:10
id:4 stop
id:0 start
----->id:0 seq:11
id:0 stop
id:1 start
----->id:1 seq:12
id:1 stop
id:2 start
----->id:2 seq:13
id:2 stop
id:1 start
id:1 stop
id:2 start
id:2 stop
id:1 start
id:1 stop
id:2 start
id:2 stop
id:4 start
id:4 stop
id:0 start
id:0 stop
id:4 start
id:4 stop
id:2 start
id:2 stop
id:3 start
----->id:3 seq:14
id:3 stop
id:4 start
----->id:4 seq:15
id:4 stop
id:2 start
id:2 stop
id:2 start
id:2 stop
id:4 start
id:4 stop
id:1 start
id:1 stop
id:1 start
id:1 stop
id:2 start
id:2 stop
id:3 start
id:3 stop
id:2 start
id:2 stop
id:3 start
id:3 stop
id:1 start
id:1 stop
id:3 start
id:3 stop
id:3 start
id:3 stop
id:3 start
id:3 stop
id:4 start
id:4 stop
id:1 start
id:1 stop
id:2 start
id:2 stop
id:2 start
id:2 stop
id:4 start
id:4 stop
id:0 start
----->id:0 seq:16
id:0 stop
id:2 start
id:2 stop
id:3 start
id:3 stop
id:0 start
id:0 stop
id:1 start
----->id:1 seq:17
id:1 stop
id:0 start
id:0 stop
id:3 start
id:3 stop
id:2 start
----->id:2 seq:18
id:2 stop
id:0 start
id:0 stop
id:3 start
----->id:3 seq:19
id:3 stop
id:4 start
----->id:4 seq:20
id:4 exit
id:2 start
id:2 exit
id:1 start
id:1 exit
id:0 start
----->id:0 seq:21
id:0 exit
id:3 start
id:3 exit
```

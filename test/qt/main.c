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

// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// 无等待锁.应用Lamport面包店算法
// Authors: jdh99 <jdh821@163.com>

#include "nowaitlock.h"
#include <stdlib.h>
#include <string.h>

#pragma pack(1)

typedef struct {
    bool* entering;
    int* number;
    // 线程数量
    int total;
} tLock;

#pragma pack()

static int max(tLock* lock);

// NWLCreate 创建锁
// total: 使用此锁的最大用户数量
// 返回锁的句柄.如果是0表示创建失败
intptr_t NWLCreate(int total) {
    tLock* lock = (tLock*)malloc(sizeof(tLock));
    if (lock == NULL) {
        return 0;
    }
    memset(lock, 0, sizeof(tLock));

    lock->entering = malloc(sizeof(bool) * (uint64_t)total);
    if (lock->entering == NULL) {
        free(lock);
        return 0;
    }
    memset(lock->entering, 0, sizeof(bool) * (uint64_t)total);

    lock->number = malloc(sizeof(int) * (uint64_t)total);
    if (lock->number == NULL) {
        free(lock);
        return 0;
    }
    memset(lock->number, 0, sizeof(int) * (uint64_t)total);

    lock->total = total;
    return (intptr_t)lock;
}

// NWLDelete 删除锁
void NWLDelete(intptr_t handle) {
    if (handle == 0) {
        return;
    }
    tLock* lock = (tLock*)malloc(sizeof(tLock));
    free(lock->entering);
    free(lock->number);
    free(lock);
}

// NWLLock 上锁.如果返回false表示上锁失败.上锁成功后必须要解锁
bool NWLLock(intptr_t handle, int id) {
    tLock* lock = (tLock*)(handle);

    if (lock->number[id] == 0) {
        lock->entering[id] = true;
        lock->number[id] = 1 + max(lock);
        lock->entering[id] = false;
    }

    for (int j = 0; j < lock->total; j++) {
        if (lock->entering[j]) {
            lock->number[id] = 0;
            return false;
        }
        if (lock->number[j] != 0 &&
            (lock->number[j] < lock->number[id] || (lock->number[j] == lock->number[id] && j < id))) {
            lock->number[id] = 0;
            return false;
        }
    }
    return true;
}

static int max(tLock* lock) {
    int max = lock->number[0];
    for (int i = 1; i < lock->total; i++) {
        if (lock->number[i] > max) {
            max = lock->number[i];
        }
    }
    return max;
}

// NWLUnlock 解锁.必须上锁成功后才能解锁
void NWLUnlock(intptr_t handle, int id) {
    tLock* lock = (tLock*)(handle);
    lock->number[id] = 0;
}

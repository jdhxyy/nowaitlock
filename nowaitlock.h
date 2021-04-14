// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// 无等待锁.应用Lamport面包店算法
// Authors: jdh99 <jdh821@163.com>

#ifndef NOWAITLOCK_H
#define NOWAITLOCK_H

#include <stdint.h>
#include <stdbool.h>

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

#endif

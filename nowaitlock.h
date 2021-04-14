// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// �޵ȴ���.Ӧ��Lamport������㷨
// Authors: jdh99 <jdh821@163.com>

#ifndef NOWAITLOCK_H
#define NOWAITLOCK_H

#include <stdint.h>
#include <stdbool.h>

// NWLCreate ������
// total: ʹ�ô���������û�����
// �������ľ��.�����0��ʾ����ʧ��
intptr_t NWLCreate(int total);

// NWLDelete ɾ����
void NWLDelete(intptr_t handle);

// NWLLock ����.�������false��ʾ����ʧ��.�����ɹ������Ҫ����
bool NWLLock(intptr_t handle, int id);

// NWLUnlock ����.���������ɹ�����ܽ���
void NWLUnlock(intptr_t handle, int id);

#endif

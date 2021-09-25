#pragma once
#ifndef QUEUE_H
#define QUEUE_H
#include "List.h"

//FIFO
typedef struct Queue {
	int size;
	List head;
	List tail;
}Queue;

//��ʼ��
void init(Queue& q);

//��ӣ��ڶ���ĩβ����ֵΪval�Ľڵ�
void push(Queue& q, int val);

//������
bool pop(Queue& q);

//�ж��Ƿ�Ϊ��
bool empty(Queue& q);

//��ӡ����
void show(Queue& q);

//���ض���Ԫ��ֵ���������Ϊ�գ�����-1
int front(Queue& q);

//���ض�βԪ��ֵ���������Ϊ�գ�����-1
int back(Queue& q);

// �൱��һ��������������ն��У�Ҳ��Ҫ���head��
void clear(Queue&q);
#endif // !QUEUE_H

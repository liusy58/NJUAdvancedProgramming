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

//初始化
void init(Queue& q);

//入队，在队列末尾插入值为val的节点
void push(Queue& q, int val);

//出队列
bool pop(Queue& q);

//判断是否为空
bool empty(Queue& q);

//打印队列
void show(Queue& q);

//返回队首元素值，如果队列为空，返回-1
int front(Queue& q);

//返回队尾元素值，如果队列为空，返回-1
int back(Queue& q);

// 相当于一个析构函数。清空队列，也需要清空head。
void clear(Queue&q);
#endif // !QUEUE_H

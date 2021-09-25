#include "Queue.h"
#include <iostream>
void init(Queue& q)
{
	q.size = 0;
	q.head = q.tail = init();
}

void push(Queue& q, int val)
{
	List tmpNode = new Node;
	tmpNode->val = val;
	tmpNode->next = nullptr;
	q.tail->next = tmpNode;
	q.tail = q.tail->next;
	q.size++;
}

bool pop(Queue& q)
{
	if (q.size == 0)
		return false;
	List p = q.head->next;
	q.head->next = p->next;
	delete p;
	q.size--;
	if(q.size  == 0){
		q.tail = q.head;
	}
	return true;
}

bool empty(Queue& q)
{
	return q.size == 0;
}

void show(Queue& q)
{
	if(q.head == nullptr){
		return;
	}
	for (List p = q.head->next; p != nullptr; p = p->next) {
		std::cout << p->val << " ";
	}
	std::cout << std::endl;
}

int front(Queue& q)
{
	if (q.size == 0)return -1;
	return q.head->next->val;
}

int back(Queue& q)
{
	if (q.size == 0)return -1;
	return q.tail->val;
}

void clear(Queue&q){
	clear(q.head);
	q.head = nullptr;
	q.tail = nullptr;
	q.size = 0;
}

#include "List.h"
#include <iostream>
List init()
{
	List head = new Node;
	head->next = nullptr;
	return head;
}

bool insert(List& head, int i, int val)
{
	if (head == nullptr || i<0) return false;
	int index = 0;
	List tmpNode = new Node;
	tmpNode->val = val;
	tmpNode->next = nullptr;

	List pre = head;
	for (List cur = head -> next; cur != nullptr; cur = cur->next, pre = pre->next) {
		if (index == i) {
			tmpNode->next = pre->next;
			pre->next = tmpNode;
			return true;
		}
		index++;
	}
// handle the case when we add a new node to the tail
	if (index == i) {
		tmpNode->next = pre->next;
		pre->next = tmpNode;
		return true;
	}
// if error we need to delete the node
	delete tmpNode;
	return false;
}

bool erase(List& head, int i)
{
	if (head == nullptr || head->next == nullptr || i<0)return false;
	int index = 0;
	List pre = head;
	for (List cur = head->next; cur != nullptr; cur = cur->next, pre = pre->next) {
		if (index == i) {
			pre->next = cur->next;
			delete cur;
			return true;
		}
		index++;
	}
	return false;
}

void show(List& head)
{
	if(head == nullptr){
		return;
	}
	for (List p = head->next; p != nullptr; p = p->next) {
		std::cout << p->val << " ";
	}
	std::cout << std::endl;
}

void clear(List& head)
{
	if(head == nullptr){
		return;
	}
	List next = nullptr;
	for (List p = head; p != nullptr; ) {
		next = p->next;
		delete p;
		p = next;
	}
	head = nullptr;
}

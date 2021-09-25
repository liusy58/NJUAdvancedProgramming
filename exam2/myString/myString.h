#pragma once
#ifndef MYSTRING_H
#define MYSTRING_H
#include <string.h>
class myString {
private:
	//字符串数组
	char* str_;
	//已分配空间的大小
	int capacity_;
	//字符串大小，不包括'\0'
	int size_;
public:
	//默认构造函数
	//默认构造空间大小为8个字节
	myString();
	myString(const char* str);
	//拷贝构造函数
	myString(const myString& str);
	//析构函数
	~myString();
	//复制重载
	myString& operator=(myString& str);
	//字符加
	myString& operator+=(char ch);
	//字符串加
	myString& operator+=(const char* str);
	//myString加
	myString& operator+=(const myString& str);
	//比较运算符重载
	bool operator<(const myString& str);
	bool operator>(const myString& str);
	bool operator==(const myString& str);
	bool operator>=(const myString& str);
	bool operator<=(const myString& str);
	bool operator!=(const myString& str);

	char& operator[](const int index);
	char operator[](const int index)const;
	void append(char ch);
	void append(char* ch);
	void append(const myString& ch);
	//扩容,每次容量增加一倍
	void Resize();
	int size() const;
	int get_capacity() const;
	//打印字符串
	void printStr();
};


#endif // MYSTRING_H

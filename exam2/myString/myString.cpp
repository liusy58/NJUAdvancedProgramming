#include "myString.h"
#include <iostream>
myString::myString()
{
	capacity_ = 8;
	size_ = 0;
	str_ = new char[capacity_];
}
myString::myString(const char* str)
{
	capacity_ = 8;
	size_ = 0;
	str_ = new char[capacity_];
	while (capacity_ <= strlen(str) + 1) {
		Resize();
	}
	size_ = strlen(str);
	memcpy(str_, str, strlen(str) + 1);
}


myString::myString(const myString& str)
{
	capacity_ = 8;
	size_ = 0;
	str_ = new char[capacity_];
	while (capacity_ <= str.size_ + 1) {
		Resize();
	}
	size_ = str.size();
	memcpy(str_, str.str_, str.size_ + 1);
}

myString::~myString()
{
	if (str_) {
		delete str_;
		str_ = nullptr;
		size_ = 0;
		capacity_ = 0;
	}
}

myString& myString::operator=(myString& str)
{
	if(this == &str)return *this;
	while (capacity_ <= str.size_ + 1) {
		Resize();
	}
	size_ = str.size();
	memcpy(str_, str.str_, str.size_ + 1);
	return *this;
}

myString& myString::operator+=(char ch)
{
	while (capacity_ <= size_ + 1) {
		Resize();
	}
	str_[size_] = ch;
	str_[size_ + 1] = '\0';
	size_ += 1;
	return *this;
}

myString& myString::operator+=(const char* str)
{
	while (capacity_ <= size_ + strlen(str) + 1) {
		Resize();
	}
	for (int i = 0; i < strlen(str); i++) {
		str_[size_ + i] = str[i];
	}
	str_[size_ + strlen(str)] = '\0';
	size_ = size_ + strlen(str);
	return *this;
}

myString& myString::operator+=(const myString& str)
{
	while (capacity_ <= size_ + str.size_ + 1) {
		Resize();
	}
	for (int i = 0; i < str.size_; i++) {
		str_[size_ + i] = str.str_[i];
	}
	str_[size_ + str.size_] = '\0';
	size_ = size_ + str.size_;
	return *this;
}

bool myString::operator<(const myString& str)
{
	int i = 0;
	while(i<size_ && i<str.size()){
		if(str_[i]<str[i]){
			return true;
		}
		if(str_[i]>str[i]){
			return false;
		}
		++i;
	}
	return i == str.size() ? false:true;	
}

bool myString::operator==(const myString& str)
{
	if(str.size() != size()){
		return false;
	}
	int i = 0;
	while(i<size_){
		if(str_[i] != str[i]){
			return false;
		}
		++i;
	}
	return true;
}

//����capacity��С
void myString::Resize()
{
	capacity_ = capacity_ * 2;
	char* arr = new char[capacity_];
	memcpy(arr, str_, size_ + 1);
	delete[] str_;
	str_= arr;
}

int myString::size() const
{
	return size_;
}


int myString::get_capacity() const
{
	return capacity_;
}

void myString::printStr()
{
	for(int i=0;i<size_;++i){
		if(str_[i] != '\0')
			printf("%c",str_[i]);
	}
	printf("\n");
}

bool myString::operator>=(const myString& str){
	return !(*this < str);
}
bool myString::operator<=(const myString& str){
	return *this < str || *this == str;
}
bool myString::operator!=(const myString& str){
	return !(*this == str);
}
bool myString::operator>(const myString& str){
	return !(*this < str) && (*this != str);
}


char myString::operator[](const int index)const{
	return str_[index];
}
char& myString::operator[](const int index){
	return str_[index];
}

void myString::append(char ch){
	*this += ch;
}
void myString::append(char* ch){
	*this += ch;
}
void myString::append(const myString& ch){
	*this += ch;
}
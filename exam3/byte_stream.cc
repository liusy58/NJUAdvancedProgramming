#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}
#include <iostream>
using namespace std;

ByteStream::ByteStream(const size_t capacity):_capacity(capacity), _input_ended(0),_bytes_read(0),_bytes_write(0),_eof(0){}


// Write a string of bytes into the stream. 
//Write as many as will fit, and return the number of bytes written.
size_t ByteStream::write(const string &data) {
    auto len = min(data.size(),remaining_capacity());
    size_t i=0;
    for(auto c:data){
        if(i>=len){
            break;
        }
        _buffer.push_back(c);
        ++i;
    }
    _bytes_write += len;
    return len;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    auto nbytes = min(len,_buffer.size());
    return string(_buffer.begin(),_buffer.begin()+nbytes);
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    auto nbytes = min(len,_buffer.size());
    _bytes_read += nbytes;
    while(nbytes--){
        _buffer.pop_front();
    }
    if(_input_ended&&_buffer.empty()){
        _eof=1;
    }
 }

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string str=peek_output(len);
    pop_output(len);
    return str;
}

void ByteStream::end_input() {
    _input_ended = true;
    if(_buffer.empty()){
        _eof=1;
    }
}

bool ByteStream::input_ended() const { 
    return _input_ended;
 }

size_t ByteStream::buffer_size() const { 
    return _buffer.size();
 }

bool ByteStream::buffer_empty() const { 
    return _buffer.empty();    
 }

bool ByteStream::eof() const { 
    return _eof;
 }

size_t ByteStream::bytes_written() const { 
    return _bytes_write;
 }

size_t ByteStream::bytes_read() const { 
    return _bytes_read;
 }

size_t ByteStream::remaining_capacity() const { 
    return _capacity - _buffer.size();
 }

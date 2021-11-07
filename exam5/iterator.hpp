#include "hashmap.hpp"


template <typename K, typename M, typename H >
using value_type = std::pair<const K, M>;

template <typename K, typename M, typename H >
class HashMap<K,M,H>::iterator:public std::iterator<std::input_iterator_tag,value_type> {
    private:
        const HashMap*hashMap;
        bool is_end = true;
        int index = 0;
        node*curr_node= nullptr;
    public:
        iterator(const HashMap*mp,bool end):hashMap(mp),is_end(end){
            hashMap = mp;
            if(!is_end){
                while(index<hashMap->bucket_count()&&hashMap->_buckets_array[index]== nullptr)
                    ++index;
                if(index<hashMap->bucket_count()){
                    curr_node = hashMap->_buckets_array[index];
                }else{
                    is_end = true;
                }
            }
        }
        iterator(const iterator&it){
            hashMap = it.hashMap;
            index = it.index;
            is_end = it.is_end;
            curr_node = it.curr_node;
        }

        friend bool operator==(const iterator& lhs,const iterator& rhs){
            if(lhs.hashMap!=rhs.hashMap){
                return false;
            }
            return (lhs.is_end&rhs.is_end) || (lhs.curr_node == rhs.curr_node);
        }
        friend bool operator!=(const iterator& lhs,const iterator& rhs){
            return !(lhs == rhs);
        }
        value_type& operator*()const{
            if(curr_node== nullptr)
                throw "Error! ";
            return curr_node->value;
        }
        iterator&operator++(){
            if(curr_node== nullptr){
                is_end = true;
                return *this;
            }
            curr_node = curr_node->next;
            if(curr_node== nullptr){
                ++index;
                while(index<hashMap->bucket_count()&&hashMap->_buckets_array[index]== nullptr)
                    ++index;
                if(index<hashMap->bucket_count()){
                    curr_node = hashMap->_buckets_array[index];
                }else{
                    is_end=true;
                }
            }
            return *this;
        }
        iterator operator++(int){
            iterator copy(*this);
            operator++();
           return copy;
        }

        iterator&operator=(iterator&other){
            if(*this == other)
                return *this;
            this->curr_node = other.curr_node;
            this->is_end = other.is_end;
            this->index = other.index;
            this->hashMap = other.hashMap;
            return *this;
        }

        iterator&operator=(const iterator&other){
            if(*this == other)
                return *this;
            this->curr_node = other.curr_node;
            this->is_end = other.is_end;
            this->index = other.index;
            this->hashMap = other.hashMap;
            return *this;
        }
        value_type*operator->() const{
            if(curr_node == nullptr)
                throw "error!";
            return &(curr_node->value);
        }
        bool key_equal(const K&key){
            if(is_end||curr_node== nullptr)
                return false;
            return curr_node->value.first == key;
        }
    };


// template <typename K, typename M, typename H>
//  class HashMap<K,M,H>::const_iterator:public std::iterator<std::input_iterator_tag,value_type>{
//     private:
//         const HashMap*hashMap;
//         bool is_end = true;
//         int index = 0;
//         node*curr_node= nullptr;
//     public:
//         explicit const_iterator(const HashMap*mp,bool end):hashMap(mp),is_end(end){
//             hashMap = mp;
//             if(!is_end){
//                 while(index<hashMap->bucket_count()&&hashMap->_buckets_array[index]== nullptr)
//                     ++index;
//                 if(index<hashMap->bucket_count()){
//                     curr_node = hashMap->_buckets_array[index];
//                 }else{
//                     is_end = true;
//                 }
//             }
//         }
//         const_iterator(const_iterator&it){
//             hashMap = it.hashMap;
//             index = it.index;
//             is_end = it.is_end;
//             curr_node = it.curr_node;
//         }
//         friend bool operator==(const const_iterator& lhs,const const_iterator& rhs){
//             return (lhs.is_end&rhs.is_end) || (lhs.curr_node == rhs.curr_node);
//         }
//         friend bool operator!=(const const_iterator& lhs,const const_iterator& rhs){
//             return !(lhs == rhs);
//         }
//         bool key_equal(const K&key){
//             if(!is_end||curr_node== nullptr)
//                 return false;
//             return curr_node->value.first == key;
//         }

//         const value_type& operator*()const{
//             if(curr_node== nullptr)
//                 throw "Error! ";
//             return curr_node->value;
//         }
//         const_iterator&operator++(){
//             if(curr_node== nullptr){
//                 is_end = true;
//                 return *this;
//             }
//             curr_node = curr_node->next;
//             if(curr_node== nullptr){
//                 ++index;
//                 while(index<hashMap->bucket_count()&&hashMap->_buckets_array[index]== nullptr)
//                     ++index;
//                 if(index<hashMap->bucket_count()){
//                     curr_node = hashMap->_buckets_array[index];
//                 }else{
//                     is_end=true;
//                 }
//             }
//             return *this;
//         }
//         const_iterator operator++(int){
//             const_iterator copy(*this);
//             operator++();
//             return copy;
//         }

//         const_iterator&operator=(const_iterator&other){
//             if(*this == other)
//                 return *this;
//             this->curr_node = other.curr_node;
//             this->is_end = other.is_end;
//             this->index = other.index;
//             this->hashMap = other.hashMap;
//             return *this;
//         }

//         const value_type*operator->()const{
//             if(curr_node == nullptr)
//                 throw "error!";
//             return &(curr_node->value);
//         }
// };
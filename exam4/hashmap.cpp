#include "hashmap.hpp"

template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap() : HashMap(kDefaultBuckets) { }


template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(size_t bucket_count, const H& hash ) :
        _size(0),
        _hash_function(hash),
        _buckets_array(bucket_count, nullptr) { }



template <typename K, typename M, typename H>
HashMap<K, M, H>::~HashMap() {
    clear();
}

template <typename K, typename M, typename H>
inline size_t HashMap<K, M, H>::size() const noexcept {
    return _size;
}

template <typename K, typename M, typename H>
inline bool HashMap<K, M, H>::empty() const noexcept {
    return size() == 0;
}

template <typename K, typename M, typename H>
inline float HashMap<K, M, H>::load_factor() const noexcept {
    return static_cast<float>(size())/bucket_count();
};

template <typename K, typename M, typename H>
size_t HashMap<K, M, H>::bucket_count() const noexcept {
    return _buckets_array.size();
};

template <typename K, typename M, typename H>
bool HashMap<K, M, H>::contains(const K& key) const noexcept {
    return find_node(key).second != nullptr;
}

template <typename K, typename M, typename H>
void HashMap<K, M, H>::clear() noexcept {
    for (auto& curr : _buckets_array) {
        while (curr != nullptr) {
            auto trash = curr;
            curr = curr->next;
            delete trash;
        }
    }
    _size = 0;
}

template <typename K, typename M, typename H>
std::pair<typename HashMap<K, M, H>::value_type*, bool>
HashMap<K, M, H>::insert(const value_type& value) {
    const auto& [key, mapped] = value;
    auto [prev, node_to_edit] = find_node(key);
    size_t index = _hash_function(key) % bucket_count();

    if (node_to_edit != nullptr) return {&(node_to_edit->value), false};
    _buckets_array[index] = new node(value, _buckets_array[index]);

    ++_size;
    return {&(_buckets_array[index]->value), true};
}


template <typename K, typename M, typename H>
bool HashMap<K, M, H>::erase(const K& key) {
    auto [prev, node_to_erase] = find_node(key);
    if (node_to_erase == nullptr) {
        return false;
    } else {
        size_t index = _hash_function(key) % bucket_count();
        (prev ? prev->next : _buckets_array[index]) = node_to_erase->next;
        delete node_to_erase;
        --_size;
        return true;
    }
}

template <typename K, typename M, typename H>
M& HashMap<K, M, H>::at(const K& key)const{
    auto [prev, node_found] = find_node(key);
    if (node_found == nullptr) {
        throw std::out_of_range("HashMap<K, M, H>::at: key not found");
    }
    return node_found->value.second;
}

template <typename K, typename M, typename H>
void HashMap<K, M, H>::debug() const {
    std::cout << std::setw(30) << std::setfill('-') << '\n' << std::setfill(' ')
              << "Printing debug information for your HashMap implementation\n"
              << "Size: " << size() << std::setw(15) << std::right
              << "Buckets: " << bucket_count() << std::setw(20) << std::right
              << "(load factor: " << std::setprecision(2) << load_factor() << ") \n\n";

    for (size_t i = 0; i < bucket_count(); ++i) {
        std::cout << "[" << std::setw(3) << i << "]:";
        auto curr = _buckets_array[i];
        while (curr != nullptr) {
            const auto& [key, mapped] = curr->value;
            // next line will not compile if << not supported for K or M
            std::cout <<  " -> " << key << ":" << mapped;
            curr = curr->next;
        }
        std::cout <<  " /" <<  '\n';
    }
    std::cout << std::setw(30) << std::setfill('-') << '\n';
}



template <typename K, typename M, typename H>
typename HashMap<K, M, H>::node_pair HashMap<K, M, H>::find_node(const K& key) const {
    size_t index = _hash_function(key) % bucket_count();
    auto curr = _buckets_array[index];
    node* prev = nullptr; // if first node is the key, return {nullptr, front}
    while (curr != nullptr) {
        const auto& [found_key, found_mapped] = curr->value;
        if (found_key == key) return {prev, curr};
        prev = curr;
        curr = curr->next;
    }
    return {nullptr, nullptr}; // key not found at all.
}




template <typename K, typename M, typename H>
void HashMap<K, M, H>::rehash(size_t new_bucket_count) {
    if (new_bucket_count == 0) {
        throw std::out_of_range("HashMap<K, M, H>::rehash: new_bucket_count must be positive.");
    }

    std::vector<node*> new_buckets_array(new_bucket_count);
    /* Optional Milestone 1: begin student code */

    // Hint: you should NOT call insert, and you should not call
    // new or delete in this function. You must reuse existing nodes.
    (void) new_buckets_array; // remove this line after you start implementing\
    /* end student code */
    for (size_t i = 0; i < bucket_count(); ++i) {
        auto curr = _buckets_array[i];
        while (curr != nullptr) {
            auto node = curr;
            auto value = node->value;
            curr = curr->next;
            auto index = _hash_function(value.first)%new_bucket_count;
            node->next = new_buckets_array[index];
            new_buckets_array[index] = node;
        }
    }
    _buckets_array = new_buckets_array;
}


template <typename K, typename M, typename H>
M& HashMap<K, M, H>::operator[](const K& key){
    if(!contains(key)) {
        auto res = new value_type(key, M());
        insert(*res);
    }
    return at(key);
}

template <typename K, typename M, typename H>
std::ostream& operator<<(std::ostream& os, const HashMap<K, M, H>& map){
    os<<"{";
    std::string str = "";
    for (size_t i = 0; i < map.bucket_count(); ++i) {
        auto curr = map._buckets_array[i];
        while (curr != nullptr) {
            auto node = curr;
            auto value = node->value;
            os<<str<<value.first<<":"<<value.second;
            str = ", ";
            curr = curr->next;
        }
    }
    os<<"}";
    return os;
}

template <typename K, typename M, typename H>
bool operator==(const HashMap<K, M, H>& lhs,
                const HashMap<K, M, H>& rhs){
    if(lhs.size()!=rhs.size())
        return false;
    for (size_t i = 0; i < lhs.bucket_count(); ++i) {
        auto curr = lhs._buckets_array[i];
        while (curr != nullptr) {
            auto node = curr;
            auto value = node->value;
            if(!rhs.contains(value.first)||(value.second != rhs.at(value.first))){
                return false;
            }
            curr = curr->next;
        }
    }
    return true;
}

template <typename K, typename M, typename H>
bool operator!=(const HashMap<K, M, H>& lhs,
                const HashMap<K, M, H>& rhs){
    return !(lhs == rhs);
}

template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(HashMap const &other){
    this->_hash_function = other._hash_function;
    this->_buckets_array = std::vector<node*>(other.bucket_count(), nullptr);
    this->_size = 0;
    for (size_t i = 0; i < other.bucket_count(); ++i) {
        auto curr = other._buckets_array[i];
        while (curr != nullptr) {
            auto value = curr->value;
            auto node = std::make_pair(value.first,value.second);
            insert(node);
            curr = curr->next;
        }
    }
}




template<typename K, typename M, typename H>
HashMap<K,M,H> &HashMap<K, M, H>::operator=(const HashMap &other) {
    if(*this == other) return *this;
    this->_hash_function = other._hash_function;
    this->_size = 0;
    this->_buckets_array = std::vector<node*>(other.bucket_count(), nullptr);
    for (size_t i = 0; i < other.bucket_count(); ++i) {
        auto curr = other._buckets_array[i];
        while (curr != nullptr) {
            auto value = curr->value;
            auto node = std::make_pair(value.first,value.second);
            insert(node);
            curr = curr->next;
        }
    }
    return *this;
}




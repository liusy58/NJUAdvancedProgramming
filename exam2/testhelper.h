#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <functional>
#include <memory>
#include <map>
#include <cstring>

const int MAGIC_BUFFER_SIZE = 1000;

namespace test_helper {
  bool alloc_trace_enabled = false;
  int alloc_mem = 0;
  int alloc_times = 0;
  std::map<void*, std::size_t> alloc_size;

  void init() {
    std::srand(std::time(NULL));
  }

  void set_alloc_size(void *ptr, std::size_t sz) {
    alloc_trace_enabled = false;
    alloc_size[ptr] = sz;
    alloc_trace_enabled = true;
  }

  std::size_t get_alloc_size(void *ptr) {
    alloc_trace_enabled = false;
    std::size_t ret;
    if (alloc_size.find(ptr) == alloc_size.end()) {
      // std::printf("*** Detected invalid memory free for address %p ***\n", ptr);
      assert(false && "Invalid memory free");
    }
    ret = alloc_size[ptr];
    alloc_trace_enabled = true;
    return ret;
  }

  std::unique_ptr<char[]> build_magic_string() {
    auto buffer = std::unique_ptr<char[]>(new char[MAGIC_BUFFER_SIZE * 2 + 1]);
    int len = std::rand() % MAGIC_BUFFER_SIZE + MAGIC_BUFFER_SIZE;
    for (int i = 0; i < len; ++i) {
      buffer[i] = (std::rand() % 26) + 'a';
    }
    buffer[len] = '\0';
    return buffer;
  }

  std::unique_ptr<char[]> copy_string(const std::unique_ptr<char[]> &src) {
    auto dest = std::unique_ptr<char[]>(new char[MAGIC_BUFFER_SIZE * 2 + 1]);
    strcpy(dest.get(), src.get());
    return dest;
  }

  bool test_has_errors;

  bool run_test(const std::function<void(void)> func) {
    static int case_counter = 1;
    // printf("Case %d:\n", case_counter++);
    test_has_errors = false;
    alloc_mem = 0;
    alloc_times = 0;
    func();
    if (test_has_errors) {
      printf("FAIL!\n");
    } else {
      printf("Pass.\n");
    }
    // printf("%d\n",test_has_errors);
    return test_has_errors;
  }

  bool test_assert(int line_no, bool stat, const char * err_message) {
    if (!stat) {
      printf("Assertion failed (LINE %d): %s\n", line_no, err_message);
      test_has_errors = true;
    }
    return stat;
  }

}

void* operator new(std::size_t sz) {
  void *ptr = std::malloc(sz);
  if (test_helper::alloc_trace_enabled) {
    test_helper::alloc_mem += static_cast<int>(sz);
    test_helper::alloc_times++;
    test_helper::set_alloc_size(ptr, sz);
  }
  return ptr;
}

void operator delete(void *ptr) noexcept {
  
  if (test_helper::alloc_trace_enabled) {
    std::size_t sz = test_helper::get_alloc_size(ptr);
    test_helper::alloc_mem -= static_cast<int>(sz);
  }
  std::free(ptr);
}
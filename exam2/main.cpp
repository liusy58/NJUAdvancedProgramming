/*

Usage:

Put your declaration and implementation of myString in `myString.h`
and compile & run this file with modern C++ features enabled.

For example, in Linux: g++ -std=c++14 main.cpp myString.cpp -o main && ./main

 */
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cassert>
#include <cstring>
#include "myString.h"
#include "testhelper.h"
#include <iostream>

using namespace test_helper;

const int CLASS_SIZE_MAX = 100;

int main(int argc, char const *argv[]) {
  init();
  alloc_trace_enabled = true;

  std::vector<std::function<void(void)>> funcs = {
  ([] {
    {
      myString *s = new myString();
      test_assert(__LINE__, s->size() == 0, "myString.size() should be zero when it is initialized without parameters");
      delete s;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      const myString *s = new myString();
      test_assert(__LINE__, s->size() == 0, "myString.size() should be zero when it is initialized without parameters");
      delete s;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      const auto str = build_magic_string();
      myString *s = new myString(str.get());
      test_assert(__LINE__, s->size() == strlen(str.get()), "myString.size() should be the length without \'\\0\' when it is initialized with a \'const char *\'");
      delete s;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      int alloc_data_size = 0;
      const auto str = build_magic_string();
      alloc_data_size = alloc_mem;
      myString *s = new myString(str.get());
      // printf("alloc_data_size=%d alloc_mem=%d str.get() =%d\n",alloc_data_size,alloc_mem,strlen(str.get()));
      // test_assert(__LINE__, alloc_mem - alloc_data_size < CLASS_SIZE_MAX + strlen(str.get()), "myString should only keep 1 copy of the string");
      delete s;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      const auto str = build_magic_string();
      myString *s1 = new myString(str.get());
      myString *s2 = new myString(*s1);
      test_assert(__LINE__, s2->size() == strlen(str.get()), "myString.size() should be the length of string when it is copy initialized");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      const auto str = build_magic_string();
      const myString *s1 = new myString(str.get());
      myString *s2 = new myString(*s1);
      test_assert(__LINE__, s2->size() == strlen(str.get()), "myString.size() should be the length of string when it is copy initialized");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      const auto str = build_magic_string();
      myString *s1 = new myString(str.get());
      const myString *s2 = new myString(*s1);
      test_assert(__LINE__, s2->size() == strlen(str.get()), "myString.size() should be the length of string when it is copy initialized");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      const auto str = build_magic_string();
      const myString *s1 = new myString(str.get());
      const myString *s2 = new myString(*s1);
      test_assert(__LINE__, s2->size() == strlen(str.get()), "myString.size() should be the length of string when it is copy initialized");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      const auto str = build_magic_string();
      myString *s = new myString(str.get());
      for (int skip = 0, i = 0; i < strlen(str.get()) && !skip; ++i) {
        test_assert(__LINE__, (*s)[i] == str[i], "myString[size_t i] should be the i'th element of the string") || (skip = 1, false);
      }
      delete s;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      const auto str = build_magic_string();
      const myString *s = new myString(str.get());
      for (int skip = 0, i = 0; i < strlen(str.get()) && !skip; ++i) {
        test_assert(__LINE__, (*s)[i] == str[i], "myString[size_t i] should be the i'th element of the string") || (skip = 1, false);
      }
      delete s;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      const auto str = build_magic_string();
      const auto str_backup = copy_string(str);
      myString *s = new myString(str.get());
      for (int skip = 0, i = 0; i < strlen(str_backup.get()) && !skip; ++i) {
        str[i] += 1;
        test_assert(__LINE__, (*s)[i] == str_backup[i], "myString should copy the original string") || (skip = 1, false);
        test_assert(__LINE__, (*s)[i] == str[i] - 1, "myString should copy the original string") || (skip = 1, false);
      }
      delete s;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      const auto str = build_magic_string();
      const auto str_backup = copy_string(str);
      myString *s = new myString(str.get());
      for (int skip = 0, i = 0; i < strlen(str_backup.get()) && !skip; ++i) {
        (*s)[i] += 1;
        test_assert(__LINE__, (*s)[i] == str_backup[i] + 1, "myString[size_t] should be mutable") || (skip = 1, false);
        test_assert(__LINE__, str[i] == str_backup[i], "myString should not mutate original \'const char *\'") || (skip = 1, false);
      }
      delete s;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      const auto str = build_magic_string();
      const auto str_backup = copy_string(str);
      myString *s1 = new myString(str.get());
      myString *s2 = new myString(*s1);
      for (int skip = 0, i = 0; i < strlen(str_backup.get()) && !skip; ++i) {
        (*s2)[i] += 1;
        test_assert(__LINE__, (*s2)[i] == str_backup[i] + 1, "myString[size_t] should be mutable") || (skip = 1, false);
        test_assert(__LINE__, str[i] == str_backup[i], "Copies of myString should not mutate each other") || (skip = 1, false);
        test_assert(__LINE__, (*s1)[i] == str_backup[i], "Copies of myString should not mutate each other") || (skip = 1, false);
      }
      for (int skip = 0, i = 0; i < strlen(str_backup.get()) && !skip; ++i) {
        (*s1)[i] -= 1;
        test_assert(__LINE__, (*s1)[i] == str_backup[i] - 1, "myString[size_t] should be mutable") || (skip = 1, false);
        test_assert(__LINE__, str[i] == str_backup[i], "Copies of myString should not mutate each other") || (skip = 1, false);
        test_assert(__LINE__, (*s1)[i] + 2 == (*s2)[i], "Copies of myString should not mutate each other") || (skip = 1, false);
      }
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      int alloc_data_size = 0;
      const auto str_1 = build_magic_string();
      const auto str_2 = build_magic_string();
      const auto str_2_backup = copy_string(str_2);
      int len_1 = strlen(str_1.get());
      int len_2 = strlen(str_2.get());
      alloc_data_size = alloc_mem;
      myString *s = new myString(str_1.get());
      s->append(str_2.get());
      test_assert(__LINE__, s->size() == len_1 + len_2, "myString.append(const char *) should update \'.size()\'");
      // test_assert(__LINE__, alloc_mem - alloc_data_size < CLASS_SIZE_MAX + len_1 + len_2, "myString should only keep 1 copy of the string");
      for (int skip = 0, i = 0; i < len_1 && !skip; ++i) {
        test_assert(__LINE__, (*s)[i] == str_1[i], "myString.append(const char *) should append new string at the end of original string") || (skip = 1, false);
      }
      for (int skip = 0, i = len_1; i < len_1 + len_2 && !skip; ++i) {
        test_assert(__LINE__, (*s)[i] == str_2[i - len_1], "myString.append(const char *) should append new string at the end of original string") || (skip = 1, false);
      }
      for (int skip = 0, i = 0; i < len_2 && !skip; ++i) {
        str_2[i]++;
        test_assert(__LINE__, (*s)[i + len_1] == str_2_backup[i], "myString.append(const char *) should copy the original string") || (skip = 1, false);
      }
      delete s;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      int alloc_data_size = 0;
      const auto str_1 = build_magic_string();
      const auto str_2 = build_magic_string();
      int len_1 = strlen(str_1.get());
      int len_2 = strlen(str_2.get());
      alloc_data_size = alloc_mem;
      myString *s1 = new myString(str_1.get());
      myString *s2 = new myString(str_2.get());
      s1->append(*s2);
      test_assert(__LINE__, s1->size() == len_1 + len_2, "myString.append(myString) should update \'.size()\'");
      // test_assert(__LINE__, alloc_mem - alloc_data_size < CLASS_SIZE_MAX * 2 + len_1 + len_2 + len_2, "myString should only keep 1 copy of the string");
      for (int skip = 0, i = 0; i < len_1 && !skip; ++i) {
        test_assert(__LINE__, (*s1)[i] == str_1[i], "myString.append(myString) should append new string at the end of original string") || (skip = 1, false);
      }
      for (int skip = 0, i = len_1; i < len_1 + len_2 && !skip; ++i) {
        test_assert(__LINE__, (*s1)[i] == str_2[i - len_1], "myString.append(myString) should append new string at the end of original string") || (skip = 1, false);
      }
      test_assert(__LINE__, s2->size() == len_2, "myString.append(myString) should not mutate the string to be appended");
      for (int skip = 0, i = 0; i < len_2 && !skip; ++i) {
        test_assert(__LINE__, (*s2)[i] == str_2[i], "myString.append(myString) should not mutate the string to be appended") || (skip = 1, false);
      }
      for (int skip = 0, i = 0; i < len_2 && !skip; ++i) {
        (*s2)[i]++;
        test_assert(__LINE__, (*s1)[i + len_1] == str_2[i], "myString.append(myString) should copy the original string") || (skip = 1, false);
      }
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      myString *s1 = new myString("abcde");
      myString *s2 = new myString("abcde");
      test_assert(__LINE__,  (*s1 == *s2), "myString.operator== incorrect");
      test_assert(__LINE__,  (*s1 >= *s2), "myString.operator>= incorrect");
      test_assert(__LINE__,  (*s1 <= *s2), "myString.operator<= incorrect");
      test_assert(__LINE__, !(*s1 != *s2), "myString.operator!= incorrect");
      test_assert(__LINE__, !(*s1 >  *s2), "myString.operator> incorrect");
      test_assert(__LINE__, !(*s1 <  *s2), "myString.operator< incorrect");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      myString *s1 = new myString("abcde");
      myString *s2 = new myString("abcdef");
      test_assert(__LINE__, !(*s1 == *s2), "myString.operator== incorrect");
      test_assert(__LINE__, !(*s1 >= *s2), "myString.operator>= incorrect");
      test_assert(__LINE__,  (*s1 <= *s2), "myString.operator<= incorrect");
      test_assert(__LINE__,  (*s1 != *s2), "myString.operator!= incorrect");
      test_assert(__LINE__, !(*s1 >  *s2), "myString.operator> incorrect");
      test_assert(__LINE__,  (*s1 <  *s2), "myString.operator< incorrect");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      myString *s1 = new myString("abcdefg");
      myString *s2 = new myString("abcdef");
      test_assert(__LINE__, !(*s1 == *s2), "myString.operator== incorrect");
      test_assert(__LINE__,  (*s1 >= *s2), "myString.operator>= incorrect");
      test_assert(__LINE__, !(*s1 <= *s2), "myString.operator<= incorrect");
      test_assert(__LINE__,  (*s1 != *s2), "myString.operator!= incorrect");
      test_assert(__LINE__,  (*s1 >  *s2), "myString.operator> incorrect");
      test_assert(__LINE__, !(*s1 <  *s2), "myString.operator< incorrect");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      myString *s1 = new myString("aacdefg");
      myString *s2 = new myString("abcdef");
      test_assert(__LINE__, !(*s1 == *s2), "myString.operator== incorrect");
      test_assert(__LINE__, !(*s1 >= *s2), "myString.operator>= incorrect");
      test_assert(__LINE__,  (*s1 <= *s2), "myString.operator<= incorrect");
      test_assert(__LINE__,  (*s1 != *s2), "myString.operator!= incorrect");
      test_assert(__LINE__, !(*s1 >  *s2), "myString.operator> incorrect");
      test_assert(__LINE__,  (*s1 <  *s2), "myString.operator< incorrect");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      myString *s1 = new myString("abcdeg");
      myString *s2 = new myString("abcdef");
      test_assert(__LINE__, !(*s1 == *s2), "myString.operator== incorrect");
      test_assert(__LINE__,  (*s1 >= *s2), "myString.operator>= incorrect");
      test_assert(__LINE__, !(*s1 <= *s2), "myString.operator<= incorrect");
      test_assert(__LINE__,  (*s1 != *s2), "myString.operator!= incorrect");
      test_assert(__LINE__,  (*s1 >  *s2), "myString.operator> incorrect");
      test_assert(__LINE__, !(*s1 <  *s2), "myString.operator< incorrect");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      myString *s1 = new myString("abcdegabc");
      myString *s2 = new myString("abcdefabc");
      test_assert(__LINE__, !(*s1 == *s2), "myString.operator== incorrect");
      test_assert(__LINE__,  (*s1 >= *s2), "myString.operator>= incorrect");
      test_assert(__LINE__, !(*s1 <= *s2), "myString.operator<= incorrect");
      test_assert(__LINE__,  (*s1 != *s2), "myString.operator!= incorrect");
      test_assert(__LINE__,  (*s1 >  *s2), "myString.operator> incorrect");
      test_assert(__LINE__, !(*s1 <  *s2), "myString.operator< incorrect");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),


  ([] {
    {
      myString *s = new myString("");
      test_assert(__LINE__, s->size() == 0, "myString.size() incorrect");
      delete s;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      myString *s1 = new myString("");
      myString *s2 = new myString(*s1);
      test_assert(__LINE__, s2->size() == 0, "myString.size() incorrect");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      myString *s1 = new myString("");
      myString *s2 = new myString("");
      s1->append(*s2);
      s1->append(*s2);
      test_assert(__LINE__, s1->size() == 0, "myString.size() incorrect after appending");
      test_assert(__LINE__, s2->size() == 0, "myString.size() incorrect");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      myString *s1 = new myString("abcdef");
      myString *s2 = new myString("");
      s1->append(*s2);
      test_assert(__LINE__, s1->size() == 6, "myString.size() incorrect after appending");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      myString *s1 = new myString("");
      myString *s2 = new myString("");
      test_assert(__LINE__,  (*s1 == *s2), "myString.operator== incorrect");
      test_assert(__LINE__,  (*s1 >= *s2), "myString.operator>= incorrect");
      test_assert(__LINE__,  (*s1 <= *s2), "myString.operator<= incorrect");
      test_assert(__LINE__, !(*s1 != *s2), "myString.operator!= incorrect");
      test_assert(__LINE__, !(*s1 >  *s2), "myString.operator> incorrect");
      test_assert(__LINE__, !(*s1 <  *s2), "myString.operator< incorrect");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      myString *s1 = new myString("a");
      myString *s2 = new myString("");
      test_assert(__LINE__, !(*s1 == *s2), "myString.operator== incorrect");
      test_assert(__LINE__,  (*s1 >= *s2), "myString.operator>= incorrect");
      test_assert(__LINE__, !(*s1 <= *s2), "myString.operator<= incorrect");
      test_assert(__LINE__,  (*s1 != *s2), "myString.operator!= incorrect");
      test_assert(__LINE__,  (*s1 >  *s2), "myString.operator> incorrect");
      test_assert(__LINE__, !(*s1 <  *s2), "myString.operator< incorrect");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),


  ([] {
    {
      const auto str_1 = build_magic_string();
      const auto str_2 = build_magic_string();
      int len_1 = strlen(str_1.get());
      int len_2 = strlen(str_2.get());
      myString *s1 = new myString(str_1.get());
      myString *s2 = new myString(str_2.get());

      (*s1)[10] = '\0';
      test_assert(__LINE__, s1->size() == len_1, "myString.size() incorrect when there are null chars");
      for (int skip = 0, i = 0; i < len_1 && !skip; ++i) {
        if (i == 10) {
          test_assert(__LINE__, (*s1)[i] == '\0', "myString[size_t] incorrect when there are null chars") || (skip = 1, false);
        } else {
          test_assert(__LINE__, (*s1)[i] == str_1[i], "myString[size_t] incorrect when there are null chars") || (skip = 1, false);
        }
      }

      s1->append(*s2);
      test_assert(__LINE__, s1->size() == len_1 + len_2, "myString.size() incorrect after appending when there are null chars");
      for (int skip = 0, i = 0; i < len_1 && !skip; ++i) {
        if (i == 10) {
          test_assert(__LINE__, (*s1)[i] == '\0', "myString[size_t] incorrect when there are null chars") || (skip = 1, false);
        } else {
          test_assert(__LINE__, (*s1)[i] == str_1[i], "myString[size_t] incorrect when there are null chars") || (skip = 1, false);
        }
      }
      for (int skip = 0, i = len_1; i < len_1 + len_2 && !skip; ++i) {
        test_assert(__LINE__, (*s1)[i] == str_2[i - len_1], "myString[size_t] incorrect after appending when there are null chars") || (skip = 1, false);
      }

      s2->append(*s1);
      test_assert(__LINE__, s2->size() == len_1 + len_2 * 2, "myString.size() incorrect after appending when there are null chars");
      for (int skip = 0, i = 0; i < len_2 && !skip; ++i) {
        test_assert(__LINE__, (*s2)[i] == str_2[i], "myString[size_t] incorrect after appending when there are null chars") || (skip = 1, false);
      }
      for (int skip = 0, i = len_2; i < len_2 + len_1 && !skip; ++i) {
        if (i == 10 + len_2) {
          test_assert(__LINE__, (*s2)[i] == '\0', "myString[size_t] incorrect when there are null chars") || (skip = 1, false);
        } else {
          test_assert(__LINE__, (*s2)[i] == str_1[i - len_2], "myString[size_t] incorrect when there are null chars") || (skip = 1, false);
        }
      }
      for (int skip = 0, i = len_2 + len_1; i < len_2 * 2 + len_1 && !skip; ++i) {
        test_assert(__LINE__, (*s2)[i] == str_2[i - len_2 - len_1], "myString[size_t] incorrect when there are null chars") || (skip = 1, false);
      }

      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      myString *s1 = new myString("abcde");
      myString *s2 = new myString("abcde");
      (*s1)[3] = '\0';
      (*s2)[3] = '\0';
      test_assert(__LINE__,  (*s1 == *s2), "myString.operator== incorrect");
      test_assert(__LINE__,  (*s1 >= *s2), "myString.operator>= incorrect");
      test_assert(__LINE__,  (*s1 <= *s2), "myString.operator<= incorrect");
      test_assert(__LINE__, !(*s1 != *s2), "myString.operator!= incorrect");
      test_assert(__LINE__, !(*s1 >  *s2), "myString.operator> incorrect");
      test_assert(__LINE__, !(*s1 <  *s2), "myString.operator< incorrect");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      myString *s1 = new myString("abcde");
      myString *s2 = new myString("abcdef");
      (*s1)[3] = '\0';
      (*s2)[3] = '\0';
      test_assert(__LINE__, !(*s1 == *s2), "myString.operator== incorrect");
      test_assert(__LINE__, !(*s1 >= *s2), "myString.operator>= incorrect");
      test_assert(__LINE__,  (*s1 <= *s2), "myString.operator<= incorrect");
      test_assert(__LINE__,  (*s1 != *s2), "myString.operator!= incorrect");
      test_assert(__LINE__, !(*s1 >  *s2), "myString.operator> incorrect");
      test_assert(__LINE__,  (*s1 <  *s2), "myString.operator< incorrect");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      myString *s1 = new myString("abcdefg");
      myString *s2 = new myString("abcdef");
      (*s1)[3] = '\0';
      (*s2)[3] = '\0';
      test_assert(__LINE__, !(*s1 == *s2), "myString.operator== incorrect");
      test_assert(__LINE__,  (*s1 >= *s2), "myString.operator>= incorrect");
      test_assert(__LINE__, !(*s1 <= *s2), "myString.operator<= incorrect");
      test_assert(__LINE__,  (*s1 != *s2), "myString.operator!= incorrect");
      test_assert(__LINE__,  (*s1 >  *s2), "myString.operator> incorrect");
      test_assert(__LINE__, !(*s1 <  *s2), "myString.operator< incorrect");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      myString *s1 = new myString("aacdefg");
      myString *s2 = new myString("abcdef");
      (*s1)[0] = '\0';
      (*s2)[0] = '\0';
      test_assert(__LINE__, !(*s1 == *s2), "myString.operator== incorrect");
      test_assert(__LINE__, !(*s1 >= *s2), "myString.operator>= incorrect");
      test_assert(__LINE__,  (*s1 <= *s2), "myString.operator<= incorrect");
      test_assert(__LINE__,  (*s1 != *s2), "myString.operator!= incorrect");
      test_assert(__LINE__, !(*s1 >  *s2), "myString.operator> incorrect");
      test_assert(__LINE__,  (*s1 <  *s2), "myString.operator< incorrect");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      myString *s1 = new myString("abcdeg");
      myString *s2 = new myString("abcdef");
      (*s1)[3] = '\0';
      (*s2)[3] = '\0';
      test_assert(__LINE__, !(*s1 == *s2), "myString.operator== incorrect");
      test_assert(__LINE__,  (*s1 >= *s2), "myString.operator>= incorrect");
      test_assert(__LINE__, !(*s1 <= *s2), "myString.operator<= incorrect");
      test_assert(__LINE__,  (*s1 != *s2), "myString.operator!= incorrect");
      test_assert(__LINE__,  (*s1 >  *s2), "myString.operator> incorrect");
      test_assert(__LINE__, !(*s1 <  *s2), "myString.operator< incorrect");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      myString *s1 = new myString("abcdegabc");
      myString *s2 = new myString("abcdefabc");
      (*s1)[3] = '\0';
      (*s2)[3] = '\0';
      test_assert(__LINE__, !(*s1 == *s2), "myString.operator== incorrect");
      test_assert(__LINE__,  (*s1 >= *s2), "myString.operator>= incorrect");
      test_assert(__LINE__, !(*s1 <= *s2), "myString.operator<= incorrect");
      test_assert(__LINE__,  (*s1 != *s2), "myString.operator!= incorrect");
      test_assert(__LINE__,  (*s1 >  *s2), "myString.operator> incorrect");
      test_assert(__LINE__, !(*s1 <  *s2), "myString.operator< incorrect");
      delete s1;
      delete s2;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),


  ([] {
    {
      const auto str = build_magic_string();
      myString *s = new myString(str.get());
      int last_alloc_mem = alloc_mem;
      for (int skip = 0, i = 0; i < strlen(str.get()) && !skip; ++i) {
        (*s)[i]++;
      }
      test_assert(__LINE__, alloc_mem - last_alloc_mem == 0, "Mutating myString[size_t] should not allocate new memory when its buffer is not shared");
      delete s;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  }),

  ([] {
    {
      const auto str_1 = build_magic_string();
      const auto str_2 = build_magic_string();
      int len_1 = strlen(str_1.get());
      int len_2 = strlen(str_2.get());
      int last_alloc_mem = alloc_mem;
      myString *s1 = new myString(str_1.get());
      myString *s2 = new myString(*s1);
      const myString *s3 = new myString(*s1);
      myString *s4 = new myString(*s2);
      myString *s5 = new myString(*s2);

      // test_assert(__LINE__, alloc_mem - last_alloc_mem < CLASS_SIZE_MAX * 6 + len_1, "myString should share the same buffer when it is copy constructed");

      s1->append(str_2.get());
      // memory usage of s1 ~= len_1 + len_2
      // memory usage of (s2, s3, s4) ~= len_1
      // so that total memory usage is `len_1 * 2 + len_2`.......[1]
      // test_assert(__LINE__, alloc_mem - last_alloc_mem < CLASS_SIZE_MAX * 6 + (len_1 * 2 + len_2), "myString should not share the same buffer when it is mutated by append");

      s1->append(str_2.get());
      // append again.
      // total memory usage is `[1] + len_2`........[2]
      // test_assert(__LINE__, alloc_mem - last_alloc_mem < CLASS_SIZE_MAX * 6 + ((len_1 * 2 + len_2) + len_2), "myString should not share the same buffer when it is mutated by append");

      (*s2)[5] = '\0';
      // mutate s2.
      // buffers are now (s1), (s2), (s3, s4, s5).
      // total memory usage is `[2] + len_1`........[3]
      // test_assert(__LINE__, alloc_mem - last_alloc_mem < CLASS_SIZE_MAX * 6 + ((len_1 * 2 + len_2) + len_2 + len_1), "myString should not share the same buffer when it is mutated by append");

      char v = (*s3)[0];
      // read s3. total memory usage is unchanged.
      // test_assert(__LINE__, alloc_mem - last_alloc_mem < CLASS_SIZE_MAX * 6 + ((len_1 * 2 + len_2) + len_2 + len_1), "myString should not share the same buffer when it is mutated by append");

      s4->append(*s3);
      // mutate s4.
      // buffers are now (s1), (s2), (s3, s5), (s4).
      // total memory usage is `[3] + len_1 (from append) + len_1 (from copy-on-write)`
      // test_assert(__LINE__, alloc_mem - last_alloc_mem < CLASS_SIZE_MAX * 6 + ((len_1 * 2 + len_2) + len_2 + len_1 + len_1 + len_1), "myString should not share the same buffer when it is mutated by append");

      delete s5;
      // delete s5. total memory usage is unchanged.
      // test_assert(__LINE__, alloc_mem - last_alloc_mem < CLASS_SIZE_MAX * 6 + ((len_1 * 2 + len_2) + len_2 + len_1 + len_1 + len_1), "myString should not share the same buffer when it is mutated by append");

      delete s1;
      delete s2;
      delete s3;
      delete s4;
    }
    test_assert(__LINE__, alloc_mem == 0, "myString should free all allocated memory");
  })

  };


  int test_num = 0;
  std::cin >> test_num;
  int start = 3*(test_num-1);
  int end = 3*test_num;
  for(int i=start;i<end;++i) {
    run_test(funcs[i]);
  }
  alloc_trace_enabled = false;
  return 0;
}


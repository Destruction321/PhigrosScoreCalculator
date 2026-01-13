#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct dynamic_array {
    void* data;
    size_t data_size;
    size_t num;
    size_t capacity;
} Array;

extern Array* volatile array;

typedef void (*Alloc_Error)(const char* , ...);

extern Alloc_Error alloc_error; ///< 用于注册错误函数的指针

void register_alloc_error(Alloc_Error error);

void check_alloc_error();

#define INITIALIZE_ARRAY(capacity, type) (initialize_array(capacity, sizeof(type)))

Array* initialize_array(size_t capacity, size_t data_size);

Array* expand_array(Array* array);

void input_data(Array* array, void* data_ptr);

void* get_data(Array* array, size_t index);

void Free(Array* array);

#endif ///< DYNAMIC_ARRAY_H
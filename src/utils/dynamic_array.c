/**
 * @file dynamic_array.h
 * 
 * @brief 通用动态数组定义文件，可在其他地方复用
 * 
 * @author 棍母
 * @version 1.91.9810
 * @date 2026-01-13
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_array.h"


Array* volatile array = NULL;

Alloc_Error alloc_error = NULL;

void register_alloc_error(Alloc_Error error) {
    alloc_error = error;
}

void check_alloc_error() {
    if (alloc_error == NULL) {
        fputs("未注册错误处理函数！", stderr);
        exit(EXIT_FAILURE);
    }
}

Array* initialize_array(size_t capacity, size_t data_size) {
    Array* array = (Array*)calloc(1, sizeof(Array));
    if (array == NULL) {
        fputs("数组初始化失败", stderr);
        exit(EXIT_FAILURE);
    }

    array->data = calloc(capacity, data_size);
    if (array->data == NULL) {
        free(array);
        fputs("数组数据区域初始化失败", stderr);
        exit(EXIT_FAILURE);
    }

    array->data_size = data_size;
    array->num = 0;
    array->capacity = capacity;

    return array;
}

Array* expand_array(Array* array) {
    if (array->num < array->capacity) {
        return array;
    }

    size_t new_capacity = array->capacity * 2;
    
    void* new_data = realloc(array->data, new_capacity * array->data_size);
    if (new_data == NULL) {
        fputs("扩容失败", stderr);
        return array;
    }
    
    array->data = new_data;
    array->capacity = new_capacity;
    return array;
}

void input_data(Array* array, void* data_ptr) {
    if (array->num >= array->capacity) {
        array = expand_array(array);
    }
    
    void* tail = (char*)array->data + (array->num * array->data_size);
    memcpy(tail, data_ptr, array->data_size);
    ++(array->num);
}

void* get_data(Array* array, size_t index) {
    if (index >= array->num) {
        fputs("索引越界", stderr);
        return NULL;
    }
    return (char*)array->data + (index * array->data_size);
}

void Free(Array* array) {
    if (array == NULL) {
        return;
    }

    if (array->data != NULL) {
        free(array->data);
    }

    free(array);
}
/**
 * @file dynamic_array.c
 *
 * @brief 通用动态数组相关函数实现
 *
 * @author 棍母
 * @version 1.91.9810
 * @date 2026-01-13
 */

#include "dynamic_array.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 重新分配数组数据区
 *
 * @param[in,out] array 数组地址，成功后更新 data 和 capacity
 * @param[in] capacity 目标容量，内部调用保证大于 0 且足以容纳现有元素
 * @return true 分配成功
 * @return false 元素大小无效、乘法溢出或分配失败，保留原数据区
 */
static bool array_reserve(Array* array, size_t capacity) {
    if (array->data_size == 0 || capacity > SIZE_MAX / array->data_size) {
        return false;
    }
    ///< 使用临时指针，失败时保留原数据区
    void* data = realloc(array->data, capacity * array->data_size);
    if (data == NULL) {
        return false;
    }
    array->data = data;
    array->capacity = capacity;
    return true;
}

bool array_init(Array* array, size_t capacity, size_t data_size) {
    *array = (Array){ .data_size = data_size };
    return data_size != 0 && (capacity == 0 || array_reserve(array, capacity));
}

bool array_push(Array* array, const void* data) {
    if (array->data_size == 0 || data == NULL) {
        return false;
    }
    if (array->num == array->capacity) {
        ///< 先计算元素数量上限，避免容量翻倍或字节数乘法溢出
        size_t limit = SIZE_MAX / array->data_size;
        if (array->capacity >= limit) {
            return false;
        }
        size_t capacity = array->capacity == 0 ? 1 :
            (array->capacity > limit / 2 ? limit : array->capacity * 2);
        if (!array_reserve(array, capacity)) {
            return false;
        }
    }
    memcpy((char*)array->data + array->num * array->data_size,
           data, array->data_size);
    ++array->num;
    return true;
}

const void* array_get(const Array* array, size_t index) {
    if (index >= array->num) {
        return NULL;
    }
    return (const char*)array->data + index * array->data_size;
}

void array_destroy(Array* array) {
    free(array->data);
    *array = (Array){0}; ///< 清除旧地址和数量，允许再次销毁
}

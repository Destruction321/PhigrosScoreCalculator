/**
 * @file test_array.c
 *
 * @brief 动态数组回归测试
 *
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 */

///< 仅数组实现使用替换函数，本文件需要调用实际 realloc
#undef realloc
#include "dynamic_array.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

static bool fail_allocation; ///< true 时模拟分配失败
/**
 * @brief 模拟数组分配失败
 *
 * @param[in] pointer 原数据区地址，可为 NULL
 * @param[in] size 申请的字节数
 * @return void* fail_allocation 为 true 时返回 NULL，否则调用实际 realloc
 */
void* test_realloc(void* pointer, size_t size) {
    return fail_allocation ? NULL : realloc(pointer, size);
}

/**
 * @brief 验证数组扩容、失败后数据保留以及销毁行为
 *
 * @return int 所有断言通过时返回 0，断言失败时终止测试
 */
int main(void) {
    Array array = {0};
    int value = 42;
    assert(array_init(&array, 0, sizeof(value)));
    assert(array_push(&array, &value));
    void* original = array.data;
    fail_allocation = true;
    assert(!array_push(&array, &value));
    assert(array.data == original && array.num == 1 && array.capacity == 1);
    assert(*(const int*)array_get(&array, 0) == value);
    assert(array_get(&array, 1) == NULL);
    fail_allocation = false;
    for (int i = 0; i < 10000; ++i) {
        assert(array_push(&array, &i));
    }
    for (int i = 0; i < 10000; ++i) {
        assert(*(const int*)array_get(&array, (size_t)i + 1) == i);
    }
    array_destroy(&array);
    array_destroy(&array);
    assert(array.data == NULL && array.num == 0 && array.capacity == 0);
    fail_allocation = true;
    assert(!array_init(&array, 10, sizeof(value)));
    array_destroy(&array);
    fail_allocation = false;
    assert(!array_init(&array, SIZE_MAX, sizeof(value)));
    array_destroy(&array);
    assert(!array_init(&array, 0, 0));
    array_destroy(&array);
    return 0;
}

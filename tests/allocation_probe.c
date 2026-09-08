/**
 * @file allocation_probe.c
 *
 * @brief 测试用内存分配与释放检查
 *
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 */

///< 取消测试构建的宏替换，使本文件可以调用实际分配函数
#undef realloc
#undef free
#include <assert.h>
#include <stdlib.h>
static int calls; ///< 分配请求次数，包括失败的请求
static int live;  ///< 尚未释放的数据区数量，扩容已有数据区不会增加数量

/**
 * @brief 程序退出时检查数组分配计数是否归零
 */
static void verify_cleanup(void) {
    assert(live == 0);
}

/**
 * @brief 统计分配并按环境变量注入失败
 *
 * @param[in] pointer 原数据区地址，可为 NULL
 * @param[in] size 请求的字节数
 * @return void* 分配结果；PHIGROS_FAIL_AT 指定的调用次序返回 NULL
 */
void* test_realloc(void* pointer, size_t size) {
    if (++calls == 1) {
        assert(atexit(verify_cleanup) == 0);
    }
    const char* fail_at = getenv("PHIGROS_FAIL_AT");
    if (fail_at != NULL && calls == atoi(fail_at)) {
        return NULL;
    }
    int new_allocation = pointer == NULL;
    void* result = realloc(pointer, size);
    if (result != NULL && new_allocation) {
        ++live;
    }
    return result;
}

/**
 * @brief 记录释放并调用实际 free
 *
 * @param[in] pointer 待释放地址，可为 NULL
 */
void test_free(void* pointer) {
    if (pointer != NULL) {
        --live;
    }
    free(pointer);
}

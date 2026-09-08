/**
 * @file dynamic_array.h
 *
 * @brief 通用动态数组定义，可在其他地方复用
 *
 * @author 棍母
 * @version 1.91.9810
 * @date 2026-01-13
 */

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief 通用动态数组结构
 *
 * 结构体由调用者持有，只有 data 使用动态内存；首次使用前应置零。
 */
typedef struct dynamic_array {
    void* data;         ///< 数据区地址，由数组管理
    size_t data_size;   ///< 单个元素的字节数
    size_t num;         ///< 已存放元素数量
    size_t capacity;    ///< 数据区可容纳的元素数量
} Array;

/**
 * @brief 初始化数组
 *
 * @param[out] array 零值或已销毁的数组地址，不可覆盖仍持有数据的数组
 * @param[in] capacity 初始容量，允许为 0
 * @param[in] data_size 单个元素的字节数，必须大于 0
 * @return true 初始化成功
 * @return false 元素大小无效、容量溢出或分配失败；仍可调用 array_destroy
 */
bool array_init(Array* array, size_t capacity, size_t data_size);

/**
 * @brief 向数组末尾复制一个元素
 *
 * @param[in,out] array 已初始化的数组地址，容量不足时尝试扩容
 * @param[in] data 待复制元素的地址，不可指向数组内部
 * @return true 添加成功
 * @return false 参数无效、容量溢出或分配失败；原有数据和数量保持不变
 */
bool array_push(Array* array, const void* data);

/**
 * @brief 获取指定索引的元素地址
 *
 * @param[in] array 数组地址
 * @param[in] idx 元素索引，范围为 0 到 num - 1
 * @return const void* 只读元素地址；索引越界时返回 NULL
 * @note 返回地址不需要单独释放，扩容或销毁数组后不可继续使用旧地址
 */
const void* array_get(const Array* array, size_t idx);

/**
 * @brief 释放数据区并将数组恢复为零值
 *
 * @param[in,out] array 有效数组对象的地址，不可为 NULL
 * @note 允许对零值或已销毁的数组重复调用；不会释放 Array 结构体本身
 */
void array_destroy(Array* array);

#endif ///< DYNAMIC_ARRAY_H

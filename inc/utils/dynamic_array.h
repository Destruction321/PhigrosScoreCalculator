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
#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdlib.h>

/**
 * @brief 通用动态数组结构
 */
typedef struct dynamic_array {
    void* data;         ///< 泛型数据指针
    size_t data_size;   ///< 存放数据类型的大小
    size_t num;         ///< 当前存放数据数量
    size_t capacity;    ///< 当前数组容量
} Array;

extern Array* volatile array; ///< 全局动态数组指针

typedef void (*Alloc_Error)(const char* , ...); ///< 错误处理函数

extern Alloc_Error alloc_error; ///< 用于注册错误函数的指针

/**
 * @brief 注册错误处理函数
 * 
 * @param[in] error 错误处理函数指针
 */
void register_alloc_error(Alloc_Error error);

/**
 * @brief 检查错误处理函数是否注册 
 */
void check_alloc_error();

/**
 * @brief 初始化数组的宏，调用函数 initialize_array
 * 
 * @param[in] capacity 初始容量
 * @param[in] type 数据类型，在内部转化为sizeof(type)
 */
#define INITIALIZE_ARRAY(capacity, type) (initialize_array(capacity, sizeof(type)))

/**
 * @brief 初始化动态数组函数，通过宏INITIALIZE_ARRAY调用
 * 
 * @param[in] capacity 初始容量
 * @param[in] data_size 数据大小
 * @return Array* 初始化完成的指针
 */
Array* initialize_array(size_t capacity, size_t data_size);

/**
 * @brief 二倍扩容
 * 
 * @param[in,out] array 待扩容数组
 * @return Array* 扩容后数组
 */
Array* expand_array(Array* array);

/**
 * @brief 存入一个数据
 * 
 * @param[in,out] array 待存放数组
 * @param[in] data_ptr 待存放数据的指针
 */
void input_data(Array* array, void* data_ptr);

/**
 * @brief 获取某一位置上的数据
 * 
 * @param[in] array 待读取数组
 * @param[in] index 读取索引，0 到 array->num - 1 之间
 * @return void* 读取的数据指针
 */
void* get_data(Array* array, size_t index);

/**
 * @brief 释放数组
 * 
 * @param[in/out] array 待释放数组
 */
void Free(Array* array);

///< 其余方法待实现

#endif ///< DYNAMIC_ARRAY_H
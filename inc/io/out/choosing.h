/**
 * @file choosing.h
 * 
 * @brief 方案选择相关函数定义
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef CHOOSING_H
#define CHOOSING_H

#include <stdlib.h>

/**
 * @brief 输出方案时的各种选择
 */
enum choice {
    INITIALISE_CHOICE = 0,
    WRITE_FILE = 48,          ///< '0'
    CHOOSE_SOLUTION = 49,     ///< '1'
    PRINT_ALL_SOLUTIONS = 50, ///< '2'
    PRINT_TEN_SOLUTIONS = 51, ///< '3'
};


typedef struct song Song;

/**
 * @brief 选择输出方式
 * 
 * @param[in] song 歌曲参数
 * @return true 选择了以某种形式输出方案；
 * @return false 用户选择直接退出
 */
bool choose(Song* song);

/**
 * @brief 选择提示
 * 
 * @param[in] counter 方案数量
 */
void choosing_tips(size_t counter);

/**
 * @brief 根据选择情况打印方案
 * 
 * @param[in] choice 选择情况
 * @param[in,out] print 标记指针，指向当前打印的方案
 * @return list_node* 下一个方案的地址
 */
size_t print_result(int choice, size_t idx);

/**
 * @brief 打印方案
 * 
 * @param[in,out] print 标记指针，指向当前打印的方案
 * @return list_node* 下一个方案的地址
 */
size_t print_solutions(size_t idx);

/**
 * @brief 选择方案。
 * 
 * @param[in] counter 方案数量
 * @param[in] song 歌曲参数
 * @return true 选择成功；
 * @return false 选择失败：用户选择直接退出
 */
bool choose_solution(size_t counter, Song* song);

#endif ///< CHOOSING_H
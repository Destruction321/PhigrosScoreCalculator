/**
 * @file print.h
 * 
 * @brief 信息打印相关函数定义
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef PRINT_H
#define PRINT_H

#include <stdbool.h>

typedef struct song Song;
typedef struct input_check Input;

/**
 * @brief 启动确认
 * 
 * @return int 确认字符
 */
int start_confirmation(void);

/**
 * @brief 退出确认
 * 
 * @return true 退出；
 * @return false 开启下一轮
 */
bool exit_confirmation(void);

/**
 * @brief 打印输入提示
 * 
 * @param[in] input 输入检查参数
 * @param[in] song 歌曲参数
 */
void print_tip(Input input, Song* song);

/**
 * @brief 直接退出确认
 * 
 * @param[in] input 输入检查参数
 * @param[in] song 歌曲参数
 * @return true 退出；
 * @return false 不退出
 */
bool direct_exit(Input input, Song* song);

/**
 * @brief 输出错误信息
 * 
 * @param[in] input 输入检查参数
 * @param[in] note 输入数据为 goal 时，提供当前 note 数以判断是否需要修改
 */
void error_message(Input input, int note);

/**
 * @brief 根据当前输入的数据名称进行清除
 * 
 * @param[in] compare_name_with_solution strncmp(name, "目标方案", length) 的结果
 * @param[in] times strncmp(name, "目标方案", length) == 0 时清除的行数
 */
void clear(int compare_name_with_solution, int times);

/**
 * @brief 表头
 * 
 * @param[in] song 歌曲参数
 */
void table_header(Song* song);

#endif ///< PRINT_H
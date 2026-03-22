/**
 * @file datasetting.h
 * 
 * @brief 数据输入相关函数定义
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef DATASETTING_H
#define DATASETTING_H

#define NOTE " note 总数"
#define GOAL  "目标分数"
#define SOLUTION  "目标方案"

#include <stdbool.h>


typedef struct song Song;
typedef struct input_check Input;
typedef enum status Status;

/**
 * @brief 设定 note 和 goal
 * 
 * @param[out] song 歌曲参数地址
 * @return true 输入完成；
 * @return false 直接退出
 */
bool set_note_and_goal(Song* song);

/**
 * @brief 输入数据
 * 
 * @param[out] data 数据地址
 * @param[in] input 输入检查参数
 * @param[in] song 歌曲参数
 * @return true 输入完成；
 * @return false 直接退出
 */
bool set_data(int* data, Input input, Song* song);

/**
 * @brief 初始化输入检查参数
 * 
 * @param[in] min 最小值
 * @param[in] max 最大值
 * @param[in] name 名称
 * @return Input 输入检查参数
 */
Input init_input(int min, int max, const char* name);

/**
 * @brief 获取最小值
 * 
 * @param[in] input 输入检查参数
 * @return int （可能具有特殊用途的）最小值
 */
int get_min(Input input);

/**
 * @brief 获取用户输入并判断合法性
 * 
 * @param[out] data 输入数据的地址
 * @param[in,out] min 最小值，在输入 goal 或选择方案时，最小值有特殊用途
 * @param[in] max 最大值
 * @return Status 返回输入结果
 */
Status scanf_int(int* data, int min, int max);

/**
 * @brief 检查输入数据的合法性
 * 
 * @param[in,out] status 状态码
 * @param[in] input 输入检查参数
 * @param[in] note 输入数据为 goal 时，提供当前 note 数以判断是否需要修改
 * @return true 输入合法；
 * @return false 输入不合法
 */
bool is_valid(Status* status, Input input, int note);

#endif ///< DATASETTING_H
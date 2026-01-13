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

typedef struct song Song;
typedef struct input_check Input;
typedef enum status Status;

/**
 * @brief 设定 note �?goal
 * 
 * @param[out] song 歌曲参数地址
 * @return true 输入完成�? * @return false 直接退�? */
bool set_note_and_goal(Song* song);

/**
 * @brief 输入数据
 * 
 * @param[out] data 数据地址
 * @param[in] input 输入检查参�? * @param[in] song 歌曲参数
 * @return true 输入完成�? * @return false 直接退�? */
bool set_data(int* data, Input input, Song* song);

/**
 * @brief 初始化输入检查参�? * 
 * @param[in] min 最小�? * @param[in] max 最大�? * @param[in] name 名称
 * @return Input 输入检查参�? */
Input init_input(int min, int max, const char* name);

/**
 * @brief 获取最小�? * 
 * @param[in] input 输入检查参�? * @return int （可能具有特殊用途的）最小�? */
int get_min(Input input);

/**
 * @brief 获取用户输入并判断合法�? * 
 * @param[out] data 输入数据的地址
 * @param[in,out] min 最小值，在输�?goal 或选择方案时，最小值有特殊用�? * @param[in] max 最大�? * @return Status 返回输入结果
 */
Status scanf_int(int* data, int min, int max);

/**
 * @brief 检查输入数据的合法�? * 
 * @param[in,out] status 状态码
 * @param[in] input 输入检查参�? * @param[in] note 输入数据�?goal 时，提供当前 note 数以判断是否需要修�? * @return true 输入合法�? * @return false 输入不合�? */
bool is_valid(Status* status, Input input, int note);

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

typedef struct song Song;
typedef struct input_check Input;
typedef enum status Status;

/**
 * @brief 设定 note �?goal
 * 
 * @param[out] song 歌曲参数地址
 * @return true 输入完成�?
 * @return false 直接退�?
 */
bool set_note_and_goal(Song* song);

/**
 * @brief 输入数据
 * 
 * @param[out] data 数据地址
 * @param[in] input 输入检查参�?
 * @param[in] song 歌曲参数
 * @return true 输入完成�?
 * @return false 直接退�?
 */
bool set_data(int* data, Input input, Song* song);

/**
 * @brief 初始化输入检查参�?
 * 
 * @param[in] min 最小�?
 * @param[in] max 最大�?
 * @param[in] name 名称
 * @return Input 输入检查参�?
 */
Input init_input(int min, int max, const char* name);

/**
 * @brief 获取最小�?
 * 
 * @param[in] input 输入检查参�?
 * @return int （可能具有特殊用途的）最小�?
 */
int get_min(Input input);

/**
 * @brief 获取用户输入并判断合法�?
 * 
 * @param[out] data 输入数据的地址
 * @param[in,out] min 最小值，在输�?goal 或选择方案时，最小值有特殊用�?
 * @param[in] max 最大�?
 * @return Status 返回输入结果
 */
Status scanf_int(int* data, int min, int max);

/**
 * @brief 检查输入数据的合法�?
 * 
 * @param[in,out] status 状态码
 * @param[in] input 输入检查参�?
 * @param[in] note 输入数据�?goal 时，提供当前 note 数以判断是否需要修�?
 * @return true 输入合法�?
 * @return false 输入不合�?
 */
bool is_valid(Status* status, Input input, int note);

#endif ///< DATASETTING_H
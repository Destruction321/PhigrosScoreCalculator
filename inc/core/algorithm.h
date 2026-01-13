/**
 * @file algorithm.h
 * 
 * @brief 算法相关函数定义
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <stdbool.h>

typedef struct song Song;
typedef struct auxiliary_arguments Args;
typedef struct list_node list_node;
typedef struct loop Loop;

/**
 * @brief 寻找方案
 * 
 * @param[in] song 歌曲参数
 * @return true 未找到方案或用户走完了流程；
 * @return false 用户不进行方案选择
 */
bool seek_solution(Song* song);

/**
 * @brief 算法
 * 
 * @param[in] song 歌曲参数
 * @return true 未找到方案或用户走完了流程；
 * @return false 用户不进行方案选择
 */
bool algorithm(Song* song);

/**
 * @brief 初始化辅助参�? * 
 * @param[in] song 歌曲参数
 * @param[in] args 辅助参数
 */
void init_args(Song* song, Args* args);

/**
 * @brief 寻找所有方�? * 
 * @param[in] song 歌曲参数
 * @param[in] args 辅助参数
 * @return true 找到至少一个方案；
 * @return false 未找到方案或用户不进行方案选择
 */
bool get_solutions(Song* song, Args args);

/**
 * @brief 寻找第一个方�? * 
 * @param[in] song 歌曲参数
 * @param[in] args 辅助参数
 * @param[in,out] loop 循环参数，计算方案数�? * @return true 找到方案�? * @return false 未找到方�? */
bool get_first_solution(Song* song, Args args, Loop* loop);

/**
 * @brief 寻找第一个方案的 perfect �? * 
 * @param[in] song 歌曲参数
 * @param[in] args 辅助参数
 * @param[in,out] loop 循环参数，计算方案数�? * @return true 找到方案�? * @return false 未找到方�? */
bool get_first_perfect(Song* song, Args args, Loop* loop);

/**
 * @brief 寻找第一个方案的 good �? * 
 * @param[in] song 歌曲参数
 * @param[in,out] loop 循环参数，计算方案数�? * @return true 找到方案�? * @return false 未找到方�? */
bool get_first_good(Song* song, Loop* loop);

/**
 * @brief 寻找第一个方案的 max_combo �? * 
 * @param[in] song 歌曲参数
 * @param[in,out] loop 循环参数，计算方案数�? * @return true 找到方案�? * @return false 未找到方�? */
bool get_first_max_combo(Song* song, Loop* loop);

/**
 * @brief 计算分数
 * 
 * @param[in] factor 公式系数�?000 / note�? * @param[in] perfect
 * @param[in] good
 * @param[in] max_combo
 * @return int 当前分数
 */
int calculate_score(int perfect, int good, int max_combo);

/**
 * @brief 寻找其他方案，原理见 README.md
 * 
 * @param[in] note note 总数
 * @param[in,out] counter 方案计数�? * @param[in,out] loop 循环参数，计算方案数�? */
void get_other_solutions(int note, Loop loop);

/**
 * @brief 更新数据
 * 
 * @param[in,out] loop 方案数据
 * @param[in,out] last_good 上一方案�?good �? * @return int 上一方案�?good �? */
int update_data(Loop* loop, int last_good);

/**
 * @brief 检查数据合理�? * 
 * @param[in,out] loop 方案数据
 * @return true 数据合理�? * @return false 数据不合�? */
bool data_is_valid(Loop loop, int note);

/**
 * @brief 添加方案
 * 
 * @param[in] tail 指向添加位置的指�? * @param[in,out] counter 方案计数�? * @param[in,out] loop 方案数据
 * @return list_node* 新的方案节点
 */

/**
 * @file algorithm.h
 * 
 * @brief 算法相关函数定义
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <stdbool.h>

typedef struct song Song;
typedef struct auxiliary_arguments Args;
typedef struct list_node list_node;
typedef struct loop Loop;

/**
 * @brief 寻找方案
 * 
 * @param[in] song 歌曲参数
 * @return true 未找到方案或用户走完了流程；
 * @return false 用户不进行方案选择
 */
bool seek_solution(Song* song);

/**
 * @brief 算法
 * 
 * @param[in] song 歌曲参数
 * @return true 未找到方案或用户走完了流程；
 * @return false 用户不进行方案选择
 */
bool algorithm(Song* song);

/**
 * @brief 初始化辅助参�?
 * 
 * @param[in] song 歌曲参数
 * @param[in] args 辅助参数
 */
void init_args(Song* song, Args* args);

/**
 * @brief 寻找所有方�?
 * 
 * @param[in] song 歌曲参数
 * @param[in] args 辅助参数
 * @return true 找到至少一个方案；
 * @return false 未找到方案或用户不进行方案选择
 */
bool get_solutions(Song* song, Args args);

/**
 * @brief 寻找第一个方�?
 * 
 * @param[in] song 歌曲参数
 * @param[in] args 辅助参数
 * @param[in,out] loop 循环参数，计算方案数�?
 * @return true 找到方案�?
 * @return false 未找到方�?
 */
bool get_first_solution(Song* song, Args args, Loop* loop);

/**
 * @brief 寻找第一个方案的 perfect �?
 * 
 * @param[in] song 歌曲参数
 * @param[in] args 辅助参数
 * @param[in,out] loop 循环参数，计算方案数�?
 * @return true 找到方案�?
 * @return false 未找到方�?
 */
bool get_first_perfect(Song* song, Args args, Loop* loop);

/**
 * @brief 寻找第一个方案的 good �?
 * 
 * @param[in] song 歌曲参数
 * @param[in,out] loop 循环参数，计算方案数�?
 * @return true 找到方案�?
 * @return false 未找到方�?
 */
bool get_first_good(Song* song, Loop* loop);

/**
 * @brief 寻找第一个方案的 max_combo �?
 * 
 * @param[in] song 歌曲参数
 * @param[in,out] loop 循环参数，计算方案数�?
 * @return true 找到方案�?
 * @return false 未找到方�?
 */
bool get_first_max_combo(Song* song, Loop* loop);

/**
 * @brief 计算分数
 * 
 * @param[in] factor 公式系数�?000 / note�?
 * @param[in] perfect
 * @param[in] good
 * @param[in] max_combo
 * @return int 当前分数
 */
int calculate_score(int perfect, int good, int max_combo);

/**
 * @brief 寻找其他方案，原理见 README.md
 * 
 * @param[in] note note 总数
 * @param[in,out] counter 方案计数�?
 * @param[in,out] loop 循环参数，计算方案数�?
 */
void get_other_solutions(int note, Loop loop);

/**
 * @brief 更新数据
 * 
 * @param[in,out] loop 方案数据
 * @param[in,out] last_good 上一方案�?good �?
 * @return int 上一方案�?good �?
 */
int update_data(Loop* loop, int last_good);

/**
 * @brief 检查数据合理�?
 * 
 * @param[in,out] loop 方案数据
 * @return true 数据合理�?
 * @return false 数据不合�?
 */
bool data_is_valid(Loop loop, int note);

/**
 * @brief 添加方案
 * 
 * @param[in] tail 指向添加位置的指�?
 * @param[in,out] counter 方案计数�?
 * @param[in,out] loop 方案数据
 * @return list_node* 新的方案节点
 */

#endif ///< ALGORITHM_H
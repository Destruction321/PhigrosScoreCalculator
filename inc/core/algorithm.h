/**
 * @file algorithm.h
 *
 * @brief 算法相关接口声明
 *
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 */

#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "struct.h"

/**
 * @brief 查找并展示指定目标分数的方案
 *
 * @param[in] song 歌曲参数地址
 * @retval RESULT_OK 正常完成，包括未找到方案
 * @retval RESULT_CANCEL 用户退出或输入流结束
 * @retval RESULT_ERROR 内存分配或输入输出失败
 * @note 数组在内部创建并统一释放，不向调用者转移所有权
 */
Result seek_solution(Song* song);

#endif ///< ALGORITHM_H

/**
 * @file data_setting.h
 *
 * @brief 数据输入相关接口声明
 *
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 */

#ifndef DATASETTING_H
#define DATASETTING_H

#include "struct.h"

/**
 * @brief 设定 note 总数和目标分数
 *
 * @param[out] song 歌曲参数地址
 * @retval RESULT_OK 输入完成；goal 为 CHANGE_NOTE 时由调用者重新设置 note
 * @retval RESULT_CANCEL 用户退出或输入流结束
 * @retval RESULT_ERROR 输入读取失败或输入参数无效
 */
Result set_note_and_goal(Song* song);

/**
 * @brief 读取并检查一个整数，必要时提示重新输入
 *
 * @param[out] data 接收合法整数的地址
 * @param[in] input 输入范围与提示参数
 * @param[in] song 歌曲参数地址，用于显示当前设定
 * @retval RESULT_OK 输入完成
 * @retval RESULT_CANCEL 用户退出或输入流结束
 * @retval RESULT_ERROR 输入读取失败或输入范围无效
 */
Result set_data(int* data, Input input, Song* song);

/**
 * @brief 根据输入类型统一设置范围和提示名称
 *
 * @param[in] kind 输入类型
 * @param[in] solution_count 可选方案数量，INPUT_SOLUTION 时须大于 0；其他类型传 0
 * @return Input 包含范围、显示名称和类型的参数，名称指向字符串常量
 * @note 未知类型或非正方案数量生成无效范围，由 set_data 返回 RESULT_ERROR
 */
Input init_input(InputKind kind, int solution_count);

#endif ///< DATASETTING_H

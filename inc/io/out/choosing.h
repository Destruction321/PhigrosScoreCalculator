/**
 * @file choosing.h
 *
 * @brief 方案选择相关接口声明
 *
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 */

#ifndef CHOOSING_H
#define CHOOSING_H

#include "struct.h"
#include "dynamic_array.h"

/**
 * @brief 查看、选择或导出方案
 *
 * @param[in] song 歌曲参数地址
 * @param[in] array 非空方案数组，只借用，不修改或释放
 * @retval RESULT_OK 正常完成，包括未找到方案
 * @retval RESULT_CANCEL 用户退出或输入流结束
 * @retval RESULT_ERROR 内存分配或输入输出失败
 */
Result choose(Song* song, const Array* array);

#endif ///< CHOOSING_H

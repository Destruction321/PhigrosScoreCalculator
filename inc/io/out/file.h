/**
 * @file file.h
 *
 * @brief 方案文件输出相关接口声明
 *
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 */

#ifndef FILE_H
#define FILE_H

#include "struct.h"
#include "dynamic_array.h"

/**
 * @brief 将全部方案写入文本文件
 *
 * @param[in] song 歌曲参数地址，用于生成文件名和表头
 * @param[in] array 只读方案数组，不在本函数中释放
 * @retval RESULT_OK 保存成功、取消导出或保留原文件
 * @retval RESULT_ERROR 路径处理或文件读写、关闭失败，或输入读取失败
 * @note 输入文件夹时遇到 EOF 按取消导出处理；路径和 FILE 由本模块管理
 */
Result write_file(const Song* song, const Array* array);

#endif ///< FILE_H

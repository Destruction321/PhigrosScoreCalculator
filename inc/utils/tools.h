/**
 * @file tools.h
 * 
 * @brief 工具函数头文件，可在其他地方复用
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/**
 * @brief 清屏并打印信息。
 *
 * @param[in] format 信息格式字符串。
 * @param[in] ... 可变参数列表。
 */
void clear_and_print(const char* format, ...);

/**
 * @brief 打印分割线。
 *
 * @param[in,out] line_type 分割线类型
 * @param[in] length 分割线长度
 * @param[in] stream 输出流
 */
void print_dividing_line(char line_type, int length, FILE* stream);

/**
 * @brief 清空输入缓冲区。
 */
void clear_buffer(void);

/**
 * @brief 获取一个字符。
 * 
 * @return int 读取到的字符
 */
int getch(void);

#endif ///< TOOLS_H
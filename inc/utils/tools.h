/**
 * @file tools.h
 * 
 * @brief 工具函数头文件，可在其他地方复用
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 */

#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>

/**
 * @brief 分割线长度
 */
enum dividing_line_length {
    LONG = 71,     ///< 无效的目标分数，请输入一个 1 到 1000000 之间的整数，按"Enter"结束输入
    MEDIUM = 47,   ///< 按"Enter"开始新一轮查找，输入其他任意字符退出
    SHORT = 39,    ///< 按"Enter"以开始，输入其他任意字符退出
    ASTERISK = 33, ///< *********************************
};


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
 * @param[in] line_type 分割线类型
 * @param[in] length 分割线长度
 * @param[in,out] stream 接收分割线的输出流
 */
void print_dividing_line(char line_type, int length, FILE* stream);

/**
 * @brief 清空输入缓冲区。
 */
void clear_buffer(void);

/**
 * @brief 获取一个字符。
 * 
 * @return int 读取到的字符；输入流结束或读取失败时返回 EOF
 */
int getch(void);

#endif ///< TOOLS_H

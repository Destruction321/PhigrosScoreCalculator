/**
 * @file tools.c
 * 
 * @brief 工具函数源文件，可在其他地方复用
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 * 
 * @copyright Copyright (c) 2025
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "tools.h"


void clear_and_print(const char* format, ...) {
    fputs("\033[2J\033[H", stdout);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void print_dividing_line(char line_type, int length, FILE* stream) {
    enum{ MAX_LINE = 150 }; ///< 分割线长度最大值
    char dividing_line[MAX_LINE] = { '\0' };
    if (line_type == '\0') {
        line_type = '?';
    }
    memset(dividing_line, line_type, length);
    dividing_line[length] = '\n';
    dividing_line[length + 1] = '\0';
    fputs(dividing_line, stream);
}

void clear_buffer(void) {
    int c = getchar();
    while ((c != '\n') && (c != EOF)) {
        c = getchar();
    }
}

int getch(void) {
    int ch = getchar();
    if ((ch != '\n') && (ch != EOF)) {
        clear_buffer(); ///< 如果不是换行或 EOF，清空缓冲区
    }
    return ch;
}
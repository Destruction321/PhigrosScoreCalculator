/**
 * @file main.c
 * 
 * @brief Phigros算分器，助力你打出114514分
 * 
 * 关于算法的说明，请参阅README.md
 * 本程序纯属娱乐，闲得没事就改改
 * 祝你在Phigros中取得好成绩（雾
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 * 
 * @copyright Copyright (c) 2025
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <setjmp.h>

#include "print.h"
#include "struct.h"
#include "tools.h"
#include "dynamic_array.h"
#include "data_setting.h"
#include "algorithm.h"


static jmp_buf env; ///< 错误处理跳转

static void program(Song* song);
static bool main_program(Song* song);
static void error_exit(const char* format, ...);


/**
 * @brief 程序的逻辑框架
 * 
 * @param[out] song 歌曲参数地址
 */
static void program(Song* song) {
    bool code = true; ///< 退出确认，true 表示未询问，false 表示已询问
    while (true) {
        if (code) {
            fputs("Phigros算分器，助力你打出114514分\n", stdout);
        }

        ///< 进入主要逻辑
        if (start_confirmation() == '\n') {
            if (main_program(song)) {
                continue;
            }
            break;
        }

        ///< 退出确认
        if (code) {
            clear_and_print("确定要退出吗？\n");
            code = false;
            continue;
        }

        break;
    }
}

/**
 * @brief 确认进入程序后的逻辑框架
 * 
 * @param[out] song 歌曲参数地址
 * @return true 开启下一轮；
 * @return false 退出
 */
static bool main_program(Song* song) {
    do {
        *song = (Song){ 0 };
        if (!set_note_and_goal(song)) {
            return false;
        }
    } while (song->goal == CHANGE_NOTE);

    if (!seek_solution(song)) {
        return false;
    }

    if (!exit_confirmation()) {
        return true;
    }

    clear_and_print("确定要退出吗？\n");
    if (!exit_confirmation()) {
        return true;
    }
    return false;
}

/**
 * @brief 主程序
 * 
 * @return int 程序退出
 * @retval 非 0 异常退出
 */
int main(void) {
    ///< 错误处理，error_exit 函数跳转至此
    if (setjmp(env) != 0) {
        Free(array);
        exit(EXIT_FAILURE);
    }
    fputs("\033[2J\033[H", stdout); ///< 清除终端
    register_alloc_error(error_exit);
    check_alloc_error();
    Song song = { 0 };
    program(&song);
    clear_and_print("感谢您的使用...");
    return 0;
}

/**
 * @brief 错误退出处理函数
 * 
 * @param[in] error 错误信息格式字符串
 * @param[in] ... 可变参数列表
 */
static void error_exit(const char* error, ...) {
    ///< 清屏并输出错误信息 error
    fputs("\033[2J\033[H", stdout);
    va_list args;
    va_start(args, error);
    vprintf(error, args);
    va_end(args);
    longjmp(env, 1); ///< 跳转至 main 函数进行错误处理
}
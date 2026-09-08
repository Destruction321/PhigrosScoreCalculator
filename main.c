/**
 * @file main.c
 *
 * @brief Phigros算分器，助力你打出114514分
 *
 * 本程序纯属娱乐，闲得没事就改改。算法说明见 MORE_DETAILS.md。
 *
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "print.h"
#include "struct.h"
#include "tools.h"
#include "data_setting.h"
#include "algorithm.h"

/**
 * @brief 执行一轮输入、计算与退出确认
 *
 * @param[out] song 本轮歌曲参数地址
 * @retval RESULT_OK 用户选择继续查找
 * @retval RESULT_CANCEL 用户退出或输入流结束
 * @retval RESULT_ERROR 本轮操作失败
 */
static Result main_program(Song* song) {
    Result result;
    do {
        *song = (Song){0};
        result = set_note_and_goal(song);
        if (result != RESULT_OK) {
            return result;
        }
    } while (song->goal == CHANGE_NOTE);

    result = seek_solution(song);
    if (result != RESULT_OK) {
        return result;
    }

    if (!exit_confirmation()) {
        return RESULT_OK;
    }

    clear_and_print("确定要退出吗？\n");
    return exit_confirmation() ? RESULT_CANCEL : RESULT_OK;
}

/**
 * @brief 程序的交互主循环
 *
 * @param[out] song 存放每轮输入的歌曲参数
 * @retval RESULT_CANCEL 用户退出或输入流结束
 * @retval RESULT_ERROR 内存分配或输入输出失败
 */
static Result program(Song* song) {
    bool confirmed = false;
    for (;;) {
        if (!confirmed) {
            fputs("Phigros算分器，助力你打出114514分\n", stdout);
        }

        int choice = start_confirmation();
        if (choice == EOF) {
            return ferror(stdin) ? RESULT_ERROR : RESULT_CANCEL;
        }

        if (choice == '\n') {
            Result result = main_program(song);
            if (result != RESULT_OK) {
                return result;
            }
            confirmed = false;
            continue;
        }

        if (confirmed) {
            return RESULT_CANCEL;
        }
        
        clear_and_print("确定要退出吗？\n");
        confirmed = true;
    }
}

/**
 * @brief 程序入口
 *
 * @return int 程序退出状态
 * @retval EXIT_SUCCESS 正常退出
 * @retval EXIT_FAILURE 内存分配或输入输出失败
 */
int main(void) {
    fputs("\033[2J\033[H", stdout);
    Song song = {0};
    Result result = program(&song);
    if (result == RESULT_ERROR || ferror(stdin)) {
        fputs("操作失败（内存分配或输入输出错误），程序已结束。\n", stderr);
        return EXIT_FAILURE;
    }
    clear_and_print("感谢您的使用...\n");
    return EXIT_SUCCESS;
}

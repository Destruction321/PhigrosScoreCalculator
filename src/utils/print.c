/**
 * @file print.c
 * 
 * @brief 信息打印相关函数实现
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 * 
 * @copyright Copyright (c) 2025
 */
#include <stdbool.h>

#include "print.h"
#include "struct.h"
#include "tools.h"


int start_confirmation(void) {
    print_dividing_line('=', SHORT, stdout);
    fputs("按\"Enter\"以开始，输入其他任意字符退出：", stdout);
    return getch();
}

bool exit_confirmation(void) {
    print_dividing_line('=', MEDIUM, stdout);
    fputs("按\"Enter\"开始新一轮查找，输入其他任意字符退出：", stdout);
    if (getch() == '\n') {
        fputs("\033[2J\033[H", stdout);
        return false;
    }
    return true;
}

void print_tip(Input input, Song* song) {
    if (input.compare_name_with_solution != 0) {
        printf("当前设定�?note 总数和目标分数分别为�?d�?d\n", song->note, song->goal);
    }
    ///< 打印分割�?    if (input.compare_name_with_goal == 0 || input.compare_name_with_solution == 0) {
        print_dividing_line('=', LONG, stdout);
    } else {
        print_dividing_line('=', MEDIUM, stdout);
    }
    ///< 打印提示信息
    printf("请输入歌�?s�?d �?%d 之间的整�?, input.name, input.min, input.max);
    if (input.compare_name_with_goal == 0) {
        fputs("，输�?-1 修改 note �?, stdout);
    } else if (input.compare_name_with_solution == 0) {
        fputs("，输�?0 将所有方案写入文�?, stdout);
    }
    fputs("）：", stdout);
}

bool direct_exit(Input input, Song* song) {
    clear(input.compare_name_with_solution, EXIT);
    if (input.compare_name_with_solution != 0) {
        print_dividing_line('=', SHORT, stdout);
    }
    fputs("您是否要退出？\n", stdout);
    print_dividing_line('=', SHORT, stdout);
    fputs("按\"Enter\"以退出，输入其他任意字符继续�?, stdout);
    if (getch() != '\n') {
        return false;
    }
    printf("\033[3F确定要退出吗？\n\033[B\033[%dG", SHORT); ///< 替换"您是否要退出？"并返回输入位�?    if (getch() != '\n') {
        return false;
    }
    return true;
}

void error_message(Input input, int note) {
    clear(input.compare_name_with_solution, ERROR_CHOICE);
    print_dividing_line('=', LONG, stdout);
    printf(
        "无效�?s，请输入一�?%d �?%d 之间的整数，按\"Enter\"结束输入",
        input.name, input.min, input.max
    );
    if (input.compare_name_with_goal != 0) {
        fputs("�?, stdout);
    } else {
        printf("；\n输入 -1 修改 note 数，当前 note 数：%d；\n", note);
        print_dividing_line('=', LONG, stdout);
        fputs("请输入：", stdout);
    }
}

void clear(int compare_name_with_solution, int times) {
    if (compare_name_with_solution == 0) {
        for (size_t i = 1; i <= times; ++i) {
            fputs("\033[F\033[2K", stdout);
        }
    } else {
        fputs("\033[2J\033[H", stdout);
    }
}

void table_header(Song* song) {
    clear_and_print("歌曲 note 总数�?d，目标分数：%d\n", song->note, song->goal);
    print_dividing_line('=', SHORT, stdout);
/**
 * @file print.c
 * 
 * @brief 信息打印相关函数实现
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 * 
 * @copyright Copyright (c) 2025
 */
#include <stdbool.h>

#include "print.h"
#include "struct.h"
#include "tools.h"


int start_confirmation(void) {
    print_dividing_line('=', SHORT, stdout);
    fputs("按\"Enter\"以开始，输入其他任意字符退出：", stdout);
    return getch();
}

bool exit_confirmation(void) {
    print_dividing_line('=', MEDIUM, stdout);
    fputs("按\"Enter\"开始新一轮查找，输入其他任意字符退出：", stdout);
    if (getch() == '\n') {
        fputs("\033[2J\033[H", stdout);
        return false;
    }
    return true;
}

void print_tip(Input input, Song* song) {
    if (input.compare_name_with_solution != 0) {
        printf("当前设定�?note 总数和目标分数分别为�?d�?d\n", song->note, song->goal);
    }
    ///< 打印分割�?
    if (input.compare_name_with_goal == 0 || input.compare_name_with_solution == 0) {
        print_dividing_line('=', LONG, stdout);
    } else {
        print_dividing_line('=', MEDIUM, stdout);
    }
    ///< 打印提示信息
    printf("请输入歌�?s�?d �?%d 之间的整�?, input.name, input.min, input.max);
    if (input.compare_name_with_goal == 0) {
        fputs("，输�?-1 修改 note �?, stdout);
    } else if (input.compare_name_with_solution == 0) {
        fputs("，输�?0 将所有方案写入文�?, stdout);
    }
    fputs("）：", stdout);
}

bool direct_exit(Input input, Song* song) {
    clear(input.compare_name_with_solution, EXIT);
    if (input.compare_name_with_solution != 0) {
        print_dividing_line('=', SHORT, stdout);
    }
    fputs("您是否要退出？\n", stdout);
    print_dividing_line('=', SHORT, stdout);
    fputs("按\"Enter\"以退出，输入其他任意字符继续�?, stdout);
    if (getch() != '\n') {
        return false;
    }
    printf("\033[3F确定要退出吗？\n\033[B\033[%dG", SHORT); ///< 替换"您是否要退出？"并返回输入位�?
    if (getch() != '\n') {
        return false;
    }
    return true;
}

void error_message(Input input, int note) {
    clear(input.compare_name_with_solution, ERROR_CHOICE);
    print_dividing_line('=', LONG, stdout);
    printf(
        "无效�?s，请输入一�?%d �?%d 之间的整数，按\"Enter\"结束输入",
        input.name, input.min, input.max
    );
    if (input.compare_name_with_goal != 0) {
        fputs("�?, stdout);
    } else {
        printf("；\n输入 -1 修改 note 数，当前 note 数：%d；\n", note);
        print_dividing_line('=', LONG, stdout);
        fputs("请输入：", stdout);
    }
}

void clear(int compare_name_with_solution, int times) {
    if (compare_name_with_solution == 0) {
        for (size_t i = 1; i <= times; ++i) {
            fputs("\033[F\033[2K", stdout);
        }
    } else {
        fputs("\033[2J\033[H", stdout);
    }
}

void table_header(Song* song) {
    clear_and_print("歌曲 note 总数�?d，目标分数：%d\n", song->note, song->goal);
    print_dividing_line('=', SHORT, stdout);
}
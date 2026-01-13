/**
 * @file datasetting.c
 * 
 * @brief 数据输入相关函数实现
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 * 
 * @copyright Copyright (c) 2025
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "data_setting.h"
#include "print.h"
#include "struct.h"
#include "tools.h"
#include "dynamic_array.h"


bool set_note_and_goal(Song* song) {
    if (!set_data(&song->note, init_input(MIN_NOTE, MAX_NOTE, " note 总数"), song)) {
        return false;
    }

    if (!set_data(&song->goal, init_input(MIN_SCORE, MAX_SCORE, "目标分数"), song)) {
        return false;
    }

    return true;
}

bool set_data(int* data, Input input, Song* song) {
    errno = EINVAL;
    int min = get_min(input);
    Status status = FAILURE;

    ///< 输入数据
    while (status == FAILURE) {
        if (errno != EIO) {
            clear(input.compare_name_with_solution, NOT_EXIT);
            print_tip(input, song);
        }

        ///< 获取用户输入并判断合法性
        status = scanf_int(data, min, input.max);
        if (is_valid(&status, input, song->note)) {
            return true;
        }

        ///< 直接退出确认
        if (errno != EINVAL) {
            continue;
        }

        if (direct_exit(input, song)) {
            return false;
        }
    }
}

Input init_input(int min, int max, const char* name) {
    static const int NAME_LENGTH = 12; ///< 四个汉字，4 * 3 = 12
    const size_t length = strnlen(name, NAME_LENGTH);
    return (Input) {
        min, max, name,
        strncmp(name, "目标方案", length), strncmp(name, "目标分数", length)
    };
}

int get_min(Input input) {
    if (input.compare_name_with_goal == 0) {
        return CHANGE_NOTE;
    } else if (input.compare_name_with_solution == 0) {
        return TO_FILE;
    } else {
        return input.min;
    }
}

Status scanf_int(int* data, int min, int max) {
    static const int INT_SIZE = 8;
    static const int LAST = 6; ///< INT_SIZE - 2, 最后一个有效整数位

    char str[INT_SIZE] = { '\0' };

    ///< 检查输入情况
    if (!fgets(str, INT_SIZE, stdin)) {
        clear_and_print("获取输入失败");
        exit(EXIT_FAILURE);
    }

    ///< 用户直接按了回车
    if (strncmp(str, "\n", 1) == 0) {
        return ENTER;
    }

    ///< 用户输入了过长的数据
    if ((str[LAST] != '\n') && (str[LAST] != '\0') && (str[LAST] != '0')) {
        clear_buffer();
        return FAILURE;
    }

    ///< 将字符串转换为整数
    char* endptr = NULL;
    long temp = strtol(str, &endptr, 0);

    ///< 检查合法性
    bool invalid = ((*endptr != '\n') && (*endptr != '\0'));
    if (invalid || (temp > max) || (temp < min)) {
        return FAILURE;
    }

    *data = (int)temp;
    return SUCCESS;
}

bool is_valid(Status* status, Input input, int note) {
    switch (*status) {
    case FAILURE:
        errno = EIO;
        error_message(input, note);
        break;
    case ENTER:
        errno = EINVAL;
        *status = FAILURE;
        break;
    default:
        return true;
    }
    return false;
}
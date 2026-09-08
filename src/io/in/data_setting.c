/**
 * @file data_setting.c
 * 
 * @brief 数据输入相关函数实现
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 */

#include "data_setting.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "print.h"
#include "struct.h"
#include "tools.h"


static int get_min(Input input);
static Status scanf_int(int* data, int min, int max);

Result set_note_and_goal(Song* song) {
    Result result = set_data(&song->note, init_input(INPUT_NOTE, 0), song);
    if (result != RESULT_OK) {
        return result;
    }
    return set_data(&song->goal, init_input(INPUT_GOAL, 0), song);
}

Result set_data(int* data, Input input, Song* song) {
    if (input.min > input.max) {
        return RESULT_ERROR;
    }
    bool show_tip = true; ///< 错误提示后保留当前界面，避免立即清除提示
    for (;;) {
        if (show_tip) {
            clear(input.kind, NOT_EXIT);
            print_tip(input, song);
        }
        show_tip = true;
        Status status = scanf_int(data, get_min(input), input.max);
        switch (status) {
        case SUCCESS:
            return RESULT_OK;
        case INPUT_END:
            return RESULT_CANCEL;
        case INPUT_ERROR:
            return RESULT_ERROR;
        case ENTER:
            if (direct_exit(input, song)) {
                return RESULT_CANCEL;
            }
            break;
        default:
            error_message(input, song->note);
            show_tip = false;
            break;
        }
    }
}

Input init_input(InputKind kind, int solution_count) {
    switch (kind) {
    case INPUT_NOTE:
        return (Input){
            .min = MIN_NOTE, .max = MAX_NOTE,
            .name = " note 总数", .kind = kind,
        };
    case INPUT_GOAL:
        return (Input){
            .min = MIN_SCORE, .max = MAX_SCORE,
            .name = "目标分数", .kind = kind,
        };
    case INPUT_SOLUTION:
        return (Input){
            .min = 1, .max = solution_count,
            .name = "目标方案", .kind = kind,
        };
    }
    ///< 未知枚举值使用无效范围，交由 set_data 报告错误
    return (Input){ .min = 1, .max = 0, .name = "未知输入", .kind = kind };
}

/**
 * @brief 根据输入类型获取实际下限
 *
 * @param[in] input 输入检查参数
 * @return int 目标分数允许 CHANGE_NOTE，方案选择允许 TO_FILE，其余使用 min
 */
static int get_min(Input input) {
    switch (input.kind) {
    case INPUT_GOAL:
        return CHANGE_NOTE;
    case INPUT_SOLUTION:
        return TO_FILE;
    default:
        return input.min;
    }
}

/**
 * @brief 读取一行十进制整数并检查范围
 *
 * @param[out] data 合法整数的接收地址，仅成功时赋值
 * @param[in] min 允许的最小值，包含特殊操作值
 * @param[in] max 允许的最大值
 * @retval SUCCESS 输入合法
 * @retval FAILURE 输入过长、格式错误或数值越界
 * @retval ENTER 用户直接按回车
 * @retval INPUT_END 输入流结束
 * @retval INPUT_ERROR 输入读取失败
 */
static Status scanf_int(int* data, int min, int max) {
    enum {
        INT_SIZE = 9, ///< 七位分数 + 换行 + 字符串结束符
        LAST = INT_SIZE - 2 ///< 七位整数后的换行位置，出现其他字符说明输入过长
    };
    char str[INT_SIZE] = { '\0' };

    ///< 检查输入情况
    if (!fgets(str, INT_SIZE, stdin)) {
        return ferror(stdin) ? INPUT_ERROR : INPUT_END;
    }

    ///< 用户直接按了回车
    if (strncmp(str, "\n", 1) == 0) {
        return ENTER;
    }

    ///< 用户输入了过长的数据
    if ((str[LAST] != '\n') && (str[LAST] != '\0')) {
        clear_buffer();
        return FAILURE;
    }

    ///< 将字符串转换为整数
    char* endptr = NULL;
    errno = 0;
    long temp = strtol(str, &endptr, 10);

    ///< 检查合法性（例如"123abc", *endptr = 'a'）
    bool invalid = ((*endptr != '\n') && (*endptr != '\0'));
    if (invalid || endptr == str || errno == ERANGE || (temp > max) || (temp < min)) {
        return FAILURE;
    }

    *data = (int)temp;
    return SUCCESS;
}

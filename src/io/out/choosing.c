/**
 * @file choosing.c
 * 
 * @brief 方案选择相关函数实现
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 */

#include "choosing.h"

#include <stdio.h>
#include "print.h"
#include "struct.h"
#include "tools.h"
#include "dynamic_array.h"
#include "data_setting.h"
#include "file.h"

enum choice {
    INITIALISE_CHOICE = 0,
    WRITE_FILE = '0',
    CHOOSE_SOLUTION = '1',
    PRINT_ALL_SOLUTIONS = '2',
    PRINT_TEN_SOLUTIONS = '3',
};

static size_t print_result(int choice, size_t idx, const Array* array);
static size_t print_solutions(size_t idx, const Array* array);
static Result choose_solution(size_t counter, Song* song, const Array* array);

static const int DIVIDING_NUM = 10; ///< 控制台输出方案时，用分割线分组的数量

Result choose(Song* song, const Array* array) {
    printf("\033[F共找到 %zu 种可行方案。\n", array->num);
    print_dividing_line('=', MEDIUM, stdout);
    size_t idx = print_solutions(0, array);
    int choice = INITIALISE_CHOICE;

    while ((idx < array->num) && (choice != CHOOSE_SOLUTION)) {
        print_dividing_line('-', MEDIUM, stdout);
        print_dividing_line('*', ASTERISK, stdout);
        fputs(
            "·输入\"0\"将全部方案写入文件；\n"
            "·输入\"1\"在当前方案中进行选择；\n"
            "·输入\"2\"立即输出全部方案；\n"
            "·输入\"3\"输出 10 个方案；\n"
            "·输入其他任意字符显示下一方案...\n", stdout
        );
        print_dividing_line('*', ASTERISK, stdout);
        fputs("请选择：", stdout);

        choice = getch();
        if (choice == EOF) {
            return ferror(stdin) ? RESULT_ERROR : RESULT_CANCEL;
        }
        if (choice == WRITE_FILE) {
            return write_file(song, array);
        }
        for (int i = 1; i < 10; ++i) {
            fputs("\033[F\033[2K", stdout); ///< 光标上移并清除当前行
        }
        idx = print_result(choice, idx, array);
    }
    return choose_solution(idx, song, array);
}

/**
 * @brief 根据用户选项继续输出方案
 *
 * @param[in] choice 用户输入的选项字符
 * @param[in] idx 下一条待输出方案的索引
 * @param[in] array 只读方案数组
 * @return size_t 下一条待输出索引；选择方案或输出完毕时返回 num
 */
static size_t print_result(int choice, size_t idx, const Array* array) {
    switch (choice) {
    case CHOOSE_SOLUTION:
        break;

    case PRINT_ALL_SOLUTIONS:
        while (idx < array->num) {
            idx = print_solutions(idx, array);
        }
        break;

    case PRINT_TEN_SOLUTIONS:
        do {
            idx = print_solutions(idx, array);
        } while ((idx % 10 != 0) && (idx < array->num));
        break;

    default:
        idx = print_solutions(idx, array);
    }
    return idx;
}

/**
 * @brief 打印一条方案
 *
 * @param[in] idx 待输出方案索引，必须小于 num
 * @param[in] array 只读方案数组
 * @return size_t 下一条方案的索引
 */
static size_t print_solutions(size_t idx, const Array* array) {
    const sol_t* data = array_get(array, idx);

    if ((idx % DIVIDING_NUM == 0) && (idx != 0)) {
        print_dividing_line('-', MEDIUM, stdout);
    }

    printf(
        "%zu. Perfect：%d, Good：%d, Max Combo：%d\n",
        ++idx, data->perfect, data->good, data->max_combo
    );
    return idx;
}

/**
 * @brief 按编号选择方案或导出全部方案
 *
 * @param[in] counter 可选择的方案数量
 * @param[in] song 歌曲参数地址
 * @param[in] array 只读方案数组
 * @retval RESULT_OK 正常完成，包括未找到方案
 * @retval RESULT_CANCEL 用户退出或输入流结束
 * @retval RESULT_ERROR 内存分配或输入输出失败
 */
static Result choose_solution(size_t counter, Song* song, const Array* array) {
    ///< 只有一个方案，不用特意选择
    if (counter == 1) {
        return RESULT_OK;
    }
    fputs("\n\n\n\n", stdout); ///< 抵消 set_data 内部的清除四行操作

    ///< 选择方案
    int choice = INITIALISE_CHOICE;
    Result result = set_data(&choice, init_input(INPUT_SOLUTION, (int)counter), song);
    if (result != RESULT_OK) {
        return result;
    }

    ///< 选择了写文件
    if (choice == TO_FILE) {
        return write_file(song, array);
    }

    ///< 找目标方案
    const sol_t* found = array_get(array, --choice);

    ///< 最终选择呈现
    table_header(song);
    printf(
        "Perfect：%d, Good：%d, Max Combo：%d\n",
        found->perfect, found->good, found->max_combo
    );
    return RESULT_OK;
}

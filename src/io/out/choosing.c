/**
 * @file choosing.c
 * 
 * @brief 方案选择相关函数实现
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 * 
 * @copyright Copyright (c) 2025
 */
#include <stdlib.h>

#include "choosing.h"
#include "print.h"
#include "struct.h"
#include "tools.h"
#include "dynamic_array.h"
#include "data_setting.h"
#include "file.h"

static const int DIVIDING_NUM = 10; ///< 控制机输出方案时，用分割线分组的数量

bool choose(Song* song) {
    size_t idx = print_solutions(0);
    int choice = INITIALISE_CHOICE;

    while ((idx < array->num) && (choice != CHOOSE_SOLUTION)) {
        print_dividing_line('-', MEDIUM, stdout);
        fputs("请选择：", stdout);

        choice = getch();
        if (choice == WRITE_FILE) {
            write_file(song);
            return true;
        }

        fputs("\033[F\033[2K\033[F\033[2K", stdout);
        idx = print_result(choice, idx);
    }
    
    return choose_solution(idx, song);
}

void choosing_tips(size_t counter) {
    printf("\033[F共找到 %zu 种可行方案。\n", counter);
    if (counter > 1) {
        print_dividing_line('=', SHORT, stdout);
        print_dividing_line('*', ASTERISK, stdout);
        fputs(
            "·输入\"0\"将全部方案写入文件；\n"
            "·输入\"1\"在当前方案中进行选择；\n"
            "·输入\"2\"立即输出全部方案；\n"
            "·输入\"3\"输出 10 个方案；\n"
            "·输入其他任意字符显示下一方案...\n", stdout
        );
        print_dividing_line('*', ASTERISK, stdout);
    }
    print_dividing_line('=', MEDIUM, stdout);
}

size_t print_result(int choice, size_t idx) {
    switch (choice) {
    case CHOOSE_SOLUTION:
        idx = array->num;
        break;

    case PRINT_ALL_SOLUTIONS:
        while (idx < array->num) {
            idx = print_solutions(idx);
        }
        break;

    case PRINT_TEN_SOLUTIONS:
        sol_t* data = NULL;
        do {
            data = (sol_t*)array->data;
            idx = print_solutions(idx);
        } while ((idx % 10 != 0) && (idx < array->num));
        break;

    default:
        idx = print_solutions(idx);
    }

    return idx;
}

size_t print_solutions(size_t idx) {
    sol_t* data = get_data(array, idx);

    if ((idx % DIVIDING_NUM == 1) && (idx != 1)) {
        print_dividing_line('-', MEDIUM, stdout);
    }

    printf(
        "%zu. Perfect：%d, Good：%d, Max Combo：%d\n",
        ++idx, data->perfect, data->good, data->max_combo
    );

    return idx;
}

bool choose_solution(size_t counter, Song* song) {
    ///< 只有一个方案，不用特意选择
    if (counter == 1) {
        return true;
    }
    fputs("\n\n\n\n", stdout); ///< 抵消 set_data 内部的清除四行操作

    ///< 选择方案
    int choice = INITIALISE_CHOICE;
    if (!set_data(&choice, init_input(1, counter, SOLUTION), song)) {
        return false;
    }

    ///< 选择了写文件
    if (choice == TO_FILE) {
        write_file(song);
        return true;
    }

    ///< 找目标方案
    sol_t* found = get_data(array, --choice);

    ///< 最终选择呈现
    table_header(song);
    printf(
        "Perfect：%d, Good：%d, Max Combo：%d\n",
        found->perfect, found->good, found->max_combo
    );

    return true;
}
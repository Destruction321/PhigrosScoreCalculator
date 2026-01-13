/**
 * @file struct.h
 * 
 * @brief 公共头文�? * 
 * 提供了一些公共的宏定义和数据结构
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef STRUCT_H
#define STRUCT_H

#include <stdlib.h>

/**
 * @brief 数据范围
 * 
 * 包含一些具有特殊用途的最小�? */
enum scope {
    MIN_NOTE = 1,
    MAX_NOTE = 5000,
    MIN_SCORE = 0,
    MAX_SCORE = 1000000,
    CHANGE_NOTE = -1, ///< 输入目标分数（goal）时，输�?-1 修改 note �?    TO_FILE = 0,      ///< 选择方案时，输入 0 直接写入文件
};

/**
 * @brief 公式参数
 * 
 * 参数来源见README.md
 */
enum formula_factor {
    PERFECT_FACTOR = 180,
    GOOD_FACTOR = 117,
    MAX_COMBO_FACTOR = 20,
    BASIC_ADDED = 9,
};

/**
 * @brief clear 函数不同情况下删除的行数
 */
enum clear_lines {
    //! ========================================    //! 你是否要退出？/确认要退出吗�?    //! ================================    //! �?Enter"以退出，输入其他任意字符继续�?    NOT_EXIT = 4,

    //! 请输入歌曲目标方案（1 �?n 之间的整数）�?    EXIT = 1,

    //! ========================================    //! 请输入歌曲目标方案（1 �?n 之间的整数）�?    ERROR_CHOICE = 2,
};

/**
 * @brief 输出方案时的各种选择
 */
enum choice {
    INITIALISE_CHOICE = 0,
    WRITE_FILE = 48,          ///< '0'
    CHOOSE_SOLUTION = 49,     ///< '1'
    PRINT_ALL_SOLUTIONS = 50, ///< '2'
    PRINT_TEN_SOLUTIONS = 51, ///< '3'
    TEN = 10,
};

/**
 * @brief 分割线长�? */
enum dividing_line_length {
    LONG = 71,     ///< 无效的目标分数，请输入一�?1 �?1000000 之间的整数，�?Enter"结束输入�?    MEDIUM = 47,   ///< �?Enter"开始新一轮查找，输入其他任意字符退出：
    SHORT = 39,    ///< �?Enter"以开始，输入其他任意字符退出：
    ASTERISK = 33, ///< *********************************
};

/**
 * @brief 文件路径长度限制
 */
enum file {
    FILE_LENGTH = 200,          ///< 文件路径最大长�?    SUBFOLDER_NAME_LENGTH = 27, ///< 子文件夹路径最大长�?    FOLDER_NAME_LAST = 25,      ///< SUBFOLDER_NAME_LENGTH - 2, 最后一个有效字符位的后一�?};

/**
 * @brief 状态码
 */
typedef enum status {
    SUCCESS = 0,
    FAILURE = -1,
    ENTER = 10, ///< '\\n'�?ASCII 码�?} Status;

/**
 * @brief 歌曲参数结构
 */
typedef struct song {
    int note; ///< 歌曲 note 总数�?~5000 之间的整�?    int goal; ///< 歌曲目标分数�?~1000000 之间的整数，输入 -1 可修�?note
} Song;

/**
 * @brief 方案组合参数结构
 */
typedef struct solution {
    int perfect;
    int good;
    int max_combo;
} s_list;

/**
 * @brief 输入检查参数结�? */
typedef struct input_check {
    int min;
    int max;
    const char* name; ///< 当前输入数据的名�?    int compare_name_with_solution;
    int compare_name_with_goal;
} Input;

/**
 * @brief 算法辅助参数结构
 */
typedef struct auxiliary_arguments {
    double perfect_score; ///< 单个 perfect 的分�?    double good_score;    ///< 单个 good 的分�?    double delta_score;   ///< perfect_score - good_score
} Args;

/**
 * @brief 循环参数结构
 */
typedef struct loop {
    int loop_perfect;
    int loop_good;
    int loop_max_combo;
} Loop;

/**
 * @file struct.h
 * 
 * @brief 公共头文�?
 * 
 * 提供了一些公共的宏定义和数据结构
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef STRUCT_H
#define STRUCT_H

#include <stdlib.h>

/**
 * @brief 数据范围
 * 
 * 包含一些具有特殊用途的最小�?
 */
enum scope {
    MIN_NOTE = 1,
    MAX_NOTE = 5000,
    MIN_SCORE = 0,
    MAX_SCORE = 1000000,
    CHANGE_NOTE = -1, ///< 输入目标分数（goal）时，输�?-1 修改 note �?
    TO_FILE = 0,      ///< 选择方案时，输入 0 直接写入文件
};

/**
 * @brief 公式参数
 * 
 * 参数来源见README.md
 */
enum formula_factor {
    PERFECT_FACTOR = 180,
    GOOD_FACTOR = 117,
    MAX_COMBO_FACTOR = 20,
    BASIC_ADDED = 9,
};

/**
 * @brief clear 函数不同情况下删除的行数
 */
enum clear_lines {
    //! ========================================    //! 你是否要退出？/确认要退出吗�?
    //! ================================    //! �?Enter"以退出，输入其他任意字符继续�?
    NOT_EXIT = 4,

    //! 请输入歌曲目标方案（1 �?n 之间的整数）�?
    EXIT = 1,

    //! ========================================    //! 请输入歌曲目标方案（1 �?n 之间的整数）�?
    ERROR_CHOICE = 2,
};

/**
 * @brief 输出方案时的各种选择
 */
enum choice {
    INITIALISE_CHOICE = 0,
    WRITE_FILE = 48,          ///< '0'
    CHOOSE_SOLUTION = 49,     ///< '1'
    PRINT_ALL_SOLUTIONS = 50, ///< '2'
    PRINT_TEN_SOLUTIONS = 51, ///< '3'
    TEN = 10,
};

/**
 * @brief 分割线长�?
 */
enum dividing_line_length {
    LONG = 71,     ///< 无效的目标分数，请输入一�?1 �?1000000 之间的整数，�?Enter"结束输入�?
    MEDIUM = 47,   ///< �?Enter"开始新一轮查找，输入其他任意字符退出：
    SHORT = 39,    ///< �?Enter"以开始，输入其他任意字符退出：
    ASTERISK = 33, ///< *********************************
};

/**
 * @brief 文件路径长度限制
 */
enum file {
    FILE_LENGTH = 200,          ///< 文件路径最大长�?
    SUBFOLDER_NAME_LENGTH = 27, ///< 子文件夹路径最大长�?
    FOLDER_NAME_LAST = 25,      ///< SUBFOLDER_NAME_LENGTH - 2, 最后一个有效字符位的后一�?
};

/**
 * @brief 状态码
 */
typedef enum status {
    SUCCESS = 0,
    FAILURE = -1,
    ENTER = 10, ///< '\\n'�?ASCII 码�?
} Status;

/**
 * @brief 歌曲参数结构
 */
typedef struct song {
    int note; ///< 歌曲 note 总数�?~5000 之间的整�?
    int goal; ///< 歌曲目标分数�?~1000000 之间的整数，输入 -1 可修�?note
} Song;

/**
 * @brief 方案组合参数结构
 */
typedef struct solution {
    int perfect;
    int good;
    int max_combo;
} s_list;

/**
 * @brief 输入检查参数结�?
 */
typedef struct input_check {
    int min;
    int max;
    const char* name; ///< 当前输入数据的名�?
    int compare_name_with_solution;
    int compare_name_with_goal;
} Input;

/**
 * @brief 算法辅助参数结构
 */
typedef struct auxiliary_arguments {
    double perfect_score; ///< 单个 perfect 的分�?
    double good_score;    ///< 单个 good 的分�?
    double delta_score;   ///< perfect_score - good_score
} Args;

/**
 * @brief 循环参数结构
 */
typedef struct loop {
    int loop_perfect;
    int loop_good;
    int loop_max_combo;
} Loop;

#endif ///< STRUCT_H
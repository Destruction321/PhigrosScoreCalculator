/**
 * @file struct.h
 * 
 * @brief 公共头文件
 * 
 * 提供了一些公共的常量定义和数据结构
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef STRUCT_H
#define STRUCT_H

/**
 * @brief 数据范围
 * 
 * 包含一些具有特殊用途的最小值
 */
enum scope {
    MIN_NOTE = 1,
    MAX_NOTE = 5000,
    MIN_SCORE = 0,
    MAX_SCORE = 1000000,
    CHANGE_NOTE = -1, ///< 输入目标分数（goal）时，输入 -1 修改 note 数
    TO_FILE = 0,      ///< 选择方案时，输入 0 直接写入文件
};


/**
 * @brief 状态码
 */
typedef enum status {
    SUCCESS = 0,
    FAILURE = -1,
    ENTER = 10, ///< '\\n'的 ASCII 码值
} Status;

/**
 * @brief 歌曲参数结构
 */
typedef struct song {
    int note; ///< 歌曲 note 总数，1~5000 之间的整数
    int goal; ///< 歌曲目标分数，0~1000000 之间的整数，输入 -1 可修改 note
} Song;

/**
 * @brief 方案组合参数结构
 */
typedef struct solution {
    int perfect;
    int good;
    int max_combo;
} sol_t;

/**
 * @brief 输入检查参数结构
 */
typedef struct input_check {
    int min;
    int max;
    const char* name; ///< 当前输入数据的名称
    int compare_name_with_solution;
    int compare_name_with_goal;
} Input;

#endif ///< STRUCT_H
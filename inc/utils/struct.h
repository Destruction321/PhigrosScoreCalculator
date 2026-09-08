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
 */

#ifndef STRUCT_H
#define STRUCT_H

/**
 * @brief 数据范围
 * 
 * 也包含一些具有特殊用途的数值
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
 * @brief 单次整数读取的状态码
 */
typedef enum status {
    SUCCESS = 0,
    FAILURE = -1,
    INPUT_END = 11, ///< 输入流结束
    INPUT_ERROR = 12, ///< 输入读取失败
    ENTER = 10, ///< 换行符的 ASCII 码值，表示直接按回车
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
    int perfect;   ///< Perfect 数量
    int good;      ///< Good 数量
    int max_combo; ///< 最大连击数
} sol_t;

/**
 * @brief 输入数据的类型，与显示给用户的名称无关
 */
typedef enum input_kind {
    INPUT_NOTE,     ///< 输入歌曲 note 总数
    INPUT_GOAL,     ///< 输入目标分数
    INPUT_SOLUTION, ///< 输入目标方案编号
} InputKind;

/**
 * @brief 输入检查参数结构
 */
typedef struct input_check {
    int min; ///< 常规输入下限，特殊操作值由 get_min 处理
    int max; ///< 输入上限
    const char* name; ///< 显示给用户的名称，不参与输入类型判断
    InputKind kind; ///< 当前输入类型
} Input;

/**
 * @brief 跨模块流程结果，区分完成、退出和失败
 */
typedef enum result {
    RESULT_OK,     ///< 正常完成，未找到方案也属于正常完成
    RESULT_CANCEL, ///< 用户退出或输入流结束
    RESULT_ERROR,  ///< 内存分配或输入输出失败
} Result;

#endif ///< STRUCT_H

/**
 * @file print.h
 * 
 * @brief 信息打印相关函数定义
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 */

#ifndef PRINT_H
#define PRINT_H

#include <stdbool.h>
#include "struct.h"

/**
 * @brief clear 函数不同情况下删除的行数
 */
enum clear_lines {
    /// ===============================================
    /// 你是否要退出？/确认要退出吗？
    ///
    /// =======================================
    /// 按"Enter"以退出，输入其他任意字符继续：
    NOT_EXIT = 4,

    /// 请输入歌曲目标方案（1 到 n 之间的整数）：
    EXIT = 1,

    /// ===============================================
    /// 请输入歌曲目标方案（1 到 n 之间的整数）：
    ERROR_CHOICE = 2,
};

/**
 * @brief 启动确认
 * 
 * @return int 确认字符；输入流结束或读取失败时返回 EOF
 */
int start_confirmation(void);

/**
 * @brief 退出确认
 * 
 * @return true 退出；
 * @return false 开启下一轮
 */
bool exit_confirmation(void);

/**
 * @brief 打印输入提示
 * 
 * @param[in] input 输入检查参数
 * @param[in] song 歌曲参数
 */
void print_tip(Input input, Song* song);

/**
 * @brief 直接退出确认
 * 
 * @param[in] input 输入检查参数
 * @param[in] song 保留的歌曲参数，当前实现未使用
 * @return true 确认退出，或首次确认时遇到 EOF；
 * @return false 不退出
 */
bool direct_exit(Input input, Song* song);

/**
 * @brief 输出错误信息
 * 
 * @param[in] input 输入检查参数
 * @param[in] note 输入数据为 goal 时，提供当前 note 数以判断是否需要修改
 */
void error_message(Input input, int note);

/**
 * @brief 根据当前输入类型清除提示区域
 * 
 * @param[in] kind 当前输入类型
 * @param[in] times 选择方案时清除的行数，其余输入类型直接清屏
 */
void clear(InputKind kind, int times);

/**
 * @brief 表头
 * 
 * @param[in] song 歌曲参数
 */
void table_header(Song* song);

#endif ///< PRINT_H

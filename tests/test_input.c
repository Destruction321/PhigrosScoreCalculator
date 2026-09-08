/**
 * @file test_input.c
 *
 * @brief 验证输入类型与显示名称分离后的特殊选项和范围检查
 *
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 */

#include "data_setting.h"

#include <assert.h>

/**
 * @brief 使用测试脚本提供的输入验证类型判断
 *
 * @return int 所有断言通过时返回 0
 * @note 标准输入依次提供 -1、0、0、1；最后一个 0 应被音符数范围拒绝
 */
int main(void) {
    Song song = {0};
    int value = 99;
    assert(set_data(&value, init_input((InputKind)-1, 0), &song) == RESULT_ERROR);
    assert(set_data(&value, init_input(INPUT_SOLUTION, 0), &song) == RESULT_ERROR);
    assert(value == 99);

    ///< 故意使用另一种输入的显示名称，判断仍须以 kind 为准
    Input goal = init_input(INPUT_GOAL, 0);
    goal.name = "目标方案";
    assert(set_data(&value, goal, &song) == RESULT_OK);
    assert(value == CHANGE_NOTE);

    Input solution = init_input(INPUT_SOLUTION, 10);
    solution.name = "随便选个方案";
    assert(set_data(&value, solution, &song) == RESULT_OK);
    assert(value == TO_FILE);

    Input note = init_input(INPUT_NOTE, 0);
    note.name = "目标分数";
    assert(set_data(&value, note, &song) == RESULT_OK);
    assert(value == MIN_NOTE);
    return 0;
}

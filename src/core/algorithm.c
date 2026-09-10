/**
 * @file algorithm.c
 * 
 * @brief 算法相关函数实现
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 */

#include "algorithm.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "print.h"
#include "struct.h"
#include "tools.h"
#include "dynamic_array.h"
#include "choosing.h"

static const double ACCURACY_SCORE = 900000.0; ///< 判定分
static double factor = 0; ///< 公式系数（5000 / note）

/**
 * @brief 算法辅助参数结构
 */
typedef struct auxiliary_arguments {
    double perfect_score; ///< 单个 perfect 的分值
    double good_score;    ///< 单个 good 的分值
    double delta_score;   ///< perfect_score - good_score
} Args;

/**
 * @brief 查找过程中的当前判定与连击组合
 */
typedef struct loop {
    int loop_perfect;   ///< 当前尝试的 Perfect 数量
    int loop_good;      ///< 当前尝试的 Good 数量
    int loop_max_combo; ///< 当前尝试的最大连击数
} Loop;


/**
 * @brief 计分公式与组合递推系数
 *
 * 公式推导见 MORE_DETAILS.md。
 */
enum formula_factor {
    PERFECT_FACTOR = 180,
    GOOD_FACTOR = 117,
    MAX_COMBO_FACTOR = 20,
    BASIC_ADDED = 9,
};

static Result algorithm(Song* song);
static void init_args(Song* song, Args* args);
static Result get_solutions(Song* song, Args args);
static bool get_first_solution(Song* song, Args args, Loop* loop);
static bool get_first_perfect(Song* song, Args args, Loop* loop);
static bool get_first_good(Song* song, Loop* loop);
static bool get_first_max_combo(Song* song, Loop* loop);
static int calculate_score(int perfect, int good, int max_combo);
static bool get_other_solutions(int note, Loop loop, Array* array);
static int update_data(Loop* loop, int last_good);
static bool data_is_valid(Loop loop, int note);
static bool append_solution(Array* array, Loop loop);


Result seek_solution(Song* song) {
    ///< 特殊分数枚举
    enum {
        ALL_PERFECT = 1000000,
        ALL_GOOD = 685000,
        ALL_BAD_AND_MISS = 0,
    };

    table_header(song);
    switch (song->goal) {
    case ALL_BAD_AND_MISS:
        fputs("放置即可。", stdout);
        break;

    case ALL_PERFECT:
        fputs("获得 AP 即可。", stdout);
        break;

    case ALL_GOOD:
        fputs("全部 good 即可。", stdout);
        break;

    default:
        return algorithm(song); ///< 不是特殊分数，进入计算流程
    }
    return RESULT_OK;
}

/**
 * @brief 检查分数范围并进入方案查找
 *
 * @param[in] song 歌曲参数地址
 * @retval RESULT_OK 正常完成，包括未找到方案
 * @retval RESULT_CANCEL 用户退出或输入流结束
 * @retval RESULT_ERROR 内存分配或输入输出失败
 */
static Result algorithm(Song* song) {
    Args args = { 0 };
    init_args(song, &args);

    ///< 过滤不合理数据
    if (args.delta_score == 0.0) {
        return RESULT_OK;
    }
    return get_solutions(song, args);
}

/**
 * @brief 初始化单个判定分值并检查目标分数
 *
 * @param[in] song 歌曲参数地址
 * @param[out] args 算法参数地址；目标超出检查范围时将 delta_score 置零
 */
static void init_args(Song* song, Args* args) {
    static const double GOOD_RATIO = 0.65;
    static const int SCALE_FACTOR = 1000;

    ///< 先按比例放大判定分值，保留现有计算顺序
    args->perfect_score = SCALE_FACTOR * ACCURACY_SCORE / (song->note);
    args->good_score = args->perfect_score * GOOD_RATIO;

    ///< 除回比例系数，恢复单个判定的分值
    args->perfect_score /= SCALE_FACTOR;
    args->good_score /= SCALE_FACTOR;

    ///< 校验
    args->delta_score = args->perfect_score - args->good_score;
    if ((MAX_SCORE - song->goal < args->delta_score) || (song->goal < args->good_score)) {
        clear_and_print(
            "不存在可使 note 总数为 %d 的谱面打出 %d 分的方案。\n",
            song->note, song->goal
        );
        args->delta_score = 0.0;
    }
}

/**
 * @brief 查找方案并管理本轮数组的生命周期
 *
 * @param[in] song 歌曲参数地址
 * @param[in] args 已初始化的算法参数
 * @retval RESULT_OK 正常完成，包括未找到方案
 * @retval RESULT_CANCEL 用户退出或输入流结束
 * @retval RESULT_ERROR 内存分配或输入输出失败
 * @note 所有已创建数组的返回路径均经过 cleanup，不向其他模块转移所有权
 */
static Result get_solutions(Song* song, Args args) {
    Array array = {0}; ///< 本轮持有的数组，其他模块仅借用
    Result result = RESULT_ERROR;
    if (!array_init(&array, 1000, sizeof(sol_t))) {
        goto cleanup;
    }

    fputs("计算中……\n", stdout);
    Loop loop = {0};
    if (!get_first_solution(song, args, &loop)) {
        clear_and_print(
            "未找到可使 note 总数为 %d 的谱面打出 %d 分的方案。\n",
            song->note, song->goal
        );
        result = RESULT_OK;
        goto cleanup;
    }

    if (!append_solution(&array, loop)
        || !get_other_solutions(song->note, loop, &array)
    ) {
        goto cleanup;
    }
    result = choose(song, &array);

cleanup:
    ///< 无解、失败、用户退出和正常完成均在此释放数组
    array_destroy(&array);
    return result;
}

/**
 * @brief 将内部计算组合转换成方案并加入数组
 *
 * @param[in,out] array 方案数组地址，必要时扩容
 * @param[in] loop 当前计算组合
 * @return true 添加成功
 * @return false 添加失败，保留原数组内容
 */
static bool append_solution(Array* array, Loop loop) {
    sol_t solution = {loop.loop_perfect, loop.loop_good, loop.loop_max_combo};
    return array_push(array, &solution);
}

/**
 * @brief 初始化查找起点并寻找第一个方案
 *
 * @param[in] song 歌曲参数地址
 * @param[in] args 算法辅助参数
 * @param[out] loop 查找状态，成功时存放第一个可行组合
 * @return true 找到方案
 * @return false 未找到方案
 * @note 本函数不写入数组，由调用者添加方案
 */
static bool get_first_solution(Song* song, Args args, Loop* loop) {
    factor = 5000.0 / song->note;

    loop->loop_perfect = (
        (song->goal > ACCURACY_SCORE)
        ? (song->note - 1)
        : (int)(song->goal / args.perfect_score)
    );
    return get_first_perfect(song, args, loop);
}

/**
 * @brief 从当前上限递减 perfect 并查找组合
 *
 * @param[in] song 歌曲参数地址
 * @param[in] args 算法辅助参数
 * @param[in,out] loop 当前查找状态
 * @return true 找到可行组合
 * @return false 未找到可行组合
 */
static bool get_first_perfect(Song* song, Args args, Loop* loop) {
    do {
        if (song->goal - loop->loop_perfect * args.perfect_score < args.delta_score) {
            continue;
        }
        /**
         * 假设 FULL COMBO
         * 则 good = note - perfect，max_combo = note
         */
        int score = calculate_score(
            loop->loop_perfect,
            song->note - loop->loop_perfect,
            song->note
        );
        if (score < song->goal) {
            break;
        }
        ///< 从 good 为 0 开始检查当前 perfect 下的组合
        loop->loop_good = 0;
        if (get_first_good(song, loop)) {
            return true;
        }
    } while (--(loop->loop_perfect) >= 0);
    return false;
}

/**
 * @brief 固定 perfect，递增 good 并检查最大连击
 *
 * @param[in] song 歌曲参数地址
 * @param[in,out] loop 当前查找状态
 * @return true 找到可行组合
 * @return false 当前 perfect 下未找到可行组合
 */
static bool get_first_good(Song* song, Loop* loop) {
    do {
        /**
         * 假设所有 perfect 和 good 全部连击，中间未断连
         * 则 max_combo = perfect + good
         */
        int score = calculate_score(
            loop->loop_perfect,
            loop->loop_good,
            loop->loop_perfect + loop->loop_good
        );
        if (score < song->goal) {
            continue;
        }

        ///< 假设 max_combo = 1
        score = calculate_score(loop->loop_perfect, loop->loop_good, 1);
        if (score > song->goal) {
            break;
        }

        ///< 反算最大连击并检查组合
        if (get_first_max_combo(song, loop)) {
            return true;
        }
    } while (++(loop->loop_good) <= song->note - loop->loop_perfect);
    return false;
}

/**
 * @brief 反算最大连击并验证分数和断连数量
 *
 * @param[in] song 歌曲参数地址
 * @param[in,out] loop 已有 perfect 和 good，写入反算的最大连击
 * @return true 当前组合可行
 * @return false 连击、断连数量或分数不符合要求
 */
static bool get_first_max_combo(Song* song, Loop* loop) {
    loop->loop_max_combo = (
        (int)round((
            song->goal / factor
            - PERFECT_FACTOR * loop->loop_perfect
            - GOOD_FACTOR * loop->loop_good
        ) / MAX_COMBO_FACTOR)
    );
    if (loop->loop_max_combo <= 0 ||
        loop->loop_max_combo > loop->loop_perfect + loop->loop_good) {
        return false;
    }
    int bad_and_miss = (
        (loop->loop_perfect + loop->loop_good - 1) / loop->loop_max_combo
    );

    int real_bad_and_miss = song->note - loop->loop_perfect - loop->loop_good;
    if (bad_and_miss > real_bad_and_miss) {
        return false;
    }

    int score = calculate_score(
        loop->loop_perfect,
        loop->loop_good,
        loop->loop_max_combo
    );

    if (score != song->goal) {
        return false;
    }
    return true;
}

/**
 * @brief 根据当前谱面的比例系数计算整数分数
 *
 * @param[in] perfect Perfect 数量
 * @param[in] good Good 数量
 * @param[in] max_combo 最大连击数
 * @return int 四舍五入后的分数
 * @note factor 由 get_first_solution 按本轮 note 数设置
 */
static int calculate_score(int perfect, int good, int max_combo) {
    return (int)round(
        factor * (
            PERFECT_FACTOR * perfect + GOOD_FACTOR * good + MAX_COMBO_FACTOR * max_combo
        )
    );
}

/**
 * @brief 从第一个方案继续递推并保存其他方案
 *
 * @param[in] note 歌曲 note 总数
 * @param[in] loop 第一个可行组合的副本
 * @param[in,out] array 已存有第一个方案的数组
 * @return true 完成查找
 * @return false 添加方案失败，已添加内容由调用者统一释放
 */
static bool get_other_solutions(int note, Loop loop, Array* array) {
    int last_good = loop.loop_good;
    while ((loop.loop_perfect >= 0) && (last_good <= note)) {
        if (loop.loop_max_combo <= GOOD_FACTOR) {
            last_good = update_data(&loop, last_good);
        } else {
            loop.loop_good += MAX_COMBO_FACTOR;
            loop.loop_max_combo -= GOOD_FACTOR;
        }
        
        if (data_is_valid(loop, note)) {
            if (!append_solution(array, loop)) {
                return false;
            }
        }
    }
    return true;
}

/**
 * @brief 递减 perfect 并回调 good 与最大连击
 *
 * @param[in,out] loop 当前计算组合
 * @param[in] last_good 上一轮 good 的起点
 * @return int 更新后的 good 起点
 */
static int update_data(Loop* loop, int last_good) {
    do {
        --(loop->loop_perfect);
        loop->loop_max_combo += (
            BASIC_ADDED + GOOD_FACTOR * (loop->loop_good - last_good) / MAX_COMBO_FACTOR
        );

        if (loop->loop_max_combo > loop->loop_perfect + loop->loop_good) {
            last_good += MAX_COMBO_FACTOR;
            loop->loop_max_combo -= GOOD_FACTOR;
        }
        loop->loop_good = last_good;
    } while (loop->loop_max_combo <= 0);
    return last_good;
}

/**
 * @brief 检查递推组合是否满足音符数和连击约束
 *
 * @param[in] loop 待检查组合，调用者保证最大连击大于 0
 * @param[in] note 歌曲 note 总数
 * @return true 通过约束检查
 * @return false 未通过约束检查
 */
static bool data_is_valid(Loop loop, int note) {
    int perfect_and_good = loop.loop_perfect + loop.loop_good;
    int bad_and_miss = (perfect_and_good - 1) / loop.loop_max_combo;
    int min_note = perfect_and_good + bad_and_miss;

    return (
        (loop.loop_max_combo <= perfect_and_good)
        && (min_note <= note)
        && (loop.loop_perfect >= 0)
    );
}

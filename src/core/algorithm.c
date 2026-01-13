/**
 * @file algorithm.c
 * 
 * @brief 算法相关函数实现
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 * 
 * @copyright Copyright (c) 2025
 */
#include <math.h>

#include "algorithm.h"
#include "print.h"
#include "struct.h"
#include "tools.h"
#include "dynamic_array.h"
#include "choosing.h"

static const double ACCURACY_SCORE = 900000.0; ///< 判定分
static double factor = 0; ///< 公式系数（5000 / note）


bool seek_solution(Song* song) {
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
    return true;
}

bool algorithm(Song* song) {
    Args args = { 0 };
    init_args(song, &args);

    ///< 过滤不合理数据
    if (args.delta_score == 0.0) {
        return true;
    }
    return get_solutions(song, args);
}

void init_args(Song* song, Args* args) {
    static const double GOOD_RATIO = 0.65;
    static const int SCALE_FACTOR = 1000;

    ///< 取到更多小数位，增加精度
    args->perfect_score = SCALE_FACTOR * ACCURACY_SCORE / (song->note);
    args->good_score = args->perfect_score * GOOD_RATIO;

    ///< 还原精度
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

bool get_solutions(Song* song, Args args) {
    static const int SIZE = 1000; ///< 动态数组初始大小
    array = INITIALIZE_ARRAY(SIZE, s_list);
    fputs("计算中……\n", stdout);

    Loop loop = { 0 };
    if (get_first_solution(song, args, &loop)) {
        get_other_solutions(song->note, loop);
        choosing_tips(array->num);
        bool code = choose(song);
        Free(array);
        return code;
    }

    clear_and_print(
        "未找到可使 note 总数为 %d 的谱面打出 %d 分的方案。\n",
        song->note, song->goal
    );
    return true;
}

bool get_first_solution(Song* song, Args args, Loop* loop) {
    factor = 5000.0 / song->note;

    loop->loop_perfect = (
        (song->goal > ACCURACY_SCORE)
        ? (song->note - 1)
        : (int)(song->goal / args.perfect_score)
    );
    
    return get_first_perfect(song, args, loop);
}

bool get_first_perfect(Song* song, Args args, Loop* loop) {
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
        ///< get_first_good
        loop->loop_good = 0;
        if (get_first_good(song, loop)) {
            return true;
        }
    } while (--(loop->loop_perfect) >= 0);
    return false;
}

bool get_first_good(Song* song, Loop* loop) {
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

        ///< get_first_max_combo
        loop->loop_max_combo = loop->loop_perfect + loop->loop_good;
        if (get_first_max_combo(song, loop)) {
            return true;
        }

    } while (++(loop->loop_good) <= song->note - loop->loop_perfect);

    return false;
}

bool get_first_max_combo(Song* song, Loop* loop) {
    do {
        int bad_and_miss = (
            (loop->loop_perfect + loop->loop_good - 1) / loop->loop_max_combo
        );

        if (bad_and_miss > song->note - loop->loop_perfect - loop->loop_good) {
            break;
        }

        int score = calculate_score(
            loop->loop_perfect,
            loop->loop_good,
            loop->loop_max_combo
        );

        if (score != song->goal) {
            continue;
        }
        
        input_data(array, loop); ///< 找到第一个方案，加入数组
        return true;

    } while (--(loop->loop_max_combo) > 0);

    return false;
}

int calculate_score(int perfect, int good, int max_combo) {
    return (int)round(
        factor * (
            PERFECT_FACTOR * perfect + GOOD_FACTOR * good + MAX_COMBO_FACTOR * max_combo
        )
    );
}

void get_other_solutions(int note, Loop loop) {
    int last_good = loop.loop_good;
    while ((loop.loop_perfect >= 0) && (last_good <= note)) {
        if (loop.loop_max_combo <= GOOD_FACTOR) {
            last_good = update_data(&loop, last_good);
        } else {
            loop.loop_good += MAX_COMBO_FACTOR;
            loop.loop_max_combo -= GOOD_FACTOR;
        }
        if (is_valid(loop, note)) {
            input_data(array, &loop);
        }
    }
}

int update_data(Loop* loop, int last_good) {
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

bool is_valid(Loop loop, int note) {
    int perfect_and_good = loop.loop_perfect + loop.loop_good;
    int bad_and_miss = (perfect_and_good - 1) / loop.loop_max_combo;
    int min_note = perfect_and_good + bad_and_miss;
    return (
        (loop.loop_max_combo <= perfect_and_good)
        && (min_note <= note)
        && (loop.loop_perfect >= 0)
    );
}

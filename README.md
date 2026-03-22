# 关于本程序的说明

- **注：本文档中的公式在 GitHub 中无法正常显示，请克隆到本地后读取**

## 一、关于本程序中使用的 ANSI 转义码的说明

1. `\033`：八进制 **27**，对应 ASCII 码的 **“ESC”**；
2. `\033[nF`：光标上移$n$行；
3. `\033[B`：光标下移一行；
4. `\033[nG`：光标移动到第$n$列；
5. `\033[2K`：删除本行；
6. `\033[2J`：清屏；
7. `\033[H`：光标移动到控制台左上角。

## 二、关于算法的说明

1. ### 基本公式

    1. 歌曲分数：

        $Phigros$ 的**歌曲分数** ($score$) 由最高 $900000$ 分的**判定分** ($accuracy\_score$) 与最高 $100000$ 分的**连击分** ($combo\_score$​) 组成，即：

        $$
        score = accuracy\_score + combo\_score \tag{1}
        $$

    2. 判定分：

        设歌曲的**总音符数**为 $note$​，则：
        1. 单个 $perfect$ 判定分 ($perfect\_score$​) 的计算公式为：

            $$
            perfect\_score = \frac{900000}{note} \tag{2}
            $$

        2. $good$ 判定分 ($good\_score$​) 为$perfect$判定分的 $65\%$，即：

            $$
            good\_score = 0.65 \ perfect\_score \tag{3}
            $$

        3. 总判定分为：

            $$
            accuracy\_score = perfect \cdot perfect\_score + good \cdot good\_score \tag{4}
            $$

    3. 连击分：

        设**最大连击数**为 $max\_combo$，则：

        $$
        combo\_score = \frac{100000}{note} \cdot max\_combo \tag{5}
        $$

2. ### 算法原理

    将 $(2)$ 代入 $(3)$ 得：

    $$
    good\_score = 0.65 \ perfect\_score = \frac{585000}{note} \tag{6}
    $$

    将 $(2)(6)$ 代入 $(4)$ 得：

    $$
    \begin{aligned}
        accuracy\_score
            &= perfect \cdot perfect\_score + good \cdot good\_score\\
            &= \frac{900000}{note} \cdot perfect + \frac{585000}{note} \cdot good
        \tag{7}
    \end{aligned}
    $$

    将 $(5)(7)$ 代入 $(1)$ 得：

    $$
    \begin{aligned}
        score
            &= accuracy\_score + combo\_score\\
            &= \frac{900000}{note} \cdot perfect
             + \frac{585000}{note} \cdot good
             + \frac{100000}{note} \cdot {max\_combo}\\
            &= \frac{5000}{note} \ (180 \ perfect + 117 \ good + 20 \ max\_combo)
        \tag{8}
    \end{aligned}
    $$

    式$(8)$中的 $note$ 和 $score$ 为用户输入的参数，设 $score$ 为**目标分数** $goal$ ，$\frac{5000}{note}$ 为**比例系数** $k$​，则有：

    $$
    \begin{cases}
        goal = k \ (180 \ perfect + 117 \ good + 20 \ max\_combo)\\
        k = \frac{5000}{note} \tag{9}
    \end{cases}
    $$

    式$(9)$便是算法的原理公式。

3. ### 算法设计

    根据 $Phigros$ 当前的游戏情况，对于式$(9)$中的各参数，我们可得出以下限制条件：

    $$
    \begin{cases}
        note \in [1,\ 5000]\\
        goal \in \{0\} \cup [good\_score, \ 1000000 - (perfect\_score - good\_score)] \cup \{1000000\}\\
        perfect \in [0, \ min\{note, \frac{goal}{perfect\_score}\}]\\
        \begin{cases}
            max\_combo \ge 1\\
            \lceil \frac{note}{max\_combo} \rceil - 1 \le note - perfect - good \tag{10}
        \end{cases}
    \end{cases}
    $$

    注：**所有参数均为整数。**

    根据式$(9)$和条件$(10)$，可设计算法如下：
    1. 数据结构设计：

        设计**输入检查**、**方案组合**、**算法**、**歌曲**五个参数结构，并使用动态数组存储方案。其中歌曲参数包括用户输入的 $note$、$goal$；算法参数包含了计算过程中用到的各个辅助参数；方案组合参数负责存储满足式$(9)$与条件$(10)$的参数组合；输入检查参数检查用户输入的数据是否合法。具体结构如下：
        1. 输入检查参数结构：

            ```c
            typedef struct input_check {
                int min;
                int max;
                const char* name; ///< 当前输入数据的名称
                int compare_name_with_solution_list;
                int compare_name_with_goal;
            } Input;
            ```

        2. 方案组合参数结构：

            ```c
            typedef struct solution_list {
                int perfect;
                int good;
                int max_combo;
            } s_list;
            ```

        3. 算法参数结构：

            ```c
            typedef struct auxiliary_arguments {
                double perfect_score; ///< 单个 perfect 的分值
                double good_score;    ///< 单个 good 的分值
                double delta_score;   ///< perfect_score - good_score
            } Args;
            ```

        4. 歌曲参数结构：

            ```c
            typedef struct song {
                int note;    ///< 歌曲 note 总数
                int goal;    ///< 歌曲目标分数
            } Song;
            ```

    2. 寻找方案前的一些准备：
        1. $goal$​ 的取值范围中存在一些特殊分数，可提前过滤：

            ```c
            bool seek_solution(Song* song) {
                enum {
                    ALL_PERFECT = 1000000,
                    ALL_GOOD = 685000,
                    ALL_BAD_AND_MISS = 0
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
                    return algorithm(song);
                }
                return true;
            }
            ```

        2. 获取 $perfect\_score$ 和 $good\_score$：

            ```c
            void init_args(Song* song, Args* args) {
                static const double GOOD_RATIO = 0.65;
                static const int SCALE_FACTOR = 1000;

                ///< 取到更多小数位，增加精度
                args->perfect_score = SCALE_FACTOR * ACCURACY_SCORE / (song->note);
                args->good_score = args->perfect_score * GOOD_RATIO;

                ///< 还原精度
                args->perfect_score /= SCALE_FACTOR;
                args->good_score /= SCALE_FACTOR;
                args->delta_score = args->perfect_score - args->good_score;
                if (MAX_SCORE - song->goal < args->delta_score || song->goal < args->good_score) {
                    clear_and_print(
                        "不存在可使 note 总数为 %d 的谱面打出 %d 分的方案。",
                        song->note, song->goal
                    );
                    args->delta_score = 0.0;
                }
            }
            ```

    3. 寻找第一个符合条件的方案组合：
        1. 基本思路：使用双层层循环遍历 $perfect$，$good$，再反代回式(9)，解出对应的 $max\_combo$，验证是否满足条件(10)，直到找到第一个符合条件的参数组合为止。
            - 由于 $perfect$ 数和 $max\_combo$ 数较大的情况更符合直觉，所以 $perfect$ 和 $max\_combo$ 以其最大值作为起点，$good$ 以最小值作为起点；

        2. 第一层循环：寻找 $perfect$。
            - 若当前 $perfect$ 值对应的最大分数（即除 $perfect$ 以外都是 $good$）已经小于 $goal$，说明往后所有方案组合都不可能达到 $goal$，没有必要再找下去，标志着第一个方案已经找完。

            若：

            $\begin{cases}
                delta\_score \gets perfect\_score - good\_score\\
                perfect\_score \gets perfect \cdot perfect\_score\\
                score \gets k \ (63 \ perfect + 137 \ note) = k \ (180 \ perfect + 117 \ (note - perfect) + 20 \ note)
            \end{cases}$

            则具体逻辑如下图：

            ```mermaid
            graph TD
                Start([Start])
                init_perfect[perfect = max_perfect]
                check_perfect{perfect ≥ 0?}
                check_max_score{goal - perfect_score < delta_score?}
                check_min_score{score < goal?}
                get_first_good[get_first_good]
                get_solutions_list{get a solution_list?}
                set_perfect[perfect = perfect - 1]
                End([End])

                Start --> init_perfect --> check_perfect
                check_perfect -- no --> End
                check_perfect -- yes --> check_max_score
                check_max_score -- no --> check_min_score
                check_max_score -- yes --> set_perfect
                check_min_score -- yes --> End
                check_min_score -- no --> get_first_good --> get_solutions_list
                get_solutions_list -- yes --> End
                get_solutions_list -- no --> set_perfect
                set_perfect --> check_perfect
            ```

        3. 第二层循环：寻找 $good$，此时需要注意两点：
            1. 若当前 $perfect$ 和 $good$ 值对应的最大分数（即所有 $perfect$ 和 $good$ 判定一次性全部连击无断连）已经小于 $goal$，说明对于当前的 $perfect$ 和 $good$ 值，往后所有 $max\_combo$ 值都不可能达到 $goal$，此时需要换下一个 $good$ 值；

            2. 若当前 $perfect$ 和 $good$ 值对应的最小分数（即 $max\_combo = 1$）已经大于 $goal$，说明对于当前的 $perfect$ 值，往后所有方案组合都不可能达到 $goal$，没有必要再找下去，此时需要换下一个$perfect$值；

            若：

            $\begin{cases}
                max\_good \gets note - perfect\\
                max\_score \gets k \ (200 \ perfect + 137 \ good) = k \ (180 \ perfect + 117 \ good + 20 \ (perfect + good))\\
                min\_score \gets k \ (180 \ perfect + 117 \ good + 20)
            \end{cases}$

            则具体逻辑如下图：

            ```mermaid
            graph TD
                Start([Start])
                init_good[good = 0]
                check_good{good ≤ max_good?}
                check_max_score{max_score < goal?}
                check_min_score{min_score > goal?}
                get_first_max_combo[get_first_max_combo]
                get_solutions_list{get a solution_list?}
                set_good[good = good + 1]
                End([End])

                Start --> init_good --> check_good
                check_good -- no --> End
                check_good -- yes --> check_max_score
                check_max_score -- no --> check_min_score
                check_max_score -- yes --> set_good
                check_min_score -- yes --> End
                check_min_score -- no --> get_first_max_combo --> get_solutions_list
                get_solutions_list -- yes --> End
                get_solutions_list -- no --> set_good
                set_good --> check_good
            ```

        4. 第三层循环：寻找 $max\_combo$：

            经过实际计算，通过此方法算出的 $max\_combo$ 误差几乎全部出现在 3 位小数，因此直接四舍五入造成的精度损失不影响最终的计算，具体逻辑如下图：

            ```mermaid
            graph TD
                Start([Start])
                check_bad_and_miss{min_bad_and_miss > real_bad_and_miss?}
                check_score{score = goal?}
                add_solution_list[add_solution_list]
                End([End])

                Start --> check_bad_and_miss
                check_bad_and_miss -- yes --> End
                check_bad_and_miss -- no --> check_score
                check_score -- yes --> add_solution_list --> End
                check_score -- no --> End
            ```

    4. 寻找其它满足条件的方案组合

        根据式$(9)$，在 $note$ 和 $goal$ 的值不变的情况下，我们可得出 $perfect$、$good$、$max\_combo$ 之间有如下变化关系：

        $perfect$ 值不变时，若 $good$ 的值**增大**（或减小）$20$，对应 $max\_combo$ 的值**减小**（或增大）$117$。由于在寻找第一个方案组合时，$good$ 以最小值作为起点，故这里 $good$​ 值应增大，即：

        $$
        \begin{cases}
            good \gets good + 20\\
            max\_combo \gets max\_combo - 117 \tag{11}
        \end{cases}
        $$

        式$(11)$中，$max\_combo$ 必然会**递减至不大于** $117$ 的值，此时说明本次 $perfect$ 值对应方案组合已经找完，需要换下一个 $perfect$ 值进行寻找，即：

        $$
        perfect \gets perfect - 1 \tag{12}
        $$

        设上一个方案的 $good$ 值为 $last\_good$，则 $good$ 值需要回调至 $last\_good$ 来寻找新 $perfect$​ 值的方案组合，即：

        $$
        good \gets last\_good \tag{13}
        $$

        将式$(12)(13)$代入式$(9)$，设回调 $good$ 后的新的 $max\_combo$ 起始值为 $new\_max\_combo$，得：

        $$
        \begin{aligned}
            goal
                &= k \ (180 \ perfect + 117 \ good + 20 \ max\_combo)\\
                &= k \ [180 \ (perfect - 1) + 117 \ last\_good + 20 \ new\_max\_combo]
            \tag{14}\\
        \end{aligned}
        $$

        解式$(14)$，得：

        $$
        new\_max\_combo = max\_combo + 9 + 117 \cdot \frac {good - last\_good}{20} \tag{15}
        $$

        即：

        $$
        max\_combo \gets max\_combo + 9 + 117 \cdot \frac {good - last\_good}{20} \tag{16}
        $$

        在式$(16)$中，根据式$(11)$，有 $good - last\_good = 20n, n \in N$，则 $\frac {good - last\_good}{20} \ge 0$，得 $max\_combo$ 在每个 $perfect$ 周期查找完毕后都会比上一周期多出一个**基础值** $9$，而 $perfect$ 是**递减**的，$good$ 的值在每次周期结束后都会**重置为最初的值**，即 $last\_good$ ，也就是说，$max\_combo$ 的值**一定会在某一周期结束后超过** $perfect + good$ 的值，这与条件$(10)$矛盾。此时的 $last\_good$ 值已经不再有符合条件的方案组合，需要更新，同时也需要更新 $good$ 和 $max\_combo$ 的值，即：

        $$
        \begin{cases}
            last\_good \gets last\_good + 20\\
            good \gets last\_good\\
            max\_combo \gets max\_combo - 117 \tag{17}
        \end{cases}
        $$

        相对于 $max\_combo$ 每次递减的 $117$ 来说，基础值 $9$ 很小，做完上述操作后，$max\_combo$ **有小于 $0$ 的风险**，此时需重复式$(12)$ ~ 式$(16)$，直至 $max\_combo$ 恢复至大于 $0$。

    5. 将符合条件的方案组合加入至方案数组：

        在更新方案之前，我们需要先更新数据：

        ```c
        void update_data(Args* args) {
            args->perfect_and_good = args->loop_perfect + args->loop_good;
            args->bad_and_miss = (args->perfect_and_good - 1) / args->loop_max_combo;
            args->min_note = args->perfect_and_good + args->bad_and_miss;
        }
        ```

        根据分析，可能有三种非法方案：

        $$
        \begin{cases}
            max\_combo > perfect + good\\
            perfect < 0\\
            min\_note > note \tag{18}
        \end{cases}
        $$

        其中 $min\_note = perfect + good + \lceil \frac{perfect + good}{max\_combo} \rceil - 1$

        这三种情况分别对应：
        1. $max\_combo$ 超界；
        2. $perfect$ 自减下溢，这种情况标志着**所有方案都已找完**，没有必要再找下去；
        3. 达成该方案所需最小$note$数超出上界；

        排除以上情况后，添加方案的逻辑如下：

        ```c
        update_data(args);
        if (args->loop_max_combo > args->perfect_and_good
            || args->min_note > song->note
            || args->loop_perfect < 0) {
            continue;
        }
        tail = add_solution_list(tail, args); ///< 返回最后一个方案的地址，方便下次添加
        ```

    6. 查找结束：

        除 $perfect < 0$ 外，还有一种情况标志着方案已经找完：$last\_good > note$。该情况说明达成 $goal$ 所需最小 $good$ 数已经**超过**歌曲的 $note$ 总数，此时,以后的所有方案所需 $note$ 数都多于总 $note$ 数，没有继续查找的必要。

4. #### 结论

    综合上述分析，设 $delta\_good = good - last\_good$，可得寻找其他方案的逻辑如下：

    ```mermaid
    graph TD
        Start([Start])
        init_good_and_tail[
            last_good = good
            tail = Head->next
        ]
        check_perfect{
            perfect ≥ 0?
            last_good ≤ note?
        }
        check_117{max_combo > 117?}
        update_good[
            good = good + 20
            max_combo = max_combo - 117
        ]
        update_perfect[
            perfect = perfect - 1
            max_combo = max_combo + 9 + 117 · delta_good / 20
        ]
        check_max_combo{max_combo > perfect + good}
        update_last_good[
            last_good = last_good + 20
            max_combo = max_combo - 117
        ]
        init_good[good = last_good]
        check_negative{max_combo ≤ 0?}
        update_data[update_data]
        check_solution_list{
            min_note > note?
            OR mcb > perfect_and_good?
            OR perfect < 0?
        }
        add_solution_list[add_solution_list]
        End([End])

        Start --> init_good_and_tail --> check_perfect
        check_perfect -- yes --> check_117
        check_perfect -- no --> End
        check_117 -- yes --> update_good --> update_data
        check_117 -- no --> update_perfect
        check_max_combo -- yes --> update_last_good --> init_good
        check_max_combo -- no --> init_good
        init_good --> check_negative
        check_negative -- yes --> update_perfect
        check_negative -- no --> update_data
        update_perfect --> check_max_combo
        update_data --> check_solution_list
        check_solution_list -- no --> add_solution_list --> check_perfect
        check_solution_list -- yes --> check_perfect
    ```

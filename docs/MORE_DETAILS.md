# 关于本程序的说明

本文对照当前 `src/core/algorithm.c` 说明计分关系、查找过程和方案存储。头文件组织与资源管理另见 [CODE_STRUCTURE.md](CODE_STRUCTURE.md)。

公式表示数学关系；代码中的浮点运算、四舍五入和提前过滤会在对应位置单独说明。

## 一、关于本程序中使用的 ANSI 转义码的说明

1. `\033`：八进制 **27**，对应 ASCII 码的 **“ESC”**；
2. `\033[nF`：光标上移$n$行；
3. `\033[B`：光标下移一行；
4. `\033[nG`：光标移动到第$n$列；
5. `\033[2K`：删除本行；
6. `\033[2J`：清屏；
7. `\033[H`：光标移动到控制台左上角。

## 二、关于算法的说明

1. 基本公式

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

2. 算法原理

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

   用户输入音符总数 $note$ 与**目标分数** $goal$。先考虑式$(8)$中的原始分数恰好等于 $goal$ 的情况，设 $\frac{5000}{note}$ 为**比例系数** $k$，则有：

   $$
   \begin{cases}
       goal = k \ (180 \ perfect + 117 \ good + 20 \ max\_combo)\\
       k = \frac{5000}{note} \tag{9}
   \end{cases}
   $$

   式$(9)$表示原始分数恰好等于目标分数时的关系。当前 `calculate_score()` 实际返回的是对式$(8)$四舍五入后的整数，因此还应区分：

   $$
   \begin{aligned}
       S &= k(180P + 117G + 20C)\\
       score &= \operatorname{round}(S)
   \end{aligned}
   $$

   其中 $P$、$G$、$C$ 分别代表 `perfect`、`good`、`max_combo`。对于非负分数，数学上 `round(S) == goal` 对应 $goal - 0.5 \le S < goal + 0.5$，不能直接将它与 $S = goal$ 混为一谈。实现还使用了 `double`，边界判断需考虑浮点表示。

   后文的递推保持 $180P + 117G + 20C$ 不变，因此保持原始分数不变，也保持按该关系计算的整数分数不变。

3. 算法设计

   以下先说明组合应该满足的约束，再说明当前代码如何查找。

   1. 输入范围与组合约束

      程序接受 $note \in \lbrack 1,5000 \rbrack$、$goal \in \lbrack 0,1000000 \rbrack$ 的整数。这是程序的输入限制，不代表每个目标分数都有方案。

      设 $N=note$、$P=perfect$、$G=good$、$C=max\_combo$，并令 $H=P+G$。对于至少有一个 Perfect 或 Good 的组合，应满足：

      $$
      \begin{cases}
          P,G,C \in \mathbb{Z}\\
          P \ge 0,\quad G \ge 0,\quad 1 \le H \le N\\
          1 \le C \le H\\
          \left\lceil \dfrac{H}{C} \right\rceil - 1 \le N-H
      \end{cases}
      \tag{10}
      $$

      这里的 $N-H$ 是 Bad 与 Miss 的总数量。若最大连击为 $C$，需要把 $H$ 个能够续连的判定分成至少 $\lceil H/C\rceil$ 段；段与段之间至少需要一个断连判定，因此最少断连次数为：

      $$
      bad\_and\_miss_{\min}
      =\left\lceil \frac{P+G}{C}\right\rceil-1
      =\left\lfloor \frac{P+G-1}{C}\right\rfloor
      $$

      **分子是 Perfect 与 Good 的总数，不是谱面总音符数。** 对于非负分子和正分母，代码中的整数除法 `(perfect_and_good - 1) / max_combo` 就对应最后一个表达式。

      当 $P=G=0$ 时，最大连击为 0，属于单独处理的零分情况，不能代入上述除法。

      这些约束检查的是判定数量与连击组合，不生成具体谱面上的操作顺序。

   2. 数据结构与职责

      当前使用的数据结构及其职责如下：

      | 类型        | 定义位置                    | 用途                             |
      | ----------- | --------------------------- | -------------------------------- |
      | `Song`      | `inc/utils/struct.h`        | 输入的 note 总数和目标分数       |
      | `Input`     | `inc/utils/struct.h`        | 输入范围、提示名称与输入类型     |
      | `InputKind` | `inc/utils/struct.h`        | 区分音符总数、目标分数与方案编号 |
      | `sol_t`     | `inc/utils/struct.h`        | 对外展示、保存的方案组合         |
      | `Result`    | `inc/utils/struct.h`        | 区分完成、退出和执行失败         |
      | `Args`      | `src/core/algorithm.c`      | 单个判定分值等算法辅助参数       |
      | `Loop`      | `src/core/algorithm.c`      | 查找过程中的当前组合             |
      | `Array`     | `inc/utils/dynamic_array.h` | 动态存放 `sol_t` 元素            |

      例如，对外保存的方案使用以下结构：

      ```c
      typedef struct solution {
          int perfect;   ///< Perfect 数量
          int good;      ///< Good 数量
          int max_combo; ///< 最大连击数
      } sol_t;
      ```

      `Input.kind` 使用 `InputKind` 枚举，包含 `INPUT_NOTE`、`INPUT_GOAL` 和 `INPUT_SOLUTION`。`name` 只用于显示，不参与类型判断；`init_input(kind, solution_count)` 统一设置输入范围与名称，方案编号的上限由可选方案数量决定。

      内部辅助函数使用 `static`，`algorithm.h` 只公开 `seek_solution()`。

      `Loop` 与 `sol_t` 是不同类型，保存时通过 `append_solution()` 显式构造 `sol_t`，再调用 `array_push()` 复制元素。

   3. 寻找方案前的准备

      **特殊分数。** `seek_solution()` 对零分、满分和全 Good 分数直接给出提示，其余情况进入 `algorithm()`：

      ```c
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
      ```

      这些分支只给出一个直接可用的建议，不枚举该分数的全部组合。例如 `685000` 分提示“全部 good 即可”，并不声称这是唯一方案。

      **判定分值与提前过滤。** `init_args()` 计算单个 Perfect、Good 分值及其差值：

      ```c
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
      ```

      这里先乘再除 `SCALE_FACTOR` 是现有计算顺序，不能据此声称获得了额外有效精度。`delta_score == 0.0` 在调用方充当“不再继续查找”的标记。

      当前代码过滤 `goal < good_score` 或 `MAX_SCORE - goal < delta_score` 的情况。这是使用未取整分值的范围检查，不是完整可达性判定；它与后面的整数四舍五入检查也不是同一个条件。范围内仍可能无解，边界的完备性需要单独验证。

   4. 寻找第一个符合条件的组合

      总体调用顺序为：

      ```text
      get_first_solution
      -> get_first_perfect
      -> get_first_good
      -> get_first_max_combo
      ```

      前两层尝试 Perfect 与 Good；最后一步直接反算最大连击，**不是第三层枚举循环**。找到组合后只更新 `Loop`，由 `get_solutions()` 将其加入数组。

      **第一步：确定 Perfect 起点。** `get_first_solution()` 设置比例系数 `factor = 5000.0 / note`。当目标分数大于 900000 时，从 `note - 1` 开始；否则从 `(int)(goal / perfect_score)` 开始。满分已在外层单独处理。

      **第二步：递减 Perfect。** `get_first_perfect()` 对每个候选值执行：

      1. 若 `goal - perfect * perfect_score < delta_score`，跳过当前 Perfect。
      2. 按其余音符全部 Good、全连计算当前 Perfect 下的上限分数；若上限小于目标，结束本层查找。
      3. 从 Good 为 0 开始进入下一层；找到可行组合则返回，否则递减 Perfect。

      这里 `perfect_score` 始终表示单个 Perfect 的分值，不能在推导中又将同名变量赋值为总 Perfect 分。

      ```mermaid
      flowchart TD
          A["设置 Perfect 起点"] --> B{"差值小于 delta_score？"}
          B -- 否 --> C{"当前 Perfect 下的全连上限小于目标？"}
          C -- 是 --> N["返回 false"]
          C -- 否 --> D["Good 置 0，调用 get_first_good"]
          D --> E{"找到组合？"}
          E -- 是 --> Y["返回 true"]
          E -- 否 --> F["Perfect 减 1"]
          B -- 是 --> F["Perfect 减 1"]
          F --> G{"Perfect 大于等于 0？"}
          G -- 是 --> B
          G -- 否 --> N
      ```

      **第三步：递增 Good。** 对固定的 Perfect，Good 从 0 增加到 `note - perfect`。用于剪枝的两个分数为：

      $$
      \begin{aligned}
          max\_score &= \operatorname{round}\bigl(k(200P+137G)\bigr)\\
          min\_score &= \operatorname{round}\bigl(k(180P+117G+20)\bigr)
      \end{aligned}
      $$

      前者假定所有 Perfect 与 Good 连成一段；后者将最大连击取为 1，用作下界。若上限仍小于目标，增加 Good；若下界大于目标，结束当前 Perfect 的查找；其余情况尝试反算最大连击。

      ```mermaid
      flowchart TD
          A["Good 从 0 开始"] --> B{"max_score 小于目标？"}
          B -- 否 --> C{"min_score 大于目标？"}
          C -- 是 --> N["返回 false"]
          C -- 否 --> D["调用 get_first_max_combo"]
          D --> E{"组合可行？"}
          E -- 是 --> Y["返回 true"]
          E -- 否 --> F
          B -- 是 --> F["Good 加 1"]
          F --> G{"Good 不超过 note - Perfect？"}
          G -- 是 --> B
          G -- 否 --> N
      ```

      **第四步：反算最大连击并验证。** 候选最大连击由以下关系得到：

      $$
      C=\operatorname{round}\left(\frac{goal/k-180P-117G}{20}\right)
      $$

      代码随后检查 $1\le C\le P+G$、最少断连数量，以及重新计算的整数分数是否等于目标。不能只对反算结果取整就直接认定有解。

      ```mermaid
      flowchart TD
          A["反算并取整 Max Combo"] --> B{"1 ≤ C ≤ P + G？"}
          B -- 否 --> N["返回 false"]
          B -- 是 --> C["计算最少断连次数"]
          C --> D{"最少断连次数 ≤ note - P - G？"}
          D -- 否 --> N
          D -- 是 --> E{"calculate_score 等于目标？"}
          E -- 否 --> N
          E -- 是 --> Y["返回 true，组合保留在 Loop 中"]
      ```

      整数分数实际由以下实现取得：

      ```c
      static int calculate_score(int perfect, int good, int max_combo) {
          return (int)round(
              factor * (
                  PERFECT_FACTOR * perfect + GOOD_FACTOR * good + MAX_COMBO_FACTOR * max_combo
              )
          );
      }
      ```

   5. 从第一个方案递推其他方案

      设加权和 $T=180P+117G+20C$。对固定的 Perfect，如果 Good 增加 20、最大连击减少 117，则：

      $$
      117\times20+20\times(-117)=0
      $$

      因此该变化保持 $T$ 不变：

      $$
      \begin{cases}
          G \gets G+20\\
          C \gets C-117
      \end{cases}
      \tag{11}
      $$

      当当前最大连击不大于 117 时，继续这一步会得到非正最大连击，程序改为递减 Perfect：

      $$
      P' = P-1
      \tag{12}
      $$

      设 Good 回调到当前起点 $L=last\_good$：

      $$
      G'=L
      \tag{13}
      $$

      要求新旧组合的加权和相同：

      $$
      180P+117G+20C=180(P-1)+117L+20C'
      \tag{14}
      $$

      整理得到：

      $$
      C'=C+9+117\frac{G-L}{20}
      \tag{15}
      $$

      其中 $G-L$ 由步长为 20 的递推形成，因此上述除法在这条递推路径上得到整数。对应的更新式为：

      $$
      C \gets C+9+117\frac{G-L}{20}
      \tag{16}
      $$

      `update_data()` 随后可能把 `last_good` 增加 20，并把最大连击减少 117，再令 `good = last_good`：

      $$
      \begin{cases}
          L \gets L+20\\
          C \gets C-117\\
          G \gets L
      \end{cases}
      \tag{17}
      $$

      **注意代码的执行顺序：** 判断 `max_combo > perfect + good` 时，Perfect 已递减，Good 还未重置。这个判断并不代替最终合法性检查；重置后还会调用 `data_is_valid()`。如果最大连击仍不大于 0，`update_data()` 会继续下一次 Perfect 更新。

      当前函数实现如下：

      ```c
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
      ```

   6. 验证并保存递推得到的组合

      设 $H=P+G$，则当前组合至少需要的总音符数为：

      $$
      min\_note=H+\left\lceil\frac{H}{C}\right\rceil-1
      \tag{18}
      $$

      在最大连击为正的前提下，当前 `data_is_valid()` 检查最大连击不超过 $P+G$、最少音符数不超过 `note`，以及 Perfect 非负：

      ```c
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
      ```

      Good 非负、最大连击为正由调用它的递推过程维持。这个内部函数不是任意外部输入的完整校验器。

      保存方案时不再使用链表，而是显式构造 `sol_t` 后写入动态数组：

      ```c
      static bool append_solution(Array* array, Loop loop) {
          sol_t solution = {loop.loop_perfect, loop.loop_good, loop.loop_max_combo};
          return array_push(array, &solution);
      }
      ```

      递推过程如下；数组写入失败会立即返回 `false`，不能继续写入：

      ```c
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
      ```

      ```mermaid
      flowchart TD
          A["从第一个组合开始，last_good = good"] --> B{"Perfect ≥ 0 且 last_good ≤ note？"}
          B -- 否 --> Y["返回 true，递推结束"]
          B -- 是 --> C{"Max Combo 大于 117？"}
          C -- 是 --> D["Good 加 20，Max Combo 减 117"]
          C -- 否 --> E["update_data：更新 Perfect、Good 与 Max Combo"]
          D --> F{"data_is_valid 通过？"}
          E --> F
          F -- 否 --> B
          F -- 是 --> G{"append_solution 成功？"}
          G -- 是 --> B
          G -- 否 --> N["返回 false，交由调用者释放数组"]
      ```

4. 查找结果与算法说明的边界

   上面的递推等式证明了它保持加权和不变。**它本身并不证明当前的起点选择、提前过滤和递推遍历不会漏掉任何合法方案。** 文档因此将“完成当前查找流程”与“已经证明所有输入下的枚举完备性”区分开来。

   当前 `tests/run_tests.py` 会验证一个导出样例的方案数量、分数和连击约束，也覆盖无解、多轮运行及资源失败路径。这些测试不能代替对所有 note、goal 的穷举对照或完备性证明。

   若继续研究算法正确性，可以针对小 note 数，用直接枚举 $P$、$G$、$C$ 的结果与当前算法对照，尤其检查四舍五入边界和提前过滤条件。

## 三、动态数组与错误返回

`get_solutions()` 持有本轮的 `Array`，算法将方案写入数组，选择和文件模块只借用它。无解属于正常完成；内存不足、用户退出等路径也必须释放数组。

```c
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

    if (!append_solution(&array, loop) ||
        !get_other_solutions(song->note, loop, &array)) {
        goto cleanup;
    }
    result = choose(song, &array);

cleanup:
    ///< 无解、失败、用户退出和正常完成均在此释放数组
    array_destroy(&array);
    return result;
}
```

这里 `Array` 结构体是局部变量，只有内部数据区需要动态分配。`array_destroy()` 会释放数据区并把结构体恢复为零值。

| 结果            | 含义                     |
| --------------- | ------------------------ |
| `RESULT_OK`     | 正常完成，包括未找到方案 |
| `RESULT_CANCEL` | 用户退出或输入流结束     |
| `RESULT_ERROR`  | 内存分配或输入输出失败   |

`get_first_solution()` 的 `false` 表示未找到组合；`append_solution()` 与 `get_other_solutions()` 的 `false` 表示数组写入失败。它们处于不同层次，不能混用返回值含义。

`cleanup` 是当前函数内的统一清理位置，不使用 `setjmp/longjmp`。更多接口约定见 [CODE_STRUCTURE.md](CODE_STRUCTURE.md)。

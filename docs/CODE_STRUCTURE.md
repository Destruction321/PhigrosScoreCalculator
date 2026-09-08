# 代码怎么分，内存谁来管

这份说明介绍当前代码的接口与资源管理；计分推导、查找流程和对应代码片段见 [MORE_DETAILS.md](MORE_DETAILS.md)。

## 头文件和源文件

- `.h` 声明其他模块需要调用的接口；`.c` 实现这些接口。
- 只在当前 `.c` 中使用的函数加 `static`，声明也留在这个 `.c` 内。
- 公用的 `Song`、`sol_t`、`Input`、输入类型 `InputKind` 和流程结果 `Result` 定义在 `struct.h`。
- 算法内部的 `Args`、`Loop`、公式系数留在 `algorithm.c`，不用让输入和文件模块认识它们。
- 头文件包含自身需要的类型定义。源文件先包含对应头文件，再包含实现使用的标准库和其他接口。
- `file.h` 不再暴露平台相关宏；Windows / POSIX 的目录操作差异放在 `file.c` 内部。

例如，`algorithm.h` 只公开 `seek_solution()`；查找 Perfect、Good 的步骤是算法模块自己的事。

## 输入类型与提示名称

`Input.kind` 使用 `InputKind` 枚举判断当前输入类型，`name` 只负责显示。修改提示文字不会改变输入范围、特殊选项或清屏方式。

```c
Input note_input = init_input(INPUT_NOTE, 0);
Input goal_input = init_input(INPUT_GOAL, 0);
Input solution_input = init_input(INPUT_SOLUTION, 10);
```

`init_input()` 根据类型统一设置名称和常规范围。第二个参数是可选方案数量，仅 `INPUT_SOLUTION` 使用；其他类型传 0。

| 输入类型 | 常规范围 | 特殊输入 |
| --- | --- | --- |
| `INPUT_NOTE` | `MIN_NOTE` 到 `MAX_NOTE` | 无 |
| `INPUT_GOAL` | `MIN_SCORE` 到 `MAX_SCORE` | `-1` 重新设置 note |
| `INPUT_SOLUTION` | 1 到可选方案数量 | `0` 导出全部方案 |

`get_min()` 根据枚举放宽特殊操作的输入下限；打印模块也使用枚举决定提示内容，以及清除若干行还是整个屏幕。

未知枚举值或非正的方案数量会使工厂函数生成无效范围，`set_data()` 在读取前返回 `RESULT_ERROR`。枚举本身不会自动检查运行时数值。

`InputKind` 表示“正在输入什么”，`Status` 表示“一次读取的结果”，`Result` 表示“一步流程的结果”，三者用途不同。

## 一轮计算负责一个数组

`algorithm.c` 的 `get_solutions()` 是方案数组的所有者：

```text
Array array = {0}
    → array_init()
    → 查找并添加方案
    → choose(song, &array)
        → 读取方案
        → write_file(song, &array)
    → cleanup: array_destroy(&array)
```

`Array` 结构体是局部变量，只有 `array.data` 是堆内存。`choose()` 和 `write_file()` 借用 `const Array*`，不释放数组，也不保存指针供下一轮使用。

`array_get()` 返回的元素指针同样只是借用：下一次扩容可能让旧指针失效，销毁数组后也不能继续访问。

`array_push()` 的数据参数不能指向数组内部，因为扩容可能移动原缓冲区。算法先创建局部 `sol_t`，再把它复制进去。

## 失败怎么返回

跨模块的交互函数使用三种结果：

| 返回值          | 含义                                 |
| --------------- | ------------------------------------ |
| `RESULT_OK`     | 正常完成；没有找到方案也属于正常完成 |
| `RESULT_CANCEL` | 用户退出，或输入流结束               |
| `RESULT_ERROR`  | 内存分配或输入输出失败               |

取消一次导出会在文件模块内转换为正常完成，让用户可以继续使用程序。底层数组操作只需要成功或失败，因此使用 `bool`。

失败沿调用链返回，拥有资源的函数负责清理。`goto cleanup` 只跳到当前函数末尾，不跨越其他函数。于是无需全局数组、错误回调或 `setjmp/longjmp`。

`array_destroy()` 释放后会把结构体恢复为零值，因此同一个有效的 `Array` 对象可以重复销毁。初始化则只能用于零值或已经销毁的数组，不能覆盖一个仍持有数据的数组。

## 文件资源

路径长度有上限，因此使用局部字符数组，无需 `malloc`。目录在导出时创建，`FILE*` 在文件模块内打开、检查写入结果，并在统一出口关闭；关闭失败也会报告保存失败。

`write_file()` 负责组织流程，内部的静态辅助函数分工如下：

| 函数 | 职责与资源归属 |
| --- | --- |
| `print_subfolder_tip()` | 输出详细说明和路径示例 |
| `normalize_subfolder()` | 原地统一路径分隔符，检查各级名称；不进行输入输出 |
| `read_subfolder()` | 读取与重试，处理默认目录、取消、EOF 和过长输入，再调用路径检查 |
| `prepare_file_path()` | 调用 `read_subfolder()`，拼接路径并创建缺失目录 |
| `confirm_overwrite()` | 检查文件并询问覆盖；自行打开和关闭检查用文件 |
| `write_solutions()` | 写入表头与方案；借用文件指针，不负责关闭 |
| `write_file()` | 打开写入用文件，在 `cleanup` 统一关闭并报告结果 |

准备路径或覆盖确认返回 `RESULT_CANCEL` 时，外层将其转换为正常完成，且不会打开写入用文件。失败通过返回值传递；辅助函数不跨函数跳转。`write_solutions()` 没有需要释放的自有资源，写入失败直接返回 `false` 即可。

## 验证

在项目根目录运行：

```powershell
python tests/run_tests.py
```

测试使用 GCC 严格警告编译，逐个检查头文件能否独立包含，并覆盖数组扩容、容量溢出、重复销毁、分配失败、多轮运行、输入结束及文件导出。测试构建注入分配计数和失败，不改变正式程序的分配器；生成文件都放在独立临时目录。

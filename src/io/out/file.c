/**
 * @file file.c
 *
 * @brief 文件操作相关函数实现
 *
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 */

#include "file.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#define PATH_SEPARATOR '\\'
#else
#include <sys/stat.h>
#define PATH_SEPARATOR '/'
#endif

#include "tools.h"

enum { FILE_LENGTH = 200, SUBFOLDER_LENGTH = 27 };

/**
 * @brief 创建单层目录
 *
 * @param[in] path 目录路径
 * @return true 创建成功或系统返回 EEXIST
 * @return false 创建失败
 * @note EEXIST 也可能表示同名文件存在，后续打开方案文件时继续检查
 */
static bool create_folder(const char* path) {
#ifdef _WIN32
    int result = _mkdir(path);
#else
    int result = mkdir(path, 0755);
#endif
    return result == 0 || errno == EEXIST;
}

/**
 * @brief 逐级创建目录
 *
 * @param[in,out] path 可写的相对路径字符串，临时截断后会恢复分隔符
 * @return true 每一级创建成功或返回 EEXIST
 * @return false 某一级目录创建失败
 */
static bool create_folders(char* path) {
    for (char* cursor = path; *cursor; ++cursor) {
        if (*cursor != PATH_SEPARATOR) {
            continue;
        }
        *cursor = '\0';
        bool ok = create_folder(path);
        *cursor = PATH_SEPARATOR;
        if (!ok) {
            return false;
        }
    }
    return create_folder(path);
}

/**
 * @brief 打印子目录输入说明、路径示例与取消方式
 */
static void print_subfolder_tip(void) {
    clear_and_print(
        "是否要指定文件夹存储文件？\n"
        "如果要，请输入在\"solutions\"下的文件夹路径，否则直接键入\"Enter\"即可；\n"
        "·示例：输入\"folder1%cfolder2\"，则文件将会被放在\"solutions%cfolder1%cfolder2\"下。\n"
        "·输入\"0\"直接退出...\n",
        PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR
    );
    print_dividing_line('=', LONG, stdout);
    fputs("请输入文件夹路径", stdout);
}

/**
 * @brief 统一路径分隔符并检查子目录名称
 *
 * @param[in,out] subfolder 可写的、以空字符结束的路径字符串
 * @return true 路径通过当前检查
 * @return false 路径含绝对路径形式、冒号、空目录名、点目录或末尾点和空格
 * @note 即使检查失败，也可能已替换分隔符；不读取输入或打印提示
 */
static bool normalize_subfolder(char* subfolder) {
    bool invalid = subfolder[0] == '/' || subfolder[0] == '\\' || strchr(subfolder, ':') != NULL;
    for (char* cursor = subfolder; *cursor; ++cursor) {
        if (*cursor == '/' || *cursor == '\\') {
            *cursor = PATH_SEPARATOR;
        }
    }
    const char* component = subfolder;
    do {
        const char* end = strchr(component, PATH_SEPARATOR);
        size_t length = end ? (size_t)(end - component) : strlen(component);
        bool invalid_component = (
            length == 0
            || (length == 1 && component[0] == '.')
            || (length == 2 && strncmp(component, "..", 2) == 0)
            || component[length - 1] == '.'
            || component[length - 1] == ' '
        );
        if (invalid_component) {
            invalid = true;
        }
        component = end ? end + 1 : NULL;
    } while (component != NULL);
    return !invalid;
}

/**
 * @brief 读取子目录路径，处理默认值、取消与校验失败后的重试
 *
 * @param[out] subfolder 子目录路径缓冲区，空行使用 default
 * @param[in] size 缓冲区字节数，内部传入 SUBFOLDER_LENGTH
 * @retval RESULT_OK 路径通过检查
 * @retval RESULT_CANCEL 用户输入 0 或输入流结束
 * @retval RESULT_ERROR 输入读取失败
 */
static Result read_subfolder(char* subfolder, size_t size) {
    print_subfolder_tip();
    for (;;) {
        if (fgets(subfolder, (int)size, stdin) == NULL) {
            return ferror(stdin) ? RESULT_ERROR : RESULT_CANCEL;
        }
        if (strchr(subfolder, '\n') == NULL && !feof(stdin)) {
            clear_buffer();
            fputs("路径过长，请重新输入：", stdout);
            continue;
        }
        subfolder[strcspn(subfolder, "\r\n")] = '\0';
        if (strcmp(subfolder, "0") == 0) {
            return RESULT_CANCEL;
        }
        if (subfolder[0] == '\0') {
            snprintf(subfolder, size, "default");
        }
        if (normalize_subfolder(subfolder)) {
            return RESULT_OK;
        }
        fputs("请输入 solutions 下的有效相对路径：", stdout);
    }
}

/**
 * @brief 读取子目录、拼接文件路径并创建缺失的目录
 *
 * @param[out] path 接收包含文件名的相对路径
 * @param[in] size path 缓冲区的字节数
 * @param[in] song 歌曲参数地址，用于生成文件名
 * @retval RESULT_OK 路径和目录准备完成
 * @retval RESULT_CANCEL 用户取消导出或输入流结束
 * @retval RESULT_ERROR 输入读取、路径拼接或目录创建失败
 */
static Result prepare_file_path(char* path, size_t size, const Song* song) {
    char subfolder[SUBFOLDER_LENGTH] = {0}; ///< 用户输入的子目录
    char folder[FILE_LENGTH] = {0}; ///< solutions 下的目录路径
    Result result = read_subfolder(subfolder, sizeof(subfolder));
    if (result != RESULT_OK) {
        return result;
    }

    int length = snprintf(
        folder, sizeof(folder), "solutions%c%s", PATH_SEPARATOR, subfolder
    );
    if (length < 0 || (size_t)length >= sizeof(folder)) {
        return RESULT_ERROR;
    }
    length = snprintf(
        path, size,
        "%s%c%dnotes,goal=%d.txt",
        folder, PATH_SEPARATOR, song->note, song->goal
    );
    if (length < 0 || (size_t)length >= size || !create_folders(folder)) {
        return RESULT_ERROR;
    }
    return RESULT_OK;
}

/**
 * @brief 检查已有文件并询问是否覆盖
 *
 * @param[in] path 目标文件路径
 * @retval RESULT_OK 文件不存在，或用户同意覆盖
 * @retval RESULT_CANCEL 用户选择保留原文件，包括确认时遇到 EOF
 * @retval RESULT_ERROR 检查文件、关闭检查用文件或读取输入失败
 * @note 本函数仅打开检查用文件，并在返回前关闭，不创建或截断目标文件
 */
static Result confirm_overwrite(const char* path) {
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        return errno == ENOENT ? RESULT_OK : RESULT_ERROR;
    }
    if (fclose(fp) != 0) {
        return RESULT_ERROR;
    }

    printf("文件 \"%s\" 已存在，输入 r 替换，其他键保留：", path);
    int choice = getch();
    if (ferror(stdin)) {
        return RESULT_ERROR;
    }
    if (choice == 'r' || choice == 'R') {
        return RESULT_OK;
    }
    puts("已保留原文件。");
    return RESULT_CANCEL;
}

/**
 * @brief 写入表头和全部方案，每行最多两个方案
 *
 * @param[in,out] fp 已打开的输出文件，只借用，由调用者关闭
 * @param[in] song 歌曲参数地址
 * @param[in] array 只读方案数组
 * @return true 所有 fprintf 调用成功，最终刷新结果仍需由调用者检查
 * @return false 写入失败
 */
static bool write_solutions(FILE* fp, const Song* song, const Array* array) {
    if (fprintf(fp, "%dnotes,goal=%d\n", song->note, song->goal) < 0) {
        return false;
    }
    for (size_t i = 0; i < array->num; ++i) {
        const sol_t* solution = array_get(array, i);
        int write_result = fprintf(
            fp, "%4zu. Perfect：%-4d, Good：%-4d, Max Combo：%-4d%s",
            i + 1, solution->perfect, solution->good, solution->max_combo,
            ((i % 2 == 0) && (i + 1 < array->num)) ? "\t" : "\n"
        );
        if (write_result < 0) {
            return false;
        }
    }
    return true;
}

Result write_file(const Song* song, const Array* array) {
    char path[FILE_LENGTH] = {0}; ///< 包含文件名的相对路径
    FILE* fp = NULL; ///< 写入用文件由本函数打开并统一关闭
    Result result = prepare_file_path(path, sizeof(path), song);
    if (result == RESULT_OK) {
        result = confirm_overwrite(path);
    }
    if (result == RESULT_CANCEL) {
        ///< 取消导出或保留原文件，均允许继续使用程序
        return RESULT_OK;
    }
    if (result == RESULT_ERROR) {
        goto cleanup;
    }

    result = RESULT_ERROR;
    fp = fopen(path, "w");
    if (fp == NULL || !write_solutions(fp, song, array)) {
        goto cleanup;
    }
    result = RESULT_OK;

cleanup:
    ///< fclose 还会刷新缓冲区，关闭失败也应视为保存失败
    if (fp != NULL && fclose(fp) != 0) {
        result = RESULT_ERROR;
    }
    if (result == RESULT_OK) {
        printf("已将全部方案写入文件 \"%s\"。\n", path);
    } else {
        fprintf(stderr, "方案文件保存失败：%s\n", path);
    }
    return result;
}

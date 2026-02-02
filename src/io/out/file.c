/**
 * @file file.c
 * 
 * @brief 文件操作相关实现
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 * 
 * @copyright Copyright (c) 2025
 */
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

#include "file.h"
#include "struct.h"
#include "tools.h"
#include "dynamic_array.h"


int create_folder(const char* path) {
#ifdef _WIN32
    return _mkdir(path);
#else
    return mkdir(path, 0755);
#endif
}

void write_file(Song* song) {
    Status status = SUCCESS;
    FILE* fp = create_file(song, &status);
    if (fp == NULL) {
        fputs("\033[2J\033[H", stdout);
        if (status == SUCCESS) {
            printf("已保留原文件\"%dnotes,goal=%d.txt\"。\n", song->note, song->goal);
        }
        return;
    }
    fprintf(fp, "\t\t\t\t\t\t\t\t\t\t%dnotes,goal=%d\n", song->note, song->goal);

    print_dividing_line('=', LONG + ASTERISK, fp);

    s_list* data = (s_list*)array->data;
    for (size_t i = 1; i <= array->num; ++i) {
        fprintf(
            fp, "%4zu. Perfect：%-4d, Good：%-4d, Max Combo：%-4d",
            i, data->perfect, data->good, data->max_combo
        );
        if (i % 2 == 0) { ///< 文件里两个方案一行
            fputc('\n', fp);
        } else {
            fputc('\t', fp);
        }
    }

    fclose(fp);
    clear_and_print(
        "已将全部方案写入文件 \"%dnotes,goal=%d.txt\"。\n",
        song->note, song->goal
    );
}


FILE* create_file(Song* song, Status* status) {
    /**
     * file_path: 文件完整路径
     * folder_path: 文件所在的文件夹路径
     * check_folder_path: 检测指针，若为 NULL，说明用户选择不创建文件
     */
    char file_path[FILE_LENGTH] = { '\0' };
    char folder_path[FILE_LENGTH] = { '\0' };
    char* check_folder_path = get_folder_path(file_path, folder_path, song);

    if (check_folder_path == NULL) {
        *status = FAILURE;
        return NULL;
    }

    if (!check_file_exist(file_path, check_folder_path, song)) {
        *status = SUCCESS;
        return NULL;
    }

    ///< 写文件
    FILE* fp = fopen(file_path, "w");
    while (fp == NULL) {
        ///< 访问失败，但失败原因不是文件不存在
        if (errno != ENOENT) {
            break;
        }
        ///< 没有对应目录，尝试创建文件夹
        if (create_folder(folder_path) == 0) {
            fp = fopen(file_path, "w");
        }
        break;
    }

    if (fp == NULL) {
        file_error(file_path, check_folder_path);
    }

    free(check_folder_path);
    return fp;
}

char* get_folder_path(char* file_path, char* folder_path, Song* song) {
    ///< 获取solutions文件夹路径
    getcwd(folder_path, FILE_LENGTH);

    ///< 拼接获取目标文件夹的上级路径
    snprintf(
        folder_path, FILE_LENGTH,
        "%s%csolutions%c",
        folder_path, PATH_SEPARATOR, PATH_SEPARATOR
    );

    ///< 获取自定义文件夹名称
    char* subfolder = set_file_path(file_path, song);
    if (subfolder == NULL) {
        return NULL;
    }

    ///< 拼接获取文件完整路径
    snprintf(folder_path, FILE_LENGTH, "%s%s", folder_path, subfolder);
    return subfolder;
}

char* set_file_path(char* file_path, Song* song) {
    char* subfolder = (char*)calloc(1, SUBFOLDER_NAME_LENGTH);
    if (subfolder == NULL) {
        alloc_error("内存分配失败");
    }

    clear_and_print(
        "是否要指定文件夹存储文件？\n"
        "如果要，请输入在\"solutions\"下的文件夹路径，否则直接键入\"Enter\"即可；\n"
        "·示例：输入\"folder1%cfolder2\"，则文件将会被放在\"solutions%cfolder1%cfolder2\"下。\n"
        "·输入\"0\"直接退出...\n", PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR
    );

    print_dividing_line('=', LONG, stdout);
    fputs("请输入文件夹路径", stdout);

    ///< 用户选择直接存入默认文件夹
    if (!set_folder_path(subfolder)) {
        free(subfolder);
        return NULL;
    }

    ///< 获取当前工作目录，并拼接完整路径
    getcwd(file_path, FILE_LENGTH);
    snprintf(
        file_path, FILE_LENGTH,
        "%s%csolutions%c%s%c%dnotes,goal=%d.txt",
        file_path, PATH_SEPARATOR,
        PATH_SEPARATOR, subfolder, PATH_SEPARATOR, song->note,
        song->goal
    );
    return subfolder;
}

bool set_folder_path(char* subfolder) {
    bool too_long = true;
    while (too_long) {
        printf("（不超过 %d 个半角字符,默认default文件夹）：", FOLDER_NAME_LAST);
        if (!fgets(subfolder, SUBFOLDER_NAME_LENGTH, stdin)) {
            free(subfolder);
            alloc_error("获取输入失败");
        }

        ///< 用户选择不创建文件
        if (strncmp(subfolder, "0", 1) == 0) {
            return false;
        }

        ///< 用户选择默认文件夹
        if (strncmp(subfolder, "\n", 1) == 0) {
            snprintf(subfolder, SUBFOLDER_NAME_LENGTH, "default");
            break;
        }

        too_long = (
            (subfolder[FOLDER_NAME_LAST] != '\n')
            && (subfolder[FOLDER_NAME_LAST] != '\0')
        );

        if (too_long) {
            clear_buffer();
            fputs("\033[F\033[2K路径过长，请重新输入", stdout);
        }
    }
    subfolder[strcspn(subfolder, "\n")] = '\0'; ///< 去除换行符
    return true;
}

bool check_file_exist(char* file_path, char* subfolder, Song* song) {
    if (access(file_path, F_OK) != 0) {
        ///< 访问失败，但失败原因不是文件不存在
        if (errno != ENOENT) {
            file_error(file_path, subfolder);
        }
    } else {
        ///< 文件存在，询问用户是否替换
        int choice = replace_or_skip(subfolder, song);
        if (choice != 'r' && choice != 'R') {
            return false;
        }
        if (remove(file_path) != 0) {
            file_error(file_path, subfolder);
        }
    }
    return true;
}

int replace_or_skip(char* subfolder, Song* song) {
    printf(
        "\033[2J\033[H"
        "文件夹\"solutions\\%s\"中存在文件\"note=%d,goal=%d\"，是否替换？\n",
        subfolder, song->note, song->goal
    );
    print_dividing_line('=', LONG, stdout);
    fputs("输入\"r\"替换，其他键跳过：", stdout);
    return getch();
}

void file_error(char* file_path, char* subfolder) {
    free(subfolder);
    alloc_error(
        "文件创建失败！原因：%s\n文件目标路径：%s",
        strerror(errno), file_path
    );
}
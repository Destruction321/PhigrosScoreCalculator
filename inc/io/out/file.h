/**
 * @file file.h
 * 
 * @brief 文件操作相关函数
 * 
 * @author 棍母
 * @version 11.45.14
 * @date 1919-08-10
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef FILE_H
#define FILE_H

#ifdef _WIN32
    #include <io.h>
    #include <direct.h>
    #define getcwd _getcwd
    #define access _access

    ///< 访问权限参数
    #define F_OK 0
    #define W_OK 2
    #define R_OK 4
    #define R_W_OK 6

    enum path_separator { PATH_SEPARATOR = '\\', };
#else
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    enum path_separator { PATH_SEPARATOR = '/', };
#endif

typedef struct song Song;
typedef enum status Status;

/**
 * @brief 创建文件夹。
 * 
 * 考虑到solutions文件夹路径长度，自定义文件夹名称限制在25半角字符以内
 * 
 * @param path 文件夹路径
 * @return int 状态
 * @retval 0 成功
 * @retval -1 失败
 */
int create_folder(const char* path);

/**
 * @brief 将全部方案写入文件
 * 
 * @param[in] song 歌曲参数
 */
void write_file(Song* song);

/**
 * @brief 创建方案文件
 *
 * @param[in] song 歌曲参数
 * @param[out] status 状态码，初始值 SUCCESS，不创建文件时赋值为 FAILURE
 * @return FILE* 新创建的文件地址
 */
FILE* create_file(Song* song, Status* status);

/**
 * @brief 获取目标文件夹路径与文件完整路径
 *
 * @param[in] file_path 文件路径数组
 * @param[in] folder_path 新建方案文件夹路径
 * @param[in] song 歌曲参数
 * @return char* 自定义文件夹名称
 * @retval 若为 NULL，说明用户选择不创建文件
 */
char* get_folder_path(char* file_path, char* folder_path, Song* song);

/**
 * @brief 获取完整文件路径
 * 
 * @param[in] file_path 文件路径数组
 * @param[in] song 歌曲参数
 * @return char* 自定义文件夹名称
 * @retval 若为 NULL，说明用户选择不创建文件
 */
char* set_file_path(char* file_path, Song* song);

/**
 * @brief 自定义文件夹名称
 *
 * @param[in] subfolder 文件夹名称数组，获取输入失败时释放
 * @return true 用户选择自定义路径；
 * @return false 用户选择不创建文件
 */
bool set_folder_path(char* subfolder);

/**
 * @brief 检查文件是否存在
 *
 * @param[in] file_path 文件路径
 * @param[in] subfolder 自定义文件夹名称，文件创建失败时释放
 * @param[in] song 歌曲参数，组成文件名
 * @return true 文件存在，用户选择替换文件
 * @return false 文件存在，用户选择跳过文件
 */
bool check_file_exist(char* file_path, char* subfolder, Song* song);

/**
 * @brief 替换/跳过已存在文件
 *
 * @param[in] subfolder 文件夹名称
 * @param[in] song 歌曲参数，组成文件名
 * @return int 用户选择
 * @retval 'r' 或 'R' 替换文件
 * @retval 其他键 跳过创建文件
 */
int replace_or_skip(char* subfolder, Song* song);

/**
 * @brief 文件创建失败处理函数
 * 
 * @param[in] file_path 文件路径
 * @param[in] subfolder 自定义文件夹名称，在这个函数里释放
 */
void file_error(char* file_path, char* subfolder);

#endif ///< FILE_H
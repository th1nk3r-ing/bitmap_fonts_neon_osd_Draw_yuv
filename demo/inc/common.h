/**
 * @file:   common.h
 * @note:   2013-2023,   < git.sochina.net/think3r >
 * @brief  common
 * @author: think3r
 * @date    2018/1/6
 * @note:
 * @note \n History:
   1.�?    �?: 2018/1/6
     �?    �?: think3r
     修改历史: 创建文件
 */
/*----------------------------------------------*/
/*                 头文件包�?                   */
/*----------------------------------------------*/
#include "stdio.h"


#ifndef   COMMON_H_
#define   COMMON_H_

/*----------------------------------------------*/
/*                 宏类型定�?                   */
/*----------------------------------------------*/

#ifndef OK
#define OK 0 
#endif

#ifndef TRUE
#define TRUE			1
#endif

#ifndef FALSE
#define FALSE			0
#endif

#ifndef BOOL_TYPE_
#define BOOL_TYPE_
typedef unsigned char	BOOL;
#endif

#define ALIGN_DOWN(value, align_valueue)		((value) & ((UINT)(~(UINT)((align) - 1))))
#define ALIGN_UP(value, align)		            ( ((value)-(value)%(align)) +(align))


#ifndef COLORED_PRINT
#define COLORED_PRINT
/*参�?�链�?: [通过 printf 设置 Linux 终端输出的颜色和显示方式]
                    (https://www.cnblogs.com/clover-toeic/p/4031618.html)
            [printf 打印颜色](http://blog.csdn.net/lwbeyond/article/details/40588145)*/
    
    #define RESET_COLOR          "\e[0m"
    #define BLACK                "\e[0;30m"
    #define L_BLACK              "\e[1;30m"
    #define RED                  "\e[0;31m"
    #define L_RED                "\e[1;31m"
    #define GREEN                "\e[0;32m"
    #define L_GREEN              "\e[1;32m"
    #define BROWN                "\e[0;33m"
    #define YELLOW               "\e[1;33m"
    #define BLUE                 "\e[0;34m"
    #define L_BLUE               "\e[1;34m"
    #define PURPLE               "\e[0;35m"
    #define L_PURPLE             "\e[1;35m"
    #define CYAN                 "\e[0;36m"
    #define L_CYAN               "\e[1;36m"
    #define GRAY                 "\e[0;37m"
    #define WHITE                "\e[1;37m"
    
    #define BOLD                 "\e[1m"
    #define UNDERLINE            "\e[4m"
    #define BLINK                "\e[5m"
    #define REVERSE              "\e[7m"
    #define HIDE                 "\e[8m"
    #define CLEAR                "\e[2J"
    
    #define UNDERLINE            "\e[4m"
    #define CLRLINE              "\r\e[K" //or "\e[1K\r"
                
    #define Cprintf_black(format,...)    printf(L_BLACK format RESET_COLOR, ##__VA_ARGS__)
    #define Cprintf_red(format,...)      printf(L_RED format RESET_COLOR, ##__VA_ARGS__)
    #define Cprintf_green(format,...)    printf(L_GREEN format RESET_COLOR, ##__VA_ARGS__)
    #define Cprintf_brown(format,...)    printf(BROWN format RESET_COLOR, ##__VA_ARGS__)
    #define Cprintf_yellow(format,...)   printf(YELLOW format RESET_COLOR, ##__VA_ARGS__)
    #define Cprintf_blue(format,...)     printf(L_BLUE format RESET_COLOR, ##__VA_ARGS__)
    #define Cprintf_purple(format,...)   printf(L_PURPLE format RESET_COLOR, ##__VA_ARGS__)
    #define Cprintf_cyan(format,...)     printf(L_CYAN format RESET_COLOR, ##__VA_ARGS__)
    #define Cprintf_gray(format,...)     printf(GRAY format RESET_COLOR, ##__VA_ARGS__)
    #define Cprintf_white(format,...)    printf(WHITE format RESET_COLOR, ##__VA_ARGS__)
    
    #define Cprintf_bold(format,...)     printf(BOLD format RESET_COLOR, ##__VA_ARGS__)
    #define Cprintf_underline(format,...) printf(UNDERLINE format RESET_COLOR, ##__VA_ARGS__)
    #define Cprintf_reverse(format,...)  printf(REVERSE format RESET_COLOR, ##__VA_ARGS__)

#endif

/*----------------------------------------------*/
/*                结构体定�?                    */
/*----------------------------------------------*/

typedef enum
{
    ok  =                   0x00,   
    FAILD =                 0x01,
    PARAM_INVALID   =       0x02,
    MALLOC_ERR      =       0x03,
    FILE_NOT_EXIT   =       0x10,
    FILE_TOO_BIG =     0x20,
    DATA_NOT_FIND =    0x21,
} ERR;

#endif /* COMMON_H_ */

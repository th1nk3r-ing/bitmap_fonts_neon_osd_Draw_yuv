/**
* @file    build_time.h
* @brief   Print Porgram Build Time.
* @author  think3r
* @date    2017/12/23

* @note \n History:
  1.�?    �?: 2017/12/23
    �?    �?: think3r
    修改历史: 创建文件
*/

#ifndef BUILD_TIME_H_
#define BUILD_TIME_H_

/*--------------------------------------------------------*/
/*                        宏配置区                            */
/*--------------------------------------------------------*/

//#define SHORT_DATA_CHAR__       /*日期字符串使用缩�?*/
//#define BUILD_TIMES_C_TEST_     /*对本文件进行测试*/  



typedef struct
{
    unsigned int    year;
    unsigned int    month;        
    unsigned int    day;   
    unsigned int    hour;
    unsigned int    minute;
    unsigned int    second;
    unsigned int    milliSecond;
             char   char_month[10];
    unsigned int    dayOfWeek;        /* 0:星期�?-6:星期�? */
             char   char_dayOfWeek[10];
} DATE_TIME;


#ifndef _TM_DEFINED

typedef struct  {
    int tm_sec;     /* �? ? 取�?�区间为[0,59] */
    int tm_min;     /* �? - 取�?�区间为[0,59] */
    int tm_hour;    /* �? - 取�?�区间为[0,23] */
    int tm_mday;    /* �?个月中的日期 - 取�?�区间为[1,31] */
    int tm_mon;     /* 月份（从�?月开始，0代表�?月） - 取�?�区间为[0,11] */
    int tm_year;    /* 年份，其值等于实际年份减�?1900 */
    int tm_wday;    /* 星期 ? 取�?�区间为[0,6]，其�?0代表星期天，1代表星期�?，以此类�? */
    int tm_yday;    /* 从每年的1�?1日开始的天数 ? 取�?�区间为[0,365]，其�?0代表1�?1日，1代表1�?2日，以此类推 */
    int tm_isdst;   /* 夏令时标识符，实行夏令时的时候，tm_isdst为正。不实行夏令时的进�?�，tm_isdst�?0�?
                        不了解情况tm=_isdst()为负�?*/
}TM; 

#define _TM_DEFINED

#endif


unsigned int get_Now_Time(DATE_TIME       *pDate, ...);

unsigned int mk_Build_Date(DATE_TIME *pDate, ...);



#endif

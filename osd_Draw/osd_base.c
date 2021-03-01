/**
 * @file:   osd_base
 * @note:   2017-2030, <git.oschia.net/think3r>
 * @brief:  osd base
 * @author: think3r
 * @date:   2019/8/18
 * @note:
 * @note \n History:
   1.日    期: 2019/8/18
     作    者: think3r
     修改历史: 创建文件
 */

/*----------------------------------------------*/
/*                  头文件包含                  */
/*----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "osd_base.h"

/*----------------------------------------------*/
/*                  宏类型定义                  */
/*----------------------------------------------*/
#define HZK16_FRONT_PATH        ("./HZK16")
#define ASCII8_FRONT_PATH       ("./ASCII8")


/*----------------------------------------------*/
/*                 结构体定义                   */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                  函数声明                    */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                  全局变量                    */
/*----------------------------------------------*/
/* 字库 */
uint8_t HzFrontDot[HZK16_FRONT_SIZE] = {0};         /* 汉字字库 16*16 */
uint8_t AsciiFrontDot[ASCII8_FRONT_SIZE] = {0};     /* ASCII 字库 8*16*/

/* 点阵查找表 */
uint8_t dotTableNormal_u8[256][8];        /* 正常点阵 */
uint16_t dotTableNormal_u16[256][8];      /* 正常点阵 */
uint16_t dotTableReverse_u16[256][8];     /* 左右镜像 */

/*----------------------------------------------*/
/*                  函数定义                    */
/*----------------------------------------------*/

/**
 * @function:   getTime_ms
 * @brief:      获取当前时间 ms
 * @param[in]:  void
 * @param[out]: None
 * @return:     inline uint32_t
 */
uint32_t getTime_ms(void)
{
    struct timespec curTime;

    clock_gettime(CLOCK_MONOTONIC, &curTime);
    return (uint32_t)((curTime.tv_sec * 1000) + curTime.tv_nsec / 1000000);
}

/**
 * @function:   osd_Init
 * @brief:      填充 dot table
 * @param[in]:  void
 * @param[out]: None
 * @return:     void
 */
void osd_Init(void)
{
    uint16_t i = 0;
    uint8_t tabColorMask_u8 = 0xFFu;    
    uint16_t tabColorMask = 0xFFFFu;

    for(i = 0; i < 256; i++)
    {
        dotTableNormal_u8[i][0] = (i & 0x80u) ? tabColorMask_u8 : 0x00u;
        dotTableNormal_u8[i][1] = (i & 0x40u) ? tabColorMask_u8 : 0x00u;
        dotTableNormal_u8[i][2] = (i & 0x20u) ? tabColorMask_u8 : 0x00u;
        dotTableNormal_u8[i][3] = (i & 0x10u) ? tabColorMask_u8 : 0x00u;
        dotTableNormal_u8[i][4] = (i & 0x08u) ? tabColorMask_u8 : 0x00u;
        dotTableNormal_u8[i][5] = (i & 0x04u) ? tabColorMask_u8 : 0x00u;
        dotTableNormal_u8[i][6] = (i & 0x02u) ? tabColorMask_u8 : 0x00u;
        dotTableNormal_u8[i][7] = (i & 0x01u) ? tabColorMask_u8 : 0x00u;
    
        dotTableNormal_u16[i][0] = (i & 0x80) ? tabColorMask : 0x0000;
        dotTableNormal_u16[i][1] = (i & 0x40) ? tabColorMask : 0x0000;
        dotTableNormal_u16[i][2] = (i & 0x20) ? tabColorMask : 0x0000;
        dotTableNormal_u16[i][3] = (i & 0x10) ? tabColorMask : 0x0000;
        dotTableNormal_u16[i][4] = (i & 0x08) ? tabColorMask : 0x0000;
        dotTableNormal_u16[i][5] = (i & 0x04) ? tabColorMask : 0x0000;
        dotTableNormal_u16[i][6] = (i & 0x02) ? tabColorMask : 0x0000;
        dotTableNormal_u16[i][7] = (i & 0x01) ? tabColorMask : 0x0000;

        dotTableReverse_u16[i][7] = (i & 0x80) ? tabColorMask : 0x0000;
        dotTableReverse_u16[i][6] = (i & 0x40) ? tabColorMask : 0x0000;
        dotTableReverse_u16[i][5] = (i & 0x20) ? tabColorMask : 0x0000;
        dotTableReverse_u16[i][4] = (i & 0x10) ? tabColorMask : 0x0000;
        dotTableReverse_u16[i][3] = (i & 0x08) ? tabColorMask : 0x0000;
        dotTableReverse_u16[i][2] = (i & 0x04) ? tabColorMask : 0x0000;
        dotTableReverse_u16[i][1] = (i & 0x02) ? tabColorMask : 0x0000;
        dotTableReverse_u16[i][0] = (i & 0x01) ? tabColorMask : 0x0000;
    }

    size_t readSize = 0;
    FILE * fp = NULL;

    fp =  fopen(HZK16_FRONT_PATH, "rb");
    if(fp != NULL)
    {
        readSize = fread((void *)HzFrontDot, 1, HZK16_FRONT_SIZE, fp);

        if(readSize != HZK16_FRONT_SIZE)
        {
            Cprintf_red("[%s %d]  read [%s] error! readSize:[%d], fileSize:[%d]\n",
                __FUNCTION__, __LINE__, HZK16_FRONT_PATH, (int)readSize, HZK16_FRONT_SIZE);
        }
        else
        {
            Cprintf_yellow("[%s %d]  read [%s] OK!\n", __FUNCTION__, __LINE__, HZK16_FRONT_PATH);
        }

        fclose(fp);
        fp = NULL;
    }
    else
    {
        Cprintf_red("[%s %d] error! file[%s] open failed\n",
            __FUNCTION__,__LINE__, HZK16_FRONT_PATH);

        exit(0);
    }

    fp =  fopen(ASCII8_FRONT_PATH, "rb");
    if(fp != NULL)
    {
        readSize = fread((void *)AsciiFrontDot, 1, ASCII8_FRONT_SIZE, fp);

        if(readSize != ASCII8_FRONT_SIZE)
        {
            Cprintf_red("[%s %d]  read [%s] error! readSize:[%d], fileSize:[%d]\n",
                __FUNCTION__, __LINE__, ASCII8_FRONT_PATH, (int)readSize, ASCII8_FRONT_SIZE);
        }
        else
        {
            Cprintf_yellow("[%s %d]  read [%s] OK!\n", __FUNCTION__, __LINE__, ASCII8_FRONT_PATH);
        }

        fclose(fp);
        fp = NULL;
    }
    else
    {
        Cprintf_red("[%s %d] error! file[%s] open failed\n",
            __FUNCTION__,__LINE__, ASCII8_FRONT_PATH);

        exit(0);
    }

    return;
}


/**
 * @function:   getCharFrontAddr
 * @brief:      获取汉字字库地址
 * @param[in]:  uint16_t charHz
 * @param[out]: None
 * @return:     uint8_t *
 */
uint8_t * getCharFrontAddr(uint16_t charCode)
{
    if(charCode < 0xFF)
    {
        return (AsciiFrontDot + charCode * 16);
    }
    else
    {
        uint32_t offset = (((charCode & 0xFFu) - 0xA1u) + 94u * ((charCode >> 8u) - 0xA1u) ) << 5;

        return (HzFrontDot + offset);
    }
}


/**
 * @file:   cpu_osd.c
 * @note:   2017-2030, <git.oschia.net/think3r>
 * @brief:  cpu 生成点阵
 * @author: think3r
 * @date    2019/8/18
 * @note:
 * @note \n History:
   1.日    期: 2019/8/18
     作    者:  think3r
     修改历史: 创建文件
 */

/*----------------------------------------------*/
/*                 包含头文件                   */
/*----------------------------------------------*/
#include <stdio.h>
#include "cpu_osd.h"
#include "osd_base.h"

/*----------------------------------------------*/
/*                 宏类型定义                   */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                结构体定义                    */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 函数声明                     */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 全局变量                     */
/*----------------------------------------------*/
extern uint16_t dotTableNormal_u16[256][8];      /* 正常点阵 */
extern uint16_t dotTableReverse_u16[256][8];     /* 左右镜像 */

/*----------------------------------------------*/
/*                 函数定义                     */
/*----------------------------------------------*/

/**
 * @function:   cpu_DrawAscii
 * @brief:      cpu 绘制 Ascii 点阵
 * @param[in]:  uint8_t * pFont     字符字库地址
 * @param[in]:  uint16_t * pDst     目的点阵地址(需手动计算起始像素地址)
 * @param[in]:  uint32_t pitch      行 pitch (单位像素, 2B)
 * @param[in]:  uint8_t scale       字符规模(8x16 的倍数)
 * @param[in]:  uint16_t color      像素颜色(ARGB 2byte)
 * @param[out]: None
 * @return:     void
 */
void cpu_DrawAscii( uint8_t *   pFont,
                        uint16_t *  pDst,
                        uint32_t    pitch,
                        uint8_t     scale,
                        uint16_t    color )
{
     if ((pFont == NULL) || (pDst == NULL))
    {
        Cprintf_red("[%s %d]  \n", __FUNCTION__, __LINE__);
        return;
    }

    uint8_t hIdx, k, j;
    uint16_t * pDotTableValue = NULL;
    uint64_t * pTemp64 = (uint64_t *)pDst;  /* 用 64bit 加速行搬移*/
    uint16_t  *pTemp = (uint16_t *)pDst;

#if 1   /* 展开部分循环后, 性能可提升 1/3 */
    switch (scale)
    {
        case 1:
            for (hIdx = 0; hIdx < FRONT_HEIGHT; hIdx++)
            {
                pDotTableValue = dotTableNormal_u16[pFont[hIdx]];
                for (k = 0; k < ASCII_WIDTH; k++)
                {
                    pTemp[hIdx * pitch + k] = pDotTableValue[k] & color;
                }
            }
            break;
        case 2:
            for (hIdx = 0; hIdx < FRONT_HEIGHT; hIdx++)
            {
                pDotTableValue = dotTableNormal_u16[pFont[hIdx]];
                for (k = 0; k < ASCII_WIDTH; k++)
                {
                    pTemp[(hIdx * pitch + k) * 2 + 0] = pDotTableValue[k] & color;
                    pTemp[(hIdx * pitch + k) * 2 + 1] = pDotTableValue[k] & color;
                }
            }
            break;
        case 3:
            for (hIdx = 0; hIdx < FRONT_HEIGHT; hIdx++)
            {
                pDotTableValue = dotTableNormal_u16[pFont[hIdx]];
                for (k = 0; k < ASCII_WIDTH; k++)
                {
                    pTemp[(hIdx * pitch + k) * 3 + 0] = pDotTableValue[k] & color;
                    pTemp[(hIdx * pitch + k) * 3 + 1] = pDotTableValue[k] & color;
                    pTemp[(hIdx * pitch + k) * 3 + 2] = pDotTableValue[k] & color;
                }
            }
            break;
        case 4:
            for (hIdx = 0; hIdx < FRONT_HEIGHT; hIdx++)
            {
                pDotTableValue = dotTableNormal_u16[pFont[hIdx]];
                for (k = 0; k < ASCII_WIDTH; k++)
                {
                    pTemp[(hIdx * pitch + k) * 4 + 0] = pDotTableValue[k] & color;
                    pTemp[(hIdx * pitch + k) * 4 + 1] = pDotTableValue[k] & color;
                    pTemp[(hIdx * pitch + k) * 4 + 2] = pDotTableValue[k] & color;
                    pTemp[(hIdx * pitch + k) * 4 + 3] = pDotTableValue[k] & color;
                }
            }
            break;
        default:
            break;
    }

    for (hIdx = 0; hIdx < FRONT_HEIGHT; hIdx++)
    {
        /* 扩展剩余的行(复制已经生成的行点阵数据) */
        for(k = 1; k < scale; k ++)
        {
            /* 每行需要搬移的数据量为 (ASCII_WIDTH * 2 / 8) */
            for(j = 0; j < (ASCII_WIDTH * 2 / 8 * scale); j++)
            {
                pTemp64[(scale * hIdx + k) * pitch / 4 + j] = pTemp64[scale * hIdx * pitch / 4 + j];
            }
        }
    }
#else
    for (hIdx = 0; hIdx < FRONT_HEIGHT; hIdx++)
    {
        pDotTableValue = dotTableNormal_u16[pFont[hIdx]];

        /* 生成字符的行 */
        for (k = 0; k < ASCII_WIDTH; k++)
        {
            /* 扩展字符的行宽度 */
            for(j = 0; j < scale; j++)
            {
                pTemp[(hIdx * pitch + k) * scale + j] = pDotTableValue[k] & color;
            }
        }

        /* 扩展剩余的行(复制已经生成的行点阵数据) */
        for(k = 1; k < scale; k ++)
        {
            /* 每行需要搬移的数据量为 (ASCII_WIDTH * 2 / 8) */
            for(j = 0; j < (ASCII_WIDTH * 2 / 8 * scale); j++)
            {
                pTemp64[(scale * hIdx + k) * pitch / 4 + j] = pTemp64[scale * hIdx * pitch / 4 + j];
            }
        }
    }
#endif

    return;
}


/**
 * @function:   cpu_DrawAscii
 * @brief:      cpu 绘制 汉字点阵
 * @param[in]:  uint8_t * pFont     字符字库地址
 * @param[in]:  uint16_t * pDst     目的点阵地址(需手动计算起始像素地址)
 * @param[in]:  uint32_t pitch      行 pitch (单位像素, 2B)
 * @param[in]:  uint8_t scale       字符规模(8x16 的倍数)
 * @param[in]:  uint16_t color      像素颜色(ARGB 2byte)
 * @param[out]: None
 * @return:     void
 */
void cpu_DrawChinese(      uint8_t *   pFont,
                           uint16_t *  pDst,
                           uint32_t    pitch,
                           uint8_t     scale,
                           uint16_t    color )
{
    if ((pFont == NULL) || (pDst == NULL))
    {
        Cprintf_red("[%s %d]  \n", __FUNCTION__, __LINE__);
        return;
    }

    uint8_t hIdx, k, j;
    uint16_t * pDotTableValue_1 = NULL;
    uint16_t * pDotTableValue_2 = NULL;     /*汉字基础宽度为 16 像素, 字库中为 2Byte */
    uint16_t  *pTemp = (uint16_t *)pDst;
    uint64_t * pTemp64 = (uint64_t *)pDst;

#if 1   /* 展开部分循环后, 性能可提升 1/3 */
    switch (scale)
    {
        case 1:
            for (hIdx = 0; hIdx < FRONT_HEIGHT; hIdx++)
            {
                pDotTableValue_1 = dotTableNormal_u16[pFont[hIdx * 2]];
                pDotTableValue_2 = dotTableNormal_u16[pFont[hIdx * 2 + 1]];
                for (k = 0; k < ASCII_WIDTH; k++)
                {
                    pTemp[hIdx * pitch + k] = pDotTableValue_1[k] & color;
                    pTemp[hIdx * pitch + k + ASCII_WIDTH] = pDotTableValue_2[k] & color;
                }
            }
            break;
        case 2:
            for (hIdx = 0; hIdx < FRONT_HEIGHT; hIdx++)
            {
                pDotTableValue_1 = dotTableNormal_u16[pFont[hIdx * 2]];
                pDotTableValue_2 = dotTableNormal_u16[pFont[hIdx * 2 + 1]];
                for (k = 0; k < ASCII_WIDTH; k++)
                {
                    pTemp[(hIdx * pitch + k) * 2 + 0] = pDotTableValue_1[k] & color;
                    pTemp[(hIdx * pitch + k) * 2 + 1] = pDotTableValue_1[k] & color;
                    pTemp[(hIdx * pitch + ASCII_WIDTH + k) * 2 + 0] = pDotTableValue_2[k] & color;
                    pTemp[(hIdx * pitch + ASCII_WIDTH + k) * 2 + 1] = pDotTableValue_2[k] & color;
                }
            }
            break;
        case 3:
            for (hIdx = 0; hIdx < FRONT_HEIGHT; hIdx++)
            {
                pDotTableValue_1 = dotTableNormal_u16[pFont[hIdx * 2]];
                pDotTableValue_2 = dotTableNormal_u16[pFont[hIdx * 2 + 1]];
                for (k = 0; k < ASCII_WIDTH; k++)
                {
                    pTemp[(hIdx * pitch + k) * 3 + 0] = pDotTableValue_1[k] & color;
                    pTemp[(hIdx * pitch + k) * 3 + 1] = pDotTableValue_1[k] & color;
                    pTemp[(hIdx * pitch + k) * 3 + 2] = pDotTableValue_1[k] & color;
                    pTemp[(hIdx * pitch + ASCII_WIDTH + k) * 3 + 0] = pDotTableValue_2[k] & color;
                    pTemp[(hIdx * pitch + ASCII_WIDTH + k) * 3 + 1] = pDotTableValue_2[k] & color;
                    pTemp[(hIdx * pitch + ASCII_WIDTH + k) * 3 + 2] = pDotTableValue_2[k] & color;
                }
            }
            break;
        case 4:
            for (hIdx = 0; hIdx < FRONT_HEIGHT; hIdx++)
            {
                pDotTableValue_1 = dotTableNormal_u16[pFont[hIdx * 2]];
                pDotTableValue_2 = dotTableNormal_u16[pFont[hIdx * 2 + 1]];
                for (k = 0; k < ASCII_WIDTH; k++)
                {
                    pTemp[(hIdx * pitch + k) * 4 + 0] = pDotTableValue_1[k] & color;
                    pTemp[(hIdx * pitch + k) * 4 + 1] = pDotTableValue_1[k] & color;
                    pTemp[(hIdx * pitch + k) * 4 + 2] = pDotTableValue_1[k] & color;
                    pTemp[(hIdx * pitch + k) * 4 + 3] = pDotTableValue_1[k] & color;
                    pTemp[(hIdx * pitch + ASCII_WIDTH + k) * 4 + 0] = pDotTableValue_2[k] & color;
                    pTemp[(hIdx * pitch + ASCII_WIDTH + k) * 4 + 1] = pDotTableValue_2[k] & color;
                    pTemp[(hIdx * pitch + ASCII_WIDTH + k) * 4 + 2] = pDotTableValue_2[k] & color;
                    pTemp[(hIdx * pitch + ASCII_WIDTH + k) * 4 + 3] = pDotTableValue_2[k] & color;
                }
            }
            break;
        default:
            break;
    }

    for (hIdx = 0; hIdx < FRONT_HEIGHT; hIdx++)
    {
        for(k = 1; k < scale; k ++)
        {
            for(j = 0; j < (HZ_WIDTH * 2 / 8 * scale); j++)
            {
                pTemp64[(scale * hIdx + k) * pitch / 4 + j] = pTemp64[scale * hIdx * pitch / 4 + j];
            }
        }
    }
#else
    for (hIdx = 0; hIdx < FRONT_HEIGHT; hIdx++)
    {
        pDotTableValue_1 = dotTableNormal_u16[pFont[hIdx * 2]];
        pDotTableValue_2 = dotTableNormal_u16[pFont[hIdx * 2 + 1]];

        for (k = 0; k < ASCII_WIDTH; k++)
        {
            for(j = 0; j < scale; j++)
            {
                pTemp[(hIdx * pitch + k) * scale + j] = pDotTableValue_1[k] & color;
                pTemp[(hIdx * pitch + k + ASCII_WIDTH) * scale + j] = pDotTableValue_2[k] & color;
            }
        }

        for(k = 1; k < scale; k ++)
        {
            for(j = 0; j < (HZ_WIDTH * 2 / 8 * scale); j++)
            {
                pTemp64[(scale * hIdx + k) * pitch / 4 + j] = pTemp64[scale * hIdx * pitch / 4 + j];
            }
        }
    }

#endif

    return;
}


/**
 * @file:   neon_osd.c
 * @note:   2017-2030, <git.oschia.net/think3r>
 * @brief:  neon 加速点阵生成
 * @author: think3r
 * @date:   2019/8/18
 * @note:
 * @note \n History:
   1.日    期: 2019/8/18
     作    者:  think3r
     修改历史: 创建文件
 */

/* neon 共有 16 个 128bit; 即: 16 个(16Byre) */

/*----------------------------------------------*/
/*                 包含头文件                   */
/*----------------------------------------------*/
#include <stdio.h>
#include <arm_neon.h>
#include "neon_osd.h"
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
/* 点阵查找表 */
extern uint8_t dotTableNormal_u8[256][8];      /* 正常点阵 */
extern uint16_t dotTableNormal_u16[256][8];      /* 正常点阵 */
extern uint16_t dotTableReverse_u16[256][8];     /* 左右镜像 */


/*----------------------------------------------*/
/*                 函数定义                     */
/*----------------------------------------------*/

/**
 * @function:   neon_CreatAsciiDot_u16
 * @brief:      neon 加速, 绘制 Ascii 点阵(单像素 16 bit)
 * @param[in]:  uint8_t * pFont     字符字库地址
 * @param[in]:  uint16_t * pDst     目的点阵地址(需手动计算起始像素地址)
 * @param[in]:  uint32_t pitch      行 pitch (单位像素, 2B)
 * @param[in]:  uint8_t scale       字符规模(8x16 的倍数)
 * @param[in]:  uint16_t color      像素颜色(ARGB 2byte)
 * @param[out]: None
 * @return:     void
 */
void neon_CreatAsciiDot_u16(uint8_t * pFont, uint16_t * pDst, uint32_t pitch, uint8_t scale, uint16_t color)
{
    if ((pFont == NULL) || (pDst == NULL))
    {
        Cprintf_red("[%s %d]  \n", __func__, __LINE__);
        return;
    }

    uint32_t i = 0;
    uint8_t idx = 0;                /* 在点阵 tab 中的 idx */
    uint16_t * pSroreDst = NULL;    /* 指向点阵存储的地址 */
    uint16x8_t lineDotSrc_16x8, tmpDot_16x8;
    uint16x8_t lineDotDst_16x8 = vdupq_n_u16(color);   /* 初始化 GRB 分量 */

    if(scale == 1)
    {
        for(i = 0; i < FRONT_HEIGHT; i ++)
        {
            idx = *(pFont + i);
            pSroreDst = pDst + pitch * i;

            lineDotSrc_16x8 = vld1q_u16(dotTableNormal_u16[idx]);           /* 从速查表加载点阵 */
            tmpDot_16x8 = vandq_u16(lineDotSrc_16x8, lineDotDst_16x8);  /* 生成对应的像素值 */
            vst1q_u16(pSroreDst, tmpDot_16x8);                          /* 存储 */
        }
    }
    else if (scale == 2)
    {
        uint16x8x2_t tmp_16x8x2;            /* 存放交织后的数据 */
        uint16_t * pStoreDst2;              /* 奇数行存储地址 */

        for(i = 0; i < FRONT_HEIGHT; i ++)
        {
            idx = *(pFont + i);
            pSroreDst = pDst + pitch * i * scale;
            pStoreDst2 = pSroreDst + pitch;

            lineDotSrc_16x8 = vld1q_u16(dotTableNormal_u16[idx]);           /* 从速查表加载点阵 */
            tmp_16x8x2.val[0] = vandq_u16(lineDotSrc_16x8, lineDotDst_16x8);  /* 生成对应的像素值 */

            tmp_16x8x2.val[1] = tmp_16x8x2.val[0];
            vst2q_u16(pSroreDst, tmp_16x8x2);
            vst2q_u16(pStoreDst2, tmp_16x8x2);
        }

    }
    else if (scale == 3)
    {
        uint16x8x3_t out_16x8x3;                                            /* 存放交织后的数据 */
        uint16_t * pStoreDst2;
        uint16_t * pStoreDst3;

        for(i = 0; i < FRONT_HEIGHT; i++)
        {
            idx = *(pFont + i);
            pSroreDst = pDst + pitch * i * scale;
            pStoreDst2 = pSroreDst + pitch;
            pStoreDst3 = pSroreDst + pitch * 2;

            lineDotSrc_16x8 = vld1q_u16(dotTableNormal_u16[idx]);                   /* 从速查表加载点阵 */
            out_16x8x3.val[0] = vandq_u16(lineDotSrc_16x8, lineDotDst_16x8);    /* 生成对应的像素值 */

            out_16x8x3.val[1] = out_16x8x3.val[0];                              /* 行扩充 */
            out_16x8x3.val[2] = out_16x8x3.val[0];

            vst3q_u16(pSroreDst, out_16x8x3);                                   /* 行存储 */
            vst3q_u16(pStoreDst2, out_16x8x3);                                  /* 列扩充 */
            vst3q_u16(pStoreDst3, out_16x8x3);
        }
    }
    else if(scale == 4)
    {
        uint16x8x4_t out_16x8x4;                                            /* 存放交织后的数据 */
        uint16_t * pStoreDst2;
        uint16_t * pStoreDst3;
        uint16_t * pStoreDst4;

        for(i = 0; i < FRONT_HEIGHT; i++)
        {
            idx = *(pFont + i);
            pSroreDst = pDst + pitch * i * scale;
            pStoreDst2 = pSroreDst + pitch;
            pStoreDst3 = pSroreDst + pitch * 2;
            pStoreDst4 = pSroreDst + pitch * 3;

            lineDotSrc_16x8 = vld1q_u16(dotTableNormal_u16[idx]);                   /* 从速查表加载点阵 */
            out_16x8x4.val[0] = vandq_u16(lineDotSrc_16x8, lineDotDst_16x8);    /* 生成对应的像素值 */

            out_16x8x4.val[1] = out_16x8x4.val[0];                              /* 行扩充 */
            out_16x8x4.val[2] = out_16x8x4.val[0];
            out_16x8x4.val[3] = out_16x8x4.val[0];

            vst4q_u16(pSroreDst, out_16x8x4);                                   /* 行存储 */
            vst4q_u16(pStoreDst2, out_16x8x4);                                  /* 列扩充 */
            vst4q_u16(pStoreDst3, out_16x8x4);
            vst4q_u16(pStoreDst4, out_16x8x4);
        }
    }
    else
    {
        Cprintf_yellow("[%s %d]  notSupport! scale:[%d]\n", __func__, __LINE__, scale);
    }

    return;
}


/**
 * @function:   neon_CreatChineseDot_u16
 * @brief:      neon 加速, 绘制汉字点阵
 * @param[in]:  uint8_t * pFont         字符字库地址
 * @param[in]:  uint16_t * pDst         目的点阵地址(需手动计算起始像素地址)
 * @param[in]:  uint32_t pitch          行 pitch (单位像素, 2B)
 * @param[in]:  uint8_t scale           字符规模(16x16 的倍数)
 * @param[in]:  uint16_t color          像素颜色(ARGB 2byte)
 * @param[out]: None
 * @return:     void
 */
void neon_CreatChineseDot_u16(uint8_t * pFont, uint16_t * pDst, uint32_t pitch, uint8_t scale, uint16_t color)
{
    if ((pFont == NULL) || (pDst == NULL))
    {
        Cprintf_red("[%s %d]  \n", __func__, __LINE__);
        return;
    }

    uint32_t i = 0;
    uint8_t idx_1 = 0, idx_2 = 0;                /* 在点阵 tab 中的 idx_1 */
    uint16_t * pSroreDst = NULL;    /* 指向点阵存储的地址 */
    uint16x8_t tmpDot_16x8_1, tmpDot_16x8_2;
    uint16x8_t lineDotDst_16x8 = vdupq_n_u16(color);   /* 初始化 GRB 分量 */

    if(scale == 1)
    {
        uint16x8x2_t tmp_16x8x2_1, tmp_16x8x2_2;

        for(i = 0; i < FRONT_HEIGHT; i ++)
        {
            idx_1 = *(pFont + i * 2);                       /* 行前 8bit idx */
            idx_2 = *(pFont + i * 2 + 1);                   /* 行后 8bit idx */

            pSroreDst = pDst + pitch * i;

            tmpDot_16x8_1 = vld1q_u16(dotTableNormal_u16[idx_1]);           /* 从速查表加载点阵 */
            tmp_16x8x2_1.val[0] = vandq_u16(tmpDot_16x8_1, lineDotDst_16x8);

            tmpDot_16x8_2 = vld1q_u16(dotTableNormal_u16[idx_2]);
            tmp_16x8x2_1.val[1] = vandq_u16(tmpDot_16x8_2, lineDotDst_16x8);

            tmp_16x8x2_2 = vuzpq_u16(tmp_16x8x2_1.val[0], tmp_16x8x2_1.val[1]);

            vst2q_u16(pSroreDst, tmp_16x8x2_2);                          /* 存储 */
        }
    }
    else if (scale == 2)
    {
        uint16_t * pStoreDst2;
        uint16x8x2_t tmp_16x8x2_1, tmp_16x8x2_2;

        for(i = 0; i < FRONT_HEIGHT; i ++)
        {
            idx_1 = *(pFont + i * 2);                       /* 行前 8bit idx */
            idx_2 = *(pFont + i * 2 + 1);                   /* 行后 8bit idx */

            pSroreDst = pDst + pitch * i * scale;           /* 偶数行存储地址 */
            pStoreDst2 = pSroreDst + pitch;                 /* 奇数行存储地址 */

            tmpDot_16x8_1 = vld1q_u16(dotTableNormal_u16[idx_1]);                       /* 从速查表加载点阵 */
            tmp_16x8x2_1.val[0] = vandq_u16(tmpDot_16x8_1, lineDotDst_16x8);     /* 生成对应的像素值 */
            tmp_16x8x2_1.val[1] = tmp_16x8x2_1.val[0];                        /* 行前 8bit 像素扩展 */

            tmpDot_16x8_2 = vld1q_u16(dotTableNormal_u16[idx_2]);
            tmp_16x8x2_2.val[0] = vandq_u16(tmpDot_16x8_2, lineDotDst_16x8);
            tmp_16x8x2_2.val[1] = tmp_16x8x2_2.val[0];

            vst2q_u16(pSroreDst, tmp_16x8x2_1);                  /* 存储行前 8bit 像素 */
            vst2q_u16(pSroreDst + HZ_WIDTH, tmp_16x8x2_2);       /* 存储行后 8bit 像素 */

            vst2q_u16(pStoreDst2, tmp_16x8x2_1);
            vst2q_u16(pStoreDst2 + HZ_WIDTH, tmp_16x8x2_2);
        }

    }
    else if (scale == 3)
    {
        uint16_t * pStoreDst2;
        uint16_t * pStoreDst3;
        uint16x8x3_t out_16x8x3_1, out_16x8x3_2;    /* 存放交织后的数据 */

        for(i = 0; i < FRONT_HEIGHT; i++)
        {
            idx_1 = *(pFont + i * 2);
            idx_2 = *(pFont + i * 2 + 1);

            pSroreDst = pDst + pitch * i * scale;
            pStoreDst2 = pSroreDst + pitch;
            pStoreDst3 = pSroreDst + pitch * 2;

            tmpDot_16x8_1 = vld1q_u16(dotTableNormal_u16[idx_1]);                   /* 从速查表加载点阵 */
            out_16x8x3_1.val[0] = vandq_u16(tmpDot_16x8_1, lineDotDst_16x8);    /* 生成对应的像素值 */
            out_16x8x3_1.val[1] = out_16x8x3_1.val[0];
            out_16x8x3_1.val[2] = out_16x8x3_1.val[0];

            tmpDot_16x8_2 = vld1q_u16(dotTableNormal_u16[idx_2]);                   /* 从速查表加载点阵 */
            out_16x8x3_2.val[0] = vandq_u16(tmpDot_16x8_2, lineDotDst_16x8);    /* 生成对应的像素值 */
            out_16x8x3_2.val[1] = out_16x8x3_2.val[0];
            out_16x8x3_2.val[2] = out_16x8x3_2.val[0];

            vst3q_u16(pSroreDst, out_16x8x3_1);          /* 行存储 */
            vst3q_u16(pSroreDst + 24, out_16x8x3_2);
            vst3q_u16(pStoreDst2, out_16x8x3_1);
            vst3q_u16(pStoreDst2 + 24, out_16x8x3_2);
            vst3q_u16(pStoreDst3, out_16x8x3_1);
            vst3q_u16(pStoreDst3 + 24, out_16x8x3_2);

        }
    }
    else if(scale == 4)
    {
        uint16_t * pStoreDst2;
        uint16_t * pStoreDst3;
        uint16_t * pStoreDst4;
        uint16x8x4_t out_16x8x4_1, out_16x8x4_2;

        for(i = 0; i < FRONT_HEIGHT; i++)
        {
            idx_1 = *(pFont + i * 2);
            idx_2 = *(pFont + i * 2 + 1);

            pSroreDst = pDst + pitch * i * scale;
            pStoreDst2 = pSroreDst + pitch;
            pStoreDst3 = pSroreDst + pitch * 2;
            pStoreDst4 = pSroreDst + pitch * 3;

            tmpDot_16x8_1 = vld1q_u16(dotTableNormal_u16[idx_1]);                   /* 从速查表加载点阵 */
            out_16x8x4_1.val[0] = vandq_u16(tmpDot_16x8_1, lineDotDst_16x8);    /* 生成对应的像素值 */
            out_16x8x4_1.val[1] = out_16x8x4_1.val[0];
            out_16x8x4_1.val[2] = out_16x8x4_1.val[0];
            out_16x8x4_1.val[3] = out_16x8x4_1.val[1];

            tmpDot_16x8_2 = vld1q_u16(dotTableNormal_u16[idx_2]);                   /* 从速查表加载点阵 */
            out_16x8x4_2.val[0] = vandq_u16(tmpDot_16x8_2, lineDotDst_16x8);    /* 生成对应的像素值 */
            out_16x8x4_2.val[1] = out_16x8x4_2.val[0];
            out_16x8x4_2.val[2] = out_16x8x4_2.val[0];
            out_16x8x4_2.val[3] = out_16x8x4_2.val[1];


            vst4q_u16(pSroreDst, out_16x8x4_1);          /* 行存储 */
            vst4q_u16(pSroreDst + 32, out_16x8x4_2);
            vst4q_u16(pStoreDst2, out_16x8x4_1);
            vst4q_u16(pStoreDst2 + 32, out_16x8x4_2);
            vst4q_u16(pStoreDst3, out_16x8x4_1);
            vst4q_u16(pStoreDst3 + 32, out_16x8x4_2);
            vst4q_u16(pStoreDst4, out_16x8x4_1);
            vst4q_u16(pStoreDst4 + 32, out_16x8x4_2);
        }
    }
    else
    {
        Cprintf_yellow("[%s %d]  notSupport! scale:[%d]\n", __func__, __LINE__, scale);
    }

    return;
}


/**
 * @function:   neon_CreatAsciiDot_yuv_u8
 * @brief:      生成 YUV 叠加 ascii 所需的点阵掩码(非绘制 YUV)
 * @param[in]:  uint8_t * pFont         字符字库地址
 * @param[in]:  uint8_t * pDst          目的点阵地址(需手动计算起始像素地址)
 * @param[in]:  uint32_t  pitch         行 pitch (单位像素, 1B)
 * @param[in]:  uint8_t   scale         字符规模(8x16 的倍数, 只支持 x2, x4)
 * @param[out]: None
 * @return:     void
 */
void neon_CreatAsciiDot_yuv_u8(uint8_t * pFont, uint8_t * pDst, uint32_t pitch, uint8_t scale)
{
    if ((pFont == NULL) || (pDst == NULL))
    {
        Cprintf_red("[%s %d]  pFont:[%p], pDst:[%p]\n", __func__, __LINE__, pFont, pDst);
        return;
    }

    uint8_t i = 0, idx = 0;        /* 在点阵 tab 中的 idx */
    uint8_t * pSroreDst = NULL;    /* 指向点阵存储的地址 */

    if(scale == 2)
    {
        uint8x8x2_t tmp_8x8x2;            /* 存放交织后的数据 */
        uint8_t * pStoreDst2 = NULL;      /* 奇数行存储地址 */

        for(i = 0; i < FRONT_HEIGHT; i ++)
        {
            idx = *(pFont + i);
            pSroreDst = pDst + pitch * i * 2;
            pStoreDst2 = pSroreDst + pitch;

            tmp_8x8x2.val[0] = vld1_u8 (dotTableNormal_u8[idx]);           /* 从速查表加载点阵 */
            tmp_8x8x2.val[1] = tmp_8x8x2.val[0];
            vst2_u8(pSroreDst, tmp_8x8x2);
            vst2_u8(pStoreDst2, tmp_8x8x2);
        }
    }
    else if (scale == 4)
    {
        uint8x8x4_t tmp_8x8x4;            /* 存放交织后的数据 */
        uint8_t * pStoreDst2;
        uint8_t * pStoreDst3;
        uint8_t * pStoreDst4;

        for(i = 0; i < FRONT_HEIGHT; i ++)
        {
            idx = *(pFont + i);
            pSroreDst = pDst + pitch * i * 4;
            pStoreDst2 = pSroreDst + pitch;
            pStoreDst3 = pSroreDst + pitch * 2;
            pStoreDst4 = pSroreDst + pitch * 3;

            tmp_8x8x4.val[0] = vld1_u8 (dotTableNormal_u8[idx]);           /* 从速查表加载点阵 */
            tmp_8x8x4.val[1] = tmp_8x8x4.val[0];
            tmp_8x8x4.val[2] = tmp_8x8x4.val[0];
            tmp_8x8x4.val[3] = tmp_8x8x4.val[1];
            vst4_u8(pSroreDst, tmp_8x8x4);
            vst4_u8(pStoreDst2, tmp_8x8x4);
            vst4_u8(pStoreDst3, tmp_8x8x4);
            vst4_u8(pStoreDst4, tmp_8x8x4);
        }
    }
    else
    {
        Cprintf_yellow("[%s %d]  notSupport! scale:[%d]\n", __func__, __LINE__, scale);
    }

    return;
}


/**
 * @function:   neon_CreatChineseDot_yuv_u8
 * @brief:      生成 YUV 叠加 汉字 所需的点阵掩码, 只支持 x2, x4; (非绘制 YUV)
 * @param[in]:  uint8_t * pFont         字符字库地址
 * @param[in]:  uint8_t * pDst          目的点阵地址(需手动计算起始像素地址)
 * @param[in]:  uint32_t  pitch         行 pitch (单位像素, 1B)
 * @param[in]:  uint8_t   scale         字符规模(16x16 的倍数, 只支持 x2, x4)
 * @param[out]: None
 * @return:     void
 */
void neon_CreatChineseDot_yuv_u8(uint8_t * pFont, uint8_t * pDst, uint32_t pitch, uint8_t scale)
{
    if ((pFont == NULL) || (pDst == NULL))
    {
        Cprintf_red("[%s %d]  \n", __func__, __LINE__);
        return;
    }

    uint8_t i = 0, idx_1 = 0, idx_2 = 0;         /* 在点阵 tab 中的 idx */
    uint8_t * pSroreDst = NULL;    /* 指向点阵存储的地址 */

    if(scale == 2)
    {
        uint8x8x2_t tmp_8x8x2_1, tmp_8x8x2_2;            /* 存放交织后的数据 */
        uint8_t * pStoreDst2 = NULL;                     /* 奇数行存储地址 */

        for(i = 0; i < FRONT_HEIGHT; i ++)
        {
            idx_1 = *(pFont + i * 2);                       /* 行前 8bit idx */
            idx_2 = *(pFont + i * 2 + 1);                   /* 行后 8bit idx */

            pSroreDst = pDst + pitch * i * 2;
            pStoreDst2 = pSroreDst + pitch;

            tmp_8x8x2_1.val[0] = vld1_u8 (dotTableNormal_u8[idx_1]);           /* 从速查表加载点阵 */
            tmp_8x8x2_1.val[1] = tmp_8x8x2_1.val[0];

            tmp_8x8x2_2.val[0] = vld1_u8 (dotTableNormal_u8[idx_2]);           /* 从速查表加载点阵 */
            tmp_8x8x2_2.val[1] = tmp_8x8x2_2.val[0];

            vst2_u8(pSroreDst, tmp_8x8x2_1);                  /* 存储行前 8bit 像素 */
            vst2_u8(pSroreDst + HZ_WIDTH, tmp_8x8x2_2);       /* 存储行后 8bit 像素 */

            vst2_u8(pStoreDst2, tmp_8x8x2_1);                  /* 存储行前 8bit 像素 */
            vst2_u8(pStoreDst2 + HZ_WIDTH, tmp_8x8x2_2);       /* 存储行后 8bit 像素 */
        }
    }
    else if(scale == 4)
    {
        /* each:[0.810 us] */
        uint8x8x4_t tmp_8x8x4_1, tmp_8x8x4_2;            /* 存放交织后的数据 */
        uint8_t * pStoreDst2;
        uint8_t * pStoreDst3;
        uint8_t * pStoreDst4;

        for(i = 0; i < FRONT_HEIGHT; i ++)
        {
            idx_1 = *(pFont + i * 2);                       /* 行前 8bit idx */
            idx_2 = *(pFont + i * 2 + 1);                   /* 行后 8bit idx */

            pSroreDst = pDst + pitch * i * 4;
            pStoreDst2 = pSroreDst + pitch;
            pStoreDst3 = pSroreDst + pitch * 2;
            pStoreDst4 = pSroreDst + pitch * 3;

            tmp_8x8x4_1.val[0] = vld1_u8 (dotTableNormal_u8[idx_1]);           /* 从速查表加载点阵 */
            tmp_8x8x4_1.val[1] = tmp_8x8x4_1.val[0];
            tmp_8x8x4_1.val[2] = tmp_8x8x4_1.val[0];
            tmp_8x8x4_1.val[3] = tmp_8x8x4_1.val[1];

            tmp_8x8x4_2.val[0] = vld1_u8 (dotTableNormal_u8[idx_2]);           /* 从速查表加载点阵 */
            tmp_8x8x4_2.val[1] = tmp_8x8x4_2.val[0];
            tmp_8x8x4_2.val[2] = tmp_8x8x4_2.val[0];
            tmp_8x8x4_2.val[3] = tmp_8x8x4_2.val[1];

            vst4_u8(pSroreDst, tmp_8x8x4_1);
            vst4_u8(pSroreDst + HZ_WIDTH * 2, tmp_8x8x4_2);
            vst4_u8(pStoreDst2, tmp_8x8x4_1);
            vst4_u8(pStoreDst2 + HZ_WIDTH * 2, tmp_8x8x4_2);
            vst4_u8(pStoreDst3, tmp_8x8x4_1);
            vst4_u8(pStoreDst3 + HZ_WIDTH * 2, tmp_8x8x4_2);
            vst4_u8(pStoreDst4, tmp_8x8x4_1);
            vst4_u8(pStoreDst4 + HZ_WIDTH * 2, tmp_8x8x4_2);
        }
    }
    else
    {
        Cprintf_yellow("[%s %d]  notSupport! scale:[%d]\n", __func__, __LINE__, scale);
    }

#if 0    //使用 dotTableNormal_u16, 但效率更低
    if(scale == 2)
    {
        /* each:[0.380 us] */
        uint8x16_t tmp_8x16_1, tmp_8x16_2;            /* 存放交织后的数据 */
        uint8_t * pStoreDst2;                           /* 奇数行存储地址 */

        for(i = 0; i < FRONT_HEIGHT; i ++)
        {
            idx_1 = *(pFont + i * 2);                       /* 行前 8bit idx */
            idx_2 = *(pFont + i * 2 + 1);                   /* 行后 8bit idx */

            pSroreDst = pDst + pitch * i * 2;
            pStoreDst2 = pSroreDst + pitch;

            tmp_8x16_1 = vld1q_u8( (uint8_t *) (dotTableNormal_u16[idx_1]) );           /* 从速查表加载点阵 */
            tmp_8x16_2 = vld1q_u8( (uint8_t *) (dotTableNormal_u16[idx_2]) );           /* 从速查表加载点阵 */

            vst1q_u8(pSroreDst, tmp_8x16_1);                  /* 存储行前 8bit 像素 */
            vst1q_u8(pSroreDst + HZ_WIDTH, tmp_8x16_2);       /* 存储行后 8bit 像素 */

            vst1q_u8(pStoreDst2, tmp_8x16_1);                  /* 存储行前 8bit 像素 */
            vst1q_u8(pStoreDst2 + HZ_WIDTH, tmp_8x16_2);       /* 存储行后 8bit 像素 */
        }
    }
    else if (scale == 4)
    {
        /* each:[0.835 us] */
        uint8x16x2_t tmp_8x16x2_1, tmp_8x16x2_2;            /* 存放交织后的数据 */
        uint8_t * pStoreDst2;                           /* 奇数行存储地址 */
        uint8_t * pStoreDst3;
        uint8_t * pStoreDst4;

        for(i = 0; i < FRONT_HEIGHT; i ++)
        {
            idx_1 = *(pFont + i * 2);                       /* 行前 8bit idx */
            idx_2 = *(pFont + i * 2 + 1);                   /* 行后 8bit idx */

            pSroreDst = pDst + pitch * i * 4;
            pStoreDst2 = pSroreDst + pitch;
            pStoreDst3 = pSroreDst + pitch * 2;
            pStoreDst4 = pSroreDst + pitch * 3;

            tmp_8x16x2_1.val[0] = vld1q_u8( (uint8_t *) (dotTableNormal_u16[idx_1]) );           /* 从速查表加载点阵 */
            tmp_8x16x2_1.val[1] = tmp_8x16x2_1.val[0];
            tmp_8x16x2_2.val[0] = vld1q_u8( (uint8_t *) (dotTableNormal_u16[idx_2]) );           /* 从速查表加载点阵 */
            tmp_8x16x2_2.val[1] = tmp_8x16x2_2.val[0];

            vst2q_u8(pSroreDst, tmp_8x16x2_1);                  /* 存储行前 8bit 像素 */
            vst2q_u8(pSroreDst + HZ_WIDTH * 2, tmp_8x16x2_2);       /* 存储行后 8bit 像素 */

            vst2q_u8(pStoreDst2, tmp_8x16x2_1);
            vst2q_u8(pStoreDst2 + HZ_WIDTH * 2, tmp_8x16x2_2);

            vst2q_u8(pStoreDst3, tmp_8x16x2_1);
            vst2q_u8(pStoreDst3 + HZ_WIDTH * 2, tmp_8x16x2_2);

            vst2q_u8(pStoreDst4, tmp_8x16x2_1);
            vst2q_u8(pStoreDst4 + HZ_WIDTH * 2, tmp_8x16x2_2);
        }
    }
    else
    {
        Cprintf_yellow("[%s %d]  notSupport! scale:[%d]\n", __func__, __LINE__, scale);
    }
#endif   /* end of (#if 0  //使用 dotTableNormal_u16, 但效率更低) */

    return;
}


/**
 * @function:   _neon_DrawDot_Y
 * @brief:      绘制 Y 分量
 * @param[in]:  uint32_t startIdx
 * @param[in]:  uint8_t * m_pDst
 * @param[in]:  int8_t * m_pDot
 * @param[in]:  uint16_t x16_Cnt
 * @param[in]:  uint8_t lineIdx
 * @param[in]:  uint8_t j
 * @param[in]:  uint8x16_t dst_8x16_1
 * @param[in]:  uint8x16_t y_8x16_1
 * @param[in]:  uint8x16_t dot_8x16_1
 * @param[in]:  YUV_DRAW_PARAM * pParam
 * @param[out]: None
 * @return:     static void inline
 */
static void inline _neon_DrawDot_Y(uint32_t startIdx,
                             uint8_t * m_pDst,    uint8_t * m_pDot,
                             uint32_t x16_Cnt,    uint32_t lineIdx,  uint32_t j,
                             uint8x16_t dst_8x16_1, uint8x16_t y_8x16_1, uint8x16_t dot_8x16_1,
                             uint8x16_t dst_8x16_2, uint8x16_t y_8x16_2, uint8x16_t dot_8x16_2,
                             YUV_DRAW_PARAM * pParam)
{
    startIdx = pParam->osdStartY * pParam->frmStride + pParam->osdStartX;
    m_pDst = pParam->pFrmVirAddr[0] + startIdx;

    y_8x16_1 = vdupq_n_u8(pParam->Y);
    y_8x16_2 = y_8x16_1;

    /* Y 赋值 */
    for(lineIdx = 0; lineIdx < pParam->dotHeight; lineIdx++)
    {
        for(j = 0; j < x16_Cnt / 2; j ++)
        {
            dot_8x16_1 = vld1q_u8(m_pDot);
            dot_8x16_2 = vld1q_u8(m_pDot + 16);

            dst_8x16_1 = vld1q_u8(m_pDst);          /* dst 加载 */
            dst_8x16_2 = vld1q_u8(m_pDst + 16);

            /* dest = vbslq_u8(mask, src, dest); */
            dst_8x16_1 = vbslq_u8(dot_8x16_1, y_8x16_1, dst_8x16_1);    /* 位选择 */
            dst_8x16_2 = vbslq_u8(dot_8x16_2, y_8x16_2, dst_8x16_2);

            vst1q_u8(m_pDst, dst_8x16_1);           /* 存储 */
            vst1q_u8(m_pDst + 16, dst_8x16_2);

            m_pDst += 32;
            m_pDot += 32;
        }

        if (x16_Cnt % 2) {
            dot_8x16_1 = vld1q_u8(m_pDot);
            dst_8x16_1 = vld1q_u8(m_pDst);          /* dst 加载 */
            dst_8x16_1 = vbslq_u8(dot_8x16_1, y_8x16_1, dst_8x16_1);    /* 位选择 */
            vst1q_u8(m_pDst, dst_8x16_1);           /* 存储 */
            m_pDot += 16;
            m_pDst += 16;
        }

        m_pDst = pParam->pFrmVirAddr[0] + startIdx + lineIdx * pParam->frmStride;
        m_pDot = pParam->pDot + lineIdx * pParam->dotPitch;
    }

    return;
}


/**
 * @function:   neon_DrawDot_YUV420_Gray
 * @brief:      在 OSD 上绘制灰度, Y 应为 planar (连续存放)
 * @param[in]:  YUV_DRAW_PARAM * pParam
 * @param[out]: None
 * @return:     void
 */
void neon_DrawDot_YUV_Gray(YUV_DRAW_PARAM * pParam)
{
    if((pParam == NULL) || (pParam->dotWidth % 16))
    {
        Cprintf_red("[%s %d]  pParam:[%p], pitch:[%d]\n",
                    __func__, __LINE__, pParam, pParam->dotWidth);
        return;
    }

    uint32_t startIdx = 0;
    uint8_t * m_pDst = NULL;
    uint8_t * m_pDot = NULL;
    uint32_t lineIdx = 0, j = 0;
    uint32_t x16_Cnt = 0;

    uint8x16_t dst_8x16_1, dst_8x16_2;
    uint8x16_t y_8x16_1, y_8x16_2;
    uint8x16_t dot_8x16_1, dot_8x16_2;


    m_pDot = pParam->pDot;
    x16_Cnt = pParam->dotWidth / 16; /* Y 操作一次赋值 16 个像素 */

    _neon_DrawDot_Y(startIdx,
                    m_pDst, m_pDot,
                    x16_Cnt, lineIdx, j,
                    dst_8x16_1, y_8x16_1, dot_8x16_1,
                    dst_8x16_2, y_8x16_2, dot_8x16_2,
                    pParam);

    return;
}

//#define OSD_ONLY_SUPPORT_GRAY
#ifndef OSD_ONLY_SUPPORT_GRAY

/**
 * @function:   neon_DrawDot_I420_YV12
 * @brief:      在  I420 和 YV12 上绘制 OSD
 * @param[in]:  YUV_DRAW_PARAM * pParam
 * @param[out]: None
 * @return:     void
 */
void neon_DrawDot_I420_YV12(YUV_DRAW_PARAM * pParam)
{
    if((pParam == NULL) || (pParam->dotWidth % 16))
    {
        Cprintf_red("[%s %d]  pParam:[%p], pitch:[%d]\n",
                    __func__, __LINE__, pParam, pParam->dotWidth);
        return;
    }

    printf("[%s %d]  \n", __func__, __LINE__);

    uint32_t startIdx = 0;
    uint8_t * m_pDst = NULL;
    uint8_t * m_pDot = NULL;
    uint32_t lineIdx = 0, j = 0;
    uint32_t x16_Cnt = 0;

    uint8x16_t dst_8x16_1, dst_8x16_2;
    uint8x16_t y_8x16_1, y_8x16_2;
    uint8x16_t dot_8x16_1, dot_8x16_2;

    m_pDot = pParam->pDot;
    x16_Cnt = pParam->dotWidth / 16; /* Y 操作一次赋值 16 个像素 */

    _neon_DrawDot_Y(startIdx,
                    m_pDst, m_pDot,
                    x16_Cnt, lineIdx, j,
                    dst_8x16_1, y_8x16_1, dot_8x16_1,
                    dst_8x16_2, y_8x16_2, dot_8x16_2,
                    pParam);

    /* U 赋值 */
    uint8x16x2_t dot_8x16x2;

    /* YUV420, U 的 w,h 采样各一半 */
    startIdx = pParam->osdStartY / 2 * pParam->frmStride / 2 + pParam->osdStartX / 2;

    y_8x16_1 = vdupq_n_u8(pParam->U);
    for(lineIdx = 0; lineIdx < pParam->dotHeight / 2; lineIdx++)    /* 高度缩减一半 */
    {
        m_pDst = pParam->pFrmVirAddr[1] + startIdx + lineIdx * pParam->frmStride / 2 ;
        m_pDot = pParam->pDot + lineIdx * 2u * pParam->dotPitch;

        for(j = 0; j < x16_Cnt / 2; j ++)
        {
            dot_8x16x2 = vld2q_u8(m_pDot);

            dst_8x16_1 = vld1q_u8(m_pDst);

            dst_8x16_1 = vbslq_u8(dot_8x16x2.val[0], y_8x16_1, dst_8x16_1);    /* 位选择 */

            vst1q_u8(m_pDst, dst_8x16_1);     /* 存储 */

            m_pDst += 16;
            m_pDot += 32;
        }

        // TODO : 优化此处代码
        if (x16_Cnt % 2) {
            uint8x8x2_t dot_8x8x2 = vld2_u8(m_pDot);
            uint8x8_t dst_8x8_1 = vld1_u8(m_pDst);
            dst_8x8_1 = vbsl_u8(dot_8x8x2.val[0], vget_high_u8(y_8x16_1), dst_8x8_1);
            vst1_u8(m_pDst, dst_8x8_1);
            m_pDst += 8;
            m_pDot += 16;
        }
    }

    /* V 赋值 */
    y_8x16_1 = vdupq_n_u8(pParam->V);

    for(lineIdx = 0; lineIdx < pParam->dotHeight / 2; lineIdx++)
    {
        m_pDst = pParam->pFrmVirAddr[2] + startIdx + lineIdx * pParam->frmStride / 2;
        m_pDot = pParam->pDot + lineIdx * 2u * pParam->dotPitch;

        for(j = 0; j < x16_Cnt / 2; j ++)
        {
            dot_8x16x2 = vld2q_u8(m_pDot);

            dst_8x16_1 = vld1q_u8(m_pDst);

            dst_8x16_1 = vbslq_u8(dot_8x16x2.val[0], y_8x16_1, dst_8x16_1);    /* 位选择 */

            vst1q_u8(m_pDst, dst_8x16_1);     /* 存储 */

            m_pDst += 16;
            m_pDot += 32;
        }

        if (x16_Cnt % 2) {
            uint8x8x2_t dot_8x8x2 = vld2_u8(m_pDot);
            uint8x8_t dst_8x8_1 = vld1_u8(m_pDst);
            dst_8x8_1 = vbsl_u8(dot_8x8x2.val[0], vget_high_u8(y_8x16_1), dst_8x8_1);
            vst1_u8(m_pDst, dst_8x8_1);
            m_pDst += 8;
            m_pDot += 16;
        }
    }

    return;
}

/**
 * @function:   neon_DrawDot_I420_YV12
 * @brief:      在  I420 和 YV12 上绘制 OSD
 * @param[in]:  YUV_DRAW_PARAM * pParam
 * @param[out]: None
 * @return:     void
 */
void neon_DrawDot_NV12_NV21(YUV_DRAW_PARAM * pParam)
{
    if((pParam == NULL) || (pParam->dotWidth % 16))
    {
        Cprintf_red("[%s %d]  pParam:[%p], pitch:[%d]\n",
                    __func__, __LINE__, pParam, pParam->dotWidth);
        return;
    }

    uint32_t startIdx = 0;
    uint8_t * m_pDst = NULL;
    uint8_t * m_pDot = NULL;
    uint32_t lineIdx = 0, j = 0;
    uint32_t x16_Cnt = 0;

    uint8x16_t dst_8x16_1, dst_8x16_2;
    uint8x16_t y_8x16_1, y_8x16_2;
    uint8x16_t dot_8x16_1, dot_8x16_2;

    m_pDot = pParam->pDot;
    x16_Cnt = (pParam->dotWidth / 16); /* Y 操作一次赋值 16 个像素 */

    _neon_DrawDot_Y(startIdx,
                    m_pDst, m_pDot,
                    x16_Cnt, lineIdx, j,
                    dst_8x16_1, y_8x16_1, dot_8x16_1,
                    dst_8x16_2, y_8x16_2, dot_8x16_2,
                    pParam);


    /* uv 起始偏移地址 */
    startIdx = pParam->osdStartY * pParam->frmStride / 2 + pParam->osdStartX;

    uint16x8_t uv_16x8;
    uint16_t uv_Value = (pParam->yuvFrmFomat == YUV_420_NV12)
                          ? ((uint16_t)pParam->V * 256 + pParam->U)
                          : ((uint16_t)pParam->U * 256 + pParam->V);

    uv_16x8 = vdupq_n_u16(uv_Value);
    y_8x16_1 = vreinterpretq_u8_u16(uv_16x8);
    y_8x16_2 = y_8x16_1;

    /* Y 赋值 */
    for(lineIdx = 0; lineIdx < pParam->dotHeight / 2; lineIdx++)
    {
        m_pDst = pParam->pFrmVirAddr[1] + startIdx + lineIdx * pParam->frmStride;
        m_pDot = pParam->pDot + lineIdx * 2 * pParam->dotPitch;

        for(j = 0; j < x16_Cnt / 2; j ++)
        {
            dot_8x16_1 = vld1q_u8(m_pDot);
            dot_8x16_2 = vld1q_u8(m_pDot + 16);

            dst_8x16_1 = vld1q_u8(m_pDst);          /* dst 加载 */
            dst_8x16_2 = vld1q_u8(m_pDst + 16);

            /* dest = vbslq_u8(mask, src, dest); */
            dst_8x16_1 = vbslq_u8(dot_8x16_1, y_8x16_1, dst_8x16_1);    /* 位选择 */
            dst_8x16_2 = vbslq_u8(dot_8x16_2, y_8x16_2, dst_8x16_2);

            vst1q_u8(m_pDst, dst_8x16_1);           /* 存储 */
            vst1q_u8(m_pDst + 16, dst_8x16_2);

            m_pDst += 32;
            m_pDot += 32;
        }
        
       if (x16_Cnt % 2) {
           dot_8x16_1 = vld1q_u8(m_pDot);
           dst_8x16_1 = vld1q_u8(m_pDst);          /* dst 加载 */           
           dst_8x16_1 = vbslq_u8(dot_8x16_1, y_8x16_1, dst_8x16_1);    /* 位选择 */           
           vst1q_u8(m_pDst, dst_8x16_1);           /* 存储 */
           m_pDst += 16;
           m_pDot += 16;
       }
    }
}

/**
 * @function:   neon_DrawDot_YUYV_UYVY
 * @brief:      在 422 的   YUYV 和 UYVY 上绘制 OSD
 * @param[in]:  YUV_DRAW_PARAM * pParam
 * @param[out]: None
 * @return:     void
 */
void neon_DrawDot_YUYV_UYVY(YUV_DRAW_PARAM * pParam)
{
    if((pParam == NULL) || (pParam->dotWidth % 16))
    {
        Cprintf_red("[%s %d]  pParam:[%p], pitch:[%d]\n",
                    __func__, __LINE__, pParam, pParam->dotWidth);
        return;
    }

    uint32_t startIdx;
    uint8_t * m_pDst;
    uint8_t * m_pDot;
    uint32_t lineIdx, j;
    uint32_t x16_Cnt;

    uint8x16_t dst_8x16_1, dst_8x16_2;
    uint8x16_t y_8x16_1, y_8x16_2;
    uint8x8x2_t tmpDot_8x8x2_1, tmpDot_8x8x2_2;
    uint8x16_t dot_8x16_1, dot_8x16_2;

    /* uv 起始偏移地址 */
    startIdx = pParam->osdStartY * pParam->frmStride + pParam->osdStartX * 2;
    x16_Cnt = pParam->dotWidth / 16; /* Y 操作一次赋值 16 个像素 */

    /* 生成 src 颜色矩阵 */
    uint32x4_t uv_32x4;
    uint32_t uv_Value = (pParam->yuvFrmFomat == YUV_422_YUYV)
                          ? ( (uint32_t)(pParam->V << 24u) + (uint32_t)(pParam->Y << 16u)
                                + (uint32_t)(pParam->U << 8u) + (uint32_t)pParam->Y)
                          : ( (uint32_t)(pParam->Y << 24u) + (uint32_t)(pParam->V << 16u)
                                + (uint32_t)(pParam->Y << 8u) + (uint32_t)pParam->U);

    uv_32x4 = vdupq_n_u32(uv_Value);
    y_8x16_1 = vreinterpretq_u8_u32(uv_32x4);
    y_8x16_2 = y_8x16_1;

    /* Y 赋值 */
    for(lineIdx = 0; lineIdx < pParam->dotHeight; lineIdx++)
    {
        m_pDst = pParam->pFrmVirAddr[0] + startIdx + lineIdx * pParam->frmStride;
        m_pDot = pParam->pDot + lineIdx * pParam->dotPitch;

        for(j = 0; j < x16_Cnt; j ++)
        {
            /* 生成 mask 颜色掩码 */
            tmpDot_8x8x2_1.val[0] = vld1_u8(m_pDot);
            tmpDot_8x8x2_1.val[1] = tmpDot_8x8x2_1.val[0];
            vst2_u8((uint8_t *)&dot_8x16_1, tmpDot_8x8x2_1);

            tmpDot_8x8x2_2.val[0] = vld1_u8(m_pDot + 8);
            tmpDot_8x8x2_2.val[1] = tmpDot_8x8x2_2.val[0];
            vst2_u8((uint8_t *)&dot_8x16_2, tmpDot_8x8x2_2);

            dst_8x16_1 = vld1q_u8(m_pDst);          /* dst 加载 */
            dst_8x16_2 = vld1q_u8(m_pDst + 16);

            /* dest = vbslq_u8(mask, src, dest); */
            dst_8x16_1 = vbslq_u8(dot_8x16_1, y_8x16_1, dst_8x16_1);    /* 位选择 */
            dst_8x16_2 = vbslq_u8(dot_8x16_2, y_8x16_2, dst_8x16_2);

            vst1q_u8(m_pDst, dst_8x16_1);           /* 存储 */
            vst1q_u8(m_pDst + 16, dst_8x16_2);

            m_pDst += 32;
            m_pDot += 16;
        }
    }
}


#endif


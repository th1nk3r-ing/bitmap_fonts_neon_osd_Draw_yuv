/**
 * @file:   osd_base.h
 * @note:   2017-2030, <git.oschia.net/think3r>
 * @brief:: osd_base.c 的头文件
 * @author: think3r
 * @date:   2019/8/18
 * @note:
 * @note \n History:
   1.日    期: 2019/8/18
     作    者:  think3r
     修改历史: 创建文件
 */

#ifndef __OSD_BASE_H__
#define __OSD_BASE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/*----------------------------------------------*/
/*                  头文件包含                  */
/*----------------------------------------------*/
#include <stdint.h>

/*----------------------------------------------*/
/*                  宏类型定义                  */
/*----------------------------------------------*/
#define bool    char
#define true    (1)
#define false   (0)

#define Cprintf_green(format,...)    printf("\e[1;32m" format "\e[0m", ##__VA_ARGS__)
#define Cprintf_yellow(format,...)    printf("\e[1;33m" format "\e[0m", ##__VA_ARGS__)
#define Cprintf_red(format,...)    printf("\e[1;31m" format "\e[0m", ##__VA_ARGS__)

#define     ASCII_WIDTH         (8u)        /* ASCII 宽度 */
#define     HZ_WIDTH            (16u)       /* 汉字宽度 */
#define     FRONT_HEIGHT        (16u)       /* 字符高度 */
#define     ASCII_NUM           (128u)      /* ASCII 字符个数 */
#define     HZK16_FRONT_SIZE    (267616u)   /* 汉字字库大小 */
#define     ASCII8_FRONT_SIZE   (4096u)

/* bt.601 下的 GRB 转 yuv */
#define     GET_Y_BT601(R,G,B)  ((uint8_t)(16 +  0.257 * (R) + 0.504 * (G) + 0.098 * (B)))
#define     GET_U_BT601(R,G,B)  ((uint8_t)(128 - 0.148 * (R) - 0.291 * (G) + 0.439 * (B)))
#define     GET_V_BT601(R,G,B)  ((uint8_t)(128 + 0.439 * (R) - 0.368 * (G) - 0.071 * (B)))

/* bt.709 下的 GRB 转 yuv */
#define     GET_Y_BT709(R,G,B)  ((uint8_t)(16   + 0.183 * (R) + 0.614 * (G) + 0.062 * (B)))
#define     GET_U_BT709(R,G,B)  ((uint8_t)(128 - 0.101 * (R) - 0.339 * (G) + 0.439 * (B)))
#define     GET_V_BT709(R,G,B)  ((uint8_t)(128 + 0.439 * (R) - 0.399 * (G) - 0.040 * (B)))


/*----------------------------------------------*/
/*                 结构体定义                   */
/*----------------------------------------------*/
typedef enum _YUV_FORMAT_
{
    YUV_ERR_YPTE = 0,
    
    YUV_420_I420 = 0x01,    /* [YYYYYYYY] [UU] [VV] */
    YUV_420_YV12 = 0x02,    /* [YYYYYYYY] [VV] [UU] */
    YUV_420_NV12 = 0x03,    /* [YYYYYYYY] [UVUV]    */
    YUV_420_NV21 = 0x04,    /* [YYYYYYYY] [VUVU]    */

    YUV_422_P422 = 0x11,    /* [YYYY] [UU] [VV] */
    YUV_422_S422 = 0x12,    /* [YYYY] [UVUV]    */
    YUV_422_YUYV = 0x13,    /* [UYVY UYVY]      */
    YUV_422_UYVY = 0x14,    /* [YUYV YUYV]      */

    YUV_FORMAT_BUTT
}YUV_FORMAT;

typedef struct _OSD_YUV_DRAW_PARAM_
{
    uint8_t * pFrmVirAddr[3];   /* yuv-frm 的 Y[0], U[1], V[2] 虚拟地址 (uv 顺序要对)  */
    uint8_t * pDot;

    YUV_FORMAT yuvFrmFomat;
    uint32_t frmWidth;      /* 暂时不用 */
    uint32_t frmStride;
    uint32_t frmHeight;

    uint32_t dotWidth;      /* 点阵内存的实际宽度 */
    uint32_t dotPitch;      /* 点阵内存的 pitch */
    uint32_t dotHeight;

    uint32_t osdStartX;
    uint32_t osdStartY;

    uint8_t  Y;             /* 要填充字体的 yuv 颜色 */
    uint8_t  U;
    uint8_t  V;
    uint8_t  res[0];
}YUV_DRAW_PARAM;

/*----------------------------------------------*/
/*                  函数声明                    */
/*----------------------------------------------*/
extern uint32_t getTime_ms(void);
extern void osd_Init(void);
extern uint8_t * getCharFrontAddr(uint16_t charCode);

/*----------------------------------------------*/
/*                  全局变量                    */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                  函数定义                    */
/*----------------------------------------------*/


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __OSD_BASE_H__ */

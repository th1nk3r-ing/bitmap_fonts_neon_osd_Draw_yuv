/**
 * @file:   neon_osd.h
 * @note:   2017-2030, <git.oschia.net/think3r>
 * @brief:  neon_osd.c 的头文件
 * @author: think3r
 * @date:   2019/8/18
 * @note:
 * @note \n History:
   1.日    期: 2019/8/18
     作    者:  think3r
     修改历史: 创建文件
 */


#ifndef __NEON_OSD_H__
#define __NEON_OSD_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/*----------------------------------------------*/
/*                 包含头文件                   */
/*----------------------------------------------*/
#include <stdint.h>
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
extern void neon_CreatAsciiDot_u16(uint8_t * pFont, uint16_t * pDst, 
                                                uint32_t pitch, uint8_t scale, uint16_t color);
                                            
extern void neon_CreatChineseDot_u16(uint8_t * pFont, uint16_t * pDst, 
                                               uint32_t pitch, uint8_t scale, uint16_t color);
                                            
extern void neon_CreatAsciiDot_yuv(uint8_t * pFont, uint8_t * pDst, 
                                                uint32_t pitch, uint8_t scale);

extern void neon_CreatChineseDot_yuv(uint8_t * pFont, uint8_t * pDst, 
                                                uint32_t pitch, uint8_t scale);

extern void neon_DrawDot_YUV_Gray(YUV_DRAW_PARAM * pParam);

#ifndef OSD_ONLY_SUPPORT_GRAY

extern void neon_DrawDot_I420_YV12(YUV_DRAW_PARAM * pParam);

extern void neon_DrawDot_NV12_NV21(YUV_DRAW_PARAM * pParam);

extern void neon_DrawDot_YUYV_UYVY(YUV_DRAW_PARAM * pParam);

#endif

/*----------------------------------------------*/
/*                 全局变量                     */
/*----------------------------------------------*/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __NEON_OSD_H__ */

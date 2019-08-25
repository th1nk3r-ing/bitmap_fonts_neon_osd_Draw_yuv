/**
 * @file:   cpu_osd.h
 * @note:   2017-2030, <git.oschia.net/think3r>
 * @brief:  cpu_osd.c 的头文件
 * @author: think3r
 * @date:   2019/8/18
 * @note:
 * @note \n History:
   1.日    期: 2019/8/18
     作    者:  think3r
     修改历史: 创建文件
 */


#ifndef __CPU_OSD_NEW_H__
#define __CPU_OSD_NEW_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*----------------------------------------------*/
/*                 包含头文件                   */
/*----------------------------------------------*/
#include <stdint.h>

/*----------------------------------------------*/
/*                 宏类型定义                   */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                结构体定义                    */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 函数声明                     */
/*----------------------------------------------*/
extern void cpu_DrawAscii( uint8_t *   pFont,
                                 uint16_t *  pDst,
                                 uint32_t    pitch,
                                 uint8_t     scale,
                                 uint16_t    color );

extern void cpu_DrawChinese(      uint8_t *   pFont,
                                    uint16_t *  pDst,
                                    uint32_t    pitch,
                                    uint8_t     scale,
                                    uint16_t    color );

/*----------------------------------------------*/
/*                 全局变量                     */
/*----------------------------------------------*/


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __CPU_OSD_NEW_H__ */

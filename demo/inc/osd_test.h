/**
 * @file:   osd_test.h
 * @note:   2017-2030, <git.oschia.net/think3r>
 * @brief:  osd_test.c 的头文件
 * @author: think3r
 * @date:   2019/8/18
 * @note:
 * @note \n History:
   1.日    期: 2019/8/18
     作    者: think3r
     修改历史: 创建文件
 */

#ifndef __OSD_TEST_H__
#define __OSD_TEST_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*----------------------------------------------*/
/*                  头文件包含                  */
/*----------------------------------------------*/
#include <stdint.h>
#include "osd_base.h"

/*----------------------------------------------*/
/*                  宏类型定义                  */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 结构体定义                   */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                  函数声明                    */
/*----------------------------------------------*/
extern uint32_t calcBeyondCompare_Pitch(uint32_t charNum, uint8_t scale, uint8_t charWidth);
extern uint32_t osdCreatDot_demo(void * pMem, uint8_t scale, uint8_t charWidth, bool beDraw_YUV);
extern void speadTest_CreatDot_u16(void * pMem, uint32_t charNum, uint8_t scale, uint8_t charWidth);
extern void osd_Spead_Test(void * pMem, uint32_t memSize);

extern uint32_t speadTest_CreatYuvDot(void * pMem, uint32_t charNum, uint8_t scale, uint8_t charWidth);


/*----------------------------------------------*/
/*                  全局变量                    */
/*----------------------------------------------*/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __OSD_TEST_H__ */

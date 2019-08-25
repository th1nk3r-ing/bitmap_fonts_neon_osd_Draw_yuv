/**
 * @file:   neon_intrinsics_test.h
 * @note:   2017-2030, <git.oschia.net/think3r>
 * @brief:  neon_intrinsics_test.c 的头文件
 * @author: think3r
 * @date    2019/8/18
 * @note:
 * @note \n History:
   1.日    期: 2019/8/18
     作    者: think3r
     修改历史: 创建文件
 */

#ifndef __NEON_INTRINSICS_TEST_H__
#define __NEON_INTRINSICS_TEST_H__


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

/*----------------------------------------------*/
/*                 结构体定义                   */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                  函数声明                    */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                  全局变量                    */
/*----------------------------------------------*/


extern void converI420ToNv21(uint32_t cnt, uint32_t width, uint32_t height,
                                    uint8_t * pSrcUAddr, uint8_t * pSrcVAddr, uint8_t * pDstLumaAddr);
extern void move_Y_test(uint32_t cnt, uint32_t width, uint32_t height, uint8_t * pSrcLumaAddr, uint8_t * pDstLumaAddr);
extern void neonTestFunc(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __NEON_INTRINSICS_TEST_H__ */

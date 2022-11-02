/**
 * @file:   neon_intrinsics_test.c
 * @note:   2017-2030, <git.oschia.net/think3r>
 * @brief:  测试 intrinsics 函数
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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arm_neon.h>
#include "osd_base.h"
#include "neon_intrinsics_test.h"

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
uint16_t dotTableNeonTest_u16[256][8];    /* neon 测试用 */
uint8_t  dotTableNeonTest_u8[256];    /* neon 测试用 */
uint16_t neonTestBuf[256];

/*----------------------------------------------*/
/*                  函数定义                    */
/*----------------------------------------------*/

static inline void prt_Dividing_line( uint8_t lineWidth , char * debugInfo, bool bStart)
{
    uint32_t k = 0, len = 0;

    len = (lineWidth - strlen(debugInfo)) / 2;

    for(k = 0; k < len; k++)
    {
        printf("*");
    }

    if(bStart)
    {
        Cprintf_red("[%s]  start", debugInfo);
    }
    else
    {
        Cprintf_red("[%s]   End ", debugInfo);
    }

    for(k = 0; k < len; k++)
    {
        printf("*");
    }

    if(bStart)
    {
        printf("\n");
    }
    else
    {
        printf("\n\n");        
    }

    return;
}


/**
 * @function:   prt_Mem_u16
 * @brief:      打印内存 u16
 * @param[in]:  uint16_t * pMem
 * @param[in]:  uint32_t size
 * @param[in]:  char * debugInfo
 * @param[out]: None
 * @return:     static void
 */
static void prt_Mem_u16(uint16_t * pMem, uint32_t size, char * debugInfo)
{

    if((pMem == NULL) || (debugInfo == NULL) || (size == 0))
    {
        Cprintf_green("[%s %d] ERR!!!!! pMem:[%p], debugInfo:[%p], size:[%d]\n",
            __func__, __LINE__, pMem, debugInfo, size);
        return;
    }

    prt_Dividing_line(88, debugInfo, true);

    uint32_t i = 0;

    for(i = 0; i < size; i++)
    {
        if(i % 8 == 0)
        {
            Cprintf_green("idx:[%03d] ->   ", i);
        }

        printf("[0x%04x]  ", pMem[i]);

        if(((i+1) % 8 == 0) && (i != 0))
        {
            printf("\n");
        }
    }

    prt_Dividing_line(88, debugInfo, false);

    return;
}

/**
 * @function:   prt_Mem_u8
 * @brief:      打印内存 u8
 * @param[in]:  uint8_t * pMem
 * @param[in]:  uint32_t size
 * @param[in]:  char * debugInfo
 * @param[out]: None
 * @return:     static void
 */
static void prt_Mem_u8(uint8_t * pMem, uint32_t size, char * debugInfo)
{

    if((pMem == NULL) || (debugInfo == NULL) || (size == 0))
    {
        Cprintf_green("[%s %d] ERR!!!!! pMem:[%p], debugInfo:[%p], size:[%d]\n",
            __func__, __LINE__, pMem, debugInfo, size);
        return;
    }

    prt_Dividing_line(71, debugInfo, true);

    uint32_t i = 0;

    for(i = 0; i < size; i++)
    {
        if(i % 8 == 0)
        {
            Cprintf_green("idx:[%03d] ->   ", i);
        }

        printf("[0x%02x]  ", pMem[i]);

        if(((i+1) % 8 == 0) && (i != 0))
        {
            printf("\n");
        }
    }

    prt_Dividing_line(71, debugInfo, false);

    return;
}

/**
 * @function:   neon_Init_Value
 * @brief:      neon 初始化
 * @param[in]:  void
 * @param[out]: None
 * @return:     void
 */
void neon_Init_Value(void)
{

#if 1    //1
    memset(neonTestBuf, 0x00, sizeof(neonTestBuf));

    uint8x16_t tmp_8x16 = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

    vst1q_u8((uint8_t *)neonTestBuf, tmp_8x16);     /* 存储 */
    prt_Mem_u8((uint8_t *)neonTestBuf, 32, "neon_init =");
#endif   /* end of (#if 0  //1) */


#if 1    //2
    memset(neonTestBuf, 0x00, sizeof(neonTestBuf));

    uint16x8x2_t tmp_16x8x2;

    tmp_16x8x2.val[1] = vdupq_n_u16(0x10FF);      /* 初始化指令测试 */
    vst1q_u16(neonTestBuf + 16, tmp_16x8x2.val[1]);
    prt_Mem_u16(neonTestBuf, 48, "neon_init vdupq_n_u16");
#endif   /* end of (#if 0  //2) */


#if 1    //3
    memset(neonTestBuf,  0x00, sizeof(neonTestBuf));

    uint16x8x3_t argbx3;
    argbx3.val[0] = vdupq_n_u16(0x0101);      /* 初始化指令测试 */
    argbx3.val[1] = vdupq_n_u16(0x0202);      /* 初始化指令测试 */
//    argbx3.val[2] = vdupq_n_u16(0x0404);      /* 初始化指令测试 */
    argbx3.val[2] = vld1q_u16(dotTableNeonTest_u16[255]);

/* uint16x8x3_t.val[0].[3] 和 uint16x8x3_t.val[0].[3]  和  uint16x8x3_t.val[1].[3]   存放至    neonTestBuf 开头*/
//    vst3q_lane_u16(neonTestBuf, argbx3, 3);

    vst3q_u16(neonTestBuf, argbx3);

    prt_Mem_u16(neonTestBuf, 64, "neon_init vld1q_u16");
#endif   /* end of (#if 0  //3) */

}

void neon_Vcombine(void)
{
#if 1    //neon_Vcombine
    uint8_t uTest[8] = {0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01 };
    uint8_t vTest[8] = {0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02 };
    uint8_t outTest[32] = {0};

    uint8x8_t u_8x8, v_8x8;
    uint8x8x2_t tmp_8x8x2;
    uint8x16_t vu8x16;
    u_8x8 = vld1_u8(uTest);
    v_8x8 = vld1_u8(vTest);
    vu8x16 = vcombine_u8(u_8x8, v_8x8);

    vst1q_u8(outTest, vu8x16);  /* store 16个vu分量数值 */

    prt_Mem_u8(outTest, 32, "vcombine_u8");
#endif   /* end of (#if 0  //neon_Vcombine) */


#if 1    //vreinterpretq_u8_u16
    uint8x16_t y_8x16_2;    
    uint16x8_t uv_16x8 ;
    uint16_t testValue = 0x0102; 

    uv_16x8 = vdupq_n_u16(testValue);
    y_8x16_2 = vreinterpretq_u8_u16(uv_16x8); 
    
    vst1q_u8((uint8_t *)neonTestBuf, y_8x16_2);  /* store 16个vu分量数值 */

    prt_Mem_u8((uint8_t *)neonTestBuf, 32, "vreinterpretq_u8_u16");
#endif   /* end of (#if 0  //vreinterpretq_u8_u16) */


#if 1    //强制转换
    memset(neonTestBuf,  0x00, sizeof(neonTestBuf));
    tmp_8x8x2.val[0] = vld1_u8(uTest);
    tmp_8x8x2.val[1] = vld1_u8(vTest);    
    vst2_u8((uint8_t *)&vu8x16, tmp_8x8x2);
    vst1q_u8((uint8_t *)neonTestBuf, vu8x16);
     
    prt_Mem_u8((uint8_t *)neonTestBuf, 32, "vzip_u8");
#endif   /* end of (#if 0  //强制转换) */


    return;
}

/**
 * @function:   neon_Zip_unZip
 * @brief:      vzip
 * @param[in]:  void  
 * @param[out]: None
 * @return:     void
 */
void neon_Zip_unZip(void)
{
    uint16x8_t u_16x8, v_16x8;
    uint16x8x2_t tmp_16x8x2;

#if 1    //vzipq_u16
    memset(neonTestBuf,  0x00, sizeof(neonTestBuf));
    u_16x8 = vld1q_u16(dotTableNeonTest_u16[1]);
    v_16x8 = vld1q_u16(dotTableNeonTest_u16[2]);
    tmp_16x8x2 = vzipq_u16 (u_16x8, u_16x8);
    vst1q_u16(neonTestBuf, tmp_16x8x2.val[0]);
    vst1q_u16(neonTestBuf + 8, tmp_16x8x2.val[1]);

    prt_Mem_u16(neonTestBuf, 64, "vzipq_u16");    
#endif   /* end of (#if 0  //vzipq_u16) */


#if 0    //vuzpq_u16
    memset(neonTestBuf,  0x00, sizeof(neonTestBuf));
    uint16x8_t tmpDot_16x8_1, tmpDot_16x8_2;

    tmpDot_16x8_1 = vld1q_u16((uint16_t *)dotTableNeonTest_u16);
    tmpDot_16x8_2 = vld1q_u16((uint16_t *)(dotTableNeonTest_u16 + 1));
    tmp_16x8x2 = vuzpq_u16(tmpDot_16x8_1, tmpDot_16x8_2);

    vst2q_u16(neonTestBuf, tmp_16x8x2);

    prt_Mem_u16(neonTestBuf, 128, "vuzpq_u16");
#endif   /* end of (#if 0  //vuzpq_u16) */


    uint8x8_t u_8x8;
    uint8x8x2_t tmp_8x8x2;    

#if 1    //vzip_u8    
    memset(neonTestBuf,  0x00, sizeof(neonTestBuf));
    u_8x8 = vld1_u8(dotTableNeonTest_u8);
    tmp_8x8x2 = vzip_u8(u_8x8, u_8x8);
    vst1_u8((uint8_t *)neonTestBuf, tmp_8x8x2.val[0]);
    vst1_u8((uint8_t *)neonTestBuf + 8, tmp_8x8x2.val[1]);
 
    prt_Mem_u8((uint8_t *)neonTestBuf, 32, "vzip_u8");    
#endif   /* end of (#if 0  //vzip_u8) */


#if 1    //vzip-2    
    memset(neonTestBuf,  0x00, sizeof(neonTestBuf));
    u_8x8 = vld1_u8(dotTableNeonTest_u8);
    tmp_8x8x2 = vzip_u8(u_8x8, u_8x8);
    
    vst2_u8((uint8_t *)neonTestBuf, tmp_8x8x2);
    
    prt_Mem_u8((uint8_t *)neonTestBuf, 32, "vst2_u8");    
#endif   /* end of (#if 0  //vzip-2) */


    return;
}

void neon_Load_Value(void)
{
    uint16x8x2_t tmp_16x8x2;
    uint8x8_t tmp_8x8;
    uint8x8x2_t tmp_8x8x2;

#if 0    //vld1q_u16
    memset(neonTestBuf,  0x00, sizeof(neonTestBuf));

    tmp_16x8x2.val[0] = vld1q_u16(dotTableNeonTest_u16);
    tmp_16x8x2.val[1] = vld1q_u16(dotTableNeonTest_u16 + 1);

    vst2q_u16(neonTestBuf, tmp_16x8x2);

    prt_Mem_u16(neonTestBuf, 32, "vld1q_u16");
#endif   /* end of (#if 0  //vld1q_u16) */


#if 1    //vld2q_u16
    memset(neonTestBuf,  0x00, sizeof(neonTestBuf));
    
    tmp_16x8x2 = vld2q_u16(dotTableNeonTest_u16[0]);

    vst2q_u16(neonTestBuf, tmp_16x8x2);

    prt_Mem_u16(neonTestBuf, 32, "vld2q_u16");
#endif   /* end of (#if 0  //vld2q_u16) */

#if 0    //vld1q_u16
    memset(neonTestBuf,  0x00, sizeof(neonTestBuf));

    tmp_8x8 = vld1_u8(dotTableNeonTest_u8);

    vst1_u8((uint8_t *)neonTestBuf, tmp_8x8);
    prt_Mem_u8((uint8_t *)neonTestBuf, 32, "vld1_u8");
#endif   /* end of (#if 0  //vld1q_u16) */

#if 0    //vld1_dup_u8
    memset(neonTestBuf,  0x00, sizeof(neonTestBuf));
    
    tmp_8x8 = vld1_dup_u8(dotTableNeonTest_u8 + 1);

    vst1_u8((uint8_t *)neonTestBuf, tmp_8x8);
    prt_Mem_u8((uint8_t *)neonTestBuf, 32, "vld1_dup_u8");
#endif   /* end of (#if 0  //vld1_dup_u8) */

#if 1    //tmp_8x8x2
    memset(neonTestBuf,  0x00, sizeof(neonTestBuf));

    tmp_8x8x2 = vld2_u8(dotTableNeonTest_u8);

    vst1_u8((uint8_t *)neonTestBuf, tmp_8x8x2.val[0]);
    prt_Mem_u8((uint8_t *)neonTestBuf, 32, "tmp_8x8x2");    
#endif   /* end of (#if 0  //tmp_8x8x2) */

    return;
}

/**
 * @function:   neon_Bit_Calc
 * @brief:      位运算
 * @param[in]:  void  
 * @param[out]: None
 * @return:     void
 */
void neon_Bit_Calc(void)
{
    uint16x8_t u_16x8, v_16x8;
    uint16x8x2_t tmp_16x8x2;

#if 1    //1
    memset(neonTestBuf,  0x00, sizeof(neonTestBuf));
    u_16x8 = vaddq_u16(tmp_16x8x2.val[1], tmp_16x8x2.val[0]);   /* 按位加指令测试 */
    vst1q_u16(neonTestBuf + 16, u_16x8);

    prt_Mem_u16(neonTestBuf, sizeof(neonTestBuf) / 2, "vaddq_u16");
#endif   /* end of (#if 0  //1) */


#if 1    //2
    memset(neonTestBuf,  0x00, sizeof(neonTestBuf));
    u_16x8 = vandq_u16(tmp_16x8x2.val[1], tmp_16x8x2.val[0]);   /* 按位与指令测试 */
    vst1q_u16(neonTestBuf + 16, u_16x8);

    prt_Mem_u16(neonTestBuf, sizeof(neonTestBuf) / 2, "vandq_u16");    
#endif   /* end of (#if 0  //2) */

    return;
}

/**
 * @function:   neon_Conditional_Storage
 * @brief:      vst 条件存储测试
 * @param[in]:  void
 * @param[out]: None
 * @return:     void
 */
void neon_Conditional_Storage(void)
{
    memset(neonTestBuf, 0x00, sizeof(neonTestBuf));

    uint8_t i = 0;
    uint8_t * pDst = (uint8_t *) neonTestBuf;

    uint8x16_t store_mask = {0,0,0,0, 0,0,0xff,0xff,
                            0xff,0xff,0xff,0xff, 0xff,0xff,0xff,0xf0};
    uint8x16_t tmp_dest = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    uint8x16_t dst_8x16;

    for(i = 0; i < 16; i ++)
    {
        pDst[i] = 0xF0u + i;
    }

    dst_8x16 = vld1q_u8(pDst);          /* dst 加载 */

    /* dest = vbslq_u8(mask, src, dest); */
    dst_8x16 = vbslq_u8(store_mask, tmp_dest, dst_8x16);    /* 位选择 */

    vst1q_u8((uint8_t *)neonTestBuf, dst_8x16);     /* 存储 */

    prt_Mem_u8((uint8_t *)neonTestBuf, 16, "vbslq_u8");
}

void neon_Pointer(void)
{
    memset(neonTestBuf,  0x00, sizeof(neonTestBuf));

    uint8x16x2_t storeMask_8x16x2;
    uint8x16_t * pStoreMask_8x16_1;    
    uint8x16_t * pStoreMask_8x16_2;    

#if 1    //指针等效于 .val[0]
    storeMask_8x16x2.val[0] = vld1q_u8(dotTableNeonTest_u8);
//    storeMask_8x16x2.val[1] = storeMask_8x16x2.val[0];
    storeMask_8x16x2.val[1] = vdupq_n_u8(0xFF);

    vst2q_u8( (uint8_t *)neonTestBuf, storeMask_8x16x2);    
    prt_Mem_u8((uint8_t *)neonTestBuf, 32, "uint8x16x2_t");    

    
    memset(neonTestBuf,  0x00, sizeof(neonTestBuf));
    pStoreMask_8x16_1 = (uint8x16_t *)&storeMask_8x16x2 + 1;
    vst1q_u8( (uint8_t *)neonTestBuf, *pStoreMask_8x16_1);    
    prt_Mem_u8((uint8_t *)neonTestBuf, 32, "pointer!");
#endif   /* end of (#if 0  //指针等效于 .val[0]) */


#if 1    //1
    union {
        uint8x16x2_t  x1;
        uint8x16_t    x2[2];
    } temp;
    
    temp.x1.val[0] = vld1q_u8(dotTableNeonTest_u8);
    temp.x1.val[1] = vdupq_n_u8(0xFF);

    vst2q_u8( (uint8_t *)neonTestBuf, temp.x1);    
    prt_Mem_u8((uint8_t *)neonTestBuf, 32, "union"); 
    
    memset(neonTestBuf,  0x00, sizeof(neonTestBuf));
    vst1q_u8( (uint8_t *)neonTestBuf, temp.x2[0]);    
    prt_Mem_u8((uint8_t *)neonTestBuf, 32, "union-1");
#endif   /* end of (#if 0  //1) */


#if 0    
    uint8x8x2_t storeMask_8x8x2;
    uint8x16_t * pTestPointer_8x16;

    storeMask_8x8x2.val[0] = vdup_n_u8(0x01);
    storeMask_8x8x2.val[1] = vdup_n_u8(0xFF);

    pTestPointer_8x16 = (uint8x16_t *)(&storeMask_8x8x2);

    vst1q_u8( (uint8_t *)neonTestBuf, *pTestPointer_8x16);    
    prt_Mem_u8((uint8_t *)neonTestBuf, 32, "pTestPointer_8x16");    
#endif 

}

/**
 * @function:   neonTestFunc
 * @brief:      neon 测试函数
 * @param[in]:  void
 * @param[out]: None
 * @return:     void
 */
void neonTestFunc(void)
{
    uint16x8_t u_16x8, v_16x8;
    uint16x8x2_t tmp_16x8x2;

    uint32_t i = 0, j = 0;

    for(i = 0; i < 256; i ++)
    {
        for(j = 0; j < 8; j ++)
        {
            dotTableNeonTest_u16[i][j] = (uint16_t)(i * 256u + j);
        }

        dotTableNeonTest_u8[i] = (uint8_t)i;

//        printf("%03d  -> [0x%04x], [0x%04x], [0x%04x], [0x%04x], [0x%04x], [0x%04x], [0x%04x], [0x%04x]\n",
//            i, dotTableNeonTest_u16[i][0], dotTableNeonTest_u16[i][1], dotTableNeonTest_u16[i][2], dotTableNeonTest_u16[i][3],
//            dotTableNeonTest_u16[i][4], dotTableNeonTest_u16[i][5], dotTableNeonTest_u16[i][6], dotTableNeonTest_u16[i][7]);
    }

    memset(neonTestBuf, 0x00, sizeof(neonTestBuf));


//    neon_Init_Value();

    neon_Zip_unZip();

//    neon_Load_Value();
    
    neon_Vcombine();

//    neon_Conditional_Storage();

//    neon_Pointer();

    return;
}



/**
 * @function:   move_Y_test
 * @brief:      搬移测试函数
 * @param[in]:  uint32_t width
 * @param[in]:  uint32_t height
 * @param[in]:  uint8_t * pSrcLumaAddr
 * @param[in]:  uint8_t * pDstLumaAddr
 * @param[out]: None
 * @return:     void
 */
void move_Y_test(uint32_t cnt, uint32_t width, uint32_t height, uint8_t * pSrcLumaAddr, uint8_t * pDstLumaAddr)
{
    uint32_t i = 0, j = 0, cntIdx = 0;
    uint32_t startTime = 0, endTime = 0;
    uint8_t * pSrcLumaAddr_Local = NULL;
    uint8_t * pDstLumaAddr_Local = NULL;

    startTime = getTime_ms();


#if 1    // memcpy 测试
    for (cntIdx = 0; cntIdx < cnt; cntIdx ++)
    {
        pSrcLumaAddr_Local = pSrcLumaAddr;
        pDstLumaAddr_Local = pDstLumaAddr;

        memcpy(pDstLumaAddr_Local, pSrcLumaAddr_Local, width * height);
    }
#endif   /* end of (#if 0  //memcpy 测试) */


#if 0    //8x16
    uint8x16_t y_8x16;
    for (cntIdx = 0; cntIdx < cnt; cntIdx ++)
    {
        pSrcLumaAddr_Local = pSrcLumaAddr;
        pDstLumaAddr_Local = pDstLumaAddr;

        for(i = 0; i < height; i ++)
        {
            for(j = 0; j < width / 16; j++)
            {
                y_8x16 = vld1q_u8(pSrcLumaAddr_Local);      /* neon_Load_Value 16个y分量像素值 */
                vst1q_u8(pDstLumaAddr_Local, y_8x16);       /* store 16个y分量像素值 */
                pDstLumaAddr_Local += 16;
                pSrcLumaAddr_Local += 16;
            }
        }
    }
#endif   /* end of (#if 0  //8x16) */

#if 0    //8x16x2
    uint8x16x2_t y_8x16x2;
    for (cntIdx = 0; cntIdx < cnt; cntIdx ++)
    {
        pSrcLumaAddr_Local = pSrcLumaAddr;
        pDstLumaAddr_Local = pDstLumaAddr;

        for(i = 0; i < height; i ++)
        {
            for(j = 0; j < width / 32; j++)
            {
                y_8x16x2 = vld2q_u8(pSrcLumaAddr_Local);      /* neon_Load_Value 16个y分量像素值 */
                vst2q_u8(pDstLumaAddr_Local, y_8x16x2);       /* store 16个y分量像素值 */
                pDstLumaAddr_Local += 32;
                pSrcLumaAddr_Local += 32;
            }
        }
    }
#endif   /* end of (#if 0  //8x16x2) */

#if 0    //8x16x3
    uint8x16x3_t y_8x16x3;
    for (cntIdx = 0; cntIdx < cnt; cntIdx ++)
    {
        pSrcLumaAddr_Local = pSrcLumaAddr;
        pDstLumaAddr_Local = pDstLumaAddr;

        for(i = 0; i < height; i ++)
        {
            for(j = 0; j < width / 48; j++)
            {
                y_8x16x3 = vld3q_u8(pSrcLumaAddr_Local);      /* neon_Load_Value 16个y分量像素值 */
                vst3q_u8(pDstLumaAddr_Local, y_8x16x3);       /* store 16个y分量像素值 */
                pDstLumaAddr_Local += 48;
                pSrcLumaAddr_Local += 48;
            }
        }
    }
#endif   /* end of (#if 0  //8x16x3) */

#if 0    //8x16x4
    uint8x16x4_t y_8x16x4;
    for (cntIdx = 0; cntIdx < cnt; cntIdx ++)
    {
        pSrcLumaAddr_Local = pSrcLumaAddr;
        pDstLumaAddr_Local = pDstLumaAddr;

        for(i = 0; i < height; i ++)
        {
            for(j = 0; j < width / 64; j++)
            {
                y_8x16x4 = vld4q_u8(pSrcLumaAddr_Local);      /* neon_Load_Value 16个y分量像素值 */
                vst4q_u8(pDstLumaAddr_Local, y_8x16x4);       /* store 16个y分量像素值 */
                pDstLumaAddr_Local += 64;
                pSrcLumaAddr_Local += 64;
            }
        }
    }
#endif   /* end of (#if 0  //8x16x4   ) */

    endTime = getTime_ms();

    if(cnt > 1)
    {
        Cprintf_green("[%s %d]  cnt:[%d], using:[%d ms]\n",
            __func__, __LINE__, cnt, endTime - startTime);
    }

    return;
}

/**
 * @function:   converI420ToNv21
 * @brief:      交织 I420 的 UV 分量
 * @param[in]:  uint32_t cnt
 * @param[in]:  uint32_t width
 * @param[in]:  uint32_t height
 * @param[in]:  uint8_t * pSrcUAddr
 * @param[in]:  uint8_t * pSrcVAddr
 * @param[in]:  uint8_t * pDstLumaAddr
 * @param[out]: None
 * @return:     void
 */
void converI420ToNv21(uint32_t cnt, uint32_t width, uint32_t height,
                           uint8_t * pSrcUAddr, uint8_t * pSrcVAddr, uint8_t * pDstLumaAddr)
{
    uint32_t i = 0, j = 0, cntIdx = 0;
    uint32_t startTime = 0, endTime = 0;
    uint8_t * pSrcUAddr_Local = NULL;
    uint8_t * pSrcVAddr_Local = NULL;
    uint8_t * pDstLumaAddr_Local = NULL;

    startTime = getTime_ms();

#if 1    //CPU 搬移
    for (cntIdx = 0; cntIdx < cnt; cntIdx ++)
    {
        pSrcUAddr_Local = pSrcUAddr;
        pSrcVAddr_Local = pSrcVAddr;
        pDstLumaAddr_Local = pDstLumaAddr;

        for (j = 0; j < height / 2; j++)    /* uv 总数据量为 Y 的一半 */
        {
            for (i = 0; i < width / 2; i++)
            {
                pDstLumaAddr_Local[0] = pSrcVAddr_Local[0];
                pDstLumaAddr_Local[1] = pSrcUAddr_Local[0];

                pDstLumaAddr_Local += 2;
                pSrcVAddr_Local ++;
                pSrcUAddr_Local ++;
            }
        }
    }
#endif   /* end of (#if 0  //CPU 搬移) */

#if 0    //改进算法 1
    uint8x8x2_t tmp_8x8x2;

    for (cntIdx = 0; cntIdx < cnt; cntIdx ++)
    {
        pSrcUAddr_Local = pSrcUAddr;
        pSrcVAddr_Local = pSrcVAddr;
        pDstLumaAddr_Local = pDstLumaAddr;

        for (j = 0; j < height / 2; j++)    /* uv 总数据量为 Y 的一半 */
        {
            for (i = 0; i < width / 16; i++)
            {
                tmp_8x8x2.val[1] = vld1_u8(pSrcUAddr_Local);
                tmp_8x8x2.val[0] = vld1_u8(pSrcVAddr_Local);

                vst2_u8(pDstLumaAddr_Local, tmp_8x8x2);  /* store 16个vu分量数值 */
                pSrcUAddr_Local += 8;
                pSrcVAddr_Local += 8;
                pDstLumaAddr_Local += 16;
            }
        }
    }
#endif   /* end of (#if 0  //改进算法 1) */


#if 0    //my 改进算法 1            (最优解!)
    uint8x16x2_t tmp_8x16x2;
    for (cntIdx = 0; cntIdx < cnt; cntIdx ++)
    {
        pSrcUAddr_Local = pSrcUAddr;
        pSrcVAddr_Local = pSrcVAddr;
        pDstLumaAddr_Local = pDstLumaAddr;

        for (j = 0; j < height / 2; j++)    /* uv 总数据量为 Y 的一半 */
        {
            for (i = 0; i < width / 32; i++)
            {
                tmp_8x16x2.val[0] = vld1q_u8(pSrcVAddr_Local);
                tmp_8x16x2.val[1] = vld1q_u8(pSrcUAddr_Local);

                vst2q_u8(pDstLumaAddr_Local, tmp_8x16x2);
                pSrcUAddr_Local += 16;
                pSrcVAddr_Local += 16;
                pDstLumaAddr_Local += 32;
            }
        }
    }
#endif   /* end of (#if 0  //my 改进算法 1) */

#if 0    //my 改进算法 2
    uint8x16x2_t tmp_8x16x2_1, tmp_8x16x2_2;
    for (cntIdx = 0; cntIdx < cnt; cntIdx ++)
    {
        pSrcUAddr_Local = pSrcUAddr;
        pSrcVAddr_Local = pSrcVAddr;
        pDstLumaAddr_Local = pDstLumaAddr;

        for (j = 0; j < height / 2; j++)    /* uv 总数据量为 Y 的一半 */
        {
            for (i = 0; i < width / 64; i++)
            {
                tmp_8x16x2_1.val[0] = vld1q_u8(pSrcVAddr_Local);
                tmp_8x16x2_1.val[1] = vld1q_u8(pSrcUAddr_Local);
                tmp_8x16x2_2.val[0] = vld1q_u8(pSrcVAddr_Local + 16);
                tmp_8x16x2_2.val[1] = vld1q_u8(pSrcUAddr_Local + 16);

                vst2q_u8(pDstLumaAddr_Local, tmp_8x16x2_1);
                vst2q_u8(pDstLumaAddr_Local + 32, tmp_8x16x2_2);

                pSrcUAddr_Local += 32;
                pSrcVAddr_Local += 32;
                pDstLumaAddr_Local += 64;
            }
        }
    }
#endif   /* end of (#if 0  //my 改进算法 2) */

#if 0    //未通过测试
    uint8x16x4_t tmp_8x16x4;
    for (cntIdx = 0; cntIdx < cnt; cntIdx ++)
    {
        pSrcUAddr_Local = pSrcUAddr;
        pSrcVAddr_Local = pSrcVAddr;
        pDstLumaAddr_Local = pDstLumaAddr;

        for (j = 0; j < height / 2; j++)    /* uv 总数据量为 Y 的一半 */
        {
            for (i = 0; i < width / 64; i++)
            {
                tmp_8x16x4.val[0] = vld1q_u8(pSrcVAddr_Local);
                tmp_8x16x4.val[1] = vld1q_u8(pSrcUAddr_Local); /* neon_Load_Value 8  个 u 分量像素值 */

                tmp_8x16x4.val[2] = vld1q_u8(pSrcVAddr_Local + 16);
                tmp_8x16x4.val[3] = vld1q_u8(pSrcUAddr_Local + 16); /* neon_Load_Value 8  个 v 分量像素值 */

                vst4q_u8(pDstLumaAddr_Local, tmp_8x16x4);  /* store 16个vu分量数值 */
                pSrcUAddr_Local += 32;
                pSrcVAddr_Local += 32;
                pDstLumaAddr_Local += 64;
            }
        }
    }
#endif   /* end of (#if 0  //未通过测试) */

    endTime = getTime_ms();

    Cprintf_green("[%s %d]  cnt:[%d], using:[%d ms]\n",
        __func__, __LINE__, cnt, endTime - startTime);
}



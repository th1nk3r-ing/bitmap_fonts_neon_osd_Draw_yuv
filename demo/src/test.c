/**
 * @file:   test.c
 * @note:   @think3r
 * @brief:  neon osd 点阵绘制
 * @author: think3r
 * @date    2019/7/30
 * @note:
 * @note \n History:
   1.日    期: 2019/7/30
     作    者: think3r
     修改历史: 创建文件
 */

/*----------------------------------------------*/
/*                 包含头文件                   */
/*----------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "build_time.h"
#include "osd_base.h"
#include "neon_osd.h"
#include "osd_test.h"
#include "neon_intrinsics_test.h"

/*----------------------------------------------*/
/*                 宏类型定义                   */
/*----------------------------------------------*/
#define TEST_FRM_WIDTH      (1920u)
#define TEST_FRM_HEIGHT     (1080u)

#define SRC_I420_YUV_FILE_NAME "1920_1080_p420_8_I420.yuv"
#define DST_I420_YUV_FILE_NAME "1920_1080_p420_8_I420_out.yuv"

#define SRC_YV12_YUV_FILE_NAME "1920_1080_p420_10_YV12.yuv"
#define DST_YV12_YUV_FILE_NAME "1920_1080_p420_10_YV12_out.yuv"

#define SRC_NV12_YUV_FILE_NAME "1920_1080_s420_8_NV12.yuv"
#define DST_NV12_YUV_FILE_NAME "1920_1080_s420_8_NV12_out.yuv"

#define SRC_NV21_YUV_FILE_NAME "1920_1080_s420_8_NV21.yuv"
#define DST_NV21_YUV_FILE_NAME "1920_1080_s420_8_NV21_out.yuv"

#define SRC_YUYV_YUV_FILE_NAME "1920_1080_p422_8_YUYV.yuv"
#define DST_YUYV_YUV_FILE_NAME "1920_1080_p422_8_YUYV_out.yuv"

#define SRC_UYVY_YUV_FILE_NAME "1920_1080_p422_8_UYVY.yuv"
#define DST_UYVY_YUV_FILE_NAME "1920_1080_p422_8_UYVY_out.yuv"

/*----------------------------------------------*/
/*                结构体定义                    */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 函数声明                     */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 全局变量                     */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 函数定义                     */
/*----------------------------------------------*/



/**
 * @function:   readSrcYuvFile
 * @brief:      读取源文件
 * @param[in]:  void * pMem
 * @param[in]:  char * pFileName
 * @param[in]:  uint32_t fileSize
 * @param[out]: None
 * @return:     void
 */
void readSrcYuvFile(void * pMem, char * pFileName, uint32_t fileSize)
{
    FILE * fp =  fopen(pFileName, "rb");
    if(fp != NULL)
    {
        size_t readSize = fread(pMem, 1, fileSize, fp);

        if(readSize != fileSize)
        {
            Cprintf_red("[%s %d]  read [%s] error! readSize:[%d], fileSize:[%d]\n",
                __FUNCTION__, __LINE__, pFileName, (int)readSize, fileSize);
        }
        else
        {
            Cprintf_yellow("[%s %d]  read [%s] OK!\n", __FUNCTION__, __LINE__, pFileName);
        }

        fclose(fp);
    }
    else
    {
        Cprintf_red("[%s %d] ERROR! open file:[%s] failed!\n\n",
            __FUNCTION__,__LINE__, pFileName);
    }

    return;
}

/**
 * @function:   writeDstYuvFile
 * @brief:      写 YUV 文件
 * @param[in]:  void * pMem
 * @param[in]:  char * pFileName
 * @param[in]:  uint32_t fileSize
 * @param[out]: None
 * @return:     void
 */
void writeDstYuvFile(void * pMem, char * pFileName, uint32_t fileSize)
{
    FILE * fp =  fopen(pFileName, "wb");
    if(fp != NULL)
    {
        size_t readSize = fwrite(pMem, 1, fileSize, fp);

        if(readSize != fileSize)
        {
            Cprintf_red("[%s %d]  write [%s] error! readSize:[%d], fileSize:[%d]\n",
                __FUNCTION__, __LINE__, pFileName, (int)readSize, fileSize);
        }
        else
        {
            Cprintf_yellow("[%s %d]  write [%s] OK!\n", __FUNCTION__, __LINE__, pFileName);
        }

        fflush(fp);

        fclose(fp);
    }
    else
    {
        Cprintf_red("[%s %d] ERROR! open file:[%s] failed!\n\n",
            __FUNCTION__,__LINE__, pFileName);
    }

    return;    
}


/**
 * @function:   writeBinFile
 * @brief:      bin 文件 deug
 * @param[in]:  void * pMem
 * @param[in]:  size_t memSize
 * @param[out]: None
 * @return:     void
 */
void writeBinFile(void * pMem, size_t memSize)
{
    if((pMem == NULL) || (memSize == 0))
    {
        Cprintf_green("[%s %d]  error!!!!! pMem:[%p], memSize:[%lu]\n",
            __FUNCTION__, __LINE__, pMem, memSize);
        return;
    }

    FILE * fp = NULL;
    uint32_t i = 0;
    uint16_t * pData = NULL;
    char fileName[64];

    /* bin 数据写回 */
    do
    {
        sprintf(fileName, "osdOut.bin");
        fp = fopen(fileName, "wb+");

        fwrite(pMem, 1, memSize, fp);
        fflush(fp);
        Cprintf_green("[%s %d]  write bin file:[%s] Success!\n",
            __FUNCTION__, __LINE__, fileName);

        fclose(fp);
    }while(0);

    do
    {
        sprintf(fileName, "osdOut_copy.bin");
        fp = fopen(fileName, "wb+");

        fwrite(pMem, 1, memSize, fp);
        fflush(fp);
        Cprintf_green("[%s %d]  write bin file:[%s] Success!\n",
            __FUNCTION__, __LINE__, fileName);

        fclose(fp);
    }while(0);

    return;
}

void testYUV_I420_2_NV21(void * pSrc, size_t memSize, uint32_t width, uint32_t height)
{
    /* I420 -> NV12
      {[YYYYYYYY] [UU] [VV]} -> {[YYYYYYYY] [VUVU]} */

    readSrcYuvFile(pSrc, SRC_I420_YUV_FILE_NAME, memSize);

    move_Y_test(1000, width, height, pSrc, pSrc + memSize);

    converI420ToNv21(1000, width, height,
                     ((uint8_t *)pSrc) + width * height,
                     ((uint8_t *)pSrc) + width * height  + width * height / 4,
                     ((uint8_t *)pSrc + memSize) + width * height);

    writeDstYuvFile(pSrc + memSize, "I420-2-NV21_out.yuv", memSize);

    return;
}

/**
 * @function:   test_CreatOsdDot_u16_func
 * @brief:      osd u16 点阵功能测试
 * @param[in]:  void * pMem
 * @param[in]:  uint32_t memSize
 * @param[in]:  uint8_t test_Scale
 * @param[in]:  uint8_t charWidth
 * @param[out]: None
 * @return:     void
 */
void test_CreatOsdDot_u16_func(void * pMem, uint32_t memSize, uint8_t test_Scale, uint8_t charWidth)
{
    osdCreatDot_demo(pMem, test_Scale, charWidth, true);

    writeBinFile(pMem, memSize);

    return;
}


/**
 * @function:   test_CreatOsdDot_u16_Spead
 * @brief:      osd u16 点阵速度测试
 * @param[out]: None
 * @return:     void
 */
void test_CreatOsdDot_u16_Spead(void * pMem, uint32_t memSize)
{
    uint32_t test_CharCnt = 2560000;
    uint8_t test_Scale = 2;
    uint8_t charWidth;

    charWidth = ASCII_WIDTH;
//    charWidth = HZ_WIDTH;

    speadTest_CreatDot_u16(pMem, test_CharCnt, test_Scale, charWidth);

    writeBinFile(pMem, memSize);

    return;
}


/**
 * @function:   test_CreatOsdDot_YUV_func_spead
 * @brief:      osd u8_yuv 点阵功能和速度测试
 * @param[in]:  void * pMem
 * @param[in]:  uint32_t memSize
 * @param[out]: None
 * @return:     void
 */
void test_CreatOsdDot_YUV_func_spead(void * pMem, uint32_t memSize)
{
    uint8_t test_Scale = 2;             /* scale 仅支持 x2, x4 */
//    uint8_t charWidth = ASCII_WIDTH;
    uint8_t charWidth = HZ_WIDTH;
    uint32_t test_CharCnt = 2560000;

    if((test_Scale != 2) && (test_Scale != 4))
    {
        Cprintf_red("[%s %d]  err! no-support-scale:[%d]\n", __FUNCTION__, __LINE__, test_Scale);
        return;
    }

    speadTest_CreatYuvDot(pMem, test_CharCnt, test_Scale, charWidth);

    writeBinFile(pMem, memSize);
}


/**
 * @function:   test_DrawOsd_YUV_func
 * @brief:      yuv 上画 osd dot 测试(功能性)
 * @param[in]:  void * pMem
 * @param[in]:  YUV_FORMAT yuvFormat
 * @param[out]: None
 * @return:     void
 */
void test_DrawOsd_YUV_func(void * pMem, YUV_FORMAT yuvFormat)
{
    void * pSrc = NULL;
    void * pDot = NULL;

    uint32_t yuvFileSize = 0;
    uint8_t test_Scale = 2;
    uint32_t dotPitch = 0;
    uint8_t charWidth = 0;
    float R, G, B;
    YUV_DRAW_PARAM drawParam;

    memset(&drawParam, 0x00, sizeof(drawParam));

    charWidth = ASCII_WIDTH;
//    charWidth = HZ_WIDTH;

    R = 255.0;
    G = 255.0;
    B = 0.0;

    drawParam.osdStartX = 64 * 10;
    drawParam.osdStartY = 160;

    pDot = pMem + TEST_FRM_WIDTH * TEST_FRM_HEIGHT * 4;

    dotPitch = osdCreatDot_demo(pDot, test_Scale, charWidth, true);

    writeBinFile(pDot, TEST_FRM_WIDTH * TEST_FRM_HEIGHT);

    drawParam.pDot = pDot;
    drawParam.dotWidth = dotPitch;
    drawParam.dotPitch = drawParam.dotWidth;
    drawParam.dotHeight = test_Scale * FRONT_HEIGHT;
    drawParam.Y = GET_Y_BT709(R, G, B);
    drawParam.U = GET_U_BT709(R, G, B);
    drawParam.V = GET_V_BT601(R, G, B);

    if((yuvFormat == YUV_420_I420) || (yuvFormat == YUV_420_YV12))
    {
        yuvFileSize = TEST_FRM_WIDTH * TEST_FRM_HEIGHT * 3 / 2;

        if(yuvFormat == YUV_420_I420)
        {
            readSrcYuvFile(pMem, SRC_I420_YUV_FILE_NAME, yuvFileSize);
        }
        else
        {
            readSrcYuvFile(pMem, SRC_YV12_YUV_FILE_NAME, yuvFileSize);
        }

        drawParam.frmStride = TEST_FRM_WIDTH;
        drawParam.frmWidth  = drawParam.frmStride;
        drawParam.frmHeight = TEST_FRM_HEIGHT;
        drawParam.pFrmVirAddr[0] = pMem;
        drawParam.yuvFrmFomat = yuvFormat;

        if(yuvFormat == YUV_420_I420)
        {
            drawParam.pFrmVirAddr[1] = drawParam.pFrmVirAddr[0] + drawParam.frmStride * drawParam.frmHeight;
            drawParam.pFrmVirAddr[2] = drawParam.pFrmVirAddr[1] + drawParam.frmStride * drawParam.frmHeight / 4;
        }
        else
        {
            drawParam.pFrmVirAddr[2] = drawParam.pFrmVirAddr[0] + drawParam.frmStride * drawParam.frmHeight;
            drawParam.pFrmVirAddr[1] = drawParam.pFrmVirAddr[2] + drawParam.frmStride * drawParam.frmHeight / 4;
        }

        neon_DrawDot_I420_YV12(&drawParam);

        if(yuvFormat == YUV_420_I420)
        {
            writeDstYuvFile(pMem, DST_I420_YUV_FILE_NAME, yuvFileSize);
        }
        else
        {
            writeDstYuvFile(pMem, DST_YV12_YUV_FILE_NAME, yuvFileSize);
        }
    }
    else if((yuvFormat == YUV_420_NV12) || (yuvFormat == YUV_420_NV21))
    {
        yuvFileSize = TEST_FRM_WIDTH * TEST_FRM_HEIGHT * 3 / 2;

        if(yuvFormat == YUV_420_NV12)
        {
            readSrcYuvFile(pMem, SRC_NV12_YUV_FILE_NAME, yuvFileSize);
        }
        else
        {
            readSrcYuvFile(pMem, SRC_NV21_YUV_FILE_NAME, yuvFileSize);
        }

        drawParam.frmStride = TEST_FRM_WIDTH;
        drawParam.frmWidth  = drawParam.frmStride;
        drawParam.frmHeight = TEST_FRM_HEIGHT;
        drawParam.pFrmVirAddr[0] = pMem;
        drawParam.pFrmVirAddr[1] = drawParam.pFrmVirAddr[0] + drawParam.frmStride * drawParam.frmHeight;
        /* drawParam.pFrmVirAddr[2] = drawParam.pFrmVirAddr[1] + 1; */
        drawParam.yuvFrmFomat = yuvFormat;

        neon_DrawDot_NV12_NV21(&drawParam);

        if(yuvFormat == YUV_420_NV12)
        {
            writeDstYuvFile(pMem, DST_NV12_YUV_FILE_NAME, yuvFileSize);
        }
        else
        {
            writeDstYuvFile(pMem, DST_NV21_YUV_FILE_NAME, yuvFileSize);
        }
    }
    else if((yuvFormat == YUV_422_YUYV) || (yuvFormat == YUV_422_UYVY))
    {
        yuvFileSize = TEST_FRM_WIDTH * TEST_FRM_HEIGHT * 2;

        if(yuvFormat == YUV_422_YUYV)
        {
            readSrcYuvFile(pMem, SRC_YUYV_YUV_FILE_NAME, yuvFileSize);
        }
        else
        {
            readSrcYuvFile(pMem, SRC_UYVY_YUV_FILE_NAME, yuvFileSize);
        }

        drawParam.frmStride = TEST_FRM_WIDTH * 2;
        drawParam.frmWidth  = drawParam.frmStride / 2;
        drawParam.frmHeight = TEST_FRM_HEIGHT;
        drawParam.pFrmVirAddr[0] = pMem;                /* yuyv/uyvy 仅需填充起始地址 */
        /* drawParam.pFrmVirAddr[1] = drawParam.pFrmVirAddr[0] + 1;
        drawParam.pFrmVirAddr[2] = drawParam.pFrmVirAddr[0] + 3; */
        drawParam.yuvFrmFomat = yuvFormat;

        neon_DrawDot_YUYV_UYVY(&drawParam);

        if(yuvFormat == YUV_422_YUYV)
        {
            writeDstYuvFile(pMem, DST_YUYV_YUV_FILE_NAME, yuvFileSize);
        }
        else
        {
            writeDstYuvFile(pMem, DST_UYVY_YUV_FILE_NAME, yuvFileSize);
        }
    }

    return;
}

void test_DrawOsdDot_YUV_spead(void * pMem, YUV_FORMAT yuvFormat, uint32_t drawCnt)
{
    uint8_t test_Scale = 2;
    uint32_t test_CharCnt = 40;

    void * pDot = NULL;
    uint32_t startTime = 0, endTime = 0;
    uint32_t yuvFileSize = 0, i = 0;
    uint32_t dotPitch = 0;
    uint8_t charWidth = 0;
    uint32_t realCharCnt = 0;
    float R, G, B;
    YUV_DRAW_PARAM drawParam_1, drawParam_2, * pDrawParam = NULL;
    void (* pDrawDot2YUV_func)(YUV_DRAW_PARAM * pParam) = NULL;

    memset(&drawParam_1, 0x00, sizeof(drawParam_1));
    memset(&drawParam_2, 0x00, sizeof(drawParam_2));

    charWidth = HZ_WIDTH;
    pDrawParam = &drawParam_1;

    R = 128.0;
    G = 255.0;
    B = 30.0;

    drawParam_1.osdStartX = 0;
    drawParam_1.osdStartY = 0;

    pDot = pMem + TEST_FRM_WIDTH * TEST_FRM_HEIGHT * 6;

    dotPitch = speadTest_CreatYuvDot(pDot, test_CharCnt, test_Scale, charWidth);

    drawParam_1.pDot = pDot;
    drawParam_1.dotWidth = dotPitch;
    drawParam_1.dotPitch = drawParam_1.dotWidth;
    drawParam_1.dotHeight = test_Scale * FRONT_HEIGHT;
    drawParam_1.Y = GET_Y_BT709(R, G, B);
    drawParam_1.U = GET_U_BT709(R, G, B);
    drawParam_1.V = GET_V_BT601(R, G, B);

    if(yuvFormat == YUV_420_I420)
    {
        yuvFileSize = TEST_FRM_WIDTH * TEST_FRM_HEIGHT * 3 / 2;

        readSrcYuvFile(pMem, SRC_I420_YUV_FILE_NAME, yuvFileSize);
        memcpy(pMem + yuvFileSize, pMem, yuvFileSize);      /* 拷贝两份, 防止 cache 影响测试的性能数据 */

        drawParam_1.yuvFrmFomat = yuvFormat;
        drawParam_1.frmStride = TEST_FRM_WIDTH;
        drawParam_1.frmWidth  = drawParam_1.frmStride;
        drawParam_1.frmHeight = TEST_FRM_HEIGHT;
        drawParam_1.pFrmVirAddr[0] = pMem;
        drawParam_1.pFrmVirAddr[1] = drawParam_1.pFrmVirAddr[0] + drawParam_1.frmStride * drawParam_1.frmHeight;
        drawParam_1.pFrmVirAddr[2] = drawParam_1.pFrmVirAddr[1] + drawParam_1.frmStride * drawParam_1.frmHeight / 4;

        drawParam_2 = drawParam_1;
        drawParam_2.pFrmVirAddr[0] = pMem + yuvFileSize;
        drawParam_2.pFrmVirAddr[1] = drawParam_2.pFrmVirAddr[0] + drawParam_2.frmStride * drawParam_2.frmHeight;
        drawParam_2.pFrmVirAddr[2] = drawParam_2.pFrmVirAddr[1] + drawParam_2.frmStride * drawParam_2.frmHeight / 4;

        pDrawDot2YUV_func = neon_DrawDot_I420_YV12;
    }
    else if(yuvFormat == YUV_420_NV12)
    {
        yuvFileSize = TEST_FRM_WIDTH * TEST_FRM_HEIGHT * 3 / 2;

        readSrcYuvFile(pMem, SRC_NV12_YUV_FILE_NAME, yuvFileSize);
        memcpy(pMem + yuvFileSize, pMem, yuvFileSize);

        drawParam_1.yuvFrmFomat = yuvFormat;
        drawParam_1.frmStride = TEST_FRM_WIDTH;
        drawParam_1.frmWidth  = drawParam_1.frmStride;
        drawParam_1.frmHeight = TEST_FRM_HEIGHT;
        drawParam_1.pFrmVirAddr[0] = pMem;
        drawParam_1.pFrmVirAddr[1] = drawParam_1.pFrmVirAddr[0] + drawParam_1.frmStride * drawParam_1.frmHeight;

        drawParam_2 = drawParam_1;
        drawParam_2.pFrmVirAddr[0] = pMem + yuvFileSize;
        drawParam_2.pFrmVirAddr[1] = drawParam_2.pFrmVirAddr[0] + drawParam_2.frmStride * drawParam_2.frmHeight;

        pDrawDot2YUV_func = neon_DrawDot_NV12_NV21;
    }

/* ***************************************************************************************************************** */
    startTime = getTime_ms();

    for(i = 0; i < drawCnt; i++)
    {
        pDrawParam = (i % 2) ? (&drawParam_1) : (&drawParam_2);

        pDrawDot2YUV_func(pDrawParam);
    }

    endTime = getTime_ms();

    realCharCnt = drawParam_1.dotWidth / charWidth / test_Scale;
    Cprintf_red(" finish-Draw-I420! drawCnt:[%d], charCnt:[%d], scale:[%d x %d], using:[%d ms]\n",
                            drawCnt, realCharCnt, drawParam_1.dotWidth, drawParam_1.dotHeight,
                            endTime - startTime);
    Cprintf_red("\t ---> each-line:[%1.3f us], each-char:[%1.3f us]\n",
                    1000.0 * (endTime - startTime) / drawCnt,
                    1000.0 * (endTime - startTime) / drawCnt / (realCharCnt));

/* ***************************************************************************************************************** */

    if(yuvFormat == YUV_420_I420)
    {
        writeDstYuvFile(pMem +yuvFileSize, DST_I420_YUV_FILE_NAME, yuvFileSize);
    }
    else if( yuvFormat == YUV_420_NV12)
    {
        writeDstYuvFile(pMem +yuvFileSize, DST_NV12_YUV_FILE_NAME, yuvFileSize);
    }

    return;
}


/**
 * @function:   main
 * @brief:      主函数
 * @param[in]:  void
 * @param[out]: None
 * @return:     int
 */
int main(void)
{
    uint32_t width = TEST_FRM_WIDTH, height = TEST_FRM_HEIGHT;
    size_t memSize = width * height * 5;         /* 按照 yuv444 申请最大的内存*/
    void * pMem = NULL;

    mk_Build_Date(NULL);

    /* 内存申请 */
    pMem = malloc(memSize * 4);
    memset(pMem, 0x00, memSize * 4);

//    neonTestFunc();

    osd_Init();

  /****************************** u16 dot *******************************/

    /* creat-u16-dot-test(功能性) */
    // test_CreatOsdDot_u16_func(pMem, memSize / 2, 1, ASCII_WIDTH);
//    test_CreatOsdDot_u16_func(pMem, memSize / 2, 2, ASCII_WIDTH);
//    test_CreatOsdDot_u16_func(pMem, memSize / 2, 3, ASCII_WIDTH);
//    test_CreatOsdDot_u16_func(pMem, memSize / 2, 4, ASCII_WIDTH);
//    test_CreatOsdDot_u16_func(pMem, memSize / 2, 1, HZ_WIDTH);
//    test_CreatOsdDot_u16_func(pMem, memSize / 2, 2, HZ_WIDTH);
//    test_CreatOsdDot_u16_func(pMem, memSize / 2, 3, HZ_WIDTH);
//    test_CreatOsdDot_u16_func(pMem, memSize / 2, 4, HZ_WIDTH);

    /* creat-u16-dot-spead-test(效率测试) */
//    test_CreatOsdDot_u16_Spead(pMem, memSize / 2);

   /****************************** yuv dot *******************************/

//  test_CreatOsdDot_YUV_func_spead(pMem, memSize / 2);

  /****************************** yuv-draw *******************************/

    /* draw-yuv-test(功能性) */
    test_DrawOsd_YUV_func(pMem, YUV_420_I420);
//    test_DrawOsd_YUV_func(pMem, YUV_420_YV12);
//    test_DrawOsd_YUV_func(pMem, YUV_420_NV12);
//    test_DrawOsd_YUV_func(pMem, YUV_420_NV21);
//    test_DrawOsd_YUV_func(pMem, YUV_422_YUYV);
//    test_DrawOsd_YUV_func(pMem, YUV_422_UYVY);

    /* draw-yuv-speadTest(性能测试) */
//    test_DrawOsdDot_YUV_spead(pMem, YUV_420_I420, 10 * 10000);
//    test_DrawOsdDot_YUV_spead(pMem, YUV_420_NV12, 100000);

    return 0;
}




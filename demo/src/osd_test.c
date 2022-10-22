/**
 * @file:   osd_test.c
 * @note:   2017-2030, <git.oschia.net/think3r>
 * @brief:  osd ����
 * @author: think3r
 * @date:   2019/8/18
 * @note:
 * @note \n History:
   1.��    ��: 2019/8/18
     ��    ��: think3r
     �޸���ʷ: �����ļ�
 */

/*----------------------------------------------*/
/*                  ͷ�ļ�����                  */
/*----------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "osd_test.h"
#include "cpu_osd.h"
#include "neon_osd.h"

/*----------------------------------------------*/
/*                  �����Ͷ���                  */
/*----------------------------------------------*/
#define COLOR_ARGB1555      (0x05FFu)     /* ������ɫ */

#define NEW_CPU_TEST
// #define NEON_TEST
#define TEST_SCALE (1)

/*----------------------------------------------*/
/*                 �ṹ�嶨��                   */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                  ��������                    */
/*----------------------------------------------*/
extern uint32_t getTime_ms(void);

/*----------------------------------------------*/
/*                  ȫ�ֱ���                    */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                  ��������                    */
/*----------------------------------------------*/

/**
 * @function:   calcBeyondCompare_Pitch
 * @brief:      ���� BC ֧�ֵ���� pitch
 * @param[in]:  uint32_t charNum
 * @param[in]:  uint8_t scale
 * @param[out]: None
 * @return:     uint32_t
 */
uint32_t calcBeyondCompare_Pitch(uint32_t charNum, uint8_t scale, uint8_t charWidth)
{
    uint32_t pitch_uint8 = 0;   /* ��λ:�ֽ�, ���� beyondCompare       debug */

    pitch_uint8 = charNum * charWidth * scale * 2;

    do
    {
        if(pitch_uint8 > 1000)
        {
            charNum --;
            pitch_uint8 = charNum * charWidth * scale * 2;
        }
        else
        {
            break;
        }
    }while(pitch_uint8 > 1000);

    Cprintf_red("Beyond Compare Pitch:[%u]\n", pitch_uint8);

    return (pitch_uint8 / 2);
}

static uint32_t demo_neon_AsciiDot_u16(void * pMem, uint8_t scale)
{
    uint8_t * pCharAddr = NULL;
    char * pChar = NULL;
    uint32_t i = 0;

    char * testOsdString = "hello world ! @";
    uint32_t stringLen = (uint32_t)strlen(testOsdString);
    uint32_t pitch = ASCII_WIDTH * stringLen * scale;   /* �� pitch, ��λ:���� */
    uint16_t * charDstStartX = NULL;                    /* ��ǰ�ַ�����ʼ��ַ, ��λ:���� */

    Cprintf_red("[%s %d]  pitch:[%d]\n", __FUNCTION__, __LINE__, pitch * 2);

    for(i = 0; i < stringLen; i ++)
    {
        pChar = testOsdString + i;
        pCharAddr = getCharFrontAddr(*pChar);
        charDstStartX = (uint16_t *)pMem + i * ASCII_WIDTH * scale;

        printf("[%s %d]  [%c]\n", __FUNCTION__, __LINE__, *pChar);

        neon_CreatAsciiDot_u16(pCharAddr, charDstStartX, pitch, scale, COLOR_ARGB1555);
    }

    return pitch;
}


static uint32_t demo_neon_ChineseDot_u16(void * pMem, uint8_t scale)
{
    uint8_t * pCharAddr = NULL;
    char * pChar = NULL;
    uint32_t i = 0;

    uint16_t  HzValue = 0x0000u;
    char * testOsdStringHz = "ѧϰ��ѧ��ƨ��";
    uint32_t stringLenHz = (uint32_t)strlen(testOsdStringHz) / 2;
    uint32_t pitchHz = HZ_WIDTH * stringLenHz * scale;   /* �� pitchHz, ��λ:���� */
    uint16_t * charDstStartX_Hz = NULL;            /* ��ǰ�ַ�����ʼ��ַ, ��λ:���� */

    Cprintf_red("[%s %d]  pitchHz:[%d], stringLenHz:[%d]\n", __FUNCTION__, __LINE__, pitchHz * 2, stringLenHz);

    for(i = 0; i < stringLenHz; i ++)
    {
        pChar = testOsdStringHz + i * 2;
        HzValue = (uint16_t)((pChar[0] << 8u) | pChar[1]);
        pCharAddr = getCharFrontAddr(HzValue);
        charDstStartX_Hz = (uint16_t *)pMem + i * HZ_WIDTH * scale;

        printf("[%s %d]  [%s], [%#04x]\n", __FUNCTION__, __LINE__, (char *)pChar, HzValue);

        neon_CreatChineseDot_u16(pCharAddr, charDstStartX_Hz, pitchHz, scale, COLOR_ARGB1555);
    }

    return pitchHz;
}

static uint32_t demo_neon_AsciiDot_YUV(void * pMem, uint8_t scale)
{
    uint8_t * pCharAddr = NULL;
    char * pChar = NULL;
    uint32_t i = 0;

    char * testOsdString = "hello world % )";
    uint32_t stringLen = (uint32_t)strlen(testOsdString);
    uint32_t pitch = stringLen * ASCII_WIDTH * scale;   /* �� pitch, ��λ:���� */
    uint8_t * charDstStartX = NULL;                    /* ��ǰ�ַ�����ʼ��ַ, ��λ:���� */

    Cprintf_red("[%s %d]  pitch:[%d]\n", __FUNCTION__, __LINE__, pitch);

    for(i = 0; i < stringLen; i ++)
    {
        pChar = testOsdString + i;
        pCharAddr = getCharFrontAddr(*pChar);
        charDstStartX = (uint8_t *)pMem + i * ASCII_WIDTH * scale;

        printf("[%s %d]  [%c]\n", __FUNCTION__, __LINE__, *pChar);

        neon_CreatAsciiDot_yuv_u8(pCharAddr, charDstStartX, pitch, scale);
    }

    return pitch;
}


static uint32_t demo_neon_ChineseDot_YUV(void * pMem, uint8_t scale)
{
    uint8_t * pCharAddr = NULL;
    char * pChar = NULL;
    uint32_t i = 0;

    uint16_t  HzValue = 0x0000u;
    char * testOsdStringHz = "ѧϰѧϰ";
    uint32_t stringLenHz = (uint32_t)strlen(testOsdStringHz) / 2;
    uint32_t pitchHz = HZ_WIDTH * stringLenHz * scale;   /* �� pitchHz, ��λ:���� */
    uint8_t * charDstStartX_Hz = NULL;            /* ��ǰ�ַ�����ʼ��ַ, ��λ:���� */

    Cprintf_red("[%s %d]  pitchHz:[%d], stringLenHz:[%d]\n", __FUNCTION__, __LINE__, pitchHz, stringLenHz);

    for(i = 0; i < stringLenHz; i ++)
    {
        pChar = testOsdStringHz + i * 2;
        HzValue = (uint16_t)((pChar[0] << 8u) | pChar[1]);
        pCharAddr = getCharFrontAddr(HzValue);
        charDstStartX_Hz = (uint8_t *)pMem + i * HZ_WIDTH * scale;

        printf("[%s %d]  [%s], [%#04x], start:[%d]\n",
            __FUNCTION__, __LINE__, (char *)pChar, HzValue, i * HZ_WIDTH * scale);

        neon_CreatChineseDot_yuv_u8(pCharAddr, charDstStartX_Hz, pitchHz, scale);
    }

    return pitchHz;
}

/**
 * @function:   osdCreatDot_demo
 * @brief:      neon ���� osd ���Ʋ���(�����ַ��̶�)
 * @param[in]:  void * pMem
 * @param[in]:  uint8_t scale
 * @param[out]: None
 * @return:     uint32_t pitch
 */
uint32_t osdCreatDot_demo(void * pMem, uint8_t scale, uint8_t charWidth, bool beDraw_YUV)
{
    uint32_t pitch = 0;

    if(charWidth == ASCII_WIDTH)
    {
        if(!beDraw_YUV)
        {
            pitch = demo_neon_AsciiDot_u16(pMem, scale);        
        }
        else
        {
            pitch = demo_neon_AsciiDot_YUV(pMem, scale);        
        }
    }
    else
    {
        if(!beDraw_YUV)
        {
            pitch = demo_neon_ChineseDot_u16(pMem, scale);
        }
        else
        {
            pitch = demo_neon_ChineseDot_YUV(pMem, scale);
        }
    }

    return pitch;
}


/**
 * @function:   speadTest_CreatDot_u16
 * @brief:      ���� u16 ������ٶȲ��Ժ���
 * @param[in]:  void * pMem         �����ڴ��
 * @param[in]:  uint32_t charNum    ���Ե��ַ�����
 * @param[in]:  uint32_t scale      �Ŵ���, 1, 2, 3, 4
 * @param[out]: None
 * @return:     void
 */
void speadTest_CreatDot_u16(void * pMem, uint32_t charNum, uint8_t scale, uint8_t charWidth)
{
    uint32_t i = 0;
    uint32_t charIdx = 0;
    uint8_t * pCharAddr = NULL;
    uint32_t pitch = 0;                  /* ��λ:����, ����� */
    uint16_t * charDstStartX = NULL;     /* ��ǰ�ַ�����ʼ��ַ, ��λ:���� */
    uint32_t startTime = 0, endTime = 0;

    startTime = getTime_ms();

    pitch = calcBeyondCompare_Pitch(charNum, scale, charWidth);

#ifdef NEW_CPU_TEST
    Cprintf_green("[%s %d]  ************ NEW_CPU_TEST *************\n", __FUNCTION__, __LINE__);
#endif

#ifdef NEON_TEST
    Cprintf_green("[%s %d]  ************** NEON_TEST ***************\n", __FUNCTION__, __LINE__);
#endif

    for(i = 0, charIdx = 0; i < charNum; i ++, charIdx ++)
    {
        if(charWidth == ASCII_WIDTH)
        {
            /* ���ַ������ڿ���ʾ�ַ��� */
            charIdx = (charIdx < 32u) ? (charIdx + 32u) :
                      ((charIdx == 127u) ? 0 : charIdx);
            pCharAddr = getCharFrontAddr((uint16_t)charIdx);
            charDstStartX = (uint16_t *)pMem + ((i * ASCII_WIDTH * scale) % (pitch * 2));
        }
        else
        {
            charIdx = (charIdx < 0xB0A0) ? (charIdx + 0xB0A0) :
                        ((charIdx > 0xF7F0) ? 0xB0A0 : charIdx);
            pCharAddr = getCharFrontAddr((uint16_t)charIdx);
            charDstStartX = (uint16_t *)pMem + ((i * HZ_WIDTH * scale) % (pitch * 2));
        }

#ifdef NEW_CPU_TEST
        if(charWidth == ASCII_WIDTH)
        {
            cpu_DrawAscii(pCharAddr, charDstStartX, pitch, scale, COLOR_ARGB1555);
        }
        else
        {
            cpu_DrawChinese(pCharAddr, charDstStartX, pitch, scale, COLOR_ARGB1555);
        }
#endif

#ifdef NEON_TEST
        /* NEON ���� */
        if(charWidth == ASCII_WIDTH)
        {
            neon_CreatAsciiDot_u16(pCharAddr, charDstStartX, pitch, scale, COLOR_ARGB1555);
        }
        else
        {
            neon_CreatChineseDot_u16(pCharAddr, charDstStartX, pitch, scale, COLOR_ARGB1555);
        }
#endif   /* end of (#if 0  //cpu/neon) */
    }

    endTime = getTime_ms();

    Cprintf_red("[%s %d]  pitch:[%u]\n", __FUNCTION__, __LINE__, pitch * 2);

    Cprintf_yellow(" finish! charCnt:[%d], scale:[%d x %d], using:[%d ms], each:[%1.3f us]\n",
        charNum, scale * charWidth, scale * FRONT_HEIGHT,
        endTime - startTime, 1000.0 * (endTime - startTime) / charNum);

    return;
}


/**
 * @function:   speadTest_CreatYuvDot
 * @brief:      ���� YUV ������ٶȲ��Ժ���
 * @param[in]:  void * pMem        
 * @param[in]:  uint32_t charNum   
 * @param[in]:  uint8_t scale      
 * @param[in]:  uint8_t charWidth  
 * @param[out]: None
 * @return:     uint32_t
 */
uint32_t speadTest_CreatYuvDot(void * pMem, uint32_t charNum, uint8_t scale, uint8_t charWidth)
{
    uint32_t i = 0;
    uint32_t charIdx = 0;
    uint8_t * pCharAddr = NULL;
    uint32_t pitch = 0;                  /* ��λ:����, ����� */
    uint8_t * charDstStartX = NULL;     /* ��ǰ�ַ�����ʼ��ַ, ��λ:���� */
    uint32_t startTime = 0, endTime = 0;

    startTime = getTime_ms();

    pitch = (scale * charWidth * charNum < 1920) 
            ? (scale * charWidth * charNum) 
            : (scale * charWidth * charNum / 1920 * 1920);

    for(i = 0, charIdx = 0; i < charNum; i ++, charIdx ++)
    {
        if(charWidth == ASCII_WIDTH)
        {
            /* ���ַ������ڿ���ʾ�ַ��� */
            charIdx = (charIdx < 32u) ? (charIdx + 32u) :
                      ((charIdx == 127u) ? 0 : charIdx);
            pCharAddr = getCharFrontAddr((uint16_t)charIdx);
            charDstStartX = (uint8_t *)pMem + ((i * ASCII_WIDTH * scale) % pitch);
        }
        else
        {
            charIdx = (charIdx < 0xB0A0) ? (charIdx + 0xB0A0) :
                        ((charIdx > 0xF7F0) ? 0xB0A0 : charIdx);
            pCharAddr = getCharFrontAddr((uint16_t)charIdx);
            charDstStartX = (uint8_t *)pMem + ((i * HZ_WIDTH * scale) % pitch);
        }

        /* NEON ���� */
        if(charWidth == ASCII_WIDTH)
        {
            neon_CreatAsciiDot_yuv_u8(pCharAddr, charDstStartX, pitch, scale);
        }
        else
        {
            neon_CreatChineseDot_yuv_u8(pCharAddr, charDstStartX, pitch, scale);
        }
    }

    endTime = getTime_ms();

    Cprintf_red("[%s %d]  pitch:[%u]\n", __FUNCTION__, __LINE__, pitch);

    Cprintf_yellow(" finish! charCnt:[%d], scale:[%d x %d], using:[%d ms], each:[%1.3f us]\n",
        charNum, scale * charWidth, scale * FRONT_HEIGHT,
        endTime - startTime, 1000.0 * (endTime - startTime) / charNum);

    return pitch;
}


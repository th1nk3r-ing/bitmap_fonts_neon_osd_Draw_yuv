/**
 * @file:   cpu_osd.c
 * @note:   2017-2030, <git.oschia.net/think3r>
 * @brief:  cpu ���ɵ���
 * @author: think3r
 * @date    2019/8/18
 * @note:
 * @note \n History:
   1.��    ��: 2019/8/18
     ��    ��:  think3r
     �޸���ʷ: �����ļ�
 */

/*----------------------------------------------*/
/*                 ����ͷ�ļ�                   */
/*----------------------------------------------*/
#include <stdio.h>
#include "cpu_osd.h"
#include "osd_base.h"

/*----------------------------------------------*/
/*                 �����Ͷ���                   */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                �ṹ�嶨��                    */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 ��������                     */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 ȫ�ֱ���                     */
/*----------------------------------------------*/
extern uint16_t dotTableNormal_u16[256][8];      /* �������� */
extern uint16_t dotTableReverse_u16[256][8];     /* ���Ҿ��� */

/*----------------------------------------------*/
/*                 ��������                     */
/*----------------------------------------------*/

/**
 * @function:   cpu_DrawAscii
 * @brief:      cpu ���� Ascii ����
 * @param[in]:  uint8_t * pFont     �ַ��ֿ��ַ
 * @param[in]:  uint16_t * pDst     Ŀ�ĵ����ַ(���ֶ�������ʼ���ص�ַ)
 * @param[in]:  uint32_t pitch      �� pitch (��λ����, 2B)
 * @param[in]:  uint8_t scale       �ַ���ģ(8x16 �ı���)
 * @param[in]:  uint16_t color      ������ɫ(ARGB 2byte)
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
    uint64_t * pTemp64 = (uint64_t *)pDst;  /* �� 64bit �����а���*/
    uint16_t  *pTemp = (uint16_t *)pDst;

#if 1   /* չ������ѭ����, ���ܿ����� 1/3 */
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
        /* ��չʣ�����(�����Ѿ����ɵ��е�������) */
        for(k = 1; k < scale; k ++)
        {
            /* ÿ����Ҫ���Ƶ�������Ϊ (ASCII_WIDTH * 2 / 8) */
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

        /* �����ַ����� */
        for (k = 0; k < ASCII_WIDTH; k++)
        {
            /* ��չ�ַ����п�� */
            for(j = 0; j < scale; j++)
            {
                pTemp[(hIdx * pitch + k) * scale + j] = pDotTableValue[k] & color;
            }
        }

        /* ��չʣ�����(�����Ѿ����ɵ��е�������) */
        for(k = 1; k < scale; k ++)
        {
            /* ÿ����Ҫ���Ƶ�������Ϊ (ASCII_WIDTH * 2 / 8) */
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
 * @brief:      cpu ���� ���ֵ���
 * @param[in]:  uint8_t * pFont     �ַ��ֿ��ַ
 * @param[in]:  uint16_t * pDst     Ŀ�ĵ����ַ(���ֶ�������ʼ���ص�ַ)
 * @param[in]:  uint32_t pitch      �� pitch (��λ����, 2B)
 * @param[in]:  uint8_t scale       �ַ���ģ(8x16 �ı���)
 * @param[in]:  uint16_t color      ������ɫ(ARGB 2byte)
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
    uint16_t * pDotTableValue_2 = NULL;     /*���ֻ������Ϊ 16 ����, �ֿ���Ϊ 2Byte */
    uint16_t  *pTemp = (uint16_t *)pDst;
    uint64_t * pTemp64 = (uint64_t *)pDst;

#if 1   /* չ������ѭ����, ���ܿ����� 1/3 */
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


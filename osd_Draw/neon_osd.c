/**
 * @file:   neon_osd.c
 * @note:   2017-2030, <git.oschia.net/think3r>
 * @brief:  neon ���ٵ�������
 * @author: think3r
 * @date:   2019/8/18
 * @note:
 * @note \n History:
   1.��    ��: 2019/8/18
     ��    ��:  think3r
     �޸���ʷ: �����ļ�
 */

/* neon ���� 16 �� 128bit; ��: 16 ��(16Byre) */

/*----------------------------------------------*/
/*                 ����ͷ�ļ�                   */
/*----------------------------------------------*/
#include <stdio.h>
#include <arm_neon.h>
#include "neon_osd.h"
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
/* ������ұ� */
extern uint8_t dotTableNormal_u8[256][8];      /* �������� */
extern uint16_t dotTableNormal_u16[256][8];      /* �������� */
extern uint16_t dotTableReverse_u16[256][8];     /* ���Ҿ��� */


/*----------------------------------------------*/
/*                 ��������                     */
/*----------------------------------------------*/

/**
 * @function:   neon_CreatAsciiDot_u16
 * @brief:      neon ����, ���� Ascii ����(������ 16 bit)
 * @param[in]:  uint8_t * pFont     �ַ��ֿ��ַ
 * @param[in]:  uint16_t * pDst     Ŀ�ĵ����ַ(���ֶ�������ʼ���ص�ַ)
 * @param[in]:  uint32_t pitch      �� pitch (��λ����, 2B)
 * @param[in]:  uint8_t scale       �ַ���ģ(8x16 �ı���)
 * @param[in]:  uint16_t color      ������ɫ(ARGB 2byte)
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
    uint8_t idx = 0;                /* �ڵ��� tab �е� idx */
    uint16_t * pSroreDst = NULL;    /* ָ�����洢�ĵ�ַ */
    uint16x8_t lineDotSrc_16x8, tmpDot_16x8;
    uint16x8_t lineDotDst_16x8 = vdupq_n_u16(color);   /* ��ʼ�� GRB ���� */

    if(scale == 1)
    {
        for(i = 0; i < FRONT_HEIGHT; i ++)
        {
            idx = *(pFont + i);
            pSroreDst = pDst + pitch * i;

            lineDotSrc_16x8 = vld1q_u16(dotTableNormal_u16[idx]);           /* ���ٲ����ص��� */
            tmpDot_16x8 = vandq_u16(lineDotSrc_16x8, lineDotDst_16x8);  /* ���ɶ�Ӧ������ֵ */
            vst1q_u16(pSroreDst, tmpDot_16x8);                          /* �洢 */
        }
    }
    else if (scale == 2)
    {
        uint16x8x2_t tmp_16x8x2;            /* ��Ž�֯������� */
        uint16_t * pStoreDst2;              /* �����д洢��ַ */

        for(i = 0; i < FRONT_HEIGHT; i ++)
        {
            idx = *(pFont + i);
            pSroreDst = pDst + pitch * i * scale;
            pStoreDst2 = pSroreDst + pitch;

            lineDotSrc_16x8 = vld1q_u16(dotTableNormal_u16[idx]);           /* ���ٲ����ص��� */
            tmp_16x8x2.val[0] = vandq_u16(lineDotSrc_16x8, lineDotDst_16x8);  /* ���ɶ�Ӧ������ֵ */

            tmp_16x8x2.val[1] = tmp_16x8x2.val[0];
            vst2q_u16(pSroreDst, tmp_16x8x2);
            vst2q_u16(pStoreDst2, tmp_16x8x2);
        }

    }
    else if (scale == 3)
    {
        uint16x8x3_t out_16x8x3;                                            /* ��Ž�֯������� */
        uint16_t * pStoreDst2;
        uint16_t * pStoreDst3;

        for(i = 0; i < FRONT_HEIGHT; i++)
        {
            idx = *(pFont + i);
            pSroreDst = pDst + pitch * i * scale;
            pStoreDst2 = pSroreDst + pitch;
            pStoreDst3 = pSroreDst + pitch * 2;

            lineDotSrc_16x8 = vld1q_u16(dotTableNormal_u16[idx]);                   /* ���ٲ����ص��� */
            out_16x8x3.val[0] = vandq_u16(lineDotSrc_16x8, lineDotDst_16x8);    /* ���ɶ�Ӧ������ֵ */

            out_16x8x3.val[1] = out_16x8x3.val[0];                              /* ������ */
            out_16x8x3.val[2] = out_16x8x3.val[0];

            vst3q_u16(pSroreDst, out_16x8x3);                                   /* �д洢 */
            vst3q_u16(pStoreDst2, out_16x8x3);                                  /* ������ */
            vst3q_u16(pStoreDst3, out_16x8x3);
        }
    }
    else if(scale == 4)
    {
        uint16x8x4_t out_16x8x4;                                            /* ��Ž�֯������� */
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

            lineDotSrc_16x8 = vld1q_u16(dotTableNormal_u16[idx]);                   /* ���ٲ����ص��� */
            out_16x8x4.val[0] = vandq_u16(lineDotSrc_16x8, lineDotDst_16x8);    /* ���ɶ�Ӧ������ֵ */

            out_16x8x4.val[1] = out_16x8x4.val[0];                              /* ������ */
            out_16x8x4.val[2] = out_16x8x4.val[0];
            out_16x8x4.val[3] = out_16x8x4.val[0];

            vst4q_u16(pSroreDst, out_16x8x4);                                   /* �д洢 */
            vst4q_u16(pStoreDst2, out_16x8x4);                                  /* ������ */
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
 * @brief:      neon ����, ���ƺ��ֵ���
 * @param[in]:  uint8_t * pFont         �ַ��ֿ��ַ
 * @param[in]:  uint16_t * pDst         Ŀ�ĵ����ַ(���ֶ�������ʼ���ص�ַ)
 * @param[in]:  uint32_t pitch          �� pitch (��λ����, 2B)
 * @param[in]:  uint8_t scale           �ַ���ģ(16x16 �ı���)
 * @param[in]:  uint16_t color          ������ɫ(ARGB 2byte)
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
    uint8_t idx_1 = 0, idx_2 = 0;                /* �ڵ��� tab �е� idx_1 */
    uint16_t * pSroreDst = NULL;    /* ָ�����洢�ĵ�ַ */
    uint16x8_t tmpDot_16x8_1, tmpDot_16x8_2;
    uint16x8_t lineDotDst_16x8 = vdupq_n_u16(color);   /* ��ʼ�� GRB ���� */

    if(scale == 1)
    {
        uint16x8x2_t tmp_16x8x2_1, tmp_16x8x2_2;

        for(i = 0; i < FRONT_HEIGHT; i ++)
        {
            idx_1 = *(pFont + i * 2);                       /* ��ǰ 8bit idx */
            idx_2 = *(pFont + i * 2 + 1);                   /* �к� 8bit idx */

            pSroreDst = pDst + pitch * i;

            tmpDot_16x8_1 = vld1q_u16(dotTableNormal_u16[idx_1]);           /* ���ٲ����ص��� */
            tmp_16x8x2_1.val[0] = vandq_u16(tmpDot_16x8_1, lineDotDst_16x8);

            tmpDot_16x8_2 = vld1q_u16(dotTableNormal_u16[idx_2]);
            tmp_16x8x2_1.val[1] = vandq_u16(tmpDot_16x8_2, lineDotDst_16x8);

            tmp_16x8x2_2 = vuzpq_u16(tmp_16x8x2_1.val[0], tmp_16x8x2_1.val[1]);

            vst2q_u16(pSroreDst, tmp_16x8x2_2);                          /* �洢 */
        }
    }
    else if (scale == 2)
    {
        uint16_t * pStoreDst2;
        uint16x8x2_t tmp_16x8x2_1, tmp_16x8x2_2;

        for(i = 0; i < FRONT_HEIGHT; i ++)
        {
            idx_1 = *(pFont + i * 2);                       /* ��ǰ 8bit idx */
            idx_2 = *(pFont + i * 2 + 1);                   /* �к� 8bit idx */

            pSroreDst = pDst + pitch * i * scale;           /* ż���д洢��ַ */
            pStoreDst2 = pSroreDst + pitch;                 /* �����д洢��ַ */

            tmpDot_16x8_1 = vld1q_u16(dotTableNormal_u16[idx_1]);                       /* ���ٲ����ص��� */
            tmp_16x8x2_1.val[0] = vandq_u16(tmpDot_16x8_1, lineDotDst_16x8);     /* ���ɶ�Ӧ������ֵ */
            tmp_16x8x2_1.val[1] = tmp_16x8x2_1.val[0];                        /* ��ǰ 8bit ������չ */

            tmpDot_16x8_2 = vld1q_u16(dotTableNormal_u16[idx_2]);
            tmp_16x8x2_2.val[0] = vandq_u16(tmpDot_16x8_2, lineDotDst_16x8);
            tmp_16x8x2_2.val[1] = tmp_16x8x2_2.val[0];

            vst2q_u16(pSroreDst, tmp_16x8x2_1);                  /* �洢��ǰ 8bit ���� */
            vst2q_u16(pSroreDst + HZ_WIDTH, tmp_16x8x2_2);       /* �洢�к� 8bit ���� */

            vst2q_u16(pStoreDst2, tmp_16x8x2_1);
            vst2q_u16(pStoreDst2 + HZ_WIDTH, tmp_16x8x2_2);
        }

    }
    else if (scale == 3)
    {
        uint16_t * pStoreDst2;
        uint16_t * pStoreDst3;
        uint16x8x3_t out_16x8x3_1, out_16x8x3_2;    /* ��Ž�֯������� */

        for(i = 0; i < FRONT_HEIGHT; i++)
        {
            idx_1 = *(pFont + i * 2);
            idx_2 = *(pFont + i * 2 + 1);

            pSroreDst = pDst + pitch * i * scale;
            pStoreDst2 = pSroreDst + pitch;
            pStoreDst3 = pSroreDst + pitch * 2;

            tmpDot_16x8_1 = vld1q_u16(dotTableNormal_u16[idx_1]);                   /* ���ٲ����ص��� */
            out_16x8x3_1.val[0] = vandq_u16(tmpDot_16x8_1, lineDotDst_16x8);    /* ���ɶ�Ӧ������ֵ */
            out_16x8x3_1.val[1] = out_16x8x3_1.val[0];
            out_16x8x3_1.val[2] = out_16x8x3_1.val[0];

            tmpDot_16x8_2 = vld1q_u16(dotTableNormal_u16[idx_2]);                   /* ���ٲ����ص��� */
            out_16x8x3_2.val[0] = vandq_u16(tmpDot_16x8_2, lineDotDst_16x8);    /* ���ɶ�Ӧ������ֵ */
            out_16x8x3_2.val[1] = out_16x8x3_2.val[0];
            out_16x8x3_2.val[2] = out_16x8x3_2.val[0];

            vst3q_u16(pSroreDst, out_16x8x3_1);          /* �д洢 */
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

            tmpDot_16x8_1 = vld1q_u16(dotTableNormal_u16[idx_1]);                   /* ���ٲ����ص��� */
            out_16x8x4_1.val[0] = vandq_u16(tmpDot_16x8_1, lineDotDst_16x8);    /* ���ɶ�Ӧ������ֵ */
            out_16x8x4_1.val[1] = out_16x8x4_1.val[0];
            out_16x8x4_1.val[2] = out_16x8x4_1.val[0];
            out_16x8x4_1.val[3] = out_16x8x4_1.val[1];

            tmpDot_16x8_2 = vld1q_u16(dotTableNormal_u16[idx_2]);                   /* ���ٲ����ص��� */
            out_16x8x4_2.val[0] = vandq_u16(tmpDot_16x8_2, lineDotDst_16x8);    /* ���ɶ�Ӧ������ֵ */
            out_16x8x4_2.val[1] = out_16x8x4_2.val[0];
            out_16x8x4_2.val[2] = out_16x8x4_2.val[0];
            out_16x8x4_2.val[3] = out_16x8x4_2.val[1];


            vst4q_u16(pSroreDst, out_16x8x4_1);          /* �д洢 */
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
 * @brief:      ���� YUV ���� ascii ����ĵ�������(�ǻ��� YUV)
 * @param[in]:  uint8_t * pFont         �ַ��ֿ��ַ
 * @param[in]:  uint8_t * pDst          Ŀ�ĵ����ַ(���ֶ�������ʼ���ص�ַ)
 * @param[in]:  uint32_t  pitch         �� pitch (��λ����, 1B)
 * @param[in]:  uint8_t   scale         �ַ���ģ(8x16 �ı���, ֻ֧�� x2, x4)
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

    uint8_t i = 0, idx = 0;        /* �ڵ��� tab �е� idx */
    uint8_t * pSroreDst = NULL;    /* ָ�����洢�ĵ�ַ */

    if(scale == 2)
    {
        uint8x8x2_t tmp_8x8x2;            /* ��Ž�֯������� */
        uint8_t * pStoreDst2 = NULL;      /* �����д洢��ַ */

        for(i = 0; i < FRONT_HEIGHT; i ++)
        {
            idx = *(pFont + i);
            pSroreDst = pDst + pitch * i * 2;
            pStoreDst2 = pSroreDst + pitch;

            tmp_8x8x2.val[0] = vld1_u8 (dotTableNormal_u8[idx]);           /* ���ٲ����ص��� */
            tmp_8x8x2.val[1] = tmp_8x8x2.val[0];
            vst2_u8(pSroreDst, tmp_8x8x2);
            vst2_u8(pStoreDst2, tmp_8x8x2);
        }
    }
    else if (scale == 4)
    {
        uint8x8x4_t tmp_8x8x4;            /* ��Ž�֯������� */
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

            tmp_8x8x4.val[0] = vld1_u8 (dotTableNormal_u8[idx]);           /* ���ٲ����ص��� */
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
 * @brief:      ���� YUV ���� ���� ����ĵ�������, ֻ֧�� x2, x4; (�ǻ��� YUV)
 * @param[in]:  uint8_t * pFont         �ַ��ֿ��ַ
 * @param[in]:  uint8_t * pDst          Ŀ�ĵ����ַ(���ֶ�������ʼ���ص�ַ)
 * @param[in]:  uint32_t  pitch         �� pitch (��λ����, 1B)
 * @param[in]:  uint8_t   scale         �ַ���ģ(16x16 �ı���, ֻ֧�� x2, x4)
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

    uint8_t i = 0, idx_1 = 0, idx_2 = 0;         /* �ڵ��� tab �е� idx */
    uint8_t * pSroreDst = NULL;    /* ָ�����洢�ĵ�ַ */

    if(scale == 2)
    {
        uint8x8x2_t tmp_8x8x2_1, tmp_8x8x2_2;            /* ��Ž�֯������� */
        uint8_t * pStoreDst2 = NULL;                     /* �����д洢��ַ */

        for(i = 0; i < FRONT_HEIGHT; i ++)
        {
            idx_1 = *(pFont + i * 2);                       /* ��ǰ 8bit idx */
            idx_2 = *(pFont + i * 2 + 1);                   /* �к� 8bit idx */

            pSroreDst = pDst + pitch * i * 2;
            pStoreDst2 = pSroreDst + pitch;

            tmp_8x8x2_1.val[0] = vld1_u8 (dotTableNormal_u8[idx_1]);           /* ���ٲ����ص��� */
            tmp_8x8x2_1.val[1] = tmp_8x8x2_1.val[0];

            tmp_8x8x2_2.val[0] = vld1_u8 (dotTableNormal_u8[idx_2]);           /* ���ٲ����ص��� */
            tmp_8x8x2_2.val[1] = tmp_8x8x2_2.val[0];

            vst2_u8(pSroreDst, tmp_8x8x2_1);                  /* �洢��ǰ 8bit ���� */
            vst2_u8(pSroreDst + HZ_WIDTH, tmp_8x8x2_2);       /* �洢�к� 8bit ���� */

            vst2_u8(pStoreDst2, tmp_8x8x2_1);                  /* �洢��ǰ 8bit ���� */
            vst2_u8(pStoreDst2 + HZ_WIDTH, tmp_8x8x2_2);       /* �洢�к� 8bit ���� */
        }
    }
    else if(scale == 4)
    {
        /* each:[0.810 us] */
        uint8x8x4_t tmp_8x8x4_1, tmp_8x8x4_2;            /* ��Ž�֯������� */
        uint8_t * pStoreDst2;
        uint8_t * pStoreDst3;
        uint8_t * pStoreDst4;

        for(i = 0; i < FRONT_HEIGHT; i ++)
        {
            idx_1 = *(pFont + i * 2);                       /* ��ǰ 8bit idx */
            idx_2 = *(pFont + i * 2 + 1);                   /* �к� 8bit idx */

            pSroreDst = pDst + pitch * i * 4;
            pStoreDst2 = pSroreDst + pitch;
            pStoreDst3 = pSroreDst + pitch * 2;
            pStoreDst4 = pSroreDst + pitch * 3;

            tmp_8x8x4_1.val[0] = vld1_u8 (dotTableNormal_u8[idx_1]);           /* ���ٲ����ص��� */
            tmp_8x8x4_1.val[1] = tmp_8x8x4_1.val[0];
            tmp_8x8x4_1.val[2] = tmp_8x8x4_1.val[0];
            tmp_8x8x4_1.val[3] = tmp_8x8x4_1.val[1];

            tmp_8x8x4_2.val[0] = vld1_u8 (dotTableNormal_u8[idx_2]);           /* ���ٲ����ص��� */
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

#if 0    //ʹ�� dotTableNormal_u16, ��Ч�ʸ���
    if(scale == 2)
    {
        /* each:[0.380 us] */
        uint8x16_t tmp_8x16_1, tmp_8x16_2;            /* ��Ž�֯������� */
        uint8_t * pStoreDst2;                           /* �����д洢��ַ */

        for(i = 0; i < FRONT_HEIGHT; i ++)
        {
            idx_1 = *(pFont + i * 2);                       /* ��ǰ 8bit idx */
            idx_2 = *(pFont + i * 2 + 1);                   /* �к� 8bit idx */

            pSroreDst = pDst + pitch * i * 2;
            pStoreDst2 = pSroreDst + pitch;

            tmp_8x16_1 = vld1q_u8( (uint8_t *) (dotTableNormal_u16[idx_1]) );           /* ���ٲ����ص��� */
            tmp_8x16_2 = vld1q_u8( (uint8_t *) (dotTableNormal_u16[idx_2]) );           /* ���ٲ����ص��� */

            vst1q_u8(pSroreDst, tmp_8x16_1);                  /* �洢��ǰ 8bit ���� */
            vst1q_u8(pSroreDst + HZ_WIDTH, tmp_8x16_2);       /* �洢�к� 8bit ���� */

            vst1q_u8(pStoreDst2, tmp_8x16_1);                  /* �洢��ǰ 8bit ���� */
            vst1q_u8(pStoreDst2 + HZ_WIDTH, tmp_8x16_2);       /* �洢�к� 8bit ���� */
        }
    }
    else if (scale == 4)
    {
        /* each:[0.835 us] */
        uint8x16x2_t tmp_8x16x2_1, tmp_8x16x2_2;            /* ��Ž�֯������� */
        uint8_t * pStoreDst2;                           /* �����д洢��ַ */
        uint8_t * pStoreDst3;
        uint8_t * pStoreDst4;

        for(i = 0; i < FRONT_HEIGHT; i ++)
        {
            idx_1 = *(pFont + i * 2);                       /* ��ǰ 8bit idx */
            idx_2 = *(pFont + i * 2 + 1);                   /* �к� 8bit idx */

            pSroreDst = pDst + pitch * i * 4;
            pStoreDst2 = pSroreDst + pitch;
            pStoreDst3 = pSroreDst + pitch * 2;
            pStoreDst4 = pSroreDst + pitch * 3;

            tmp_8x16x2_1.val[0] = vld1q_u8( (uint8_t *) (dotTableNormal_u16[idx_1]) );           /* ���ٲ����ص��� */
            tmp_8x16x2_1.val[1] = tmp_8x16x2_1.val[0];
            tmp_8x16x2_2.val[0] = vld1q_u8( (uint8_t *) (dotTableNormal_u16[idx_2]) );           /* ���ٲ����ص��� */
            tmp_8x16x2_2.val[1] = tmp_8x16x2_2.val[0];

            vst2q_u8(pSroreDst, tmp_8x16x2_1);                  /* �洢��ǰ 8bit ���� */
            vst2q_u8(pSroreDst + HZ_WIDTH * 2, tmp_8x16x2_2);       /* �洢�к� 8bit ���� */

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
#endif   /* end of (#if 0  //ʹ�� dotTableNormal_u16, ��Ч�ʸ���) */

    return;
}


/**
 * @function:   _neon_DrawDot_Y
 * @brief:      ���� Y ����
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

    /* Y ��ֵ */
    for(lineIdx = 0; lineIdx < pParam->dotHeight; lineIdx++)
    {
        for(j = 0; j < x16_Cnt / 2; j ++)
        {
            dot_8x16_1 = vld1q_u8(m_pDot);
            dot_8x16_2 = vld1q_u8(m_pDot + 16);

            dst_8x16_1 = vld1q_u8(m_pDst);          /* dst ���� */
            dst_8x16_2 = vld1q_u8(m_pDst + 16);

            /* dest = vbslq_u8(mask, src, dest); */
            dst_8x16_1 = vbslq_u8(dot_8x16_1, y_8x16_1, dst_8x16_1);    /* λѡ�� */
            dst_8x16_2 = vbslq_u8(dot_8x16_2, y_8x16_2, dst_8x16_2);

            vst1q_u8(m_pDst, dst_8x16_1);           /* �洢 */
            vst1q_u8(m_pDst + 16, dst_8x16_2);

            m_pDst += 32;
            m_pDot += 32;
        }

        if (x16_Cnt % 2) {
            dot_8x16_1 = vld1q_u8(m_pDot);
            dst_8x16_1 = vld1q_u8(m_pDst);          /* dst ���� */
            dst_8x16_1 = vbslq_u8(dot_8x16_1, y_8x16_1, dst_8x16_1);    /* λѡ�� */
            vst1q_u8(m_pDst, dst_8x16_1);           /* �洢 */
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
 * @brief:      �� OSD �ϻ��ƻҶ�, Y ӦΪ planar (�������)
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
    x16_Cnt = pParam->dotWidth / 16; /* Y ����һ�θ�ֵ 16 ������ */

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
 * @brief:      ��  I420 �� YV12 �ϻ��� OSD
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
    x16_Cnt = pParam->dotWidth / 16; /* Y ����һ�θ�ֵ 16 ������ */

    _neon_DrawDot_Y(startIdx,
                    m_pDst, m_pDot,
                    x16_Cnt, lineIdx, j,
                    dst_8x16_1, y_8x16_1, dot_8x16_1,
                    dst_8x16_2, y_8x16_2, dot_8x16_2,
                    pParam);

    /* U ��ֵ */
    uint8x16x2_t dot_8x16x2;

    /* YUV420, U �� w,h ������һ�� */
    startIdx = pParam->osdStartY / 2 * pParam->frmStride / 2 + pParam->osdStartX / 2;

    y_8x16_1 = vdupq_n_u8(pParam->U);
    for(lineIdx = 0; lineIdx < pParam->dotHeight / 2; lineIdx++)    /* �߶�����һ�� */
    {
        m_pDst = pParam->pFrmVirAddr[1] + startIdx + lineIdx * pParam->frmStride / 2 ;
        m_pDot = pParam->pDot + lineIdx * 2u * pParam->dotPitch;

        for(j = 0; j < x16_Cnt / 2; j ++)
        {
            dot_8x16x2 = vld2q_u8(m_pDot);

            dst_8x16_1 = vld1q_u8(m_pDst);

            dst_8x16_1 = vbslq_u8(dot_8x16x2.val[0], y_8x16_1, dst_8x16_1);    /* λѡ�� */

            vst1q_u8(m_pDst, dst_8x16_1);     /* �洢 */

            m_pDst += 16;
            m_pDot += 32;
        }

        // TODO : �Ż��˴�����
        if (x16_Cnt % 2) {
            uint8x8x2_t dot_8x8x2 = vld2_u8(m_pDot);
            uint8x8_t dst_8x8_1 = vld1_u8(m_pDst);
            dst_8x8_1 = vbsl_u8(dot_8x8x2.val[0], vget_high_u8(y_8x16_1), dst_8x8_1);
            vst1_u8(m_pDst, dst_8x8_1);
            m_pDst += 8;
            m_pDot += 16;
        }
    }

    /* V ��ֵ */
    y_8x16_1 = vdupq_n_u8(pParam->V);

    for(lineIdx = 0; lineIdx < pParam->dotHeight / 2; lineIdx++)
    {
        m_pDst = pParam->pFrmVirAddr[2] + startIdx + lineIdx * pParam->frmStride / 2;
        m_pDot = pParam->pDot + lineIdx * 2u * pParam->dotPitch;

        for(j = 0; j < x16_Cnt / 2; j ++)
        {
            dot_8x16x2 = vld2q_u8(m_pDot);

            dst_8x16_1 = vld1q_u8(m_pDst);

            dst_8x16_1 = vbslq_u8(dot_8x16x2.val[0], y_8x16_1, dst_8x16_1);    /* λѡ�� */

            vst1q_u8(m_pDst, dst_8x16_1);     /* �洢 */

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
 * @brief:      ��  I420 �� YV12 �ϻ��� OSD
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
    x16_Cnt = (pParam->dotWidth / 16); /* Y ����һ�θ�ֵ 16 ������ */

    _neon_DrawDot_Y(startIdx,
                    m_pDst, m_pDot,
                    x16_Cnt, lineIdx, j,
                    dst_8x16_1, y_8x16_1, dot_8x16_1,
                    dst_8x16_2, y_8x16_2, dot_8x16_2,
                    pParam);


    /* uv ��ʼƫ�Ƶ�ַ */
    startIdx = pParam->osdStartY * pParam->frmStride / 2 + pParam->osdStartX;

    uint16x8_t uv_16x8;
    uint16_t uv_Value = (pParam->yuvFrmFomat == YUV_420_NV12)
                          ? ((uint16_t)pParam->V * 256 + pParam->U)
                          : ((uint16_t)pParam->U * 256 + pParam->V);

    uv_16x8 = vdupq_n_u16(uv_Value);
    y_8x16_1 = vreinterpretq_u8_u16(uv_16x8);
    y_8x16_2 = y_8x16_1;

    /* Y ��ֵ */
    for(lineIdx = 0; lineIdx < pParam->dotHeight / 2; lineIdx++)
    {
        m_pDst = pParam->pFrmVirAddr[1] + startIdx + lineIdx * pParam->frmStride;
        m_pDot = pParam->pDot + lineIdx * 2 * pParam->dotPitch;

        for(j = 0; j < x16_Cnt / 2; j ++)
        {
            dot_8x16_1 = vld1q_u8(m_pDot);
            dot_8x16_2 = vld1q_u8(m_pDot + 16);

            dst_8x16_1 = vld1q_u8(m_pDst);          /* dst ���� */
            dst_8x16_2 = vld1q_u8(m_pDst + 16);

            /* dest = vbslq_u8(mask, src, dest); */
            dst_8x16_1 = vbslq_u8(dot_8x16_1, y_8x16_1, dst_8x16_1);    /* λѡ�� */
            dst_8x16_2 = vbslq_u8(dot_8x16_2, y_8x16_2, dst_8x16_2);

            vst1q_u8(m_pDst, dst_8x16_1);           /* �洢 */
            vst1q_u8(m_pDst + 16, dst_8x16_2);

            m_pDst += 32;
            m_pDot += 32;
        }
        
       if (x16_Cnt % 2) {
           dot_8x16_1 = vld1q_u8(m_pDot);
           dst_8x16_1 = vld1q_u8(m_pDst);          /* dst ���� */           
           dst_8x16_1 = vbslq_u8(dot_8x16_1, y_8x16_1, dst_8x16_1);    /* λѡ�� */           
           vst1q_u8(m_pDst, dst_8x16_1);           /* �洢 */
           m_pDst += 16;
           m_pDot += 16;
       }
    }
}

/**
 * @function:   neon_DrawDot_YUYV_UYVY
 * @brief:      �� 422 ��   YUYV �� UYVY �ϻ��� OSD
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

    /* uv ��ʼƫ�Ƶ�ַ */
    startIdx = pParam->osdStartY * pParam->frmStride + pParam->osdStartX * 2;
    x16_Cnt = pParam->dotWidth / 16; /* Y ����һ�θ�ֵ 16 ������ */

    /* ���� src ��ɫ���� */
    uint32x4_t uv_32x4;
    uint32_t uv_Value = (pParam->yuvFrmFomat == YUV_422_YUYV)
                          ? ( (uint32_t)(pParam->V << 24u) + (uint32_t)(pParam->Y << 16u)
                                + (uint32_t)(pParam->U << 8u) + (uint32_t)pParam->Y)
                          : ( (uint32_t)(pParam->Y << 24u) + (uint32_t)(pParam->V << 16u)
                                + (uint32_t)(pParam->Y << 8u) + (uint32_t)pParam->U);

    uv_32x4 = vdupq_n_u32(uv_Value);
    y_8x16_1 = vreinterpretq_u8_u32(uv_32x4);
    y_8x16_2 = y_8x16_1;

    /* Y ��ֵ */
    for(lineIdx = 0; lineIdx < pParam->dotHeight; lineIdx++)
    {
        m_pDst = pParam->pFrmVirAddr[0] + startIdx + lineIdx * pParam->frmStride;
        m_pDot = pParam->pDot + lineIdx * pParam->dotPitch;

        for(j = 0; j < x16_Cnt; j ++)
        {
            /* ���� mask ��ɫ���� */
            tmpDot_8x8x2_1.val[0] = vld1_u8(m_pDot);
            tmpDot_8x8x2_1.val[1] = tmpDot_8x8x2_1.val[0];
            vst2_u8((uint8_t *)&dot_8x16_1, tmpDot_8x8x2_1);

            tmpDot_8x8x2_2.val[0] = vld1_u8(m_pDot + 8);
            tmpDot_8x8x2_2.val[1] = tmpDot_8x8x2_2.val[0];
            vst2_u8((uint8_t *)&dot_8x16_2, tmpDot_8x8x2_2);

            dst_8x16_1 = vld1q_u8(m_pDst);          /* dst ���� */
            dst_8x16_2 = vld1q_u8(m_pDst + 16);

            /* dest = vbslq_u8(mask, src, dest); */
            dst_8x16_1 = vbslq_u8(dot_8x16_1, y_8x16_1, dst_8x16_1);    /* λѡ�� */
            dst_8x16_2 = vbslq_u8(dot_8x16_2, y_8x16_2, dst_8x16_2);

            vst1q_u8(m_pDst, dst_8x16_1);           /* �洢 */
            vst1q_u8(m_pDst + 16, dst_8x16_2);

            m_pDst += 32;
            m_pDot += 16;
        }
    }
}


#endif


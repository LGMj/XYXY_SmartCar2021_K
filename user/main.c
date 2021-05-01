/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 ��ƽ    ̨�������������ܿƼ�STC16���İ�
 ����    д��chiusir
 ��E-mail  ��chiusir@163.com
 ������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
 �������¡�2020��10��28��
 �������Ϣ�ο����е�ַ��
 ����    վ��http://www.lqist.cn
 ���Ա����̡�http://longqiu.taobao.com
 ------------------------------------------------
 ��IDE��STC16
 ��Target �� keil5.2������
 ��SYS PLL�� 30MHzʹ���ڲ�����ͨ��
=================================================================
����������Ƶ��ַ��https://space.bilibili.com/95313236
����ʱ, ѡ��ʱ�� 30MHZ (�û��������޸�Ƶ��).
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"

void sysinit(void);
/******************** ������ **************************/
void main(void)
{
    sysinit();
    UART1_config(1); //����
    UART1_PutStr("uart1...\n\r");

    if (ICM20689_Init())
        UART1_PutStr("ICM20689 INIT FAIL!!!\n\r");
    else
        UART1_PutStr("ICM20689 INIT SUCCESS\n\r");

    while (1)
    {
        short data aacx, aacy, aacz;    //���ٶȴ�����ԭʼ����
        short data gyrox, gyroy, gyroz; //������ԭʼ����
        short data icm_raw_data[2] = {0};

        ICM_Get_Raw_data(&aacx, &aacy, &aacz, &gyrox, &gyroy, &gyroz); //�õ����ٶȴ���������
        icm_raw_data[0] = aacx;
        icm_raw_data[1] = gyroy;

        // printf("angle:%f, aacx:%d\n\r", atan2((float)aacz, (float)aacx) * 180.0 / PI, aacx); //�����м��ٶȼƵõ��Ƕ�ֵ

        vcan_sendware((char *)icm_raw_data, sizeof(icm_raw_data));

        delayms(10);
    }
}

void sysinit(void)
{
    WTST = 0;
    P_SW2 |= 0x80;
    IRCBAND = 1; //1��35MƵ�Σ�0��24MƵ��
                 //IRTRIM =0;
                 //LIRTRIM=0;
    CLKDIV = 0;  //24MHz��Ƶ����Ƶ����
    P_SW2 &= 0x7F;
    RSTCFG |= 1 << 4; //P5.4��λ�ܽ�
    P0M1 = 0x40;
    P0M0 = 0;    //����Ϊ׼˫���,P0.6  0.7Ϊ����ڣ���������
    P0PU = 0x40; //P0.6  0.7ʹ���ڲ�4.1K��������
    P1M1 = 0;
    P1M0 = 0; //����Ϊ׼˫���
    P3M1 = 0;
    P3M0 = 0; //����Ϊ׼˫���
    P4M1 = 0;
    P4M0 = 0; //����Ϊ׼˫���
    P5M1 = 0;
    P5M0 = 0; //����Ϊ׼˫���
    P6M1 = 0;
    P6M0 = 0; //����Ϊ׼˫���
    P7M1 = 0xFF;
    P7M0 = 0;    //����Ϊ׼˫���
    P7PU = 0xFF; //����Ϊ׼˫���,P7.0123Ϊ����ڽӰ���,P7.4567Ϊ����ڽӵ������
    P40 = 0;     //��LED��Դ
    EA = 1;
}

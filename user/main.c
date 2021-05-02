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

float data angle_c;    //�����ںϺ�ĽǶ�
#define ACC_RATIO 5.6  //���ٶȼƱ���
#define GYRO_RATIO 7.6 //�����Ǳ���
#define DT 0.01        //��������

void sysinit(void);

//----------------------------------------------------------------
//  @brief      һ�׻����˲�
//  @param      angle_m     ���ٶȼ�����
//  @param      gyro_m      ����������
//  @return     float       �����ںϺ�ĽǶ�
//----------------------------------------------------------------

float angle_calc(float angle_m, float gyro_m)
{
    float temp_angle;
    float gyro_now;
    float error_angle;
    static float last_angle;
    static uint8 first_angle;

    if (!first_angle) //�ж��Ƿ�Ϊ��һ�����б�����
    {
        //����ǵ�һ�����У����ϴνǶ�ֵ����Ϊ����ٶ�ֵһ��
        first_angle = 1;
        last_angle = angle_m;
    }

    gyro_now = gyro_m * GYRO_RATIO;
    //���ݲ������ļ��ٶ�ֵת��Ϊ�Ƕ�֮�����ϴεĽǶ�ֵ��ƫ��
    error_angle = (angle_m - last_angle) * ACC_RATIO;
    //����ƫ���������ǲ����õ��ĽǶ�ֵ���㵱ǰ�Ƕ�ֵ
    temp_angle = last_angle + (error_angle + gyro_now) * DT;
    //���浱ǰ�Ƕ�ֵ
    last_angle = temp_angle;

    return temp_angle;
}
/******************** ������ **************************/
void main(void)
{
    sysinit();
    UART1_config(1); //����
    UART1_PutStr("uart1...\n\r");
    UART4_config(2);

    if (ICM20689_Init())
        UART1_PutStr("ICM20689 INIT FAIL!!!\n\r");
    else
        UART1_PutStr("ICM20689 INIT SUCCESS\n\r");

    while (1)
    {
        uint16_t data aacx, aacy, aacz;    //���ٶȴ�����ԭʼ����
        uint16_t data gyrox, gyroy, gyroz; //������ԭʼ����
        uint16_t data icm_raw_data[3] = {0};

        ICM_Get_Raw_data(&aacx, &aacy, &aacz, &gyrox, &gyroy, &gyroz); //�õ����ٶȴ���������
        gyroy = -gyroy;
        angle_c = angle_calc(aacx, gyroy);
        icm_raw_data[0] = aacx;
        icm_raw_data[1] = gyroy;
        icm_raw_data[2] = (uint16_t)angle_c;

        // printf("angle:%f, aacx:%d, angle:%f\n\r", atan2((float)aacz, (float)aacx) * 180.0 / PI, aacx, angle_c); //�����м��ٶȼƵõ��Ƕ�ֵ

        vcan_sendware((uint8_t *)icm_raw_data, sizeof(icm_raw_data));

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

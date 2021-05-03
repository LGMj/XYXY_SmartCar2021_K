#include "Data_Process.h"

#define ACC_RATIO 2.4  //���ٶȼƱ���
#define GYRO_RATIO 7.2 //�����Ǳ���
#define DT 0.005      //��������

void refreshGyroscopeRawData(short *gyroscope_data)
{
    ICM_Get_Raw_data(&(gyroscope_data[aacx]),
                     &(gyroscope_data[aacy]),
                     &(gyroscope_data[aacz]),
                     &(gyroscope_data[gyrox]),
                     &(gyroscope_data[gyroy]),
                     &(gyroscope_data[gyroz]));
    gyroscope_data[gyroy] = 0 - gyroscope_data[gyroy];
}

//----------------------------------------------------------------
//  @brief      һ�׻����˲�
//  @param      angle_m     ���ٶȼ�����
//  @param      gyro_m      ����������
//  @return     float       �����ںϺ�ĽǶ�
//----------------------------------------------------------------
float firstOrderComplementaryFilter(float angle_m, float gyro_m)
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

//end

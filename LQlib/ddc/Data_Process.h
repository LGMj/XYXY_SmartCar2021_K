#ifndef __DATA_PROCESS_H__
#define __DATA_PROCESS_H__

#include "include.h"

/*����������ԭʼ����ö��*/
typedef enum
{
    aacx = 0,
    aacy,
    aacz,
    gyrox,
    gyroy,
    gyroz,
} GYROSCOP_DATA;

float firstOrderComplementaryFilter(float angle_m, float gyro_m);
void refreshGyroscopeRawData(short *gyroscope_data);

#endif

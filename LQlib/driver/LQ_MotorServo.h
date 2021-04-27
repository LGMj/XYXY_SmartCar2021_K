

#ifndef __LQ_MS_H_
#define __LQ_MS_H_

#include "include.h"

//���Ƶ��
#define MOTOR_FREQUENCY 12500
#define PWM1_CLOCK 1000000  //1M
#define PWM2_CLOCK 10000000 //10M

#define Servo_Delta 400                              //�������ת���Ĳ�ֵ�������ͺţ����˺Ͷ���۳��й�
#define Servo_Center 1620                            //���ֱ����ֵ��ǰ������ƫ��Ҫ��С������ƫ��Ҫ�Ӵ�
#define Servo_Left_Max (Servo_Center + Servo_Delta)  //�����ת����ֵ
#define Servo_Right_Min (Servo_Center - Servo_Delta) //�����ת����ֵ����ֵ��������÷�ʽ�йأ���ʽ

void MotorInit(u16 freq);
void MotorCtrl(int16_t motor1, int16_t motor2);
void TestMotor(void);
void ServoInit(u16 freq);
void ServoCtrl(uint16_t duty);
void TestServo(void);
#endif
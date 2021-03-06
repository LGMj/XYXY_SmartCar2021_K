/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 【平    台】北京龙邱智能科技STC16核心板
 【编    写】chiusir
 【E-mail  】chiusir@163.com
 【软件版本】V1.1 版权所有，单位使用请先联系授权
 【最后更新】2020年10月28日
 【相关信息参考下列地址】
 【网    站】http://www.lqist.cn
 【淘宝店铺】http://longqiu.taobao.com
 ------------------------------------------------
 【IDE】STC16
 【Target 】 keil5.2及以上
 【SYS PLL】 30MHz使用内部晶振，通过
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
下载时, 选择时钟 30MHZ (用户可自行修改频率).
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"

Car_Data_t data Car;

void sysinit(void);

void carDataInit(Car_Data_t *car)
{
    car->aacx_with_mix = 0;
    car->Servo.frequency = 100;
    car->Servo.left_value = 1760;
    car->Servo.mid_value = 1400;
    car->Servo.right_value = 1040;
    car->Servo.pid.err = 0;
    car->Servo.pid.kp = 0.13;
    car->Servo.pid.ki = 0;
    car->Servo.pid.kd = 0.06;
    car->Servo.pwm = 0;
}

/******************** 主函数 **************************/
void main(void)
{
    sysinit();
    carDataInit(&Car);
    UART1_config(1); //调试
    UART1_PutStr("uart1...\n\r");
    UART4_config(2);
    Timer0_init();
    ServoInit(Car.Servo.frequency);
    ServoCtrl(Car.Servo.mid_value); //1400中值 1760左值 1040右值
    MotorInit(MOTOR_FREQUENCY);
    MotorCtrl(0, 0);
    GPIO_KEY_Init();

    if (ICM20689_Init())
        UART1_PutStr("ICM20689 INIT FAIL!!!\n\r");
    else
        UART1_PutStr("ICM20689 INIT SUCCESS\n\r");

    while (1)
    {
        unsigned char k = 0xFF;
        static char run = 0;
        static char i = 0;

        i++;
        if (i >= 10)
        {
            i = 0;
            k = KEY_Read(KEY0) & KEY_Read(KEY1) & KEY_Read(KEY2) & KEY_Read(KEY3);
            if (k == 0)
            {
                run = ~run;
                printf("run:%d\n\r", run);
            }
            run == 0 ? MotorCtrl(0, 0) : MotorCtrl(-250, 0);
        }

        Car.uart_send_buffer[0] = Car.gyroscope_data[aacx];
        Car.uart_send_buffer[1] = Car.gyroscope_data[gyroy];
        Car.uart_send_buffer[2] = (int16_t)Car.aacx_with_mix;

        //printf("angle:%f, aacx:%d, angle:%f\n\r", atan2((float)Car.gyroscope_data[aacz], (float)Car.gyroscope_data[aacx]) * 180.0 / PI, \
                Car.gyroscope_data[aacx], Car.angle_x_with_mix); //反正切加速度计得到角度值

        vcan_sendware((int8_t *)(Car.uart_send_buffer), sizeof(Car.uart_send_buffer));

        delayms(10);
    }
}

void sysinit(void)
{
    WTST = 0;
    P_SW2 |= 0x80;
    IRCBAND = 0; //1：35M频段；0：24M频段
                 //IRTRIM =0;
                 //LIRTRIM=0;
    CLKDIV = 0;  //24MHz主频，分频设置
    P_SW2 &= 0x7F;
    RSTCFG |= 1 << 4; //P5.4复位管脚
    P0M1 = 0x40;
    P0M0 = 0;    //设置为准双向口,P0.6  0.7为输入口，编码器用
    P0PU = 0x40; //P0.6  0.7使能内部4.1K上拉电阻
    P1M1 = 0;
    P1M0 = 0; //设置为准双向口
    P3M1 = 0;
    P3M0 = 0; //设置为准双向口
    P4M1 = 0;
    P4M0 = 0; //设置为准双向口
    P5M1 = 0;
    P5M0 = 0; //设置为准双向口
    P6M1 = 0;
    P6M0 = 0; //设置为准双向口
    P7M1 = 0xFF;
    P7M0 = 0;    //设置为准双向口
    P7PU = 0xFF; //设置为准双向口,P7.0123为输入口接按键,P7.4567为输出口接电机驱动
    P40 = 0;     //打开LED电源
    EA = 1;
}

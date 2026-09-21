#ifndef _derives_servo_h 
#define _derives_servo_h
 
#include "main.h"
#include <stdint.h> 
/********************移植修改********************/
#define Servo_Tim_Number   htim2           //定时器序号	
#define Servo_Tim_Channel  TIM_CHANNEL_1   //定时器通道
#define Servo_Tim_Counter  200             //计数器数值
 
extern TIM_HandleTypeDef Servo_Tim_Number;
// 假设的状态枚举
 typedef enum {
    STATE_0_DEG,
    STATE_45_DEG,
    STATE_90_DEG,
    STATE_135_DEG,
    STATE_180_DEG,
    STATE_225_DEG,
    STATE_270_DEG,
    STATE_NUM // 状态总数，用于循环
} ServoState_t;
extern ServoState_t CurrentState;
void Servo_Init(const uint8_t Angle);
uint8_t Servo_Control(const uint8_t Angle);
void Servo_Test(void);
 void Servo_StateMachine(void);				// 状态机控制函数
#endif //_derives_servo_h
#ifndef motor_control_h
#define motor_control_h
#include "dma.h"

#include "usart.h"
#include <stdint.h>
#include <stdbool.h>

// 电机信息结构体
typedef struct Motor_Control {
    uint8_t addr;       // 电机地址
    uint8_t dir;        // 方向
    uint16_t vel;       // 速度(RPM)
    uint8_t acc;        // 加速度
    uint32_t clk;       // 脉冲数
    bool raF;           // 相位/绝对标志
    bool snF;           // 多机同步标志
} Motor_Control;

// 电机数量宏定义
#define MOTOR_COUNT 5

// 全局电机信息数组
extern Motor_Control motorControls[MOTOR_COUNT];
void Set_Motors_Velocity(uint16_t vel);
// 控制函数声明
void Emm_V5_Pos_Control_01(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, uint32_t clk, bool raF, bool snF);
void Emm_V5_Vel_Control_01(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, bool snF);
void Control_Motors_Position(void);
void Control_Motors_Velocity(void);

#endif  //motor_control_h


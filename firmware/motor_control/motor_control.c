#include "motor_control.h"
#include "dma.h"
#include "usart.h"
#include <stdint.h>
#include <stdbool.h>

extern	uint8_t cmd[16] = {0};

/**
  * @brief    位置模式
  * @param    addr：电机地址
  * @param    dir ：方向        ，0为CW，其余值为CCW
  * @param    vel ：速度(RPM)   ，范围0 - 5000RPM
  * @param    acc ：加速度      ，范围0 - 255，注意：0是直接启动
  * @param    clk ：脉冲数      ，范围0- (2^32 - 1)个
  * @param    raF ：相位/绝对标志，false为相对运动，true为绝对值运动
  * @param    snF ：多机同步标志 ，false为不启用，true为启用
  * @retval   地址 + 功能码 + 命令状态 + 校验字节
  */
void Emm_V5_Pos_Control_01(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, uint32_t clk, bool raF, bool snF)
{
  

  // 装载命令
  cmd[0]  =	addr;                      // 地址
  cmd[1]  = 0xFD;                      // 功能码
  cmd[2]  = dir;                       // 方向
  cmd[3]  = (uint8_t)(vel >> 8);       // 速度(RPM)高8位字节
  cmd[4]  = (uint8_t)(vel >> 0);       // 速度(RPM)低8位字节 
  cmd[5]  = acc;                       // 加速度，注意：0是直接启动
  cmd[6]  = (uint8_t)(clk >> 24);      // 脉冲数(bit24 - bit31)
  cmd[7]  = (uint8_t)(clk >> 16);      // 脉冲数(bit16 - bit23)
  cmd[8]  = (uint8_t)(clk >> 8);       // 脉冲数(bit8  - bit15)
  cmd[9]  = (uint8_t)(clk >> 0);       // 脉冲数(bit0  - bit7 )
  cmd[10] = raF;                       // 相位/绝对标志，false为相对运动，true为绝对值运动
  cmd[11] = snF;                       // 多机同步运动标志，false为不启用，true为启用
  cmd[12] = 0x6B;                  			// 校验字节
  
  // 发送命令
	HAL_UART_Transmit_DMA(&huart1,cmd,13);
}
/**
  * @brief    速度模式
  * @param    addr：电机地址
  * @param    dir ：方向       ，0为CW，其余值为CCW
  * @param    vel ：速度       ，范围0 - 5000RPM
  * @param    acc ：加速度     ，范围0 - 255，注意：0是直接启动
  * @param    snF ：多机同步标志，false为不启用，true为启用
  * @retval   地址 + 功能码 + 命令状态 + 校验字节
  */
void Emm_V5_Vel_Control_01(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, bool snF)
{
  

  // 装载命令
  cmd[0] =  addr;                       // 地址
  cmd[1] =  0xF6;                       // 功能码
  cmd[2] =  dir;                        // 方向
  cmd[3] =  (uint8_t)(vel >> 8);        // 速度(RPM)高8位字节
  cmd[4] =  (uint8_t)(vel >> 0);        // 速度(RPM)低8位字节
  cmd[5] =  acc;                        // 加速度，注意：0是直接启动
  cmd[6] =  snF;                        // 多机同步运动标志
  cmd[7] =  0x6B;                       // 校验字节
  
  // 发送命令
	HAL_UART_Transmit_DMA(&huart1,cmd,8);
}


// 全局电机信息数组
Motor_Control motorControls[MOTOR_COUNT] = 
{
    
    {0x01},
		{0x02},
		{0x03},
		{0x04},
		{0x05},
    
};

// 控制所有电机位置的函数
void Control_Motors_Position(void) 
	{
    for (int i = 0; i < MOTOR_COUNT; ++i) 
		{
       Emm_V5_Pos_Control_01
			(
        motorControls[i].addr,
        motorControls[i].dir,
        motorControls[i].vel,
        motorControls[i].acc,
        motorControls[i].clk,
        motorControls[i].raF,
        motorControls[i].snF
       );			
			HAL_Delay(10);
    }
}
//设置所有电机速度
void Set_Motors_Velocity(uint16_t vel)
{				
	//一号和	四号电机方向一致	
	Emm_V5_Vel_Control_01(3, 1, vel, 0, 0);
	HAL_Delay(10);
	Emm_V5_Vel_Control_01(1, 0, vel, 0, 0);
	HAL_Delay(10);
	Emm_V5_Vel_Control_01(4, 0, vel, 0, 0);
	HAL_Delay(10);
	Emm_V5_Vel_Control_01(2, 1, vel, 0, 0);
	HAL_Delay(10);                                            	              	
}
// 控制所有电机速度的函数
void Control_Motors_Velocity(void) 
	{
    for (int i = 0; i < MOTOR_COUNT-1; ++i) 
		{
       Emm_V5_Vel_Control_01
			(
        motorControls[i].addr,
        motorControls[i].dir,
        motorControls[i].vel,
        motorControls[i].acc,
        motorControls[i].snF
       );
			HAL_Delay(1000);
    }
}

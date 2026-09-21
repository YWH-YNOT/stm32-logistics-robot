#include "Emm_V5.h"
#include "fifo.h"
#include "dma.h"
#include "usart.h"
#include "main.h"
#include "instruct.h"
//触发回零函数
void Origin_Trigger_Return(void)
{
	// 定义回零状态标志位、回零完成后是否回零成功标志位
  uint8_t s_oflag = 0; bool s_osuc = false;

  // 定义接收数据数组、接收数据长度
  uint8_t rxCmd[128] = {0}; uint8_t rxCount = 0;
  
  // 发送命令触发多圈无限位碰撞回零（注意：0为单圈就近回零，1为单圈方向回零，2为多圈无限位碰撞回零，3为多圈限位开关回零）
  // 如果要触发单圈回零，则需要先设置单圈回零的零点位置，使用例程“设置单圈回零的零点位置”或小屏幕菜单O_Set进行设置
  // 如果要触发多圈限位回零，则需要先设置En引脚作为限位开关的输入（P_PUL设置为ESI_RCO），并按说明书接好限位开关
  Emm_V5_Origin_Trigger_Return(1, 2, 0);

  // 等待返回命令，命令数据缓存在数组rxCmd上，长度为rxCount
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1,rxFIFO.buffer,sizeof(rxFIFO.buffer));	
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT); // 禁用串口传输过半中断
  // 1ms轮询读取回零状态标志位 - 查询是否回零完成，是回零成功还是回零失败
  while(1)
  {
    // 读取回零状态标志位
    Emm_V5_Read_Sys_Params(1, S_CPOS);

    // 等待返回命令，命令数据缓存在数组rxCmd上，长度为rxCount
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1,rxFIFO.buffer,sizeof(rxFIFO.buffer));	
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT); // 禁用串口传输过半中断
    // 检查回零状态标志位，判断是否回零完成，是回零成功还是回零失败
    s_oflag = (rxFIFO.buffer[2] & 0x0C); // 取正在回零标志位0x04和回零失败标志位0x08
    if(s_oflag == 0x00) { s_osuc = true;	break; } // 正在回零标志位和回零失败标志位都为0，则回零成功，退出循环
    if(s_oflag  & 0x08) { s_osuc = false;	break; } // 回零失败标志位位为1，则回零失败，退出循环

    // 延时1ms轮询读取
    HAL_Delay(1);
  }

  // 验证回零完成后是否回零成功
  if(s_osuc) 
		{ 
			HAL_UART_Transmit_DMA(&huart1,"yydsyyds",8);
		} 
	else 
		{
			HAL_UART_Transmit_DMA(&huart1,"nono",4);		 
		}


}
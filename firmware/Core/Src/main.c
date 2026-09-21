/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Emm_V5.h"
#include "fifo.h"
#include "motor_control.h"
#include <string.h>
#include "derives_servo.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
	uint8_t receiveData[50];
	volatile int flag1	=	0;
	volatile int can_send = 1;
//	static	uint8_t cmd[16] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART6_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
//	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,receiveData,sizeof(receiveData));
//	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT); // 禁用串口传输过半中断
	HAL_UARTEx_ReceiveToIdle_DMA(&huart6,receiveData,sizeof(receiveData));
	__HAL_DMA_DISABLE_IT(&hdma_usart6_rx, DMA_IT_HT); // 禁用串口传输过半中断
	Servo_Init(0);
	HAL_Delay(1000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {	

		if(flag1==1)
		{		
			
			if(receiveData[0]==1)
			{
//				Set_Motors_Velocity(80);
//				 Emm_V5_Vel_Control_01(3, 0, 80, 0, 0);
				 CurrentState = STATE_0_DEG;
				 Servo_StateMachine();
					 HAL_Delay(1000);
			}
			else if(receiveData[0]==2)
			{
//				Set_Motors_Velocity(0);
//				 Emm_V5_Vel_Control_01(3, 0, 0, 0, 0);
				 CurrentState = STATE_45_DEG;
				 Servo_StateMachine();
				HAL_Delay(1000);
			}
			else if(receiveData[0]==3)
			{	
//				if(can_send==1)
//				{
//					Emm_V5_Pos_Control_01(3, 0, 50, 0, 3200, 0, 0);
//					can_send = 0;
//				}
//				HAL_Delay(1000);
//				if(can_send==1)
//				{
//					Emm_V5_Pos_Control_01(3, 0, 150, 0, 3200, 0, 0);
//					can_send = 0;
//				}
				  CurrentState = STATE_90_DEG;
				  Servo_StateMachine();
				HAL_Delay(1000);
			}
			else if(receiveData[0]==4)
			{
				  CurrentState = STATE_180_DEG;
				  Servo_StateMachine();
				HAL_Delay(1000);
			}
			
			int len = sizeof(receiveData) / 4;
			for(int i=0;i<len;i++)
			{
				fifo_enQueue(receiveData[i]);
				receiveData[i]=0;
			}
			
			flag1=0;
		}			
		

//		  HAL_UART_Transmit_DMA(&huart6,receiveData,sizeof(receiveData));				
//			HAL_UART_Transmit_DMA(&huart1,rxFIFO.buffer,sizeof(rxFIFO.buffer));	

////			free_rxFIFOqueue();
//			HAL_UARTEx_ReceiveToIdle_DMA(&huart1, receiveData, sizeof(receiveData)); // 接收完毕后重启    
//			__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT); 	
//////			while(!can_send);//等待上一组数据传输完成	
			
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart1)
    {
        // 当前传输完成，可以发送下一个缓冲区
        can_send = 1;
    }
}
 void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
 
	if(huart == &huart6)
	{		
		flag1 = 1;     
	}
		HAL_UART_Transmit_DMA(&huart6,receiveData,sizeof(receiveData));		
		HAL_UARTEx_ReceiveToIdle_DMA(&huart6,receiveData,sizeof(receiveData));		
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT); // 禁用串口传输过半中断
}
void HAL_UART_ErrorCallback(UART_HandleTypeDef * huart)
{
    if(huart==&huart6)
    {
			flag1=1;
			HAL_UARTEx_ReceiveToIdle_DMA(&huart6, receiveData, sizeof(receiveData)); // 接收完毕后重启
			__HAL_DMA_DISABLE_IT(&hdma_usart6_rx, DMA_IT_HT);
			memset(receiveData, 0, sizeof(receiveData));							   // 清除接收缓存        
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
		
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

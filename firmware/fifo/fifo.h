#ifndef __fifo_h
#define __fifo_h

#include "stm32f4xx_hal.h"
#define 	FIFO_SIZE   128

//队列的结构体
typedef struct 
	{
		uint16_t buffer[FIFO_SIZE];
		__IO uint8_t ptrWrite;
		__IO uint8_t ptrRead;
	} FIFO_t;

extern __IO FIFO_t rxFIFO;

void fifo_initQueue(void);					
void fifo_enQueue(uint16_t data); 	//入队
uint16_t fifo_deQueue(void);				//出队
uint8_t fifo_isEmpty(void);				//判断队列是否为空
uint16_t fifo_queueLength(void);		//计算队列长度
void free_rxFIFOqueue(void);              //清空队列
#endif


//queue:队列
//buffer_zone:缓冲区
//fifo:先进先出
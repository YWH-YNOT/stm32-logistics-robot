#include "fifo.h"


__IO FIFO_t rxFIFO = {0};

/**
	* @brief   初始化队列
	* @param   无
	* @retval  无
	*/
void initQueue(void)
{
	rxFIFO.ptrRead  = 0;
	rxFIFO.ptrWrite = 0;
}

/**
	* @brief   入队
	* @param   无
	* @retval  无
	*/
void fifo_enQueue(uint16_t data)
{
	rxFIFO.buffer[rxFIFO.ptrWrite] = data;
	
	++rxFIFO.ptrWrite;
	
	if(rxFIFO.ptrWrite >= FIFO_SIZE)
	{
		rxFIFO.ptrWrite = 0;
	}
}

/**
	* @brief   出队
	* @param   无
	* @retval  无
	*/
uint16_t fifo_deQueue(void)
{
	uint16_t element = 0;

	element = rxFIFO.buffer[rxFIFO.ptrRead];

	++rxFIFO.ptrRead;

	if(rxFIFO.ptrRead >= FIFO_SIZE)
	{
		rxFIFO.ptrRead = 0;
	}

	return element;
}

/**
	* @brief   判断空队列
	* @param   无
	* @retval  无
	*/
uint8_t fifo_isEmpty(void)
{
	if(rxFIFO.ptrRead == rxFIFO.ptrWrite)
	{
		return 1;
	}

	return 0;
}

/**
	* @brief   计算队列长度
	* @param   无
	* @retval  无
	*/
uint16_t fifo_queueLength(void)
{
	if(rxFIFO.ptrRead <= rxFIFO.ptrWrite)
	{
		return (rxFIFO.ptrWrite - rxFIFO.ptrRead);
	}
	else
	{
		return (FIFO_SIZE - rxFIFO.ptrRead + rxFIFO.ptrWrite);
	}
}
/**
	* @brief   清空队列
	* @param   无
	* @retval  无
	*/
void free_rxFIFOqueue(void)
{
	uint16_t lenth = sizeof(rxFIFO.buffer) / 4;
	for(int i=0;i<lenth;i++)
	{
		rxFIFO.buffer[i]=1;
	}	
}
	
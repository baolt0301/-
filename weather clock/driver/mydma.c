#include "stm32f10x.h"
#include "spi_lcd.h"

uint8_t dma_data;
uint8_t dma_tc=1;

void mydma_init(void)
{
    DMA_InitTypeDef DMA_InitStruct;
    DMA_InitStruct.DMA_BufferSize = 1;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&dma_data;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
    DMA_Init(DMA1_Channel3,&DMA_InitStruct);

    DMA_ITConfig(DMA1_Channel3,DMA_IT_TC,ENABLE);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority =0;
    NVIC_Init(&NVIC_InitStruct);

    DMA_Cmd(DMA1_Channel3,DISABLE);
}

void DMA1_Channel3_IRQHandler(void)
{
    while(DMA_GetITStatus(DMA1_IT_TC3)== RESET);
    DMA_ClearITPendingBit(DMA1_IT_TC3);
	dma_tc=1;
//	spi_cs(1);
}

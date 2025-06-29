#include "stm32f10x.h"
#include "st7735s.h"
#include "mydma.h"

void spi_cs(uint8_t BitVal)
{
    GPIO_WriteBit(CS_PORT,CS_PIN,(BitAction)BitVal);
}

void myspi_init(void)
{

    GPIO_InitTypeDef GPIO_InitStructer;
    GPIO_InitStructer.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructer.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructer.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructer);
    GPIO_SetBits(GPIOA,GPIO_Pin_5 | GPIO_Pin_7);
    GPIO_InitStructer.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructer.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOA,&GPIO_InitStructer);

    SPI_InitTypeDef SPI_InitStructer;
    SPI_InitStructer.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4 ;
    SPI_InitStructer.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructer.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructer.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructer.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructer.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructer.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructer.SPI_NSS = SPI_NSS_Soft;
    SPI_Init(SPI1,&SPI_InitStructer);

    SPI_Cmd(SPI1,ENABLE);
    GPIO_SetBits(CS_PORT,CS_PIN);
}
void spi_send(uint8_t data)
{
    while(!dma_tc);
	dma_tc = 0;
    // SPI_I2S_SendData(SPI1,data);
    dma_data = data;
    DMA_Cmd(DMA1_Channel3,DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel3,1);
    DMA_Cmd(DMA1_Channel3,ENABLE);
	SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
}


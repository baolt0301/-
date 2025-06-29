#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void myi2c_W_SCL(uint8_t BitVal)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)BitVal);
	Delay_us(10);
}
void myi2c_W_SDA(uint8_t BitVal)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)BitVal);
	Delay_us(10);
}
uint8_t myi2c_R_SDA(void)
{
	uint8_t bit;
	bit = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	Delay_us(10);
	return bit;
}

void myi2c_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructer;
	GPIO_InitStructer.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructer.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructer.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructer);
	GPIO_SetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11);
}

void myi2c_start(void)
{
	myi2c_W_SDA(1);
	myi2c_W_SCL(1);
	myi2c_W_SDA(0);
	myi2c_W_SCL(0);
}

void myi2c_stop(void)
{
	myi2c_W_SDA(0);
	myi2c_W_SCL(1);
	myi2c_W_SDA(1);
}

void myi2c_sendbyte(uint8_t data)
{
	for(uint8_t i=0;i<8;i++)
	{
		myi2c_W_SDA(data & (0x80>>i));
		myi2c_W_SCL(1);
		myi2c_W_SCL(0);
	}

}
uint8_t myi2c_receivebyte(void)
{
	uint8_t byte =0x00;
	myi2c_W_SDA(1);

	for(int i=0;i<8;i++)
	{
		myi2c_W_SCL(1);
		if(myi2c_R_SDA() == 1)
			byte |= (0x80 >> i);
		myi2c_W_SCL(0);
	}
	return byte;
}

void myi2c_sendack(uint8_t ackbit)
{

	myi2c_W_SDA(ackbit);
	myi2c_W_SCL(1);
	myi2c_W_SCL(0);
}

uint8_t myi2c_receiveack(void)
{
	uint8_t ackbit;
	myi2c_W_SDA(1);
	myi2c_W_SCL(1);
	ackbit = myi2c_R_SDA();
	myi2c_W_SCL(0);
	return ackbit;
}



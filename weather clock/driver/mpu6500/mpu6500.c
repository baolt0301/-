#include "stm32f10x.h"                  // Device header
#include "myi2c.h"
#include "mpu6500.h"

#define MPU6500_ADDRESS 0xD0

void mpu6500_W_reg(uint8_t regaddress,uint8_t data)
{
	myi2c_start();
	myi2c_sendbyte(MPU6500_ADDRESS);
	myi2c_receiveack();
	myi2c_sendbyte(regaddress);
	myi2c_receiveack();
	myi2c_sendbyte(data);
	myi2c_receiveack();
	myi2c_stop();
}

uint8_t mpu6500_R_reg(uint8_t regaddress)
{
	uint8_t data;
	myi2c_start();
	myi2c_sendbyte(MPU6500_ADDRESS);
	myi2c_receiveack();
	myi2c_sendbyte(regaddress);
	myi2c_receiveack();
	myi2c_start();
	myi2c_sendbyte(MPU6500_ADDRESS | 0x01);
	myi2c_receiveack();
	data = myi2c_receivebyte();
	myi2c_sendack(1);
	myi2c_stop();
	return data;
}


void mpu6500_init(void)
{
	myi2c_init();
	
	mpu6500_W_reg(MPU6500_PWR_MGMT_1,0x01);
	mpu6500_W_reg(MPU6500_PWR_MGMT_2,0x00);
	mpu6500_W_reg(MPU6500_SMPLRT_DIV,0x09);
	mpu6500_W_reg(MPU6500_CONFIG,0x06);
	mpu6500_W_reg(MPU6500_GYRO_CONFIG,0x18);
	mpu6500_W_reg(MPU6500_ACCEL_CONFIG,0x18);
}

uint8_t mpu6500_getid(void)
{
	return mpu6500_R_reg(MPU6500_WHO_AM_I);
}


//获取mpu6500数据
void mpu6500_getdata(int16_t *AccX,int16_t *AccY,int16_t *AccZ,
					int16_t *GyroX,int16_t *GyroY,int16_t *GyroZ,float *temp)
{
	uint8_t dataH,dataL;
	dataH = mpu6500_R_reg(MPU6500_ACCEL_XOUT_H);
	dataL = mpu6500_R_reg(MPU6500_ACCEL_XOUT_L);
	*AccX = (dataH << 8) | dataL; 
	
	dataH = mpu6500_R_reg(MPU6500_ACCEL_YOUT_H);
	dataL = mpu6500_R_reg(MPU6500_ACCEL_YOUT_L);
	*AccY = (dataH << 8) | dataL; 
	
	dataH = mpu6500_R_reg(MPU6500_ACCEL_ZOUT_H);
	dataL = mpu6500_R_reg(MPU6500_ACCEL_ZOUT_L);
	*AccZ = (dataH << 8) | dataL;
	
	dataH = mpu6500_R_reg(MPU6500_GYRO_XOUT_H);
	dataL = mpu6500_R_reg(MPU6500_GYRO_XOUT_L);
	*GyroX = (dataH << 8) | dataL; 
	dataH = mpu6500_R_reg(MPU6500_GYRO_YOUT_H);
	dataL = mpu6500_R_reg(MPU6500_GYRO_YOUT_L);
	*GyroY = (dataH << 8) | dataL; 
	dataH = mpu6500_R_reg(MPU6500_GYRO_ZOUT_H);
	dataL = mpu6500_R_reg(MPU6500_GYRO_ZOUT_L);
	*GyroZ = (dataH << 8) | dataL; 

	dataH = mpu6500_R_reg(MPU6500_TEMP_OUT_H);
	dataL = mpu6500_R_reg(MPU6500_TEMP_OUT_L);
	*temp = (int16_t)(((dataH << 8) | dataL)-2000)/333.87f + 21.0f; 
}


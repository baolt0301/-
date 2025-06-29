#ifndef _MPU6500_H
#define _MPU6500_H

#define MPU6500_SMPLRT_DIV  0x19
#define MPU6500_CONFIG  0x1A
#define MPU6500_GYRO_CONFIG  0x1B
#define MPU6500_ACCEL_CONFIG  0x1C

#define MPU6500_ACCEL_XOUT_H  0x3B
#define MPU6500_ACCEL_XOUT_L  0x3C
#define MPU6500_ACCEL_YOUT_H  0x3D
#define MPU6500_ACCEL_YOUT_L  0x3E
#define MPU6500_ACCEL_ZOUT_H  0x3F
#define MPU6500_ACCEL_ZOUT_L  0x40
#define MPU6500_TEMP_OUT_H  0x41
#define MPU6500_TEMP_OUT_L  0x42
#define MPU6500_GYRO_XOUT_H  0x43
#define MPU6500_GYRO_XOUT_L  0x44
#define MPU6500_GYRO_YOUT_H  0x45
#define MPU6500_GYRO_YOUT_L  0x46
#define MPU6500_GYRO_ZOUT_H  0x47
#define MPU6500_GYRO_ZOUT_L  0x48

#define MPU6500_PWR_MGMT_1  0x6B
#define MPU6500_PWR_MGMT_2  0x6C
#define MPU6500_WHO_AM_I 0x75

void mpu6500_W_reg(uint8_t regaddress,uint8_t data);
uint8_t mpu6500_R_reg(uint8_t regaddress);
void mpu6500_init(void);
void mpu6500_getdata(int16_t *AccX,int16_t *AccY,int16_t *AccZ,
					int16_t *GyroX,int16_t *GyroY,int16_t *GyroZ,float *temp);
uint8_t mpu6500_getid(void);


#endif

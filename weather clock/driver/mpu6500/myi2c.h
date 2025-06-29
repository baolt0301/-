#ifndef _MYI2c_H
#define _MYI2c_H

void myi2c_init(void);
void myi2c_start(void);
void myi2c_stop(void);
void myi2c_sendbyte(uint8_t byte);
uint8_t myi2c_receivebyte(void);
void myi2c_sendack(uint8_t ackbit);
uint8_t myi2c_receiveack(void);

#endif

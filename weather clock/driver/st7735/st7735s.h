#ifndef _ST7735S_H
#define _ST7735S_H

#define CS_PORT GPIOA
#define CS_PIN GPIO_Pin_4

#define RES_PORT GPIOB
#define RES_PIN GPIO_Pin_0

#define DC_PORT GPIOB
#define DC_PIN GPIO_Pin_1

#define BLK_PORT GPIOA
#define BLK_PIN GPIO_Pin_8

#define RED  		0xf800
#define GREEN		0x07e0
#define BLUE 		0x001f
#define BLUE2 	0x1c9f
#define PINK    0xd8a7
#define ORANGE  0xfa20
#define WHITE		0xffff
#define BLACK		0x0000
#define YELLOW  0xFFE0
#define CYAN    0x07ff
#define PURPLE  0xf81f
#define PURPLE2 0xdb92
#define PURPLE3 0x8811
#define GRAY0   0xEF7D
#define GRAY1   0x8410
#define GRAY2   0x4208

void st7735_DC(uint8_t BitVal);
void st7735_BLK(uint8_t BitVal);
void st7735_RES(uint8_t BitVal);
void st7735_init(void);
void st7735_senddata(uint8_t data);
void st7735_sendindex(uint8_t index);
void st7735_send16bit(uint16_t data);
void st7735_sendindex_data(uint8_t index,uint8_t data);
void st7735_rest(void);
void st7735_SpinScreen(uint8_t locate);
void st7735_SetRegion(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end);
void st7735_Clear(uint16_t color);
void st7735_FullScreen(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
void st7735_DrawLine(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1,uint16_t Color);
void st7735_SetCursor(uint16_t x,uint16_t y);
void st7735_DrawPoint(uint16_t x,uint16_t y,uint16_t Data);
void st7735_ShowImage(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const unsigned char *p);
void st7735_showchar(uint8_t x,uint8_t y,uint16_t fc,uint16_t bc,char c);
void st7735_ShowChinese(uint8_t x,uint8_t y,uint16_t fc,uint16_t bc,char *c);
int map(char *c);
void st7735_bold_num(uint8_t x,uint8_t y,uint16_t fc,uint16_t bc,char *c);

#endif

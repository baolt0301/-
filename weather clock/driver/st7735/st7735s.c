/*
	SCL   PA5
	SDA   PA7
	RES   PB0
	DC    PB1
	CS    PA4
*/

#include "stm32f10x.h"
#include <stdbool.h>
#include "st7735s.h"
#include "spi_lcd.h"
#include "delay.h"
#include "font.h"
#include <string.h>

extern const unsigned char asc[];           //汉字字模数组
extern const unsigned char chinese_font[];  //ASCII字模数组
extern const char font_sample[];            //汉字数组

void st7735_DC(uint8_t BitVal)
{
    GPIO_WriteBit(DC_PORT,DC_PIN,(BitAction)BitVal);
}
void st7735_BLK(uint8_t BitVal)
{
    GPIO_WriteBit(BLK_PORT,BLK_PIN,(BitAction)BitVal);
}
void st7735_RES(uint8_t BitVal)
{
    GPIO_WriteBit(RES_PORT,RES_PIN,(BitAction)BitVal);
}

void st7735_init(void)
{
    // CS
    GPIO_InitTypeDef GPIO_InitStructer;
	GPIO_InitStructer.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructer.GPIO_Pin = BLK_PIN | DC_PIN | RES_PIN ;
	GPIO_InitStructer.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DC_PORT,&GPIO_InitStructer);
    GPIO_SetBits(BLK_PORT,BLK_PIN);
    GPIO_ResetBits(DC_PORT,DC_PIN);
    GPIO_SetBits(RES_PORT,RES_PIN);

    myspi_init();
    st7735_rest();

    st7735_sendindex(0x11);
    Delay_ms(120);
     //----------------------基本配置---------------------------------
	st7735_sendindex(0x36);        //【数据访问方式】，RGB/BGR,行列读写方向与水平垂直刷新等
	st7735_senddata(0x00); 	      

	st7735_sendindex(0x3A);        //【RGB图像数据格式】，后3位设定
    st7735_senddata(0x05);         //3: 12bit;  5: 16 bit； 6: 18bit ； 7: 未使用

	st7735_sendindex(0xB1);        //帧率控制（正常模式/全彩） 后跟3个数据
	st7735_senddata(0x05);
	st7735_senddata(0x3C);
	st7735_senddata(0x3C);

	st7735_sendindex(0xB2);        //帧率控制（空闲模式/ 8色） 后跟3个数据
	st7735_senddata(0x05);
	st7735_senddata(0x3C);
	st7735_senddata(0x3C);

	st7735_sendindex(0xB3);       //帧率控制（部分模式/全彩）后跟6个数据
	st7735_senddata(0x05);        //1-3个参数 点反转
	st7735_senddata(0x3C);
	st7735_senddata(0x3C);
	st7735_senddata(0x05);        //4-6个参数 列反转
	st7735_senddata(0x3C);
	st7735_senddata(0x3C);

	st7735_sendindex(0xB4);      //显示反转控制，D0-D3位有效，分别对应不同模式
	st7735_senddata(0x03);

	//------------------电源控制寄存器1-5------------------
	st7735_sendindex(0xC0);    //特定颜色模式下的电压参数,调整显示屏的亮度、对比度等显示效果
	st7735_senddata(0x2E);
	st7735_senddata(0x06);
	st7735_senddata(0x04);

	st7735_sendindex(0xC1);   //C1-C4功能同0xC1,更精细的电压调整，以达到更好的视觉效果。
	st7735_senddata(0xC0);
    st7735_senddata(0xC2);

	st7735_sendindex(0xC2);   
	st7735_senddata(0x0D);
	st7735_senddata(0x0D);

	st7735_sendindex(0xC3);  
	st7735_senddata(0x8D);
	st7735_senddata(0xEE);

	st7735_sendindex(0xC4);   
	st7735_senddata(0x8D);
	st7735_senddata(0xEE);

	st7735_sendindex(0xC5);  //设置显示屏的VCOM 电压，即显示屏公共电极的电压
	st7735_senddata(0x00);   //影响整体显示效果，调整亮度均匀，减少色彩失真

	//---------------------数据显示方式（重要）-----------------
	st7735_sendindex(0x36); //【数据显示方式格式详解】与屏幕方向息息相关，以0xC0 为例，
	st7735_senddata(0xC8);   //   MY行顺序    MX列顺序    MV行列转换   ML垂直刷新  RGB/BGR   MH水平刷新   -   -
                        //      1          1            0           0          0           0       0   0
											  //    上至下      左至右        否         关闭         RGB        关闭
											  //设置方向函数，只需要调整  MY，MX, MV 的值即可


	//-----------------------伽马序列------------------------
	                      //出厂已调好，一般无需额外调整
	st7735_sendindex(0xe0);  //伽马极性校正设置，后跟16个8位数据
	st7735_senddata(0x1B);   //涉及高,中，低三个等级调整，有效地址：D0-D5位
	st7735_senddata(0x21);
	st7735_senddata(0x10);   //可使屏幕亮度更符合人眼的感知特性，减少亮度失真导致的视觉疲劳
	st7735_senddata(0x15);   //优化色彩准确性,准确地显示出各种颜色
	st7735_senddata(0x2B);   //增强暗部细节,改善在低灰阶显示效果
	st7735_senddata(0x25);
	st7735_senddata(0x1F);
	st7735_senddata(0x23);
	st7735_senddata(0x22);
	st7735_senddata(0x22);
	st7735_senddata(0x2B);
	st7735_senddata(0x37);
	st7735_senddata(0x00);
	st7735_senddata(0x15);
	st7735_senddata(0x02);
	st7735_senddata(0x3F);

	st7735_sendindex(0xE1);    //同E0，略
	st7735_senddata(0x1A);
	st7735_senddata(0x20);
	st7735_senddata(0x0F);
	st7735_senddata(0x15);
	st7735_senddata(0x2A);
	st7735_senddata(0x25);
	st7735_senddata(0x1E);
	st7735_senddata(0x23);
	st7735_senddata(0x23);
	st7735_senddata(0x22);
	st7735_senddata(0x2B);
	st7735_senddata(0x37);
	st7735_senddata(0x00);
	st7735_senddata(0x15);
	st7735_senddata(0x02);
	st7735_senddata(0x3F);

	//---------------------自定补充操作------------------
	st7735_sendindex(0x2C);  // 【0x2c作用1】初始化设置时，配置显示参数
	//st7735_sendindex(0x21); //【打开颜色反转】正常使用无需打开
	st7735_sendindex(0x29);  //【打开屏幕】，0x28为关闭屏幕
	st7735_Clear(BLACK);    //初始清屏
}

void st7735_senddata(uint8_t data)
{
    spi_cs(0);
    st7735_DC(1);
    spi_send(data);
    spi_cs(1);
}

void st7735_sendindex(uint8_t index)
{
    spi_cs(0);
    st7735_DC(0);
    spi_send(index);
    spi_cs(1);
}

void st7735_send16bit(uint16_t data)
{
	spi_cs(0);
    st7735_DC(1);
    spi_send(data >> 8);
    spi_send(data);
    spi_cs(1);
}

void st7735_sendindex_data(uint8_t index,uint8_t data)
{
    st7735_sendindex(index);
    st7735_senddata(data);
}

void st7735_rest(void)
{
    st7735_RES(0);
    Delay_ms(100);
    st7735_RES(1);
    Delay_ms(50);
}



//设定屏幕旋转方向
void st7735_SpinScreen(uint8_t locate){
	st7735_sendindex(0x36);
	if(locate==0) st7735_senddata(0xC8); 	  //纵向，左上角（0，0）
	if(locate==1) st7735_senddata(0xA8);     //右转90°  横向
	if(locate==2) st7735_senddata(0x08);     //右转180°  纵向
	if(locate==3) st7735_senddata(0x68);     //右转270° 横向


}


//功能：选中一个矩形区域[x,y]-[x1,y1]
void st7735_SetRegion(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end)
{
	st7735_sendindex(0x2a);  //设置列地址范围，命令（0x2a）+数据（4字节数据）
	st7735_senddata(0x00);    //x范围:（0，127），含边界0 127
	st7735_senddata(x_start);
	st7735_senddata(0x00);
	st7735_senddata(x_end);

	st7735_sendindex(0x2b);//设置行地址范围 命令（0x2b）+数据（4字节数据）
	st7735_senddata(0x00); //y范围:（0，159），含边界0 127
	st7735_senddata(y_start);
	st7735_senddata(0x00);
	st7735_senddata(y_end);

	st7735_sendindex(0x2c); //【0x2c作用2】确认将像素数据写入显存，每次设置完0x2a 2x2b后，需调用0x2c
}                      //不确认将无法写入数据



//全屏填色函数
void st7735_Clear(uint16_t color){
	unsigned int i,m;
	st7735_SetRegion(0,0,127,127); //定位全屏
	st7735_sendindex(0x2C);       //确认操作，不确认无法写入
	for(i=0;i<128;i++)
		for(m=0;m<128;m++){
			st7735_send16bit(color);
    }
}

//选中区域并用颜色填充
void st7735_FullScreen(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color){
	st7735_SetRegion(x1,y1,x2,y2);//设置区域(0,0)-(127,127)
	int t=(x2-x1+1)*(y2-y1+1);
	while(t--)
	  	st7735_send16bit(color);
	st7735_SetRegion(0,0,127,127);//恢复定位全屏
}


//设置TFT显示起始点（本质就是选中一个点的区域）
void st7735_SetCursor(uint16_t x,uint16_t y){
	st7735_SetRegion(x,y,x,y);
}

void st7735_DrawPoint(uint16_t x,uint16_t y,uint16_t Data){
	st7735_SetCursor(x,y);
	st7735_send16bit(Data);
}

void st7735_DrawLine(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1,uint16_t Color){
	int dx,            // difference in x's
    dy,             // difference in y's
    dx2,            // dx,dy * 2
    dy2,
    x_inc,          // amount in pixel space to move during drawing
    y_inc,          // amount in pixel space to move during drawing
    error,          // the discriminant i.e. error i.e. decision variable
    index;          // used for looping

	st7735_SetCursor(x0,y0);
	dx = x1-x0;//݆̣xߠk
	dy = y1-y0;//݆̣yߠk

	if (dx>=0) x_inc = 1;
	else{
		x_inc = -1;
		dx    = -dx;
	}

	if (dy>=0) y_inc = 1;
	else{
		y_inc = -1;
		dy    = -dy;
	}
	dx2 = dx << 1;
	dy2 = dy << 1;
	if (dx > dy) //xߠkճԚyߠkìŇôÿٶxסʏֻԐһٶ֣ìÿٶyסʏԐɴىٶ֣
	{           //ȒПք֣˽ֈԚxߠkìӔxס֝նۭ֣
		           // initialize error term
		error = dy2 - dx;
		// draw the line
		for (index=0; index <= dx; index++){ //Ҫۭք֣˽һܡӬڽxߠk{
			//ۭ֣
			st7735_DrawPoint(x0,y0,Color);

			// test if error has overflowed
			if (error >= 0) //ˇرѨҪնݓyظҪֵ
			{
				error-=dx2;

				// move to next line
				y0+=y_inc;//նݓyظҪֵ
			} // end if error overflowed

			// adjust the error term
			error+=dy2;

			// move to the next pixel
			x0+=x_inc;//xظҪֵÿՎۭ֣۳ּ֝ն1
		} // end for
	} // end if |slope| <= 1
	else//yסճԚxסìղÿٶyסʏֻԐһٶ֣ìxסɴىٶ֣
	{//ӔyסΪ֝նۭ֣
		// initialize error term
		error = dx2 - dy;

		// draw the line
		for (index=0; index <= dy; index++)
		{
			// set the pixel
			st7735_DrawPoint(x0,y0,Color);

			// test if error overflowed
			if (error >= 0){
				error-=dy2;

				// move to next line
				x0+=x_inc;
			} // end if error overflowed

			// adjust the error term
			error+=dx2;

			// move to the next pixel
			y0+=y_inc;
		} // end for
	} // end else |slope| > 1
}

void st7735_ShowImage(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const unsigned char *p){
	st7735_SetRegion(x,y,x+length-1,y+width-1);//定位区域
	unsigned char picH,picL;
	for(int i=0;i<length*width;i++)
	{
		picL=p[2*i];
		picH=p[2*i+1]; //2个元素记录一个颜色点
		st7735_send16bit(picH<<8|picL);      //图像取模时若选则：低位在前（默认一般为此）
//		st7735_send16bit(picL<<8|picH);    //图像取模时若选则：高位在前
	}
	st7735_SetRegion(0,0,127,127);//恢复窗口
}

//输出字符
//参数：坐标，字体颜色，背景颜色，字符内容
void st7735_showchar(uint8_t x,uint8_t y,uint16_t fc,uint16_t bc,char c)
{
    int k=(c-32)*16;   //找到该字符在数组中的起始下标
	for (int i = 0; i < 16; i++)         //行16  字模高度
        for (int j = 0; j < 8; j++)      //列8   字模宽度
		{     
            if (asc[k+i]&(0x80>>j))
                st7735_DrawPoint(x+j,y+i,fc); //点亮不同位置的点，亮与不亮的点，构成字符外观
            else
                st7735_DrawPoint(x+j,y+i,bc);
		}
}



void st7735_bold_num(uint8_t x,uint8_t y,uint16_t fc,uint16_t bc,char *c)
{
    int len = strlen(c);
    for(int n = 0 ;n<len;n++)
    {
        int k = (c[n]-47)*64;
        for(int i = 0;i<32;i++)
        {
            uint16_t line_data = (asc_25[k + i * 2 ] << 8) | asc_25[k + i * 2 +1];
                for(int j=0;j<15;j++)
                {
                if(line_data&(0x8000>>j))
                    st7735_DrawPoint(x+j,y+i,fc);
                else
                    st7735_DrawPoint(x+j,y+i,bc);
                }
        }
        x+=16;  //更新位置
    }
}


void st7735_ShowChinese(uint8_t x,uint8_t y,uint16_t fc,uint16_t bc,char *c){
    int t=strlen(c);
	for(int n=0;n<t;n++)  //遍历字符串，中/字符，判断后分别输出
	{
	  //遇到字符
		if(c[n]>31&&c[n]<127){
			if(x+8>=128){ //自动换行
				x=0;y+=16;
			}
		  st7735_showchar(x,y,fc,bc,c[n]);
		  x+=8;
			continue; //能少写一个else{}
		}
		//遇到汉字
		char tem[4];
		tem[0]=c[n];tem[1]=c[n+1];tem[2]=c[n+2];tem[3]='\0';     //3位都相同确认一个汉字，2位不行
		int k=map(tem); //获取汉字对应的初始数组下标

		//字库没该汉字时
		if(k == -1){
			if(x+8>=128){ x=0;y+=16;}
			st7735_showchar(x,y,YELLOW,RED,'?');  //可自定义一个符号替代
			x+=8;
			n+=2;    //顺便跳过后半个汉字字符,一个汉字，3个位置 见721行注解
			continue;
		}

			//输出该汉字
		  if(x+16>=128){   //自动换行
			  x=0;y+=16;
			}
			for(int i = 0; i < 16; i++){
					for (int j = 0; j < 8; j++){            //左半边
						if (chinese_font[k*32+2*i]&(0x80>>j)) //因为中文2个字节，左侧恰好全为偶数，右侧全为奇数
							st7735_DrawPoint(x+j,y+i,fc);        //数组中，每连续的两个元素，点亮一行。
						else                                //故而奇数元素总是负责左半行，偶数负责右半行
							st7735_DrawPoint(x+j,y+i,bc);
					}
					for (int j = 0; j < 8; j++){              //右半边，右侧为奇数
						if (chinese_font[k*32+2*i+1]&(0x80>>j)) //k*32得到在字模数组中的初始下标
							st7735_DrawPoint(x+j+8,y+i,fc);
						else
							st7735_DrawPoint(x+j+8,y+i,bc);
					}
		  }
			x+=16;  //更新位置
		  n+=2;   //我这里一个汉字占3个位置，所以加2
   }
}

int map(char *c){
	int l1=strlen(font_sample);
	for(int i=0;i<l1;i+=3){  //遍历母串
		if((font_sample[i]==c[0])&&(font_sample[i+1]==c[1])&&(font_sample[i+2]==c[2])) //三位定一个汉字
			   return i/3;
  }
  return -1;
}





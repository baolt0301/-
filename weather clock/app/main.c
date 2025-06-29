#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <string.h>
#include "Delay.h"
#include "main.h"
#include "esp_at.h"
#include "timer.h"
#include "st7735s.h"
#include "rtc.h"
#include "weather.h"
#include "mpu6500.h"
#include "OLED.h"
#include "mydma.h"
#include "weather_icon.h"

int16_t Ax,Ay,Az,Gx,Gy,Gz;
float temp;
bool sntp_ok = false;
bool weather_ok = false;
bool temper_ok = false;

static const char *wifi_ssid = "Xiaomi15";
static const char *wifi_passward = "88888888";
static const char *weather_url="https://api.seniverse.com/v3/weather/now.json?key=SU6xxAKk6eEvnMK13&location=hefei&language=zh-Hans&unit=c";
static uint32_t rnums ;
static uint32_t font_locate = 3;

const char font_sample[]="温度开机中等待初始化失败连接准备就绪合肥℃晴阴室内多云小大雨雪夹              ";

static void timer_elapsed_callback(void)
{
    rnums ++;
    if(rnums >= 24 * 60 * 60 * 1000)
    {
        rnums = 0 ;
    }
}

static void wifi_init(void)
{
    st7735_ShowChinese(3,font_locate,WHITE,BLACK,"初始化");
    font_locate += 16 ;
    if(!esp_at_init())
    {
        st7735_ShowChinese(3,font_locate,RED,BLACK,"初始化失败");
        font_locate += 16 ;
        while (1);
    }
    st7735_ShowChinese(3,font_locate,WHITE,BLACK,"初始化wifi");
    font_locate += 16 ;
    if(!esp_at_wifi_init())
    {
        st7735_ShowChinese(3,font_locate,RED,BLACK,"初始化wifi失败");
        font_locate += 16 ;
        while (1);
    }
    st7735_ShowChinese(3,font_locate,WHITE,BLACK,"连接wifi");
    font_locate += 16 ;
    if(!esp_at_wifi_connect(wifi_ssid,wifi_passward))
    {
        st7735_ShowChinese(3,font_locate,RED,BLACK,"wifi连接失败");
        font_locate += 16 ;
        while (1);
    }
    if(!esp_at_sntp_init())
    {
        while (1);
    }
}


int main(void){

    peripher_inti();
    rtc_init();

    timer_init(10000);
    timer_elapsed_register(timer_elapsed_callback);
	timer_start();

    mydma_init();
    st7735_init();
    st7735_FullScreen(3,3,128,128,BLACK);

    mpu6500_init();
	mpu6500_W_reg(0x6B,0x00);
	mpu6500_W_reg(0x19,0x66);

    st7735_ShowChinese(3,font_locate,WHITE,BLACK,"开机中......");
    font_locate += 16 ;
    Delay_ms(1000);

    st7735_ShowChinese(3,font_locate,WHITE,BLACK,"等待WIFI...");
    font_locate += 16 ;

    Delay_ms(500);
	wifi_init();
    st7735_FullScreen(3,3,128,128,BLACK);
    font_locate = 3 ;
    st7735_ShowChinese(3,font_locate,GREEN,BLACK,"准备就绪");
    st7735_FullScreen(3,3,128,128,BLACK);
    char str[64];
	bool spin_state_1 = true;
	bool spin_state_2 = true;
	bool spin_state_3 = true;
	rnums = 0;
	uint32_t last_rnums = rnums;
	while(1)
    {
		last_rnums = rnums;
		mpu6500_getdata(&Ax,&Ay,&Az,&Gx,&Gy,&Gz,&temp);

		//控制屏幕显示方向
		if(Ay > 700 && Ax < 1000 && spin_state_1)
		{
			st7735_SpinScreen(3);
			st7735_FullScreen(3,3,128,128,BLACK);
			sntp_ok = false;
			weather_ok = false;
			temper_ok = false;
			spin_state_1 = false;
			spin_state_2 = true;
			spin_state_3 = true;
		}
		else if(Ay < -700 && Ax > -1000 && spin_state_2)
			{
			st7735_SpinScreen(1);
			st7735_FullScreen(3,3,128,128,BLACK);
			sntp_ok = false;
			weather_ok = false;
			temper_ok = false;
			spin_state_1 = true;
			spin_state_2 = false;
			spin_state_3 = true;
			}
		else if(Ax < -1000 && Az < 1700 && spin_state_3)
			{
			st7735_SpinScreen(2);
			st7735_FullScreen(3,3,128,128,BLACK);
			sntp_ok = false;
			weather_ok = false;
			temper_ok = false;
			spin_state_1 = true;
			spin_state_2 = true;
			spin_state_3 = false;
			}

        //更新时间
        if(last_rnums %10 == 0)
        {
            rtc_date_t data;
		    rtc_get_date(&data);
		    snprintf(str,sizeof(str),"%d/%d",data.month,data.day);
		    st7735_bold_num(3,40,YELLOW,BLACK,str);
		    snprintf(str,sizeof(str),"%02d:%02d",data.hour,data.minute);
		    st7735_bold_num(3,6,YELLOW,BLACK,str);
			snprintf(str,sizeof(str),":%02d",data.second);
		    st7735_ShowChinese(84,20,WHITE,BLACK,str);
        }
         //联网同步时间
        if(!sntp_ok || last_rnums %(60 * 60 * 100) == 0)
        {
            uint32_t ts;
            esp_at_get_time(&ts);
            rtc_set_timestamp(ts);
			sntp_ok = true;
        }
          //更新天气
        if(!weather_ok || last_rnums %(10 * 60 * 100) == 0)
        {
            const char *rsp;
            esp_at_get_http(weather_url, &rsp, NULL, 10000);
            weather_t weather;
            weather_parse(rsp, &weather);
			snprintf(str,sizeof(str),"%5d",rnums);
            snprintf(str,sizeof(str),"%s",weather.weather);
			if(strlen(weather.weather)-2 == 1)
			{
				st7735_ShowChinese(96,88,YELLOW,BLACK,str);
			}
			else if(strlen(weather.weather) == 6)
			{
				st7735_ShowChinese(88,88,YELLOW,BLACK,str);
			}
//			OLED_ShowSignedNum(4,14,strlen(weather.weather),2);
            snprintf(str,sizeof(str),"%s℃",weather.temperature);
            st7735_ShowChinese(88,108,BLUE,BLACK,str);

            if(strcmp(weather.weather,"阴") == 0)
                {
                    st7735_ShowImage(80,40,48,48,icon_yin);
                }
			else if(strcmp(weather.weather,"晴") == 0)
                {
                    st7735_ShowImage(80,40,48,48,icon_qing);
                }
			else if(strcmp(weather.weather,"多云") == 0)
                {
                    st7735_ShowImage(80,40,48,48,icon_duoyun);
                }
			else if(strcmp(weather.weather,"小雨") == 0 || strcmp(weather.weather,"中雨") == 0 || strcmp(weather.weather,"大雨") == 0)
                {
                    st7735_ShowImage(80,40,48,48,icon_yu);
                }
			else if(strcmp(weather.weather,"雪") == 0)
                {
                    st7735_ShowImage(80,40,48,48,icon_xue);
                }
			weather_ok = true;
        }
           //更新环境温度
        if(temper_ok || last_rnums %(5 * 100) == 0)
        {
			st7735_ShowChinese(3,80,GREEN,BLACK,"室内温度:");
			snprintf(str,sizeof(str),"%.1f℃",temp);
			st7735_ShowChinese(12,98,GREEN,BLACK,str);
			temper_ok = true ;
        }
	}
}

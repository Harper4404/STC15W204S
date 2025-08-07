//#include <reg52.h>//stc15w20s为51内核
#include <intrins.h>
#include <stc15.h>


#define uint unsigned int
#define uchar unsigned char
 
#define MAIN_Fosc 35000000UL //定义主时钟
#define BAUD 9600

//sbit PIN_LED = P3^3;//管脚定义
sbit WEI3 = P3^2; //选位
sbit WEI2 = P3^3;
sbit WEI1 = P3^6;
//P1.0~P1.5 对应a,b,c,d,e,f段
sbit Z_g = P3^7; // P3.7对应g段
sbit DOT4 = P5^4; //P5.4对应小数点

 
//函数申明,因为函数在main函数后面申明。
void delay_ms(unsigned char ms);

// 共阳数码管段码表(0-F)
unsigned char code segTable[] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, // 0-4
    0x92, 0x82, 0xF8, 0x80, 0x90, // 5-9
    0x88, 0x83, 0xC6, 0xA1, 0x86, // A-E
    0x8E  // F
};
unsigned char dispValue = 0xFF; // 显示值缓存
bit refreshFlag = 0;            // 刷新标志


void main()
{
   
  uchar i;
	int wei;


for (wei=0;wei<3;wei++)
	{
		switch(wei)
		{
			case 0: 
        for(i=0;i<16;i++)
			{ 
P1 = segTable[i]; 
Z_g = (segTable[i] >> 6)&1; 

DOT4 = 0; //使能小数点 点亮。0亮，1灭
WEI1 = 0;
            delay_ms(200);
                
            WEI1 = 1;
				}
break;		
				
			case 1:
        for(i=0;i<16;i++)
			{
P1 = segTable[i]; 
Z_g = (segTable[i] >> 6)&1; 
WEI2 = 0; 
DOT4 = 0;

            delay_ms(200);
            WEI2 = 1; 
				}
break;	
				
		case 2:


        for(i=0;i<16;i++)
		{

P1 = segTable[i]; 
Z_g = (segTable[i] >> 6)&1; 
		WEI3 = 0; 
        DOT4 = 0;
            delay_ms(200);
            WEI3 = 1; 
        }
break;
default:
	
				break;
    }
		
	}      
     

}
 
 
//========================================================================
// 函数: void delay_ms(unsigned char ms)
// 描述: 延时函数。
// 参数: ms,要延时的ms数, 这里只支持1~255ms. 自动适应主时钟.
// 返回: none.
// 版本: VER1.0
// 日期: 2022-12-10
// 备注:
//========================================================================
void delay_ms(unsigned char ms)
{
	 unsigned int i;
	 do{
		i = MAIN_Fosc / 13000;
		while(--i) ;
	 }while(--ms);
 }



void Uart_ISR() interrupt 4 {
    if(RI) {
        RI = 0;
        dispValue = SBUF; // 接收串口数据
        refreshFlag = 1;  // 设置刷新标志
    }
}

void Timer0_ISR() interrupt 1 {
    static unsigned char cnt = 0;
    if(++cnt >= 10) {    // 1ms刷新
        cnt = 0;
        refreshFlag = 1;
    }
}
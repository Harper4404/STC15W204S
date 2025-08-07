//#include <reg52.h>//stc15w20sΪ51�ں�
#include <intrins.h>
#include <stc15.h>


#define uint unsigned int
#define uchar unsigned char
 
#define MAIN_Fosc 35000000UL //������ʱ��
#define BAUD 9600

//sbit PIN_LED = P3^3;//�ܽŶ���
sbit WEI3 = P3^2; //ѡλ
sbit WEI2 = P3^3;
sbit WEI1 = P3^6;
//P1.0~P1.5 ��Ӧa,b,c,d,e,f��
sbit Z_g = P3^7; // P3.7��Ӧg��
sbit DOT4 = P5^4; //P5.4��ӦС����

 
//��������,��Ϊ������main��������������
void delay_ms(unsigned char ms);

// ��������ܶ����(0-F)
unsigned char code segTable[] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, // 0-4
    0x92, 0x82, 0xF8, 0x80, 0x90, // 5-9
    0x88, 0x83, 0xC6, 0xA1, 0x86, // A-E
    0x8E  // F
};
unsigned char dispValue = 0xFF; // ��ʾֵ����
bit refreshFlag = 0;            // ˢ�±�־


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

DOT4 = 0; //ʹ��С���� ������0����1��
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
// ����: void delay_ms(unsigned char ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��, ����ֻ֧��1~255ms. �Զ���Ӧ��ʱ��.
// ����: none.
// �汾: VER1.0
// ����: 2022-12-10
// ��ע:
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
        dispValue = SBUF; // ���մ�������
        refreshFlag = 1;  // ����ˢ�±�־
    }
}

void Timer0_ISR() interrupt 1 {
    static unsigned char cnt = 0;
    if(++cnt >= 10) {    // 1msˢ��
        cnt = 0;
        refreshFlag = 1;
    }
}
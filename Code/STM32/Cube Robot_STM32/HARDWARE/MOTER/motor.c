/*  �ļ�����motor.c
 *�ļ���������Ҫ��Ŷ������������GPIO�ĳ�ʼ���Ͷ���ٶȿ��Ʋ岹�㷨
 *��    ע��change����nΪ�ٶ�ֵ������ʵ�ʲ��ԣ����ֵ�nΪ50���߸�Сʱ����������ִ���
 *��Ϊ���ת����һ���Ļ�е�ͺ��ԣ�ת����Ҫһ����ʱ�䣬����nֵ����̫С������
 *�з��ֵ�nֵΪ500����1000ʱ�����ת������
 */
#include "stm32f10x.h"
#include "motor.h"
#include "sys.h"
#include "instruction.h"
#include "movement.h"
#include "usart.h"
#include "exti.h"
#include "timer.h"
#include <math.h>
#include "led.h"

int point_now=-1;    /*����ִ�е�����*/
u8 point_aim=0;	     /*����ִ���������һ������*/
u16 n=300;		       /*�趨�Ĳ岹����*/
u16 motor_speed=300;
//u16 m;		           /*��ǰ�Ĳ岹����*/
double dp;
double dp0[8];	  /*�岹����*/
u16 pwm[8]={1510,1475,1550,1505,2065,1770,1945,1745};

u8 end_flag=0;//ħ����ԭ��ɱ�־λ
u16 pos[500][8];



/*
 *�� �� ��:change()  
 *��������:��λ��ĩβ�ø���
 *		   ��Ч�������ǲ岹�������Ͳ岹������֪�������������͵�ǰ��λ�ü���
 *��    �룺��
 *��    ������
 *��    �ã��� vpwm()����		 
 *��    ע��Ҫʵ���Ƚ��ٶȣ�����ÿһ�ν���˺���ʱ��8��������費ͬ�Ĳ岹����
 */	
 void change(void)
{	
	 u8 j;  	
	
	 if(point_aim==lines_num)	        
   {
		 
				TIM_Cmd(TIM3, DISABLE);  				 
				point_aim=0;
				point_now=-1;
				if(send_start_flag==1)
				{
					end_flag=1;
				}
		    
				switch(movement_tag)
				 {
							 case 'Z':
							 {
								  USART_SendChar('1');
								  TIM_Cmd(TIM4, ENABLE);
								 
							 }break;
							 
							 case 'A':
							 {
								  USART_SendChar('2');
								  TIM_Cmd(TIM4, ENABLE);
							 }break;
						 
							 case 'B':
							 {
								  USART_SendChar('3');
								  TIM_Cmd(TIM4, ENABLE);
							 }break;
						 
							 case 'C':
							 {
								 USART_SendChar('4');
								  TIM_Cmd(TIM4, ENABLE);
							 }break;
						 
							 case 'D':
							 {
								  USART_SendChar('5');
								  TIM_Cmd(TIM4, ENABLE);
							 }break;
						 
							 case 'E':
							 {
								  USART_SendChar('6');
								  TIM_Cmd(TIM4, ENABLE);
							 }break;
	
							 default:
							 {
								 
							 }break;		 							 
				 }	
				 
   }
	 
   else
   {
			point_aim++;
			point_now++;	
			for(j=0;j<8;j++)
			{
				 pwm[j]=pos[point_aim][j];
			} 
		 
   }
	 
}


/*
 *�� �� ����pwm[]������º���  
 *�������������ݲ岹���岹ʱ����Ϊ20ms/16����Timer1���ƣ�ʹ���ƽ��ʵ���ٶȿ���
 *          ��һ��������ִ����һ�к�ȥ������һ�����ݣ�������change()
 *��    �룺�� 
 *��    ������
 *��    �ã���main.c����
 *��    ע��
 */	
void vpwm(void)		 
{				                                                                                                                                                                                                                                                                                                                                                                                                 
	 static u16 m=0;
	
	  m++;
		if(m==motor_speed)
		{	
			m=0;		
			change();			 	  
		}
		
		return;
}


/*������������ʼ�����ź������ӵ�GPIO����
 *��    ע��PA.1--------PA.8
 */
void MotorPin_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   /*ʹ��PinA�˿�ʱ��*/
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;	/*�˿����ã�PA0������WK_UPռ��*/
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		   /*�������*/
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   /*IO���ٶ�Ϊ50MHz*/
	
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					         /*�����趨������ʼ��GPIOA*/
	 GPIO_ResetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);						
	
}


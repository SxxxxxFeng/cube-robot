#include "stm32f10x.h"
#include "exti.h"
#include "key.h"
#include "led.h"
#include "delay.h"
#include "movement.h"
#include "usart.h"
#include "motor.h"

u8 exti_flag=0;
u8 movement_tag='Q';


/*�ⲿ�ж�2��ʼ��*/
void Exti_Init(void)
{
	 	EXTI_InitTypeDef EXTI_InitStructure;
	  KEY_Init();   	                                            /*����GPIO��ʼ��*/
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	        /*ʹ�ܸ��ù���ʱ��*/
	
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource1);  /*���ⲿ�ж�*/
	  EXTI_InitStructure.EXTI_Line=EXTI_Line1;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     /*�½��ش���*/
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	

		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13); 
	  EXTI_InitStructure.EXTI_Line=EXTI_Line13;
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     /*�½��ش���*/
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  EXTI_Init(&EXTI_InitStructure);	

	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); 
	  EXTI_InitStructure.EXTI_Line=EXTI_Line0;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;     /*�����ش���*/
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  EXTI_Init(&EXTI_InitStructure);	
	
    Ex_NVIC_Config();                                        /*�ⲿ�ж����ȼ�����*/
	
}

/*�ⲿ�ж����ȼ�����*/
void Ex_NVIC_Config(void)
{
	  NVIC_InitTypeDef NVIC_InitStructure;
	
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	    /*��ռ���ȼ�2*/ 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					  /*�����ȼ�2*/
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								/*ʹ���ⲿ�ж�ͨ��*/
  	NVIC_Init(&NVIC_InitStructure);	

		NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	    /*��ռ���ȼ�2*/ 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					  /*�����ȼ�2*/
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								/*ʹ���ⲿ�ж�ͨ��*/
  	NVIC_Init(&NVIC_InitStructure);	
	
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	    /*��ռ���ȼ�2*/ 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					  /*�����ȼ�2*/
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								/*ʹ���ⲿ�ж�ͨ��*/
  	NVIC_Init(&NVIC_InitStructure);	
	
}

/*���������������ĵ�һ�����ĸ���е��ȫ���ɿ������ò����߰�ħ�������е����
 *��    ע����������������ط������õ���1.ϵͳ�ϵ�֮�󣬶���ĽǶȺܲ����� 2.��ԭħ������֮�󣬰�ħ���ó���
 *          KEY0
 */
void EXTI1_IRQHandler(void)
{
	delay_ms(10);
	
	if(KEY0==0)
	{
		LED1=!LED1;		
		Init_MotorMovement();
		motor_speed=250;
		change();		
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�	
		TIM_Cmd(TIM3, ENABLE);
	}	
	EXTI_ClearITPendingBit(EXTI_Line1);  
	
}



/*����������ת����һ����Ҫ���յ��棬��ħ�������е��֮�����������
 *��    ע���������ӦKEY1��ע�ⲻ��EXTI13_IRQHandler
 */
void EXTI15_10_IRQHandler(void)
{
	delay_ms(10);
	
	if(KEY1==0)
	{
		LED1=!LED1;		
		PicArray_ToBufferArray(firpic_position,4);
		motor_speed=250;//�ٶ��趨
		change();		
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�	
		TIM_Cmd(TIM3, ENABLE);     
		movement_tag='Z';   //��������֮����Ҫ֪ͨAPP�����һ��ͼƬ
		
	}
	EXTI_ClearITPendingBit(EXTI_Line13); 
  }
	


/*�����������ĸ���е�ֱպϣ�ץ��ħ��
 *��    ע���������ӦKEY2
 */
void EXTI0_IRQHandler(void)
{
	
	delay_ms(10);
	
	if(KEY2==1)
	{
		LED1=!LED1;
		PicArray_ToBufferArray(firpic_position,2);
		motor_speed=250;
		change();	
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�	
		TIM_Cmd(TIM3, ENABLE);  
		
	}

	EXTI_ClearITPendingBit(EXTI_Line0);  	
}


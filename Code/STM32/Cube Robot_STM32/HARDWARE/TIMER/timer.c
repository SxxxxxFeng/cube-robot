#include "timer.h"
#include "led.h"
#include "motor.h"
#include "usart.h"
#include "exti.h"
#include "movement.h"

u8 flag_vpwm=0; /*�岹��־λ*/

/*
 *ͨ�ö�ʱ��3�жϳ�ʼ��
 *����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
 *arr���Զ���װֵ
 *psc��ʱ��Ԥ��Ƶ��
 *����ʹ�õ��Ƕ�ʱ��3!
 */
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
     /*��ʱ��TIM3��ʼ��*/
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	/*�ж����ȼ�NVIC����*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM3, DISABLE);  /*��ʹ��TIM3*/					 
}

u8 send_start_flag=0;



/*
 *ͨ�ö�ʱ��4�жϳ�ʼ��
 *����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
 *arr���Զ���װֵ
 *psc��ʱ��Ԥ��Ƶ��
 *����ʹ�õ��Ƕ�ʱ��4!
 */
void TIM4_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	/*��ʱ��TIM4��ʼ��*/
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�

	/*�ж����ȼ�NVIC����*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM4, DISABLE);  //��ʹ��TIM4			
}




/*
 *���������������趨��ʱ��3�Ķ�ʱʱ��
 *��    �룺arr:�Զ���װ�ؼĴ������ڵ�ֵ����λΪus
 *��    ������
 *��    ��:	�ڶ�ʱ���ж��б�����
 *��    ע:	Ԥ��Ƶ��ֵ�趨Ϊ71������������Ϊ1us
 */
void TIM3_Set_Time(u16 arr)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    arr--;
  	TIM_TimeBaseStructure.TIM_Period = arr;           /*��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ*/	
	  TIM_TimeBaseStructure.TIM_Prescaler =71;          /*����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ*/
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);   /*����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ*/
				
}


/*
 *���������������趨��ʱ��4�Ķ�ʱʱ��
 *��    �룺arr:�Զ���װ�ؼĴ������ڵ�ֵ����λΪus
 *��    ������
 *��    ��:	�ڶ�ʱ���ж��б�����
 *��    ע:	Ԥ��Ƶ��ֵ�趨Ϊ7199������������Ϊ0.1ms
 */
void TIM4_Set_Time(u16 arr)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    arr--;
  	TIM_TimeBaseStructure.TIM_Period = arr;           /*��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ*/	
	  TIM_TimeBaseStructure.TIM_Prescaler =7199;          /*����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ*/
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);   /*����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ*/			
}




/*��ʱ��3�жϷ������*/
void TIM3_IRQHandler(void)  
{	    
	static u8 i=1;
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  /*���TIM3�����жϷ������*/
		{
			flag_vpwm=1; 
			switch(i)	   
			{
					case 1:	  
						{
								PWM1=1;	 /*pwm1���*/ 					  
								TIM3_Set_Time(pwm[0]);
						}  break;
			
					case 2:
						{
							PWM1=0; /*pwm1���*/
							TIM3_Set_Time(2500-pwm[0]);
							flag_vpwm=1;
						}  break;
					case 3:
						{
							PWM2=1; /*pwm2���*/
							TIM3_Set_Time(pwm[1]);
						}  break;
					case 4:
						{
							PWM2=0; /*pwm2���*/
								TIM3_Set_Time(2500-pwm[1]);
							flag_vpwm=1;
						}	 break;
					case 5:
						{
							PWM3=1; /*pwm3���*/
							TIM3_Set_Time(pwm[2]);
						}  break;
					case 6:
						{
							PWM3=0; /*pwm3���*/
							TIM3_Set_Time(2500-pwm[2]);
							flag_vpwm=1;
						}	break;	  
					case 7:
						{
							PWM4=1; /*pwm4���*/
							TIM3_Set_Time(pwm[3]);
						}	break;	  
					case 8:
						{
							PWM4=0; /*pwm4���*/
							TIM3_Set_Time(2500-pwm[3]);
							flag_vpwm=1;
						}	break;	  
					case 9:
						{
							PWM5=1; /*pwm5���*/
							TIM3_Set_Time(pwm[4]);
						}	break;	  
					case 10:
						{
							PWM5=0; /*pwm5���*/
							TIM3_Set_Time(2500-pwm[4]);
							flag_vpwm=1;
						}	break;	  
					case 11:
						{
							PWM6=1; /*pwm6���*/
							TIM3_Set_Time(pwm[5]);
						}	break;	  
					case 12:
						{
							PWM6=0; /*pwm6���*/
							TIM3_Set_Time(2500-pwm[5]);
							flag_vpwm=1;
						}	break;	
			
					case 13:
						{
							PWM7=1; /*pwm6���*/
							TIM3_Set_Time(pwm[6]);
						}	break;	  
					case 14:
						{
							PWM7=0; /*pwm7���*/
							TIM3_Set_Time(2500-pwm[6]);
							flag_vpwm=1;
						}	break;	
			
					case 15:
						{
							PWM8=1; /*pwm8���*/
							TIM3_Set_Time(pwm[7]);
						}	break;	  
					case 16:
						{
							PWM8=0; /*pwm8���*/
							TIM3_Set_Time(2500-pwm[7]);
							flag_vpwm=1;
							i=0;
						}	break;	
					default:break;			
		    }	
			i++;
			TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  /*���TIM3�����жϱ�־�����д�����õ�λ�ú���Ҫ!!!!*/ 

		}	
		
}

/*��ʱ��4�жϷ������*/
void TIM4_IRQHandler(void)  
{  
	
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  /*���TIM3�����жϷ������*/
		{
				 switch(movement_tag)
				 {
							 case 'Z':
							 {
								  TIM_Cmd(TIM4, DISABLE);/*�رն�ʱ��4*/
								  movement_tag='A';
								  PicArray_ToBufferArray(secpic_position,1);
								  motor_speed=700;
							 }break;
							 
							 case 'A':
							 {
									TIM_Cmd(TIM4, DISABLE);
									movement_tag='B';
									PicArray_ToBufferArray(thirpic_position,4);//���
								  motor_speed=250;
							 }break;
						 
							 case 'B':
							 {
									TIM_Cmd(TIM4, DISABLE);
									movement_tag='C';								 
									PicArray_ToBufferArray(fourpic_position,1);//���
									motor_speed=700;								 
							 }break;
						 
							 case 'C':
							 {
									TIM_Cmd(TIM4, DISABLE);
									movement_tag='D';								 
									PicArray_ToBufferArray(fifpic_position,4);
								  motor_speed=250;
							 }break;
						 
							 case 'D':
							 {
									TIM_Cmd(TIM4, DISABLE);
									movement_tag='E';								 
									PicArray_ToBufferArray(sixpic_position,1);
								  motor_speed=700;
							 }break;
						 
							 case 'E':
							 { 
									TIM_Cmd(TIM4, DISABLE);                       /*ע����������ʱ����ܻ��ͻ*/
               		movement_tag='Q';								    					/*�ع��ʼֵ�����������ѭ��*/			
                 // USART_SendChar('7');	                      /*֪ͨ��λ��ͼƬʶ��*/
	                PicArray_ToBufferArray(retuinit_position,4);	/*���س�ʼλ��*/	
									motor_speed=250;		
									send_start_flag=1;
							 }break;
							 
							 default:
							 {
								 
							 }break;
				 
				  }
				 
			change();		
			TIM_Cmd(TIM3, ENABLE);                      /*��TIM3*/				
			TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  /*���TIMx�����жϱ�־�����д�����õ�λ�ú���Ҫ!!!!*/ 

		}

}














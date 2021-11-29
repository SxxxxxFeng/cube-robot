#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "exti.h"
#include "usart.h"
#include "timer.h"
#include "motor.h"
#include "movement.h"
#include "instruction.h"

/*
 *��ע��ʹ��оƬ�ͺ�Ϊ:STM32F103RCT6��ʹ��KEIL4
 */
	
 int main(void)
 {
	 
 	SystemInit(); 			   //ϵͳʱ�ӳ�ʼ��Ϊ72M	  SYSCLK_FREQ_72MHz
	delay_init(72);	    	 //��ʱ������ʼ��	  
	NVIC_Configuration();  //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
 	LED_Init();			       //LED�˿ڳ�ʼ��
	Exti_Init();
  USART1_Config();
	 
	MotorPin_Init();    
	Init_TotalArray();   
  TIM3_Int_Init(100,100);	   //��ʹ��ʱ�俪��3��û��ʹ�ܶ�ʱ��3�ж�
	TIM4_Int_Init(29999,7199);  //��ʹ��ʱ�俪��4����ʱ3S


		while(1)
		{                                               
						
				if(rece_flag==1)//���յ�ָ���ǰһ�������Ѿ����
				{
							TIM_Cmd(TIM3, DISABLE);       
			   
						  if(rece_string[0]=='#')
							{
										motor_speed=250;  
										lines_num=Analy_UsartString();
										end_flag=0;
							}
	
							
							
							//���ԣ����ڷ���ʾ��Ϊ��@1395%1440%1435%1500%1500%1500%1500%1500!
							else if(rece_string[0]=='@')
							{	
										motor_speed=250;  
										Analy_String(rece_string);
								    lines_num=1;									

					    }
							
							
							//���ڷ���ʾ����%1!,%2!,%3����%4����%5����%6��
							else if(rece_string[0]=='%')//���ԣ�����ħ���泯������ʱ��״̬������鿴��ʱ�Ķ���Ƕ�
							{
								
										motor_speed=250;  
								    Debug_Photo_Angle(rece_string);//�Ѿ��޸�ȫ�ֱ���lines_num

							}
					
				change();		
				TIM_Cmd(TIM3, ENABLE);   
				TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�	
				rece_flag=0;

        }	 
				
				if(flag_vpwm==1)	  
				{
						vpwm();				             
						flag_vpwm=0;
				}	
				
				if(send_start_flag==1&&end_flag==1)
				{
					 USART_SendChar('7');//����7����APPͨ��������������ָ��	
					 send_start_flag=0;
					 end_flag=0;
					
				}


    }

}



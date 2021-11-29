/*  �ļ�����movement.c
 *�ļ���������ħ����������Ҫ��ɵĶ�������������Ӧ�������С�
 *��    ע: �ڼ�������������ʱ,��ס�õ����Ǿ���λ�ã����������λ��
 */
#include "stm32f10x.h"
#include "movement.h"
#include "motor.h"
#include "usart.h"
#include "instruction.h"

static const u16 original_position[4]={1400,1400,1400,1400};          /*��ʼλ��1,2,3,4�Ŷ���ĽǶ�*/ 
static const u16 clockwise90_position[4]={430,480,450,45 0};           /*�ɳ�ʼλ��˳ʱ��ת��90��ʱ1,2,3,4�Ŷ���ĽǶ�*/
static const u16 anticlockwise90_position[4]={2400,2400,2400,2400};   /*�ɳ�ʼλ����ʱ��ת��90��ʱ1,2,3,4�Ŷ���ĽǶ�*/

static const u16 clockwise180_position[4]={2270,2310,2340,2340};    

static const u16 wrasp_position[4]={2020,1680,1680,1700};             /*��е��ץ��ħ��ʱ5,6,7,8�Ŷ���ĽǶ�*/
static const u16 loosen_position[4]={1400,1050,1060,1070};            /*��е���ɿ�ħ��ʱ5,6,7,8�Ŷ���ĽǶ�*/

//

/*
 *���յ�˳��Ϊ���ҡ�ǰ�����ϡ���
 *SD����ͼƬ��ź�ħ����λ�Ķ�Ӧ��ϵ�ǣ�1����2���ң�3��ǰ��4����5���ϣ�6����
 *��ɫʶ����֮��ħ����״̬�����˱仯��ʶ���
 *��ǰ���ҡ����¡���
 *
 */

u16 lines_num=0;//������б��
u16 initial_position[8];//��е��ץ��ħ��
u16 firpic_position[5][8];//���յ�1����Ƭ
u16 secpic_position[2][8];//���յ�2����Ƭ
u16 thirpic_position[5][8];//���յ�3����Ƭ
u16 fourpic_position[2][8];//���յ�4����Ƭ
u16 fifpic_position[5][8];//���յ�5����Ƭ
u16 sixpic_position[2][8];//���յ�6����Ƭ
u16 retuinit_position[5][8];//��������Ƭ���س�ʼ��λ�ã�����е��ץ��ħ����λ��




u16 r_clockwise90[4][8]; /*�Ҳ�˳ʱ��90��*/
u16 l_clockwise90[4][8]; /*���˳ʱ��90��*/
u16 f_clockwise90[4][8]; /*ǰ��˳ʱ��90��*/
u16 b_clockwise90[4][8]; /*���˳ʱ��90��*/

u16 r_anticlockwise90[4][8]; /*�Ҳ���ʱ��90��*/
u16 l_anticlockwise90[4][8]; /*�����ʱ��90��*/
u16 f_anticlockwise90[4][8]; /*ǰ����ʱ��90��*/
u16 b_anticlockwise90[4][8]; /*�����ʱ��90��*/

u16 r_clockwise180[4][8]; /*�Ҳ�180��*/
u16 l_clockwise180[4][8]; /*���180��*/
u16 f_clockwise180[4][8]; /*ǰ��180��*/
u16 b_clockwise180[4][8]; /*���180��*/

u16 overturnto_f[6][8]; /*��ǰ��ת90��*/
u16 overturnto_b[6][8]; /*���ת90��*/
u16 overturnto_l[6][8]; /*����ת90��*/
u16 overturnto_r[6][8]; /*���ҷ�ת90��*/



/*  ��������Init_TotalArray()
 *�������ܣ���������Ҫ�ڳ�����һ��ʼ����ĺ������ڴ˺����У��Գ�ʼ������
 *��    �룺��
 *��    ������
 *��    ע��ע����ú�����˳��,��������Ҫ530us
 */
void Init_TotalArray(void)
{
	Calcul_InitPosition(); /*�˺������ȱ���ʼ������Ϊ���ɵ�����ᱻ�Ժ�ܶຯ���õ�*/
	Calcul_FirPicPosition();
  Calcul_SecPicPosition();
  Calcul_ThirPicPosition();
  Calcul_FourPicPosition();
  Calcul_FifPicPosition();
  Calcul_SixPicPosition();
  RetuIni_AftPic();
	
	Calcul_Lclockwise90();
	Calcul_Rclockwise90();
	Calcul_Fclockwise90();
	Calcul_Bclockwise90();
	
	Calcul_Lanticlockwise90();
	Calcul_Ranticlockwise90();
	Calcul_Fanticlockwise90();
	Calcul_Banticlockwise90();
	
	Calcul_Lclockwise180();
	Calcul_Rclockwise180();
	Calcul_Fclockwise180();
	Calcul_Bclockwise180();
	
	Calcul_OverturntoB();//���ת
	Calcul_OverturntoF();//��ǰ��ת
	Calcul_OverturntoL();//����ת
	Calcul_OverturntoR();//���ҷ�ת

}


/*
 *  ��������Calcul_IniticalPosition()
 *��������: �������ĳ�ʼ�Ƕ�
 *��    ��: ��
 *��    ��: ��
 */
void Calcul_InitPosition(void)
{
	u8 i,j;
	for(i=0;i<4;i++)
	{
		initial_position[i]=original_position[i];
	}
	for(i=4,j=0;i<8;i++,j++)
	{
		initial_position[i]=wrasp_position[j];
	}
	
}


/*  ��������Init_MotorMovement()
 *�������ܣ�5,6,7,8�ɿ�ħ�������ò����߰�ħ�������е����
 *��    �룺��
 *��    ������
 *��    ע��
 */
void Init_MotorMovement(void)
{
		u8 i;
		
		for(i=0;i<8;i++)
			{
			
				pos[0][i]=initial_position[i];/*��ʼλ��*/
			}
			
		for(i=0;i<4;i++)
			{
			
				pos[1][i]=initial_position[i];
			}
			
		for(i=0;i<4;i++)
			{
			
				pos[1][i+4]=loosen_position[i];/*5,6,7,8�ɿ�ħ�������ò����߰�ħ�������е����*/
			}
			
			lines_num=1;
	}


/*  ������:Calcul_FirPicPosition()
 *��������:�������յ�һ����Ķ����������(��ץ��ħ��)
 *��    ע:������=�����-1,��������
 */
void Calcul_FirPicPosition(void)
{
	u8 i;
	
	for(i=0;i<8;i++)                     
	{
	  firpic_position[0][i]=initial_position[i];//��ִ��
	}

	for(i=0;i<8;i++)                    
	{
	  firpic_position[1][i]=initial_position[i];/*��һ�зų�ʼλ��*/
	}
	firpic_position[1][5]=loosen_position[1];/*6�ɿ�*/
	firpic_position[1][7]=loosen_position[3];/*8�ɿ�*/	

	
	for(i=0;i<8;i++)                    
	{
	  firpic_position[2][i]=initial_position[i];  /*5,6,7,8��ץ�����ص���ʼλ��*/
	}
	
	
	/*���������ջ���*/
	
	for(i=0;i<8;i++)                    
	{
		firpic_position[3][i]=firpic_position[2][i]; /*��������ʱ���Ƶڶ���*/
	}
	firpic_position[3][5]=loosen_position[1];            /*6����*/
	firpic_position[3][7]=loosen_position[3];            /*8����*/

 	for(i=0;i<8;i++)                    
	{
		firpic_position[4][i]=firpic_position[3][i]; /*��������ʱ���Ƶ�����*/
	}
	
	  /*��������*/                        
		firpic_position[4][0]=clockwise90_position[0];      /*1˳ʱ��90��,*/
		firpic_position[4][2]=anticlockwise90_position[2];  /*3��ʱ��90��*/           
}



/*  ������:Calcul_SecPicPosition()
 *��������:�������յڶ�����Ķ����������
 *��    ע:������=�����-1,������Calcul_FirPicPosition()��������������
 */
void Calcul_SecPicPosition(void)
{
	u8 i;
	
 	for(i=0;i<8;i++)                     
	{
		secpic_position[0][i]=firpic_position[4][i];
	}
	
 	for(i=0;i<8;i++)                     
	{
		secpic_position[1][i]=secpic_position[0][i];
	}
	
                      /*��������*/   
 secpic_position[1][0]=anticlockwise90_position[0];        /*1��ʱ��90��*/ 
 secpic_position[1][2]=clockwise90_position[2];	          /*3˳ʱ��90��*/ 

	
}


/*  ������:Calcul_ThirPicPosition()
 *��������:�������յ�������Ķ����������
 *��    ע:������=�����-1������ǰ��
 */
void Calcul_ThirPicPosition(void)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		thirpic_position[0][i]=secpic_position[1][i];
	}
	
	for(i=0;i<8;i++)
	{
		thirpic_position[1][i]=thirpic_position[0][i];
	}
	
	thirpic_position[1][5]=wrasp_position[1];  /*6ǰ��*/
	thirpic_position[1][7]=wrasp_position[3];  /*8ǰ��*/
	
	for(i=0;i<8;i++)
	{
		thirpic_position[2][i]=thirpic_position[1][i];  
	}
	
	thirpic_position[2][4]=loosen_position[0];   /*5����*/
	thirpic_position[2][6]=loosen_position[2];   /*7����*/
	
	for(i=0;i<8;i++)
	{
	  thirpic_position[3][i]=thirpic_position[2][i];  
	}
	
	thirpic_position[3][0]=original_position[0]; /*1�ص���ʼλ��*/
	thirpic_position[3][2]=original_position[2]; /*3�ص���ʼλ��*/
	
		/*����ǰ��*/
	for(i=0;i<8;i++)
	{
	  thirpic_position[4][i]=thirpic_position[3][i];  /*���Ƶ�һ�е��ڶ���*/
	}
	
	thirpic_position[4][1]=clockwise90_position[1];                 /*2˳ʱ�룬Сһ��*/
	thirpic_position[4][3]=anticlockwise90_position[3];             /*4��ʱ�룬��һ��*/

}


/*  ������:Calcul_FourPicPosition()
 *��������:�������յ��ĸ���Ķ����������
 *��    ע:������=�����-1�����պ���
 */
void Calcul_FourPicPosition(void)
{
	 u8 i;
	
	for(i=0;i<8;i++)
		{
			fourpic_position[0][i]=thirpic_position[4][i];   
		}
		
	for(i=0;i<8;i++)
		{
			fourpic_position[1][i]=fourpic_position[0][i];   
		}
		
	/*���պ���*/
	fourpic_position[1][1]=anticlockwise90_position[1];  /*2��ʱ��*/
	fourpic_position[1][3]=clockwise90_position[3];      /*4˳ʱ��*/
			
}

/*  ������:Calcul_FifPicPosition()
 *��������:�������յ������Ķ����������
 *��    ע:������=�����-1,��������
 */
void Calcul_FifPicPosition(void)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		fifpic_position[0][i]=fourpic_position[1][i];
	}
	
	for(i=0;i<8;i++)
	{
		fifpic_position[1][i]=fifpic_position[0][i];
	}
	
  fifpic_position[1][4]=wrasp_position[0];		/*5ǰ��*/
  fifpic_position[1][6]=wrasp_position[2];		/*7ǰ��*/	

	for(i=0;i<8;i++)
	{
		fifpic_position[2][i]=fifpic_position[1][i];
	}
	
	fifpic_position[2][5]=loosen_position[1];   /*6����*/
	fifpic_position[2][7]=loosen_position[3];   /*8����*/
	
	
	for(i=0;i<8;i++)
	{
		fifpic_position[3][i]=fifpic_position[2][i];
	}
	
	fifpic_position[3][1]=original_position[1];	/*2�ص���ʼλ��*/
	fifpic_position[3][3]=original_position[3];	/*4�ص���ʼλ��*/
		
	/*��������*/
	for(i=0;i<8;i++)
	{
		fifpic_position[4][i]=fifpic_position[3][i];
	}
	
	fifpic_position[4][0]=clockwise90_position[0];      /*1˳ʱ��*/
	fifpic_position[4][2]=anticlockwise90_position[2];  /*3��ʱ��*/
	
}


/*  ������:Calcul_SixPicPosition()
 *��������:�������յ�������Ķ����������
 *��    ע:������=�����-1���������� 
 */
void Calcul_SixPicPosition(void)
{
			u8 i;
			for(i=0;i<8;i++)
			{
				sixpic_position[0][i]=fifpic_position[4][i];
			}
			
			for(i=0;i<8;i++)
			{
				sixpic_position[1][i]=sixpic_position[0][i];
			}
			
			/*��������*/
			sixpic_position[1][0]=anticlockwise90_position[0];/*1��ʱ��*/
			sixpic_position[1][2]=clockwise90_position[2];    /*3˳ʱ��*/
			
			
}


/*  ������: RetuIni_AftPic()
 *��������: ��������6����֮��ص���ʼλ��
 *��    ע������ԭ·����·�����أ����Һ�ԭ���İڷ�λ����ͬ��ʵ����ֻ��Ҫ����
 */
void RetuIni_AftPic(void)
{
				u8 i;
			
			for(i=0;i<8;i++)
			{
				retuinit_position[0][i]=sixpic_position[1][i];
			}	
			
			for(i=0;i<8;i++)
			{
				retuinit_position[1][i]=retuinit_position[0][i];
			}	
				
			retuinit_position[1][5]=wrasp_position[1];  /*6ǰ��*/
			retuinit_position[1][7]=wrasp_position[3];  /*8ǰ��*/
			
			for(i=0;i<8;i++)
			{
				retuinit_position[2][i]=retuinit_position[1][i];
			}
			retuinit_position[2][4]=loosen_position[0];  /*5����*/
			retuinit_position[2][6]=loosen_position[2];  /*7����*/
			
			for(i=0;i<8;i++)
			{
				retuinit_position[3][i]=retuinit_position[2][i];
			}
			
			retuinit_position[3][0]=original_position[0]; /*1�ص���ʼλ��*/ 
			retuinit_position[3][2]=original_position[2]; /*3�ص���ʼλ��*/ 
				
			for(i=0;i<8;i++)
			{
				retuinit_position[4][i]=retuinit_position[3][i];
			}
			
			retuinit_position[4][4]=wrasp_position[0];    /*5ǰ��*/
			retuinit_position[4][6]=wrasp_position[2];    /*7ǰ��*/	

}


/*  ��������PicArray_ToBufferArray(u(*array)[8],line_num)
 *�������ܣ���������Ҫִ�е�������뻺�����飬��������Ҫִ��������б�
 *��    �룺��ά����ָ��(*array)[8]�������б�line_num
 *��    ���������б�
 *��    ע���ڰ����������뻺������ʱ��Ҫ�ص���ʱ������ֹ�岹�����иı�ȫ�ֱ���lines_num������������ȫ������
 */
void PicArray_ToBufferArray(u16 (*array)[8],u16 arrayline_num)
{
		u8 i,j;
	
	  lines_num=arrayline_num;

		for(j=0;j<=lines_num;j++)
		{
			
			for(i=0;i<8;i++)
			{
				pos[j][i]=*(*(array+j)+i);			
			}
		
		}
	
}

/*  ��������Calcul_Lclockwise90()
 *�������ܣ�����ħ������˳ʱ����ת90�ȵĶ��ִ������
 *��    �룺��
 *��    ������
 *�����������ʼ������
 *��    ע���ȵ���initial_position()����
 */
void Calcul_Lclockwise90(void)
{
	
	u8 i;
	
	for(i=0;i<8;i++)
	{
		l_clockwise90[0][i]=initial_position[i];
	}
	
	l_clockwise90[0][1]=anticlockwise90_position[1];/*2��ʱ��*/
	
	for(i=0;i<8;i++)
	{
		l_clockwise90[1][i]=l_clockwise90[0][i];
	}
	
	l_clockwise90[1][5]=loosen_position[1]; /*6����*/
	
	for(i=0;i<8;i++)
	{
		l_clockwise90[2][i]=l_clockwise90[1][i];
	}
	
	l_clockwise90[2][1]=original_position[1];/*2�ص���ʼλ��*/
	
	for(i=0;i<8;i++)
	{
		l_clockwise90[3][i]=l_clockwise90[2][i];
	}
	
	l_clockwise90[3][5]=wrasp_position[1]; /*6ǰ��*/
	
	
}


/*  ��������Calcul_Lanticlockwise90()
 *�������ܣ�����ħ��������ʱ����ת90�ȵĶ��ִ������
 *��    �룺��
 *��    ������
 *�����������ʼ������
 *��    ע���ȵ���initial_position()����
 */
void Calcul_Lanticlockwise90(void)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		l_anticlockwise90[0][i]=initial_position[i];
	}
	
	l_anticlockwise90[0][1]=clockwise90_position[1];/*2˳ʱ��*/
	
	for(i=0;i<8;i++)
	{
		l_anticlockwise90[1][i]=l_anticlockwise90[0][i];
	}
	
	l_anticlockwise90[1][5]=loosen_position[1]; /*6����*/
	
	for(i=0;i<8;i++)
	{
		l_anticlockwise90[2][i]=l_anticlockwise90[1][i];
	}
	
	l_anticlockwise90[2][1]=original_position[1];/*2�ص���ʼλ��*/
	
	for(i=0;i<8;i++)
	{
		l_anticlockwise90[3][i]=l_anticlockwise90[2][i];
	}
	
	l_anticlockwise90[3][5]=wrasp_position[1]; /*6ǰ��*/
	

}


/*  ��������Calcul_Clockwise180()
 *�������ܣ�����ħ��������ת180�ȵĶ��ִ������
 *��    �룺��
 *��    ������
 *�����������ʼ������
 *��    ע���ȵ���initial_position()����
 */
void Calcul_Lclockwise180(void)
{
	
	u8 i;
	
	for(i=0;i<8;i++)
	{
		l_clockwise180[0][i]=initial_position[i];
	}
	
	l_clockwise180[0][1]=clockwise180_position[1];/*2��ת180��*/
	
	for(i=0;i<8;i++)
	{
		l_clockwise180[1][i]=l_clockwise180[0][i];
	}
	
	l_clockwise90[1][5]=loosen_position[1]; /*6����*/
	
	for(i=0;i<8;i++)
	{
		l_clockwise180[2][i]=l_clockwise180[1][i];
	}
	
	l_clockwise180[2][1]=original_position[1];/*2�ص���ʼλ��*/
	
	for(i=0;i<8;i++)
	{
		l_clockwise180[3][i]=l_clockwise180[2][i];
	}
	
	l_clockwise180[3][5]=wrasp_position[1]; /*6ǰ��*/
	
}

/*  ��������Calcul_OverturntoL(void)
 *�������ܣ�����ת90��
 *��    �룺��
 *��    ������
 *�����������	Init_TotalArray()���ã��ڳ����г�ʼ��
 *��    ע��
 */
void Calcul_OverturntoL(void)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		overturnto_l[0][i]=initial_position[i];
	}
	
	overturnto_l[0][5]=loosen_position[1];/*6����*/
	overturnto_l[0][7]=loosen_position[3];/*8����*/
	
	for(i=0;i<8;i++)
	{
		overturnto_l[1][i]=overturnto_l[0][i];
	}
	
	overturnto_l[1][0]=anticlockwise90_position[0];       /*1��ʱ��*/
	overturnto_l[1][2]=clockwise90_position[2];           /*3˳ʱ�룬�������ת�Ķ���*/
	
	for(i=0;i<8;i++)
	{
		overturnto_l[2][i]=overturnto_l[1][i];
	}
	
	overturnto_l[2][5]=wrasp_position[1]; /*6ǰ��*/
	overturnto_l[2][7]=wrasp_position[3]; /*8ǰ��*/
	
	for(i=0;i<8;i++)
	{
		overturnto_l[3][i]=overturnto_l[2][i];
	}
	overturnto_l[3][4]=loosen_position[0]; /*5����*/
	overturnto_l[3][6]=loosen_position[2]; /*7����*/


	for(i=0;i<8;i++)
	{
		overturnto_l[4][i]=overturnto_l[3][i];
	}
	
	 overturnto_l[4][0]=original_position[0];/*1�ص���ʼλ��*/
	 overturnto_l[4][2]=original_position[2];/*3�ص���ʼλ��*/
	
	for(i=0;i<8;i++)
	{
		overturnto_l[5][i]=overturnto_l[4][i];
	}
	
	overturnto_l[5][4]=wrasp_position[0]; /*5ǰ��*/
	overturnto_l[5][6]=wrasp_position[2]; /*7ǰ������ʱ�����ʼλ��*/
}


/*  ��������Calcul_Rclockwise90()
 *�������ܣ�����ħ������˳ʱ����ת90�ȵĶ��ִ������
 *��    �룺��
 *��    ������
 *�����������ʼ������
 *��    ע���ȵ���initial_position()����
 */
void Calcul_Rclockwise90(void)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		r_clockwise90[0][i]=initial_position[i];
	}
	
	r_clockwise90[0][3]=anticlockwise90_position[3];/*4��ʱ��*/
	
	for(i=0;i<8;i++)
	{
		r_clockwise90[1][i]=r_clockwise90[0][i];
	}
	
	r_clockwise90[1][7]=loosen_position[3]; /*8����*/
	
	for(i=0;i<8;i++)
	{
		r_clockwise90[2][i]=r_clockwise90[1][i];
	}
	
	r_clockwise90[2][3]=original_position[3];/*4�ص���ʼλ��*/
	
	for(i=0;i<8;i++)
	{
		r_clockwise90[3][i]=r_clockwise90[2][i];
	}
	
	r_clockwise90[3][7]=wrasp_position[3]; /*8ǰ��*/
	
}



/*  ��������Calcul_Ranticlockwise90()
 *�������ܣ�����ħ��������ʱ����ת90�ȵĶ��ִ������
 *��    �룺��
 *��    ������
 *�����������ʼ������
 *��    ע���ȵ���initial_position()����
 */
void Calcul_Ranticlockwise90(void)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		r_anticlockwise90[0][i]=initial_position[i];
	}
	
    r_anticlockwise90[0][3]=clockwise90_position[3];/*4˳ʱ��*/
	
	for(i=0;i<8;i++)
	{
		r_anticlockwise90[1][i]=r_anticlockwise90[0][i];
	}
	
	r_anticlockwise90[1][7]=loosen_position[3]; /*8����*/
	
	for(i=0;i<8;i++)
	{
		r_anticlockwise90[2][i]=r_anticlockwise90[1][i];
	}
	
	r_anticlockwise90[2][3]=original_position[3];/*4�ص���ʼλ��*/
	
	for(i=0;i<8;i++)
	{
		r_anticlockwise90[3][i]=r_anticlockwise90[2][i];
	}
	
	r_anticlockwise90[3][7]=wrasp_position[3]; /*8ǰ��*/
	
}


/*  ��������Calcul_Rclockwise180()
 *�������ܣ�����ħ��������ת180�ȵĶ��ִ������
 *��    �룺��
 *��    ������
 *�����������ʼ������
 *��    ע���ȵ���initial_position()����
 */
void Calcul_Rclockwise180(void)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		r_clockwise180[0][i]=initial_position[i];
	}
	
	r_clockwise180[0][3]=clockwise180_position[3];/*4��ʱ��*/
	
	for(i=0;i<8;i++)
	{
		r_clockwise180[1][i]=r_clockwise180[0][i];
	}
	
	r_clockwise180[1][7]=loosen_position[3]; /*8����*/
	
	for(i=0;i<8;i++)
	{
		r_clockwise180[2][i]=r_clockwise180[1][i];
	}
	
	r_clockwise180[2][3]=original_position[3];/*4�ص���ʼλ��*/
	
	for(i=0;i<8;i++)
	{
		r_clockwise180[3][i]=r_clockwise180[2][i];
	}
	
	r_clockwise180[3][7]=wrasp_position[3]; /*8ǰ��*/
	
}


/*  ��������Calcul_OverturntoR(void)
 *�������ܣ����ҷ�ת90��
 *��    �룺��
 *��    ������
 *�����������	Init_TotalArray()���ã��ڳ����г�ʼ��
 *��    ע��
 */
void Calcul_OverturntoR(void)
{
		u8 i;
	
	for(i=0;i<8;i++)
	{
		overturnto_r[0][i]=initial_position[i];
	}
	
	overturnto_r[0][5]=loosen_position[1]; /*6����*/
	overturnto_r[0][7]=loosen_position[3]; /*8����*/
	
	for(i=0;i<8;i++)
	{
		overturnto_r[1][i]=overturnto_r[0][i];
	}
	
	overturnto_r[1][0]=clockwise90_position[0];       /*1˳ʱ��*/
	overturnto_r[1][2]=anticlockwise90_position[2];   /*3��ʱ��*/
	
	for(i=0;i<8;i++)
	{
		overturnto_r[2][i]=overturnto_r[1][i];
	}
	
	overturnto_r[2][5]=wrasp_position[1]; /*6ǰ��*/
	overturnto_r[2][7]=wrasp_position[3]; /*8ǰ��*/
	
	for(i=0;i<8;i++)
	{
		overturnto_r[3][i]=overturnto_r[2][i];
	}
	overturnto_r[3][4]=loosen_position[0]; /*5����*/
	overturnto_r[3][6]=loosen_position[2]; /*7����*/


	for(i=0;i<8;i++)
	{
		overturnto_r[4][i]=overturnto_r[3][i];
	}
	
	 overturnto_r[4][0]=original_position[0];/*1�ص���ʼλ��*/
	 overturnto_r[4][2]=original_position[2];/*3�ص���ʼλ��*/
	
	for(i=0;i<8;i++)
	{
		overturnto_r[5][i]=overturnto_r[4][i];
	}
	
	overturnto_r[5][4]=wrasp_position[0]; /*5ǰ��*/
	overturnto_r[5][6]=wrasp_position[2]; /*7ǰ������ʱ�����ʼλ��*/
		
}

/*  ��������Calcul_Fclockwise90()
 *�������ܣ�����ħ��ǰ��˳ʱ����ת90�ȵĶ��ִ������
 *��    �룺��
 *��    ������
 *�����������ʼ������
 *��    ע��
 */
void Calcul_Fclockwise90(void)
{
    u8 i;
	
	for(i=0;i<8;i++)
	{
		f_clockwise90[0][i]=initial_position[i];
	}
	
	f_clockwise90[0][2]=anticlockwise90_position[2];/*3��ʱ��*/
	
	for(i=0;i<8;i++)
	{
		f_clockwise90[1][i]=f_clockwise90[0][i];
	}
	
	f_clockwise90[1][6]=loosen_position[2]; /*7����*/
	
	for(i=0;i<8;i++)
	{
		f_clockwise90[2][i]=f_clockwise90[1][i];
	}
	
	f_clockwise90[2][2]=original_position[2];/*3�ص���ʼλ��*/
	
	for(i=0;i<8;i++)
	{
		f_clockwise90[3][i]=f_clockwise90[2][i];
	}
	
	f_clockwise90[3][6]=wrasp_position[2]; /*7ǰ��*/
	
}


/*  ��������Calcul_Fanticlockwise90()
 *�������ܣ�����ħ��ǰ����ʱ����ת90�ȵĶ��ִ������
 *��    �룺��
 *��    ������
 *�����������ʼ������
 *��    ע��
 */
void Calcul_Fanticlockwise90(void)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		f_anticlockwise90[0][i]=initial_position[i];
	}
	
	f_anticlockwise90[0][2]=clockwise90_position[2];/*3˳ʱ��*/
	
	for(i=0;i<8;i++)
	{
		f_anticlockwise90[1][i]=f_anticlockwise90[0][i];
	}
	
	f_anticlockwise90[1][6]=loosen_position[2]; /*7����*/
	
	for(i=0;i<8;i++)
	{
		f_anticlockwise90[2][i]=f_anticlockwise90[1][i];
	}
	
	f_anticlockwise90[2][2]=original_position[2];/*3�ص���ʼλ��*/
	
	for(i=0;i<8;i++)
	{
		f_anticlockwise90[3][i]=f_anticlockwise90[2][i];
	}
	
	f_anticlockwise90[3][6]=wrasp_position[2]; /*7ǰ��*/
	
}


/*  ��������Calcul_Fclockwise180()
 *�������ܣ�����ħ��ǰ����ת180�ȵĶ��ִ������
 *��    �룺��
 *��    ������
 *�����������ʼ������
 *��    ע��
 */
void Calcul_Fclockwise180(void)
{
    u8 i;
	
	for(i=0;i<8;i++)
	{
		f_clockwise180[0][i]=initial_position[i];
	}
	
	f_clockwise180[0][2]=clockwise180_position[2];/*3��ʱ��*/
	
	for(i=0;i<8;i++)
	{
		f_clockwise180[1][i]=f_clockwise180[0][i];
	}
	
	f_clockwise180[1][6]=loosen_position[2]; /*7����*/
	
	for(i=0;i<8;i++)
	{
		f_clockwise180[2][i]=f_clockwise180[1][i];
	}
	
	f_clockwise180[2][2]=original_position[2];/*3�ص���ʼλ��*/
	
	for(i=0;i<8;i++)
	{
		f_clockwise180[3][i]=f_clockwise180[2][i];
	}
	
	f_clockwise180[3][6]=wrasp_position[2]; /*7ǰ��*/
	
}


/*  ��������Calcul_OverturntoF(void)
 *�������ܣ���ǰ��ת90��
 *��    �룺��
 *��    ������
 *�����������	Init_TotalArray()���ã��ڳ����г�ʼ��
 *��    ע��
 */
void Calcul_OverturntoF(void)
{
	
	u8 i;
	
	for(i=0;i<8;i++)
	{
		overturnto_f[0][i]=initial_position[i];
	}
	
	overturnto_f[0][4]=loosen_position[0];/*5����*/
	overturnto_f[0][6]=loosen_position[2];/*7����*/
	
	for(i=0;i<8;i++)
	{
		overturnto_f[1][i]=overturnto_f[0][i];
	}
	
	overturnto_f[1][1]=anticlockwise90_position[1];       /*2��ʱ��*/
	overturnto_f[1][3]=clockwise90_position[3];           /*4˳ʱ�룬�����ǰ��ת�Ķ���*/
	
	for(i=0;i<8;i++)
	{
		overturnto_f[2][i]=overturnto_f[1][i];
	}
	
	overturnto_f[2][4]=wrasp_position[0]; /*5ǰ��*/
	overturnto_f[2][6]=wrasp_position[2]; /*7ǰ��*/
	
	for(i=0;i<8;i++)
	{
		overturnto_f[3][i]=overturnto_f[2][i];
	}
	overturnto_f[3][5]=loosen_position[1]; /*6����*/
	overturnto_f[3][7]=loosen_position[3]; /*8����*/


	for(i=0;i<8;i++)
	{
		overturnto_f[4][i]=overturnto_f[3][i];
	}
	
	 overturnto_f[4][1]=original_position[1];/*2�ص���ʼλ��*/
	 overturnto_f[4][3]=original_position[3];/*4�ص���ʼλ��*/
	
	for(i=0;i<8;i++)
	{
		overturnto_f[5][i]=overturnto_f[4][i];
	}
	
	overturnto_f[5][5]=wrasp_position[1]; /*6ǰ��*/
	overturnto_f[5][7]=wrasp_position[3]; /*8ǰ������ʱ�����ʼλ��*/
   
	
}


/*  ��������Calcul_	Bclockwise90()
 *�������ܣ�����ħ������˳ʱ����ת90�ȵĶ��ִ������
 *��    �룺��
 *��    ������
 *�����������ʼ������
 *��    ע��
 */
void Calcul_Bclockwise90(void)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		b_clockwise90[0][i]=initial_position[i];
	}
	
	b_clockwise90[0][0]=anticlockwise90_position[0];/*1��ʱ��*/
	
	for(i=0;i<8;i++)
	{
		b_clockwise90[1][i]=b_clockwise90[0][i];
	}
	
	b_clockwise90[1][4]=loosen_position[0]; /*5����*/
	
	for(i=0;i<8;i++)
	{
		b_clockwise90[2][i]=b_clockwise90[1][i];
	}
	
	b_clockwise90[2][0]=original_position[0];/*1�ص���ʼλ��*/
	
	for(i=0;i<8;i++)
	{
		b_clockwise90[3][i]=b_clockwise90[2][i];
	}
	
	b_clockwise90[3][4]=wrasp_position[0]; /*5ǰ��*/
	

}

/*  ��������Calcul_	Bantianticlockwise90()
 *�������ܣ�����ħ��������ʱ����ת90�ȵĶ��ִ������
 *��    �룺��
 *��    ������
 *�����������ʼ������
 *��    ע��
 */
void Calcul_Banticlockwise90(void)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		b_anticlockwise90[0][i]=initial_position[i];
	}
	
	b_anticlockwise90[0][0]=clockwise90_position[0];    /*1˳ʱ��*/
	
	for(i=0;i<8;i++)
	{
		b_anticlockwise90[1][i]=b_anticlockwise90[0][i];
	}
	
	b_anticlockwise90[1][4]=loosen_position[0];         /*5����*/
	
	for(i=0;i<8;i++)
	{
		b_anticlockwise90[2][i]=b_anticlockwise90[1][i];
	}
	
	b_anticlockwise90[2][0]=original_position[0];      /*1�ص���ʼλ��*/
	
	for(i=0;i<8;i++)
	{
		b_anticlockwise90[3][i]=b_anticlockwise90[2][i];
	}
	
	b_anticlockwise90[3][4]=wrasp_position[0];         /*5ǰ��*/
	
}

/*  ��������Calcul_	Bclockwise180()
 *�������ܣ�����ħ������˳ʱ����ת180�ȵĶ��ִ������
 *��    �룺��
 *��    ������
 *�����������ʼ������
 *��    ע��
 */
void Calcul_Bclockwise180(void)
{
	
		u8 i;
		
		for(i=0;i<8;i++)
		{
			b_clockwise180[0][i]=initial_position[i];
		}
		
		b_clockwise180[0][0]=clockwise180_position[0];/*1��ʱ��*/
		
		for(i=0;i<8;i++)
		{
			b_clockwise180[1][i]=b_clockwise180[0][i];
		}
		
		b_clockwise180[1][4]=loosen_position[0]; /*5����*/
		
		for(i=0;i<8;i++)
		{
			b_clockwise180[2][i]=b_clockwise180[1][i];
		}
		
		b_clockwise180[2][0]=original_position[0];/*1�ص���ʼλ��*/
		
		for(i=0;i<8;i++)
		{
			b_clockwise180[3][i]=b_clockwise180[2][i];
		}
		
		b_clockwise180[3][4]=wrasp_position[0]; /*5ǰ��*/
		
}


/*  ��������Calcul_OverturntoB()
 *�������ܣ����ת90��
 *��    �룺��
 *��    ������
 *�����������	Init_TotalArray()���ã��ڳ����г�ʼ��
 *��    ע��
 */
void Calcul_OverturntoB(void)
{
	
			u8 i;
		
		for(i=0;i<8;i++)
		{
			overturnto_b[0][i]=initial_position[i];
		}
		
		overturnto_b[0][4]=loosen_position[0];/*5����*/
		overturnto_b[0][6]=loosen_position[2];/*7����*/
		
		for(i=0;i<8;i++)
		{
			overturnto_b[1][i]=overturnto_b[0][i];
		}
		
		overturnto_b[1][1]=clockwise90_position[1];               /*2˳ʱ��*/
		overturnto_b[1][3]=anticlockwise90_position[3];           /*4��ʱ�룬������ת�Ķ���*/
		
		for(i=0;i<8;i++)
		{
			overturnto_b[2][i]=overturnto_b[1][i];
		}
		
		overturnto_b[2][4]=wrasp_position[0]; /*5ǰ��*/
		overturnto_b[2][6]=wrasp_position[2]; /*7ǰ��*/
		
		for(i=0;i<8;i++)
		{
			overturnto_b[3][i]=overturnto_b[2][i];
		}
		overturnto_b[3][5]=loosen_position[1]; /*6����*/
		overturnto_b[3][7]=loosen_position[3]; /*8����*/


		for(i=0;i<8;i++)
		{
			overturnto_b[4][i]=overturnto_b[3][i];
		}
		
		 overturnto_b[4][1]=original_position[1];/*2�ص���ʼλ��*/
		 overturnto_b[4][3]=original_position[3];/*4�ص���ʼλ��*/
		
		for(i=0;i<8;i++)
		{
			overturnto_b[5][i]=overturnto_b[4][i];
		}
		
		overturnto_b[5][5]=wrasp_position[1]; /*6ǰ��*/
		overturnto_b[5][7]=wrasp_position[3]; /*8ǰ������ʱ�����ʼλ��*/
   
}
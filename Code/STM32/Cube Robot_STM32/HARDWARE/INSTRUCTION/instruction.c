/*�ļ�����: instruction.c
 *�ļ����ܣ��ַ�������
 *��ע���ܹ�16��ָ��
 *��һ�飺LL RR FF BB����ת90�ȣ�
 *�ڶ��飺L2 R2 F2 B2����ת180�ȣ�
 *�����飺L' R' F' B'����ʱ��90�ȣ�
 *�����飺L^ R^ F^ B^��˳ʱ��90�ȣ�
 */

#include "stm32f10x.h"
#include "instruction.h"
#include "usart.h"
#include "movement.h"
#include "motor.h"
#include <math.h>

u16 solvecube_data[500][8];/*ִ�����ս��������*/

/*  ��������List_num_Diff
 *�������ܣ�����array1��array2Ԫ�ز�ͬ�����ڵ��к�
 *��    �룺arrayΪһά���飬8��Ԫ��
 *��    ����Ψһ��ͬԪ�����ڵ��к�
 *����������� Merge_Movement����
 *��    ע����������֮��ֻ��һ����ͬ��Ԫ��
 */
u8 List_num_Diff(u16 *array1,u16 *array2)
{
	int list_num=0;
	
	for(list_num=0;list_num<8;list_num++)
	{
		if(array1[list_num]!=array2[list_num])
		{
			break;
		}
	}
	
	return list_num;
}


/* ��������u8 Analy_One_Instruction(char1,char2) 
*�������ܣ����ݵ���ָ������ݰ���Ӧ��ִ���������double_buffer������
*��    �룺char1Ϊ����ָ��ĵ�һ���ַ���char2Ϊ����ָ��ĵڶ����ַ�
*��    ����ִ������������������֣�4,6,8
*�����������Analy_UsartString()����
*��    ע������char_buffer������
*/
u8 Analy_One_Instruction(u8 char1,u8 char2,u16(*char_buffer)[8])
{
	u8 i,j,line_num=0;
	if(char1=='L')
	{
			if(char2=='^')//L^
			{
				for(i=0;i<4;i++)
				{
						for(j=0;j<8;j++)
						{
								*(*(char_buffer+i)+j)=*(*(l_clockwise90+i)+j);
						}
				}
				line_num=4;
				
			}
			else if(char2==0x27)//L'
			{
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(l_anticlockwise90+i)+j);
							}
					}
						line_num=4;
			}
			else if(char2=='2')//L2
			{
					for(i=0;i<4;i++)
				
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(l_clockwise90+i)+j);
							}
					}
					
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i+4)+j)=*(*(l_clockwise90+i)+j);
							}
					}
					
				line_num=8;

					
			}
			else //��ת��LL
			{
				
				for(i=0;i<6;i++)
				{
						for(j=0;j<8;j++)
						{
								*(*(char_buffer+i)+j)=*(*(overturnto_l+i)+j);
						}
				}
				line_num=6;
								
			}
	
	}
	
	else if(char1=='R')
	{
			if(char2=='^')//R^
			{
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(r_clockwise90+i)+j);
							}
					}
				line_num=4;
			}
			else if(char2==0x27)//R'
			{
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(r_anticlockwise90+i)+j);
							}
					}
					line_num=4;

			}
			else if(char2=='2')//R2
			{
				
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(r_clockwise90+i)+j);
							}
					}
					
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i+4)+j)=*(*(r_clockwise90+i)+j);
							}
					}
					
				line_num=8;
				
			}
			else //��ת��RR
			{
				
				for(i=0;i<6;i++)
				{
						for(j=0;j<8;j++)
						{
								*(*(char_buffer+i)+j)=*(*(overturnto_r+i)+j);
						}
				}
				line_num=6;
								
			}
					
	}
	
	else if(char1=='F')
	{
			if(char2=='^')//F^
			{
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(f_clockwise90+i)+j);
							}
					}
					line_num=4;
			}
			else if(char2==0x27)//F'
			{
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(f_anticlockwise90+i)+j);
							}
					}
					line_num=4;

			}
			else if(char2=='2')//F2
			{
				
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(f_clockwise90+i)+j);
							}
					}
					
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i+4)+j)=*(*(f_clockwise90+i)+j);
							}
					}
					
				line_num=8;				
			}
			else //��ת��FF
			{
				
				for(i=0;i<6;i++)
				{
						for(j=0;j<8;j++)
						{
								*(*(char_buffer+i)+j)=*(*(overturnto_f+i)+j);
						}
				}
				line_num=6;
				
			}
	}
	else
	{
			if(char2=='^')//B^
			{
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(b_clockwise90+i)+j);
							}
					}
					
				line_num=4;
					
			}
			else if(char2==0x27)//B'
			{
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(b_anticlockwise90+i)+j);
							}
					}
					
					line_num=4;

			}
			else if(char2=='2')//B2
			{
				
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(b_clockwise90+i)+j);
							}
					}
					
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i+4)+j)=*(*(b_clockwise90+i)+j);
							}
					}
					
				line_num=8;				
			}
			else //��ת,BB
			{
				
				for(i=0;i<6;i++)
				{
						for(j=0;j<8;j++)
						{
								*(*(char_buffer+i)+j)=*(*(overturnto_b+i)+j);
						}
				}
				
				line_num=6;
				
			}
	
	}
	return line_num;
	
}


/* ��������u8 Merge_Movement
 *�������ܣ���������������ִ�еĶ���ָ��������һ�𣬷���double_buffer������
 *��    �룺array1_list_num��array1��������array2_list_num��array2������
 *��    �������߽ϴ������
 *���������
 *��    ע��array��һ�е�Ԫ�غ�ǰһ����ȣ�ֻ��һ��Ԫ�ز�ͬ,����ֻ��4,8����
 */
u8 Merge_Movement(u16(*array1)[8], u16(*array2)[8], u8 array1_list_num, u8 array2_list_num,u16 (*double_buffer)[8])
{
	u8 list_num, i, j;
	if (array1_list_num == array2_list_num)//��Ϊ8���߶�Ϊ4�����
	{
		for (i = 0; i<array1_list_num; i++)
		{
			for (j=0;j<8;j++)
			{
				*(*(double_buffer + i) + j) = *(*(array1 + i) + j);
			}
		
		}
		for (j=0;j<2;j++)
		{
			list_num = List_num_Diff(*(array2 + j), *(array2 + j + 1));
			for (i = 0; i<array1_list_num; i++)
			{
				*(*(double_buffer + i) + list_num) = *(*(array2 + i) + list_num);
			}
		}
		
	}

	if ((array1_list_num == 4 && array2_list_num == 8) || (array1_list_num == 8 && array2_list_num == 4))//һ����8������һ����4�����
	{
		for (i = 0; i<4; i++)
		{
			for (j=0;j<8;j++)
			{
				*(*(double_buffer + i) + j) = *(*(array1 + i) + j);
			}	
		}

		for (j = 0; j<2; j++)
		{
			list_num = List_num_Diff(*(array2 + j), *(array2 + j + 1));
			for (i = 0; i<4; i++)
			{
				*(*(double_buffer + i) + list_num) = *(*(array2 + i) + list_num);
			}
		}

		if (array1_list_num == 4 && array2_list_num == 8)
		{
			for (i = 4; i<8; i++)
			{
				for (j = 0; j<8; j++)
				{
					*(*(double_buffer + i) + j) = *(*(array2 + i) + j);
				}

			}
		}

		else
		{
			for (i = 4; i<8; i++)
			{
				for (j = 0; j<8; j++)
				{
					*(*(double_buffer + i) + j) = *(*(array1 + i) + j);
				}
			}
		}
	}


	if (array1_list_num>array2_list_num)
	{
		return array1_list_num;
	}
	else
	{
		return array2_list_num;

	}
}


/*  ��������Initial_Data(u16 *array,u8 start_num,u8 end_num)
 *�������ܣ�Ϊsolvecube_data��ֵ
 *��    ��: array:��ά����ָ��,start_num:���鿪ʼ���,end_num:����������
 *��    ��: ��
 *�������: ��Analy_UsartString()��������
 *��    ע�����������Ϊ8����ά����ָ����Ϊ��������ʱ����Ҫȷ��
 */
void Initial_Data(u16 (*array)[8],u16 start_line,u16 end_line)
{
	u8 i,j;
	
	for(j=start_line;j<end_line;j++)
	{
		for(i=0;i<8;i++)
		{
			pos[j][i]=*(*(array+(j-start_line))+i);			
		}
	
	}
}



u8 Judge_Merge(u8 char1,u8 char2)
{
	u8 merge_flag;
		
				if((char1=='L'&&char2=='R')||(char1=='R'&&char2=='L')||(char1=='F'&&char2=='B')||(char1=='B'&&char2=='F'))
				{
					merge_flag=1;
				}	
				else 
				{
					merge_flag=0;
				}
	
	return merge_flag;
	
}

/*  ������:Analy_UsartString()
 *�������ܣ��������ڴ������ַ���������ħ�����㲽�����solvecube_data������
 *��    �룺��
 *��    ����solvecube_data������б��
 *�����������SolvecubeArray_ToBufferArray()����
 *��    ע��
 */
u16 Analy_UsartString(void)
{
		u8 i=1;
		u8 j;
		u8 combine_line_add=0;
		u8 line_num1=0;
		u8 line_num2=0;
		
		u16 curr_line=1;//pos��0������ʼλ��
		u16 char1_buffer[8][8];
		u16 char2_buffer[8][8];
		u16 merge_buffer[8][8];
	
		for(j=0;j<8;j++)
		{	
			pos[0][j]=initial_position[j];//��һ���ǲ�ִ�е�
		}
	
		while(rece_string[i]!='!')
		{
				if((Judge_Merge(rece_string[i],rece_string[i+2]))&&(rece_string[i+2]!=rece_string[i+3])&&(rece_string[i]!=rece_string[i+1]))//��������ִ�е�����������
				{
						line_num1=Analy_One_Instruction(rece_string[i],rece_string[i+1],char1_buffer);
						line_num2=Analy_One_Instruction(rece_string[i+2],rece_string[i+3],char2_buffer);																																		
					
						combine_line_add=Merge_Movement(char1_buffer,char2_buffer,line_num1,line_num2,merge_buffer);
						Initial_Data(merge_buffer,curr_line,curr_line+combine_line_add);
						curr_line+=combine_line_add;
						i+=4;
				}
				else//ִ�е���������
				{
						line_num1=Analy_One_Instruction(rece_string[i],rece_string[i+1],char1_buffer);
						Initial_Data(char1_buffer,curr_line,curr_line+line_num1);
						curr_line+=line_num1;
						i+=2;
				}

		}
		
	return (curr_line-1);
		
}


/*�������������Զ���Ƕ�ʱʹ�ã��������ڽ��յ�������
 *��    ��:ʾ��:@1170%1250%1280%1190%1500%1500%1500%1500!
 *��    ��:��
 *��    ע��
 */
void Analy_String(u8 *rece_string)
{
	u8 i = 0;//�ӵڶ����ַ���ʼ����
	u8 j = 0;
	u8 buffer_data[4];
	u16 motor_data;
	u8 space_num = 0;
	u8 motor_bits;


	while (rece_string[i] != '!')
	{

		if (rece_string[i] <= '9'&&rece_string[i] >= '0')
		{
			buffer_data[j++] = rece_string[i];

		}

		else if(rece_string[i] == '%')
		{
			

			for (motor_bits = 0; motor_bits<j; motor_bits++)
			{
				motor_data += asc_to_num(buffer_data[motor_bits])*(u16)pow(10, j-1-motor_bits);//����ǰ��ս����������Ϣһ��
			}

			pos[0][space_num] = motor_data;
			pos[1][space_num] = motor_data;
			
			space_num++;
			motor_data = 0;
			j = 0;
			
		}
		
		i++;

	}
	
	for (motor_bits=0; motor_bits<j; motor_bits++)
	{
		
	motor_data += asc_to_num(buffer_data[motor_bits])*(u16)pow(10, j - 1 - motor_bits);
		
	}

	pos[0][space_num] = motor_data;
	pos[1][space_num] = motor_data;

}


/*��������:��������ʱ��Ķ���Ƕȣ�ˮƽ�Ƕ�
 *��    ��:ʾ��:%1!,%2!,%3����%4����%5����%6��
 *��    ��:ִ��������б��
 *��    ע��һ��Ҫ˳���������ͣ�����
 */
void Debug_Photo_Angle(u8 *string)
{
	
  u8 photo_num; 
	
	photo_num=asc_to_num(string[1]);
	
	switch(photo_num)
	{
		
		case 1: 
		{
			
			PicArray_ToBufferArray(firpic_position,4);
			
	  }break;
		
		case 2: 
		{
			
			PicArray_ToBufferArray(secpic_position,1);

	  }break;

		case 3: 
		{
			
			PicArray_ToBufferArray(thirpic_position,4);
			
	  }break;
		
		case 4: 
		{
			
			PicArray_ToBufferArray(fourpic_position,1);//���

	  }break;
		
		case 5: 
		{
			
			PicArray_ToBufferArray(fifpic_position,4);

	  }break;
		
		case 6: 
		{
			
			PicArray_ToBufferArray(sixpic_position,1);

	  }break;
	
		case 7: 
		{
			
			PicArray_ToBufferArray(retuinit_position,4);

	  }break;		
		
		
		default:
		{

		}break;

  }
	

}

#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
 
#define KEY0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1) /*��ȡKEY0*/
#define KEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)/*��ȡKEY1*/
#define KEY2  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) /*��ȡKEY2����Ϊ˿ӡ��ע��WK_UP*/

void KEY_Init(void);
#endif

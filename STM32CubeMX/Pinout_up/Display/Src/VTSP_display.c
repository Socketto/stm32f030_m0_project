#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "PJLeds.h"
#include "VTSP_display.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*defines*/
#define DELAY_DISPLAY 3


uint8_t MessageDisplay[] = "----------------";
uint8_t MessageDisplay_mirror[] = "----------------";

uint8_t Leds = 0x00;
uint8_t tempChar = 0x00;

uint8_t ValueChar(unsigned char valueChar)
{
	switch(valueChar)
	{
		case '#': return 0x7E; /* # */
		case '$': return 0x6D; /* $ */
		case '&': return 0x46; /* & */
		case '(': return 0x29; /* ( */
		case ')': return 0x0B; /* ) */
		case '*': return 0x21; /* * */
		case '+': return 0x70; /* + */
		case ',': return 0x10; /* , */
		case '-': return 0x40; /* - */
		case '/': return 0x52; /* / */
		case '0': return 0x3F; /* 0 */
		case '1': return 0x06; /* 1 */
		case '2': return 0x5B; /* 2 */
		case '3': return 0x4F; /* 3 */
		case '4': return 0x66; /* 4 */
		case '5': return 0x6D; /* 5 */
		case '6': return 0x7D; /* 6 */
		case '7': return 0x07; /* 7 */
		case '8': return 0x7F; /* 8 */
		case '9': return 0x6F; /* 9 */
		case ':': return 0x09; /* : */
		case ';': return 0x0D; /* ; */
		case '<': return 0x61; /* < */
		case '=': return 0x48; /* = */
		case '>': return 0x43; /* > */
		case '@': return 0x5F; /* @ */
		case 'A': return 0x77; /* A */
		case 'B': return 0x7C; /* B */
		case 'C': return 0x39; /* C */
		case 'D': return 0x5E; /* D */
		case 'E': return 0x79; /* E */
		case 'F': return 0x71; /* F */
		case 'G': return 0x3D; /* G */
		case 'H': return 0x76; /* H */
		case 'I': return 0x30; /* I */
		case 'J': return 0x1E; /* J */
		case 'K': return 0x75; /* K */
		case 'L': return 0x38; /* L */
		case 'M': return 0x15; /* M */
		case 'N': return 0x37; /* N */
		case 'O': return 0x3F; /* O */
		case 'P': return 0x73; /* P */
		case 'Q': return 0x6B; /* Q */
		case 'R': return 0x33; /* R */
		case 'S': return 0x6D; /* S */
		case 'T': return 0x78; /* T */
		case 'U': return 0x3E; /* U */
		case 'V': return 0x3E; /* V */
		case 'W': return 0x2A; /* W */
		case 'X': return 0x76; /* X */
		case 'Y': return 0x6E; /* Y */
		case 'Z': return 0x5B; /* Z */
		case '[': return 0x39; /* [ */
		case ']': return 0x0F; /* ] */
		case '^': return 0x23; /* ^ */
		case '_': return 0x08; /* _ */
		case 'a': return 0x5F; /* a */
		case 'b': return 0x7C; /* b */
		case 'c': return 0x58; /* c */
		case 'd': return 0x5E; /* d */
		case 'e': return 0x7B; /* e */
		case 'f': return 0x71; /* f */
		case 'g': return 0x6F; /* g */
		case 'h': return 0x74; /* h */
		case 'i': return 0x10; /* i */
		case 'j': return 0x0C; /* j */
		case 'k': return 0x75; /* k */
		case 'l': return 0x30; /* l */
		case 'm': return 0x14; /* m */
		case 'n': return 0x54; /* n */
		case 'o': return 0x5C; /* o */
		case 'p': return 0x73; /* p */
		case 'q': return 0x67; /* q */
		case 'r': return 0x50; /* r */
		case 's': return 0x6D; /* s */
		case 't': return 0x78; /* t */
		case 'u': return 0x1C; /* u */
		case 'v': return 0x1C; /* v */
		case 'w': return 0x14; /* w */
		case 'x': return 0x76; /* x */
		case 'y': return 0x6E; /* y */
		case 'z': return 0x5B; /* z */
		case '{': return 0x46; /* { */
		case '|': return 0x30; /* | */
		case '}': return 0x70; /* } */
		case '~': return 0x01; /* ~ */
		default: return 0x00; /* (space) */
	}
}


void ResetSegments()
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,0);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,0);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,0);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,0);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,0);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,0);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
}

void Set_Digit(unsigned char value)
{
	if((value & 0x80) == 0)
	{
		value = ValueChar(value);
	}
	if(value & 0x20)
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,1);
	if(value & 0x10)
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,1);
	if(value & 0x08)
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,1);
	if(value & 0x04)
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,1);
	if(value & 0x02)
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,1);
	if(value & 0x01)
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,1);
	if(value & 0x40)
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,1);
	osDelay(DELAY_DISPLAY);
}


void Next_Digit()
{
	ResetSegments();
	/*NEXT DIGIT*/
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,1);
	osDelay(DELAY_DISPLAY);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,0);
	osDelay(DELAY_DISPLAY);
}

void Reset_Digit()
{
	ResetSegments();
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,0);
	osDelay(DELAY_DISPLAY);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,1);
	osDelay(DELAY_DISPLAY);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,0);
	osDelay(DELAY_DISPLAY);
}


void Refresh_PJ()
{
	while(1)
	{
		Reset_Digit();
		Set_Digit(MessageDisplay[0]);
		Next_Digit();
		Set_Digit(0x80+Leds);
		Next_Digit();
		Set_Digit(MessageDisplay[1]);
		Next_Digit();
		Set_Digit(MessageDisplay[2]);
	}
}

//command = 0 char =1
void LCD_Command(char cmd, char command)
{
	tempChar = cmd>>4; //temp variable
	tempChar += cmd<<4;
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,command); // RS

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,tempChar&0x01); // D4
	tempChar = tempChar>>1;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,tempChar&0x01); // D5
	tempChar = tempChar>>1;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,tempChar&0x01); // D6
	tempChar = tempChar>>1;
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,tempChar&0x01); // D7
	tempChar = tempChar>>1;
	osDelay(DELAY_DISPLAY);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,1); // EN
	osDelay(DELAY_DISPLAY);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,0); // EN
	osDelay(DELAY_DISPLAY);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,tempChar&0x01); // D4
	tempChar = tempChar>>1;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,tempChar&0x01); // D5
	tempChar = tempChar>>1;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,tempChar&0x01); // D6
	tempChar = tempChar>>1;
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,tempChar&0x01); // D7
	osDelay(DELAY_DISPLAY);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,1); // EN
	osDelay(DELAY_DISPLAY);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,0); // EN
	osDelay(DELAY_DISPLAY);
}



void LCD_String(const uint8_t *msg)
{
	unsigned char cont = 0;
	LCD_Command(0x01,0);
	LCD_Command(0x06,0);
	while((*msg)!=0)
	{
		if(cont==0)
		{
			LCD_Command(0x80,0);
		}
		if(cont==8)
		{
			LCD_Command(0xC0,0);
		}
		LCD_Command(*msg,1);
		msg++;
		cont++;
    	}

}
void LCD_String_xy(char row,char pos,const char *msg)
{
    char location=0;
    if(row<=1)
    {
        location=(0x80) | ((pos) & 0x0f); /*Print message on 1st row and desired location*/
        LCD_Command(location,0);
    }
    else
    {
        location=(0xC0) | ((pos) & 0x0f); /*Print message on 2nd row and desired location*/
        LCD_Command(location,0);
    }
    LCD_String((uint8_t*)msg);

}

void Init_LCD()
{
	osDelay(200);
	LCD_Command(0x33,0);
	LCD_Command(0x32,0);
	LCD_Command(0x28,0);
	LCD_Command(0x01,0);
	LCD_Command(0x06,0);
	LCD_Command(0x0C,0);
}


void Refresh_LCD()
{
	Init_LCD();
	while(1)
	{
		osDelay(10);
		if(strcmp((char*)MessageDisplay_mirror,(char*)MessageDisplay) != 0)
		{
			strcpy((char*)MessageDisplay_mirror,(char*)MessageDisplay);
			LCD_String(MessageDisplay);
		}
	}
}


void PJ_LED(unsigned char led,unsigned char set)
{
	if(set == 0)
		Leds &= ~led;
	else
		Leds |= led;
}

void PJ_Display(char* message)
{
	if(strlen(message)<16)
	{
		strcpy((char*)MessageDisplay,message);
	}
}

void LCD_Display(char* message)
{
	PJ_Display(message);
}


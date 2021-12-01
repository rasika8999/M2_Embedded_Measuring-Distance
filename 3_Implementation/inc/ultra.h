/**
 * @file MrLCDATmega32.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef ultra
#define ultra

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define ultrasCrib				PORTA
#define DataDir_ultraCrib		DDRA

#define ultrasControl			PORTD
#define DataDir_ultrasControl	DDRD

#define LightSwitch				5	//for the enable pin 
#define ReadWrite				4	//for R/W pin
#define BipolarMood				3	// for the Rs pin(Register Select)

char firstColumnPositionForultra[2]={0,64};

void Initialise(void);
void Check_If_Mr_LCD_Is_Busy(void);
void Peek_A_Boo(void);
void Send_A_Command(unsigned char command);
void Send_A_Character(unsigned char character);
void Send_A_String(char *stringOfChar);
void GoToultraLocation(uint8_t x, uint8_t y);



void Initialise(void)
{
DataDir_ultrasControl|=1<<LightSwitch|1<<ReadWrite|1<<BipolarMood;	//these information will go towards the LCD

_delay_ms(15);								// Wait for the LCD to start

Send_A_Command(0x01);	// to clear the screen
_delay_ms(2);
Send_A_Command(0x38);	// TO tell LCD about 8 data lines
_delay_us(50);
Send_A_Command(0b00001110);	//Some cursor command
_delay_us(50);

}

void Check_If_ultra_Is_Busy()
{
	DataDir_ultraCrib=0x00;					//Crib will take the input from the user
	
	
	ultrasControl|=1<<ReadWrite;			// Switch on the read mode
	ultrasControl&=~(1<<BipolarMood);			//activate the command mode
	
	while (ultrasCrib>=0x80)
	{
		Peek_A_Boo();
	}
	
	DataDir_ultraCrib=0xff;					// Put crib to display output
}

void Peek_A_Boo()
{
	ultrasControl|=1<<LightSwitch;			// Switch on the light 
	
	asm volatile("nop");					//wait for some time 
	asm volatile("nop");
	
	ultrasControl&=~(1<<LightSwitch);			// Switch off the light
}


void Send_A_Command(unsigned char command)
{
	
	Check_If_Mr_LCD_Is_Busy();
	
	ultrasCrib=command;						// Load the crib with the 8 bit command
	
	ultrasControl&=~(1<<ReadWrite);			// Set the LCD to write mode
	ultrasControl&=~(1<<BipolarMood);			// Set it to the command mode	
	Peek_A_Boo();
	ultrasCrib=0;							//Erase the crib after use
}
void Send_A_Character(unsigned char character)
{
	Check_If_Mr_LCD_Is_Busy();
	
	ultrasCrib=character;					// load the 8 bit characters on the data line
	
	ultrasControl&=~(1<<ReadWrite);			// Set to the write mode
	ultrasControl|=1<<BipolarMood;			// Set it to the character mode	
	
	Peek_A_Boo();
	ultrasCrib=0;							// Erase the crib after use
	
	
}

void Send_A_String(char *stringOfChar)
{
	while(*stringOfChar>0)
	{
		Send_A_Character(*stringOfChar++);
	}
}


void GoToultraLocation(uint8_t x, uint8_t y)
{
	Send_A_Command(0x80+ firstColumnPositionForultra[y-1] + (x-1));
}


#endif
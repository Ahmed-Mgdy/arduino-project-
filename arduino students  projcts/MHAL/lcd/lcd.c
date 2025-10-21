/*
 * lcd.c
 *
 * Created: 4/14/2023 4:01:32 PM
 *  Author: Ahmed2000
 */ 

#define F_CPU 1000000ul
#include <avr/io.h>
#include "STDMACROS.h"
#include <util/delay.h>

 void LCD_send_data(char x)
 {
	 PORTA=(x &0xf0)|(PORTA & 0x0f);
	 setbit(PORTB,1);
	 clearbit(PORTB,2);
	 setbit(PORTB,3);
	 _delay_us(1);
	 clearbit(PORTB,3);
	 _delay_us(200);
	 PORTA=(x<<4)|(PORTA &0x0f);
	 setbit(PORTB,3);
	 _delay_us(1);
	 clearbit(PORTB,3);
	 _delay_ms(10);
 }
 
 void LCD_send_cmd( char x)
 {
	 PORTA=(x &0xf0)|(PORTA & 0x0f);
	 clearbit(PORTB,1);
	 clearbit(PORTB,2);
	 setbit(PORTB,3);
	 _delay_us(1);
	 clearbit(PORTB,3);
	 _delay_us(200);
	 PORTA=(x<<4)|(PORTA &0x0f);
	 setbit(PORTB,3);
	 _delay_us(1);
	 clearbit(PORTB,3);
	 _delay_ms(10);
 }
  void LCD_INTI()
  {
	  DDRA|=0xf0;
	  setbit(DDRB,1);
	  setbit(DDRB,2);
	  setbit(DDRB,3);
	  LCD_send_cmd(0x02);
	  LCD_send_cmd(0x28);
	  LCD_send_cmd(0x0c);
	  LCD_send_cmd(0x06);
	  LCD_send_cmd(0x01);
	  LCD_send_cmd(0x0f);
      
	  _delay_ms(20);
  }
  
  
 void LCD_String (char *str)
 {		
	 int i;
	 for(i=0; str[i]!=0;i++)		
	 {
		 LCD_send_data(str[i]);
	 }
 }

  
  

		  	
  


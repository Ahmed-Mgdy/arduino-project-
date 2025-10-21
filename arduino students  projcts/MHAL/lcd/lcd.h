/*
 * lcd.h
 *
 * Created: 4/14/2023 4:02:08 PM
 *  Author:
 Ahmed2000
 */ 


#ifndef LCD_H_
#define LCD_H_

void LCD_send_cmd( char x);
void LCD_send_data( char x);
void LCD_INTI() ;
void LCD_String (char *str);	




#endif /* LCD_H_ */
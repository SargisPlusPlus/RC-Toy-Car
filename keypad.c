#include "avr.h"
#include "keypad.h"

unsigned char key()
{
	//Test whether bit (on right) in IO register (on left) is set.
	//if it is not, then the key is set to a character.
	DDRA = 0xF0;
	PORTA = 0x0F;
	
	char key = ' ';
	PORTA = 0b01111111;
	if (!bit_is_set(PINA, 3))
	key='*';
	else if (!bit_is_set(PINA, 2))
	key='C';
	else if (!bit_is_set(PINA, 1))
	key='B';
	else if (!bit_is_set(PINA, 0))
	key='A';
	
	PORTA = 0b10111111;
	if (!bit_is_set(PINA, 3))
	key='D';
	else if (!bit_is_set(PINA, 2))
	key='9';
	else if (!bit_is_set(PINA, 1))
	key='6';
	else if (!bit_is_set(PINA, 0))
	key='3';
	
	PORTA = 0b11011111;
	if (!bit_is_set(PINA, 3))
	key='#';
	else if (!bit_is_set(PINA, 2))
	key='8';
	else if (!bit_is_set(PINA, 1))
	key='5';
	else if (!bit_is_set(PINA, 0))
	key='2';
	
	PORTA = 0b11101111;
	if (!bit_is_set(PINA, 3))
	key='0';
	else if (!bit_is_set(PINA, 2))
	key='7';
	else if (!bit_is_set(PINA, 1))
	key='4';
	else if (!bit_is_set(PINA, 0))
	key='1';
	
	return key;
}
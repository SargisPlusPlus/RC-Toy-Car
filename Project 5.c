
#include <avr/io.h>
#include "avr.h"
#include "keypad.h"
#include "speaker.h"
#include "lcd.h"
#include "avr/delay.h"

void HCSR04Init();
void HCSR04Trigger();

void HCSR04Init()
{
	DDRC|=(1<<PC4);
}

void HCSR04Trigger()
{
	//Send a 10uS pulse on trigger line

	PORTC|=(1<<PC4); //high

	_delay_us(215); //wait 15uS

	PORTC&=~(1<<PC4); //low
}

uint16_t GetPulseWidth()
{
	uint32_t i,result;

	for(i=0;i<100000;i++)
	{
		if(!(PINC & (1<<PC5)))
		continue;
		else
		break;
	}

	if(i==599999)
	return -1;
	
	TCCR1A=0X00;
	TCCR1B=(1<<CS11);
	TCNT1=0x00;

	for(i=0;i<100000;i++)
	{
		if(PINC & (1<<PC5))
		{
			if(TCNT1 > 10000)
			break;
			else
			continue;
		}
		else
		break;
	}

	if(i == 599999)
	return -2;

	result=TCNT1;

	TCCR1B=0x00;

	if(result > 10000)
	return -2;
	else
	return (result);
}

uint16_t r;
unsigned int d;

void distance()
{
	_delay_ms(200);
	HCSR04Trigger();
	r = GetPulseWidth();
		
	//Handle Errors
	if(r == -1)
	{
		pos_lcd(1,0);
		put_str_lcd("ERROR ERROR!");
	}
	else if(r == -2)
	{
		pos_lcd(1,0);
		//put_str_lcd("DISTANCE DIST!");
	}
	else
	{
		char buffer[20];
		d = r/58.0;
		if(d <= 2)
		{
			d = 0;
		}
		_delay_ms(100);
		sprintf(buffer, "%02dcm", d);
		pos_lcd(1,0);
		put_str_lcd("Distance: ");
		pos_lcd(1,9);
		put_str_lcd(buffer);
	}
}

void forward()
{
	CLR_BIT(DDRC, 1);
	SET_BIT(DDRC, 0);
}
void backward()
{
	CLR_BIT(DDRC, 0);
	SET_BIT(DDRC, 1);
}
void left()
{
	CLR_BIT(DDRC, 3);
	SET_BIT(DDRC, 2);
}
void right()
{
	CLR_BIT(DDRC, 2);
	SET_BIT(DDRC, 3);
}
void stopall()
{
	CLR_BIT(DDRC, 0);
	CLR_BIT(DDRC, 1);
	CLR_BIT(DDRC, 2);
	CLR_BIT(DDRC, 3);
}
void stoprev()
{
	CLR_BIT(DDRC, 0);
	CLR_BIT(DDRC, 1);
}
void stopturn()
{
	CLR_BIT(DDRC, 2);
	CLR_BIT(DDRC, 3);
}

note_t honk[] = {{A3, 0.01}};
note_t tetris[] = {{E5,0.4, 19}, {B4,0.2}, {C5,0.2}, {D5,0.4}, {C5,0.2}, {B4,0.2}, {A4,0.4}, {A4,0.2}, {C5,0.2}, {E5,0.4}, {D5,0.2}, {C5,0.2}, {B4,0.4}, {C5,0.4}, {D5,0.4}, {E5,0.4}, {C5,0.4}, {A4,0.4}, {A4,0.4}};
note_t jingle[] = {{E4,0.2, 23}, {E4,0.2}, {E4,0.4}, {E4,0.2}, {E4,0.2}, {E4,0.4}, {E4,0.2}, {G4,0.2}, {C4,0.2}, {D4,0.2}, {E4,0.8},
{F4,0.2}, {F4,0.2}, {F4,0.2}, {F4,0.2}, {F4,0.2}, {E4,0.2}, {E4,0.4}, {E4,0.2}, {D4,0.2}, {C4,0.2}, {D4,0.2}, {G4,0.4}};
note_t birthday[] = {{G4,0.4,12}, {G4,0.2}, {A4,0.4}, {G4,0.8}, {C5,0.6}, {B4, 0.8}, {G4, 0.4}, {G4, 0.2}, {A4, 0.4}, {G4, 0.4}, {D5, 0.4}, {C5, 0.8}};
	
int main(void)
{
	HCSR04Init();
	SET_BIT(DDRB, 4);
	SET_BIT(DDRB, 5);
	ini_lcd();
	clr_lcd();
	pos_lcd(0,0);
	put_str_lcd("Starting"); 
	pos_lcd(1,0);
	put_str_lcd("engine...");
	wait_avr(2000);
	clr_lcd();
	
	while(1)
	{	
		while(key() == ' ')
		{
			stopall();
			pos_lcd(0,0);
			put_str_lcd("Idle");
			distance();
		}
		if(key() == '2')
		{
			clr_lcd();
			pos_lcd(0,0);
			put_str_lcd("Going forward");
			while(key() == '2')
			{
				distance();
				if(d <= 25 && d > 2 && key() == '2')
				{
					stopall();
					backward();
					wait_avr(300);
					stopall();
					while(key() == '2'){}
				}
				else
					forward();
			}
			clr_lcd();
		}
		if(key() == '8')
		{
			clr_lcd();
			pos_lcd(0,0);
			put_str_lcd("Going backward");
			
			while(key() == '8')
			{
				distance();
				backward();
			}
			clr_lcd();
		}
		if(key() == '4')
		{
			clr_lcd();
			pos_lcd(0,5);
			put_str_lcd("Going");
			pos_lcd(1,5);
			put_str_lcd("left");
			while(key() == '4')
			{
				left();
			}
			clr_lcd();
		}
		if(key() == '6')
		{
			clr_lcd();
			pos_lcd(0,5);
			put_str_lcd("Going");
			pos_lcd(1,5);
			put_str_lcd("right");
			while(key() == '6')
			{
				right();
			}
			clr_lcd();
		}
		if(key() == '3')
		{
			clr_lcd();
			pos_lcd(0,0);
			put_str_lcd("Turning right");
			while(key() == '3')
			{
				distance();
				if(d <= 25 && d > 2 && key() == '3')
				{
					stopall();
					backward();
					wait_avr(200);
					stopall();
					while(key() == '3'){}
				}
				forward();
				right();
			}
			clr_lcd();
		}
		if(key() == '1')
		{
			clr_lcd();
			pos_lcd(0,0);
			put_str_lcd("Going forward");
			while(key() == '1')
			{
				distance();
				if(d <= 25 && d > 2 && key() == '1')
				{
					stopall();
					backward();
					wait_avr(200);
					stopall();
					while(key() == '1'){}
				}
				forward();
				left();
			}
			clr_lcd();
		}
		if(key() == '7')
		{
			clr_lcd();
			pos_lcd(0,0);
			put_str_lcd("Going backwards");
			while(key() == '7')
			{
				distance();
				backward();
				left();
			}
			clr_lcd();
		}
		if(key() == '9')
		{
			clr_lcd();
			pos_lcd(0,0);
			put_str_lcd("Backwards right");
			while(key() == '9')
			{
				distance();
				backward();
				right();
			}
			clr_lcd();
		}
		if(key() == '5')
		{
			clr_lcd();
			pos_lcd(0,5);
			put_str_lcd("HONK!!!");
			play_honk(honk,1);
			clr_lcd();
		}
		if(key() == 'A')
		{
			clr_lcd();
			pos_lcd(0,4);
			put_str_lcd("Playing:");
			pos_lcd(1,3);
			put_str_lcd("Tetris Time");
			play_song(tetris, tetris->size);
			clr_lcd();
		}
		if(key() == 'B')
		{
			clr_lcd();
			pos_lcd(0,4);
			put_str_lcd("Playing:");
			pos_lcd(1,3);
			put_str_lcd("Merry Xmas");
			play_song(jingle, jingle->size);	
			clr_lcd();
		}
		if(key() == 'C')
		{
			clr_lcd();
			pos_lcd(0,4);
			put_str_lcd("Playing:");
			pos_lcd(1,1);
			put_str_lcd("Happy Birthday");			
			play_song(birthday, birthday->size);
			clr_lcd();
		}		
	}
}
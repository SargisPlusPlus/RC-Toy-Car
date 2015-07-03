#include "speaker.h"
#include "avr.h"




void play_note(unsigned char freq, double duration)
{
	// converting back to frequency
	//
	
	double oneSec = freq/(double) 62500; ///125,000/2 (2 waits) to convert back to freq.
	oneSec = (double)1/(double) oneSec; //convert back to amount of cycles per 8 ms
	int n = oneSec; //how many n u need to run one actual sec
	n = n * duration; //how many times we wanna run it during duration
	
	for(int i = 0; i < n; i++)
	{
		SET_BIT(PORTB, 3);
		wait_avr_micro(freq);
		CLR_BIT(PORTB, 3);
		wait_avr_micro(freq);
		
		if (key() == '*')
			break;
	}
}

void play_honk(const note_t *song, int size)
{
	DDRB = 0x0f;
	unsigned int i;
	for(i=0; i < size; i++)
	{
		play_note(song[i].freq, song[i].duration);

	}
}


void play_song(const note_t *song, int size)
{
	DDRB = 0x0f;
	unsigned int i;

	for(i=0; i < size; i++)
	{
		SET_BIT(DDRB,4);
		wait_avr(10);
		play_note(song[i].freq, song[i].duration);
		CLR_BIT(DDRB,4);
		wait_avr(10);
	}
}


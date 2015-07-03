#ifndef SPEAKER_H_
#define SPEAKER_H_


// Find period by 1/Frequence of the note
// Find delay which is 1/2 * period
// mult delay by 125,000 to make it microseconds.
// 1,000,000 mHz / 8bit = 125,000

#define B2      253
#define C3      239
#define Cs3     225
#define D3      213
#define Ds3     201
#define E3      190
#define F3      179
#define Fs3     169
#define G3      159
#define Gs3     150
#define A3      142
#define As3     134
#define B3      127
#define C4      119
#define Cs4     113
#define D4		106
#define Ds4     100
#define E4      95
#define F4      89
#define Fs4     84
#define G4      80
#define Gs4     75
#define A4      71
#define As4     67
#define B4      63
#define C5      60
#define Cs5     56
#define D5      53
#define Ds5     50
#define E5      47
#define F5      45
#define Fs5     42
#define G5      40
#define Gs5     38
#define A5      36
#define As5     34
#define B5      32
#define C6      30


typedef struct note{
	unsigned char freq;
	double duration;
	unsigned int size;
}note_t;


void play_song(const note_t *song, int size);
void play_note(unsigned char freq, double duration);

#endif /* SPEAKER_H_ */
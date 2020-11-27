#define F_CPU 16000000

#define OCRVALUE16BIT(freq) (F_CPU/freq - 1)

#define OCRVALUE8BIT(freq) (F_CPU/256/freq - 1)
#define DURATIONTOPULSES(duration, bpm, freq) (((uint32_t) 60 * freq * duration) / ((uint16_t) 32 * bpm))

#define WHOLE 128
#define HALF 64
#define QUARTER 32
#define EIGTH 16
#define SIXTEENTH 8
#define THIRTYSECOND 4
#define SIXTYFOURTH 2

#define DOTTED(note) ((note * 15)/10)
#define TRIPLE(note) (note / 3)

#define PAUSE 0
#define B0  31
#define C1  33
#define CS1 35
#define D1  37
#define DS1 39
#define E1  41
#define F1  44
#define FS1 46
#define G1  49
#define GS1 52
#define A1  55
#define AS1 58
#define B1  62
#define C2  65
#define CS2 69
#define D2  73
#define DS2 78
#define E2  82
#define F2  87
#define FS2 93
#define G2  98
#define GS2 104
#define A2  110
#define BB2 117
#define B2  123
#define C3  131
#define CS3 139
#define D3  147
#define DS3 156
#define E3  165
#define F3  175
#define FS3 185
#define G3  196
#define GS3 208
#define A3  220
#define AS3 233
#define B3  247
#define C4  262
#define CS4 277
#define D4  294
#define DS4 311
#define E4  330
#define F4  349
#define FS4 370
#define G4  392
#define GS4 415
#define A4  440
#define AS4 466
#define B4  494
#define C5  523
#define CS5 554
#define D5  587
#define DS5 622
#define E5  659
#define F5  698
#define FS5 740
#define G5  784
#define GS5 831
#define A5  880
#define AS5 932
#define B5  988
#define C6  1047
#define CS6 1109
#define D6  1175
#define DS6 1245
#define E6  1319
#define F6  1397
#define FS6 1480
#define G6  1568
#define GS6 1661
#define A6  1760
#define AS6 1865
#define B6  1976
#define C7  2093
#define CS7 2217
#define D7  2349
#define DS7 2489
#define E7  2637
#define F7  2794
#define FS7 2960
#define G7  3136
#define GS7 3322
#define A7  3520
#define AS7 3729
#define B7  3951
#define C8  4186
#define CS8 4435
#define D8  4699
#define DS8 4978
#define E8 5274
#define F8 5588

#include <stdlib.h>

typedef struct note{
	uint16_t frequency;
	uint16_t ocrValue;
	uint16_t duration; //use 255 as max
	uint16_t durationInPulses;
	struct note *next;
} note;

typedef struct song{
	uint8_t tempo;
	note *firstTreble;
	note *firstMid;
	note *firstBass;
} song;

void setOCRandMilis(song *s){
	note *ptr = s->firstTreble;
	while(ptr != 0){
		if(ptr->frequency != 0){
			ptr->ocrValue = OCRVALUE16BIT(ptr->frequency);
			ptr->durationInPulses = DURATIONTOPULSES(ptr->duration, s->tempo, ptr->frequency);
		} else {
			ptr->ocrValue = OCRVALUE16BIT(440);
			ptr->durationInPulses = DURATIONTOPULSES(ptr->duration, s->tempo, 440);
		}
		ptr = ptr->next;
	}
	ptr = s->firstMid;
	while(ptr != 0){
		if(ptr->frequency != 0){
			ptr->ocrValue = OCRVALUE8BIT(ptr->frequency);
			ptr->durationInPulses = DURATIONTOPULSES(ptr->duration, s->tempo, ptr->frequency);
		} else {
			ptr->ocrValue = OCRVALUE8BIT(440);
			ptr->durationInPulses = DURATIONTOPULSES(ptr->duration, s->tempo, 440);
		}
		ptr = ptr->next;
	}
	ptr = s->firstBass;
	while(ptr != 0){
		if(ptr->frequency != 0){
			ptr->ocrValue = OCRVALUE8BIT(ptr->frequency);
			ptr->durationInPulses = DURATIONTOPULSES(ptr->duration, s->tempo, ptr->frequency);
		} else {
			ptr->ocrValue = OCRVALUE8BIT(440);
			ptr->durationInPulses = DURATIONTOPULSES(ptr->duration, s->tempo, 440);
		}
		ptr = ptr->next;
	}
}

song initializeTestSong(){
	note t1;
	note t2;
	note t3;
	note t4;
	note m1;

	t1.frequency = 880;
	t1.duration = 48; //32 = 1 seminima
	t1.next = &t2;

	t2.frequency = 1760;
	t2.duration = 16;
	t2.next = &t3;
	
	t3.frequency = 0;
	t3.duration = 32;
	t3.next = &t4;

	t4.frequency = 880;
	t4.duration = 16;
	t4.next = 0;

	m1.frequency = 440;
	m1.duration = 64;
	m1.next = 0;

	song song1;
	song1.tempo = 80;
	song1.firstTreble = &t1;
	song1.firstMid = &m1;
	song1.firstBass = 0;

	setOCRandMilis(&song1);

	return song1;
}

song initializeSemptember(){

	const uint16_t bassNotes[] = {D5, PAUSE, D5, PAUSE, DS5, PAUSE, B4, PAUSE, CS4, 
		E4, F4, FS4,FS4,FS4};
	const uint16_t bassDuration[] = {DOTTED(SIXTEENTH), THIRTYSECOND, EIGTH, EIGTH,
		EIGTH, EIGTH, EIGTH, EIGTH, QUARTER, EIGTH, EIGTH, QUARTER, EIGTH, QUARTER};
	
	note *bass1 = malloc(sizeof(note)), *bass;
	bass1->frequency = bassNotes[0];
	bass1->duration = bassDuration[0];
	bass1->next = malloc(sizeof(note));
	bass = bass1->next;
	for(int i = 1; i < sizeof(bassNotes)/sizeof(uint16_t); i++) {
		
		bass->frequency = bassNotes[i];
		bass->duration = bassDuration[i];
		bass->next = malloc(sizeof(note));
		bass = bass->next;
	}
	bass->next = 0;
	
	song september;
	september.tempo = 120;
	september.firstTreble = 0;
	september.firstMid = 0;
	september.firstBass = bass1;
	setOCRandMilis(&september);
	return september;
}

song initializeFurElise(){

	const uint16_t trebleNotes[] = {E5, DS5, E5, DS5, E5, B4, D5, C5, 
		A4};
	const uint16_t trebleDuration[] = {EIGTH, EIGTH, EIGTH, EIGTH,
		EIGTH, EIGTH, EIGTH, EIGTH, HALF};

	const uint16_t bassNotes[] = {PAUSE, A4, E5, A5};
	const uint16_t bassDuration[] = {WHOLE, EIGTH, EIGTH, QUARTER};
	
	note *bass1 = malloc(sizeof(note)), *bass;
	bass1->frequency = bassNotes[0];
	bass1->duration = bassDuration[0];
	bass1->next = malloc(sizeof(note));
	bass = bass1->next;
	for(int i = 1; i < sizeof(bassNotes)/sizeof(bassNotes[0]); i++) {
		
		bass->frequency = bassNotes[i];
		bass->duration = bassDuration[i];
		bass->next = malloc(sizeof(note));
		bass = bass->next;
	}
	bass->next = 0;
	
	note *treble1 = malloc(sizeof(note)), *treble;
	treble1->frequency = trebleNotes[0];
	treble1->duration = trebleDuration[0];
	treble1->next = malloc(sizeof(note));
	treble = treble1->next;
	for(int i = 1; i < sizeof(trebleNotes)/sizeof(uint16_t); i++) {
		
		treble->frequency = trebleNotes[i];
		treble->duration = trebleDuration[i];
		treble->next = malloc(sizeof(note));
		treble = treble->next;
	}
	treble->next = 0;

	song furElise;
	furElise.tempo = 80;
	furElise.firstTreble = treble1;
	furElise.firstMid = 0;
	furElise.firstBass = bass1;
	setOCRandMilis(&furElise);
	return furElise;
}

song initializePreludeCMajor(){

	const uint16_t trebleNotes[] = {PAUSE, E7, G7, C8, E8, G7, C8, E8, 
		PAUSE, E7, G7, C8, E8, G7, C8, E8, PAUSE, D7, A7, D8, F8, A7, D8, 
		F8, PAUSE, D7, A7, D8, F8, A7, D8, F8, PAUSE, D7, A7, D8, F8, A7, D8, 
		F8, PAUSE, D7, A7, D8, F8, A7, D8, F8};
	const uint16_t trebleDuration[] = {SIXTEENTH, SIXTEENTH, SIXTEENTH, SIXTEENTH,
	SIXTEENTH, SIXTEENTH, SIXTEENTH, SIXTEENTH,SIXTEENTH, SIXTEENTH, SIXTEENTH, SIXTEENTH,
	SIXTEENTH, SIXTEENTH, SIXTEENTH, SIXTEENTH,SIXTEENTH, SIXTEENTH, SIXTEENTH, SIXTEENTH,
	SIXTEENTH, SIXTEENTH, SIXTEENTH, SIXTEENTH,SIXTEENTH, SIXTEENTH, SIXTEENTH, SIXTEENTH,
	SIXTEENTH, SIXTEENTH, SIXTEENTH, SIXTEENTH,SIXTEENTH, SIXTEENTH, SIXTEENTH, SIXTEENTH,
	SIXTEENTH, SIXTEENTH, SIXTEENTH, SIXTEENTH,SIXTEENTH, SIXTEENTH, SIXTEENTH, SIXTEENTH,
	SIXTEENTH, SIXTEENTH, SIXTEENTH, SIXTEENTH};

	const uint16_t bassNotes[] = {C7, PAUSE, C7, PAUSE, C7, PAUSE, C7, PAUSE,
	B6, PAUSE, B6, PAUSE};
	const uint16_t bassDuration[] = {DOTTED(QUARTER), EIGTH, DOTTED(QUARTER), EIGTH,
	DOTTED(QUARTER), EIGTH, DOTTED(QUARTER), EIGTH,DOTTED(QUARTER), EIGTH, DOTTED(QUARTER), EIGTH};
	
	note *bass1 = malloc(sizeof(note)), *bass;
	bass1->frequency = bassNotes[0];
	bass1->duration = bassDuration[0];
	bass1->next = malloc(sizeof(note));
	bass = bass1->next;
	for(int i = 1; i < sizeof(bassNotes)/sizeof(bassNotes[0]); i++) {
		
		bass->frequency = bassNotes[i];
		bass->duration = bassDuration[i];
		bass->next = malloc(sizeof(note));
 		bass = bass->next;
	}
	bass->next = 0;
	
	note *treble1 = malloc(sizeof(note)), *treble;
	treble1->frequency = trebleNotes[0];
	treble1->duration = trebleDuration[0];
	treble1->next = malloc(sizeof(note));
	treble = treble1->next;
	for(int i = 1; i < sizeof(trebleNotes)/sizeof(uint16_t); i++) {
		
		treble->frequency = trebleNotes[i];
		treble->duration = trebleDuration[i];
		treble->next = malloc(sizeof(note));
		treble = treble->next;
	}
	treble->next = 0;

	song preludeCMajor;
	preludeCMajor.tempo = 80;
	preludeCMajor.firstTreble = treble1;
	preludeCMajor.firstMid = bass1;
	preludeCMajor.firstBass = 0;
	setOCRandMilis(&preludeCMajor);
	return preludeCMajor;
}

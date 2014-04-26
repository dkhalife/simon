//**
//  Software Constants
//**

#define ON				1
#define OFF				0

#define BRED			1
#define BGREEN			0
#define BBLUE			2
#define BYELLOW			3

#define SBLUE			1000
#define SRED			1200
#define SGREEN			1400
#define SYELLOW			1600

#define RED				PORTD.F6
#define GREEN			PORTD.F5
#define BLUE			PORTD.F7
#define YELLOW			PORTD.F4

#define GOOD			PORTA.F0
#define BAD				PORTA.F1
#define WIN				PORTA.F2

#define DIGIT1			PORTE.F0
#define DIGIT2			PORTE.F1
#define DIGIT3			PORTE.F2

#define DELAY			500

//**
//  Segment Configuration
//**
//      7Seg = Bit
//        a  =  0      |  0
//        b  =  1      |  1
//        c  =  6      |  2
//        d  =  5      |  3
//        e  =  4      |  4
//        f  =  2      |  5
//        g  =  3      |  6
//       dip =  7      |  u
//**
const unsigned int _pA=0b00000001;
const unsigned int _pB=0b00000010;
const unsigned int _pC=0b01000000;
const unsigned int _pD=0b00100000;
const unsigned int _pE=0b00010000;
const unsigned int _pF=0b00000100;
const unsigned int _pG=0b00001000;
const unsigned int _pH=0b10000000;

//**
//  Variable Definitions
//**
unsigned int i;
unsigned int inp;
unsigned int score;
unsigned int SOUND;
unsigned int Flag=1;
unsigned char Seed=1;
unsigned int D1, D2, D3;
unsigned int position=0;
unsigned int position_=0;
unsigned char oldstate[8];
unsigned char holdstate[8];
unsigned char series[45];

//**
//  Button Input
//**
int Input(int pin){
	if (Button(&PORTB, pin, 1, 1)) oldstate[pin] = 1;
	if (oldstate[pin] && Button(&PORTB, pin, 1, 0)){
		oldstate[pin] = 0;
		return ON;
	}
	return OFF;
}

//**
//  Pseudo-Random Number
//**
unsigned char Number(int Lim, int Y){
	unsigned char Result;
	static unsigned int Y;
	Y=(Y*32719+3)%32749;
	Result=((Y%Lim)+1);
	return Result;
}

//**
//  Number to 7-Seg:
//**
unsigned char Display(unsigned char no){
    unsigned char Pattern;
    unsigned char SEGMENT[]={
		0xFF&(~_pG)&(~_pH),              // 0 = All but G and H
		_pB|_pC,                         // 1 = B and C
		0xFF&(~_pC)&(~_pF)&(~_pH),       // 2 = All but C, F and H
		0xFF&(~_pE)&(~_pF)&(~_pH),       // 3 = All but E, F and H
		_pB|_pC|_pF|_pG,                 // 4 = B, C, F and G
		0xFF&(~_pB)&(~_pE)&(~_pH),       // 5 = All but B, E and H
		0xFF&(~_pB)&(~_pH),              // 6 = All but B and H
		_pA|_pB|_pC,                     // 7 = A, B and C
		0xFF&(~_pH),                     // 8 = All but H
		0xFF&(~_pE)&(~_pH),              // 9 = All but H
    };
    Pattern=SEGMENT[no];
    return(Pattern);
}

//**
//  TMR0 timer interrupt service routine
//  The program jumps to ISR at every 5ms
//**
void interrupt(){
	TMR0L=100;
	//**
	//  Interrup Re-Configuration:
	//  0b
	//  - Disable all interrupts (0)
	//  - Disable all interrupts (0)
	//  - Enable the TMR0 overflow interrupt (1)
	//  - Disables the INT0 external interrupt (0)
	//  - Disables the RB port change interrupt (0)
	//  - TMR0 register did not overflow (0)
	//  - The INT0 external interrupt did not occur (0)
	//  - None of the RB<7:4> pins have changed state (0)
	//  - Total : 0b00100000;
	//**
	INTCON=0x20;
	Flag++;
	if(Flag>3){
			Flag=1;
	}
	DIGIT1=DIGIT2=DIGIT3=0;
	D3=(score-score%100)/100;
	D2=(score-D3*100)/10;
	D1=(score-D3*100-D2*10);
	switch(Flag){
		case 1:
			PORTC=Display(D1);
			DIGIT1=ON;
		break;

		case 2:
			PORTC=Display(D2);
			if(score>=10){
				DIGIT2=ON;
			}
		break;

		case 3:
			PORTC=Display(D3);
			if(score>=100){
				DIGIT3=ON;
			}
		break;
	}
}

//**
//  Main Program
//**
void main() {
	//**
	//  Oscillator Configuration:
	//  o 0b0
	//  o 4 MHz (110)
	//  o Primary system clock (1)
	//  o Stable frequency (1)
	//  o System clock : Internal RC (10)
	//  o Total : 0b01101110
	//**
	OSCCON=0x6E;
	//**
	//  Comparator Configuration:
	//  o Disable all
	//  o Total : 0b00000111;
	//**
	CMCON=0x07;
	//**
	//  A/D Configuration:
	//  o 0b00
	//  o Select An0 (0000)
	//  o Convertor idle (0)
	//  o A/D Convertor enabled (1)
	//  o Total : 0b00000010;
	//**
	ADCON0=0x02;
	//**
	//  A/D Configuration:
	//  o 0b00
	//  o VRef - = VSS (0)
	//  o VRef + = VDD (0)
	//  o AN0..AN3 Analogue (1011)
	//  o Total : 0b00001011;
	//**
	ADCON1=0x0B;
	//**
	//  Timer0 Configuration:
	//  o 0b
	//  o Enable Timer0 (1)
	//  o Timer0 is configured as an 8-bit timer/counter (1)
	//  o Internal instruction cycle clock (0)
	//  o Increment on low-to-high transition on T0CKI pin (0)
	//  o Timer0 prescaler is assigned. Timer0 clock inp comes from prescaler output (0)
	//  o 1:32 Prescale value (100)
	//  o Total : 0b11000100;
	//**
	T0CON=0xC4;
	// Formula : Time = (4 x Clock Period) x Prescaler x (256 - TMR0L)
	TMR0L=100;
	//**
	//  Interrup Configuration:
	//  o 0b
	//  o Enables all unmasked interrupts (1)
	//  o Disable all interrupts (0)
	//  o Enable the TMR0 overflow interrupt (1)
	//  o Disables the INT0 external interrupt (0)
	//  o Disables the RB port change interrupt (0)
	//  o TMR0 register did not overflow (0)
	//  o The INT0 external interrupt did not occur (0)
	//  o None of the RB<7:4> pins have changed state (0)
	//  o Total : 0b10100000;
	//**
	INTCON=0xA0;
	//**
	//  PORT Directions
	//  o Inputs
	//    PORTB
	//  o Outputs
	//	  PORTA
	//	  PORTC
	//    PORTD
	//	  PORTE
	//**
	TRISA=0x00;
	TRISB=0x0F;
	TRISC=0x00;
	TRISD=0x00;
	TRISE=0x00;

	//**
	//  Initialization
	//**
	PORTA=0x00;
	PORTB=PORTB & TRISB;
	PORTC=0x00;
	PORTD=0x00;
	PORTE=0x00;
	Sound_Init(&PORTA, 3);
	
	for(i=0; i<5; i++){
		BLUE=ON;
		Delay_ms(DELAY/6);
		BLUE=OFF;
		RED=ON;
		Delay_ms(DELAY/6);
		RED=OFF;
		GREEN=ON;
		Delay_ms(DELAY/6);
		GREEN=OFF;
		YELLOW=ON;
		Delay_ms(DELAY/6);
		YELLOW=OFF;
	}

	Delay_ms(3*DELAY/4);
	score=0;
	
	while(position<45){
		/* Reset Status */
		WIN=GOOD=BAD=OFF;
		
		/* Score */
		score+=position;
		
		/* Pick New Number */
		series[position]=Number(3,Seed);
		position++;
		
		for(i=0; i<position; i++){
			/* Show Series */
			switch(series[i]){
				case BGREEN:
					GREEN=ON;
					SOUND=SGREEN;
				break;

				case BRED:
					RED=ON;
					SOUND=SRED;
				break;

				case BBLUE:
					BLUE=ON;
					SOUND=SBLUE;
				break;

				case BYELLOW:
					YELLOW=ON;
					SOUND=SYELLOW;
				break;
			}
			Sound_Play(SOUND, DELAY/3);
			Delay_ms(DELAY/3);
			RED=BLUE=GREEN=YELLOW=OFF;
			Delay_ms(DELAY/3);
		}

		position_=0;
		
		/* Player Input */
		while(position_<position){
			inp=5;
			if(Input(BGREEN)){
				GREEN=ON;
				inp=BGREEN;
				SOUND=SGREEN;
			}
			if(Input(BBLUE)){
				BLUE=ON;
				inp=BBLUE;
				SOUND=SBLUE;
			}
			if(Input(BRED)){
				RED=ON;
				inp=BRED;
				SOUND=SRED;
			}
			if(Input(BYELLOW)){
				YELLOW=ON;
				inp=BYELLOW;
				SOUND=SYELLOW;
			}
			if(inp<5){
				/* Reset Status */
				GOOD=BAD=OFF;
				/* Check Answer */
				if(inp==series[position_]){
					position_++;
					Sound_Play(SOUND, DELAY/3);
					if(position_==position){
						GOOD=ON;
					}
					Delay_ms(DELAY/3);
					RED=BLUE=GREEN=YELLOW=OFF;
				}
				else{
					position=0;
					Sound_Play(300, DELAY);
					BAD=ON;
					Delay_ms(4*DELAY/3);
					BAD=OFF;
					RED=BLUE=GREEN=YELLOW=OFF;
					Delay_ms(2*DELAY);
					asm{
						reset;
					}
					break;
				}
			}
		}
		Sound_Play(800, DELAY/6);
		Sound_Play(900, DELAY/6);
		Delay_ms(DELAY/3);
	}
	
	/* Game is done */
	while(1){
		GOOD=ON;
		Delay_ms(200);
		GOOD=OFF;
		WIN=ON;
		Delay_ms(200);
		WIN=OFF;
	}
}
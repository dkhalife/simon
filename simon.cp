#line 1 "H:/Projects/Pic Tools/Active Projects/Simon/simon.c"
#line 46 "H:/Projects/Pic Tools/Active Projects/Simon/simon.c"
const unsigned int _pA=0b00000001;
const unsigned int _pB=0b00000010;
const unsigned int _pC=0b01000000;
const unsigned int _pD=0b00100000;
const unsigned int _pE=0b00010000;
const unsigned int _pF=0b00000100;
const unsigned int _pG=0b00001000;
const unsigned int _pH=0b10000000;




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




int Input(int pin){
 if (Button(&PORTB, pin, 1, 1)) oldstate[pin] = 1;
 if (oldstate[pin] && Button(&PORTB, pin, 1, 0)){
 oldstate[pin] = 0;
 return  1 ;
 }
 return  0 ;
}




unsigned char Number(int Lim, int Y){
 unsigned char Result;
 static unsigned int Y;
 Y=(Y*32719+3)%32749;
 Result=((Y%Lim)+1);
 return Result;
}




unsigned char Display(unsigned char no){
 unsigned char Pattern;
 unsigned char SEGMENT[]={
 0xFF&(~_pG)&(~_pH),
 _pB|_pC,
 0xFF&(~_pC)&(~_pF)&(~_pH),
 0xFF&(~_pE)&(~_pF)&(~_pH),
 _pB|_pC|_pF|_pG,
 0xFF&(~_pB)&(~_pE)&(~_pH),
 0xFF&(~_pB)&(~_pH),
 _pA|_pB|_pC,
 0xFF&(~_pH),
 0xFF&(~_pE)&(~_pH),
 };
 Pattern=SEGMENT[no];
 return(Pattern);
}





void interrupt(){
 TMR0L=100;
#line 134 "H:/Projects/Pic Tools/Active Projects/Simon/simon.c"
 INTCON=0x20;
 Flag++;
 if(Flag>3){
 Flag=1;
 }
  PORTE.F0 = PORTE.F1 = PORTE.F2 =0;
 D3=(score-score%100)/100;
 D2=(score-D3*100)/10;
 D1=(score-D3*100-D2*10);
 switch(Flag){
 case 1:
 PORTC=Display(D1);
  PORTE.F0 = 1 ;
 break;

 case 2:
 PORTC=Display(D2);
 if(score>=10){
  PORTE.F1 = 1 ;
 }
 break;

 case 3:
 PORTC=Display(D3);
 if(score>=100){
  PORTE.F2 = 1 ;
 }
 break;
 }
}








void main() {









 OSCCON=0x6E;





 CMCON=0x07;








 ADCON0=0x02;








 ADCON1=0x0B;
#line 218 "H:/Projects/Pic Tools/Active Projects/Simon/simon.c"
 T0CON=0xC4;

 TMR0L=100;
#line 234 "H:/Projects/Pic Tools/Active Projects/Simon/simon.c"
 INTCON=0xA0;
#line 247 "H:/Projects/Pic Tools/Active Projects/Simon/simon.c"
 TRISB=TRISD=0x00;
 while(1){
 PORTB=0x00;
 PORTD=0xFF;
 Delay_ms(5);
 for(i=0; i<256; i++){
 PORTB=i;
 Delay_ms(5);
 PORTD=~PORTD;
 }
 }

 TRISA=0x00;
 TRISB=0x0F;
 TRISC=0x00;
 TRISD=0x00;
 TRISE=0x00;




 PORTA=0x00;
 PORTB=PORTB & TRISB;
 PORTC=0x00;
 PORTD=0x00;
 PORTE=0x00;
 Sound_Init(&PORTA, 3);

 for(i=0; i<5; i++){
  PORTD.F7 = 1 ;
 Delay_ms( 500 /6);
  PORTD.F7 = 0 ;
  PORTD.F6 = 1 ;
 Delay_ms( 500 /6);
  PORTD.F6 = 0 ;
  PORTD.F5 = 1 ;
 Delay_ms( 500 /6);
  PORTD.F5 = 0 ;
  PORTD.F4 = 1 ;
 Delay_ms( 500 /6);
  PORTD.F4 = 0 ;
 }

 Delay_ms(3* 500 /4);
 score=0;

 while(position<45){

  PORTA.F2 = PORTA.F0 = PORTA.F1 = 0 ;


 score+=position;


 series[position]=Number(3,Seed);
 position++;

 for(i=0; i<position; i++){

 switch(series[i]){
 case  0 :
  PORTD.F5 = 1 ;
 SOUND= 1400 ;
 break;

 case  1 :
  PORTD.F6 = 1 ;
 SOUND= 1200 ;
 break;

 case  2 :
  PORTD.F7 = 1 ;
 SOUND= 1000 ;
 break;

 case  3 :
  PORTD.F4 = 1 ;
 SOUND= 1600 ;
 break;
 }
 Sound_Play(SOUND,  500 /3);
 Delay_ms( 500 /3);
  PORTD.F6 = PORTD.F7 = PORTD.F5 = PORTD.F4 = 0 ;
 Delay_ms( 500 /3);
 }

 position_=0;


 while(position_<position){
 inp=5;
 if(Input( 0 )){
  PORTD.F5 = 1 ;
 inp= 0 ;
 SOUND= 1400 ;
 }
 if(Input( 2 )){
  PORTD.F7 = 1 ;
 inp= 2 ;
 SOUND= 1000 ;
 }
 if(Input( 1 )){
  PORTD.F6 = 1 ;
 inp= 1 ;
 SOUND= 1200 ;
 }
 if(Input( 3 )){
  PORTD.F4 = 1 ;
 inp= 3 ;
 SOUND= 1600 ;
 }
 if(inp<5){

  PORTA.F0 = PORTA.F1 = 0 ;

 if(inp==series[position_]){
 position_++;
 Sound_Play(SOUND,  500 /3);
 if(position_==position){
  PORTA.F0 = 1 ;
 }
 Delay_ms( 500 /3);
  PORTD.F6 = PORTD.F7 = PORTD.F5 = PORTD.F4 = 0 ;
 }
 else{
 position=0;
 Sound_Play(300,  500 );
  PORTA.F1 = 1 ;
 Delay_ms(4* 500 /3);
  PORTA.F1 = 0 ;
  PORTD.F6 = PORTD.F7 = PORTD.F5 = PORTD.F4 = 0 ;
 Delay_ms(2* 500 );
 asm{
 reset;
 }
 break;
 }
 }
 }
 Sound_Play(800,  500 /6);
 Sound_Play(900,  500 /6);
 Delay_ms( 500 /3);
 }


 while(1){
  PORTA.F0 = 1 ;
 Delay_ms(200);
  PORTA.F0 = 0 ;
  PORTA.F2 = 1 ;
 Delay_ms(200);
  PORTA.F2 = 0 ;
 }
}

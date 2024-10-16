//the code reads the values of light and force to trigger a command of shutting the door close when needed only once in every storm.

#define F_CPU 8000000UL
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

void initUART ()
{            
	UBRRH = 0 ; // high byte of UBRR
	UBRRL = 103 ;// low byte of UBRR
	UCSRC = 0x86 ;// Mode
	UCSRB = 0x18 ;// enable RX and TX
}

void sendByte(char x)
{
   while((UCSRA & 0x20) == 0)	;
	UDR = x;
}


void print_decimal(unsigned char x)
{
	char h, t;
	h = x / 100;
	x = x - h * 100;
	t = x / 10;
	x = x - t * 10;
	if (h > 0) {
		sendByte(h + '0');
		sendByte(t + '0');
		sendByte(x + '0');
		} else if (t > 0) {
		sendByte(t + '0');
		sendByte(x + '0');
	} else
	sendByte(x + '0');
}


void print_string(char *s)
{
	int i;
	for (i = 0; s[i] != '\0'; i++)
	sendByte(s[i]);
}


void print_newline()
{
	sendByte(10);
	sendByte(13);
}

int main()
{
	unsigned char force;
	unsigned char light;
        DDRB= 0XFF;
	
    initUART();
	
	
	while (1)
	{
	
	//read the force value
	ADCSRA = 0x86;
        ADMUX = 0x20;
        ADCSRA |= (1 << ADSC);
        while ((ADCSRA & (1 << ADIF)) == 0);
        force = ADCH;
	
	//read the light value
	ADCSRA=0x86; 
	ADMUX=0x25 ;   //left j, 00 ref, ADC5
	ADCSRA |= (1<<ADSC);	//  start the conversion
	while((ADCSRA&(1<<ADIF))==0);	//  wait until conversion is completed
		light = ADCH ;
		

	   /*print_string("light = ");
	   print_decimal(light);
	   print_newline();
	   print_string("force = ");
	   print_decimal(force);
	   print_newline(); 
	   */
	
	if (force<70)  //effecint for night rain 
	{ PORTB  |= (1<<0);
	   print_string("it's raining outside! ");
		print_newline();
		print_string("approximatly the precipitation rate is:");
	   print_decimal(force*7.143);
	   print_newline();
	   _delay_ms(5000);
	   PORTB  &=~ (1<<0);
	   while (force > 70) {
  // Read the force value again inside the loop
  ADCSRA = 0x86;
  ADMUX = 0x20;
  ADCSRA |= (1 << ADSC);
  while ((ADCSRA & (1 << ADIF)) == 0);
  force = ADCH;
};
	   }
	 else if  ((force<100)&&(light>40)&&(light<200)) //for day time and predicting the clouds and gloomy windy weather
	{
	   PORTB  |= (1<<0);
	   print_string("the weather is gloomy outside! ");
		print_newline();
	   _delay_ms(5000);
	   PORTB  &=~ (1<<0);
	   
	   
	   while((force<100)&&(light>40)&&(light<200))
	   
	   
  // Read the force,light value again inside the loop
 { ADCSRA = 0x86;
  ADMUX = 0x20;
  ADCSRA |= (1 << ADSC);
  while ((ADCSRA & (1 << ADIF)) == 0);
  force = ADCH;
  
  ADCSRA=0x86; 
ADMUX=0x25 ;   //left j, 00 ref, ADC5
ADCSRA |= (1<<ADSC);	//  start the conversion
while((ADCSRA&(1<<ADIF))==0);	//  wait until conversion is completed
light = ADCH ; };

	   }
	
			
	}
	return 0;
}

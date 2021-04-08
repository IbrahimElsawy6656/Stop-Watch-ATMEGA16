#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

unsigned char time[6];
unsigned char bit = 0;
unsigned char bit2 = 0;

void Timer1_init_Compare_Mode(void)
{
	TCCR1B |= (1<<CS12) | (1<<CS10) | (1<<WGM12);
	TCNT1=0;
	OCR1A = 977;
	TIMSK |= (1<<OCIE1A);
}

void Int0_init()
{
	    DDRD  &= (~(1<<PD2));
	    PORTD |= (1<<PD2);
		GICR  |= (1<<INT0);
		MCUCR |= (1<<ISC01);
}

void Int1_init()
{
	    DDRD  &= (~(1<<PD3));
		GICR  |= (1<<INT1);
		MCUCR |= (1<<ISC11) | (1<<ISC10);
}

void Int2_init()
{
	        DDRB  &= (~(1<<PB2));
		    PORTB |= (1<<PB2);
		    GICR  |= (1<<INT2);
		    MCUCSR &= ~(1<<ISC2);
}

ISR(TIMER1_COMPA_vect)
{
	time[0]++;

    for(bit2 = 0; bit2 < 5 ; bit2 = bit2 + 2)
    {
    	if(time[bit2] == 10)
    	{
    		time[bit2] = 0;
    		time[bit2+1]++;
    	}
    }
    for(bit2 = 1; bit2 < 5 ; bit2 = bit2 + 2)
        {
        	if(time[bit2] == 6)
        	{
        		time[bit2] = 0;
        		time[bit2+1]++;
        	}
        }
}

ISR(INT0_vect)
{

	bit2 = 0;
    for(; bit2 < 5 ; bit2++)
    {
    	time[bit2] = 0;
    }
}

ISR(INT1_vect)
{
	TCCR1B &= ~(1<<CS12) & ~(1<<CS10);
}

ISR(INT2_vect)
{
	TCCR1B |= (1<<CS12) | (1<<CS10) | (1<<WGM12);
}

int main(void)
{
	SREG |= (1<<7);

	Int0_init();
	Int1_init();
	Int2_init();

	DDRA |= 0x3F;
	PORTA &= 0xC0;

	DDRC |= 0x0F;
    PORTC &= 0xF0;

    Timer1_init_Compare_Mode();
    while(1)
    {
    	for(bit = 0; bit < 6 ; bit++)
    	{
        PORTA |= (1<<bit);
        PORTC = time[bit];
        _delay_us(10);
        PORTA &= ~(1<<bit);
    	}
    }
}

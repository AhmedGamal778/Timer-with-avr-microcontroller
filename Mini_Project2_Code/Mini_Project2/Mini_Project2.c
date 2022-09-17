#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
unsigned char g_seconds=0,g_ten_seconds=0,g_minutes=0,g_ten_minutes=0,g_hours=0,g_ten_hours=0;


void increment_seconds();
void increment_ten_seconds();
void increment_minutes();
void increment_ten_minutes();
void increment_hours();
void increment_ten_hours();

void display (void);
void display_seconds();
void display_ten_seconds();
void display_minutes();
void display_ten_minutes();
void display_hours();
void display_ten_hours();

void intialize_timer ();
void initialize_ports (void);
void initialize_interrupts(void);

void pause (void);
void resume (void);
void reset (void);

ISR (INT0_vect)
{
	if (~(PIND|(1<<2)))
	{
		_delay_ms(30);
		if (~(PIND|(1<<2)))
		{
			reset();
		}
	}
}
ISR (INT1_vect)
{
	if ((PIND|(1<<3)))
	{
		_delay_ms(30);
		if ((PIND|(1<<3)))
		{
			pause();
		}
	}
}
ISR (INT2_vect)
{
	if ((PINB|(1<<2)))
	{
		_delay_ms(30);
		if ((PINB|(1<<2)))
		{
			resume();
		}
	}
}
ISR(TIMER1_COMPA_vect)
{
	increment_seconds();
}


int main (void)
{
	intialize_timer ();
	initialize_interrupts();
	initialize_ports();

    while (1)
    {
    	display();
    }

}

void intialize_timer ()
{
	TCCR1A = (1<<3); // ENABLE CPMPARE A
	TCCR1B = (1<<2) | (1) | (1<<3); // FOR CLOCK PRESCALLING 1024 AND CTC
	TIMSK = (1<<4); /* Enable Timer1 Compare A Interrupt */
	TCNT1=0;
	OCR1A=976;
	SREG |= (1<<7);
}
void initialize_ports (void)
{
	DDRC |= 0x0f;
	PORTC|=0x00;
	DDRA |= 0x3f;
	PORTA|=0x00;

	DDRD &= ~(1<<2);	//INT0
	PORTD|=(1<<2);

	DDRD&= ~(1<<3);	//INT1

	DDRB&= ~(1<<2);	//INT2
	PORTB|=(1<<2);

}
void initialize_interrupts(void)
{

	MCUCR|=(1<<ISC01)|(1<<ISC11)|(1<<ISC10);
	GICR|=(1<<7)|(1<<6)|(1<<5);
}

void reset (void)
{
	TCNT1=0;
	g_seconds=0;
	g_ten_seconds=0;
	g_minutes=0;
	g_ten_minutes=0;
	g_hours=0;
	g_ten_hours=0;
}
void pause (void)
{
	TCCR1B&= ~(1 | (1<<1) | (1<<2));
}
void resume (void)
{
	TCCR1B|= (1) | (1<<2);
}


void increment_seconds()
{
	if (g_seconds==9)
	{
		g_seconds=0;
		increment_ten_seconds();
	}
	else
		g_seconds++;
}
void increment_ten_seconds()
{
	if (g_ten_seconds==5)
	{
		g_ten_seconds=0;
		increment_minutes();
	}
	else
		g_ten_seconds++;
}
void increment_minutes()
{
	if (g_minutes==9)
		{
		g_minutes=0;
		increment_ten_minutes();
		}
		else
			g_minutes++;
}
void increment_ten_minutes()
{
	if (g_ten_minutes==5)
			{
			g_ten_minutes=0;
			increment_hours();
			}
			else
				g_ten_minutes++;
}
void increment_hours()
{
	if (g_hours==9)
			{
			g_hours=0;
			increment_ten_hours();
			}
			else
				g_hours++;
}
void increment_ten_hours()
{
	if (g_ten_hours==9)
			{
			reset();
			}
			else
				g_ten_hours++;
}
void display (void)
{
	 display_seconds();
	 display_ten_seconds();
	 display_minutes();
	 display_ten_minutes();
	 display_hours();
	 display_ten_hours();
}
void display_seconds()
{
	PORTA=1;
	PORTC=g_seconds;
	 _delay_ms(1);
	PORTA=0;
}
void display_ten_seconds()
{
	PORTA=2;
	PORTC=g_ten_seconds;
	 _delay_ms(1);
	PORTA=0;
}
void display_minutes()
{
	PORTA=4;
	PORTC=g_minutes;
	 _delay_ms(1);
	PORTA=0;
}
void display_ten_minutes()
{
	PORTA=8;
	PORTC=g_ten_minutes;
	 _delay_ms(1);
	PORTA=0;
}
void display_hours()
{
	PORTA=16;
	PORTC=g_hours;
	 _delay_ms(1);
	PORTA=0;
}
void display_ten_hours()
{
	PORTA=32;
	PORTC=g_ten_hours;
	 _delay_ms(1);
	PORTA=0;
}

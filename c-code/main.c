#include<avr/io.h>
#include<avr/interrupt.h>
//#include<stdlib.h>
#define F_CPU 12000000UL  
#define LED_1ON 0x01
#define LED_1OFF 0x11
#define LED_2ON 0x02
#define LED_2OFF 0x1A
#define LED_3ON 0x03
#define LED_3OFF 0x1B
#define LED_4ON 0x04
#define LED_4OFF 0x1C
#define ADC_COMMAND 0x1D
#define  PORT_ON(port,pin) port |= (1<<pin)
#define  PORT_OFF(port,pin)  port &=  ~(1<<pin)
static void USART_init() 
{
    #undef BAUD  
    #define BAUD 9600               
    #include <util/setbaud.h>

    UBRRH = UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;
    #if USE_2X
    UCSRA |= (1 << U2X);
    #else
    UCSRA &= ~(1 << U2X);
    #endif

    /* enable rx and tx */
    UCSRB = (1<<RXEN) | (1<<TXEN);
}

uint8_t USART_get_char()
{
    while(!(UCSRA & (1<<RXC))) 
        ;
    
    return UDR;
}

void USART_send_char(uint8_t data)
{
    while(!(UCSRA & (1<<UDRE)))
        ;

    UDR = data;
}

int main(void)
{
  //DDRC = 0xff;
    PORTC = 1<<PC0;
  uint8_t command;
    
  USART_init();
  
  //unsigned  int adc_value;	// Variable to hold ADC  result

  //configure the ADC   
  //Enable interrupt function in ADC

  //8-bit otr 10-bit results
  //Enable a prescaler -determined by internel/external clock
 //1200/5=200 120/2 = 60
  //turn on the ADC features
   DDRD=0xff;	// Set Port D for LED output
 //  DDRD = 1<< PD7;
   PORTD  = 0x00;
   ADCSRA |= 1<<ADPS2;
   ADCSRA |= 1<<ADPS1;
   ADCSRA |= 1<<ADPS0;
   ADMUX |= 1<<ADLAR;
   ADMUX |= 1<<REFS0;
   ADCSRA |= 1<<ADIE;
   ADCSRA |= 1<<ADEN;
   ADCSRA |= 1<<ADFR;
  // sei();
   ADCSRA |= 1<<ADSC; 
  //start first convertion
  //Enable the global interrupt
  unsigned int adc_value;
  while(1){
    adc_value = ADCH;
//    itoa(ADCH, adc_value, 10);
    command = USART_get_char();
    if (command == ADC_COMMAND)
        {
            USART_send_char(adc_value);
        }
    if (command == LED_1ON)
        {
           PORT_ON(PORTD,4); 
        }  
    else if(command == LED_1OFF)
        {
           PORT_OFF(PORTD,4);
        }
    if(command == LED_2ON)
        {
            PORT_ON(PORTD,5); 
        }
    else if(command == LED_2OFF)
        {
            PORT_OFF(PORTD,5);  
        }
    if (command == LED_3ON)
        {
           PORT_ON(PORTD,7);
        }
    else if (command == LED_3OFF)
        {
           PORT_OFF(PORTD,7);
        }
    if (command == LED_4ON)
        {
           PORT_ON(PORTD,6);;
        }
    else if (command == LED_4OFF)
        {
           PORT_OFF(PORTD,6);
        }	
}
return(0);
}


/*
* Harkka8.c
*
* Created: 20.3.2023 18.50.45
* Author : jyri_
*/

#include <avr/io.h>
#define FOSC  16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
//UBRRn Settings from Data sheet = 103


uint8_t Buffer[];
uint8_t read_index;
uint8_t write_index;
uint8_t number_of_elements;
uint8_t storedata;
uint8_t readdata;

USART_Init(unsigned int ubrr){
	
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver */
	UCSR0B = (1<<RXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (3<<UCSZ00);
}

// Interrupt handler
ISR(USART_RX_vect){
	
	// Writing UDR0 data to buffer
	if (number_of_elements < 9){
		Buffer[write_index++] = UDR0;
		number_of_elements++;
	}
	
}

int main(void)
{
	
	USART_Init(MYUBRR);
	
	while (1)
	{
		// Writing data to buffer
		if (number_of_elements < 9){
			Buffer[write_index++] = storedata;
			number_of_elements++;	
		}
		
		//Reading data from the buffer
		if(number_of_elements>0){
			readdata = Buffer[read_index++];
			number_of_elements--;
		}
	}
}
	


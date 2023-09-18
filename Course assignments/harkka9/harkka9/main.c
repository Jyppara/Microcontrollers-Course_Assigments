/*
 * harkka9.c
 *
 * Created: 28.3.2023 8.29.06
 * Author : jyri_
 */ 

#include <avr/io.h>

#define NUM_CHANNELS 8;

void initADC (){
	// enable AD-converter 
	ADCSRA |= (1<<ADEN);
	// use internal 1,1 volt ref voltage
	ADMUX |= (1<<REFS1) | (1<<REFS0);
	// Set prescaler to 128 
	ADCSRA |= (1<< ADPS2)|(1<< ADPS1)|(1<< ADPS0);
	// enable auto triggering
	ADCSRA |= (1<< ADATE);
	// result is right aligned by default
	}
	
void readAD(uint8_t channel_number, uint16_t *ad_result){
	//set the channel
	ADMUX |= channel_number;
	// start conversion by setting ADSC to 1
	ADCSRA |= (1<< ADSC);
		while (ADCSRA & (1<< ADSC));
		*ad_result = ADCL; // low byte of result
		*ad_result |= (ADCH << 8); // high byte of result
}

int main(void)
{
	uint16_t ad_result;
	initADC();

    while (1) 
    {
		for(uint8_t channel = 0; channel < NUM_CHANNELS; channel++){
			readAD(channel, &ad_result);
			printf("ADC channel %d result: %d\n", channel, ad_result);
		}
		
    }
}


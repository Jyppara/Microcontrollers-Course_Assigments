/*
 * week4.c
 *
 * Created: 23.1.2023 9.28.52
 * Author : jyri_
 */ 

#include <avr/io.h>



void aritmethic(short int loops){
	
	for(short int i = 0 ; i < loops; i++){
		
		short int j = 10 * 100;
		j = j/ 50;
		j = j - 3;
		j += 1;
	}
	
}
		

int main(void)
{
	
    while (1) 
    {
		aritmethic(1000);
    }
}


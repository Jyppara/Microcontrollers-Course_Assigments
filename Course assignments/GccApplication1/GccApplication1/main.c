/*
 * week4.c
 *
 * Created: 23.1.2023 9.28.52
 * Author : jyri_
 */ 

#include <avr/io.h>



void aritmethic( int loops){
	
	for( int i = 0 ; i < loops; i++){
		
		int j = 1;
		j -= 1;
		j -= 1;
		j += 1;
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


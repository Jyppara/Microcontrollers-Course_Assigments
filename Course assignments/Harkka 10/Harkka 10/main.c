/*
 * Harkka 10.c
 *
 * Created: 29.3.2023 14.50.44
 * Author : jyri_
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define TASK_MAX 3

struct {
	uint8_t task_number;
	int16_t delay;
	uint16_t interval;
	int8_t run;
}Task_list[TASK_MAX];

void Init_update(void)
{
	// timer 2
	TCCR2B |= (1<<CS21);				// normal mode, prescaler 8
	TIMSK2 |= (1<<TOIE2);				// Timer overflow interrupt enable
	sei();	// enable interrupts;
	
}
void update(void)
{
	int i;
	for(i=0;i<TASK_MAX;i++) {					// check every task
		if(Task_list[i].task_number != 0) {		// not an empty slot
			if(Task_list[i].delay == 0) {	    // time to run
				Task_list[i].run += 1;          // please run this
				if(Task_list[i].interval != 0)  // periodic task
				Task_list[i].delay = Task_list[i].interval;
			} else
			Task_list[i].delay -=1;			// decrement waiting time
		}
	}
}

#define OVF_MAX 49	// 49 gives 10 ms tick.
uint16_t overflows=0;
ISR (TIMER2_OVF_vect) {
	if(overflows++ >= OVF_MAX) {
		update();
		overflows = 0;
	}
}

void Add_task(uint8_t number, uint8_t task_number, int16_t delay, int16_t interval)
{
	Task_list[number].task_number = task_number;
	Task_list[number].delay= delay;
	Task_list[number].interval=interval;
}

// here you can define your own tasks
#define TASK_1 1

uint8_t a = 0;
void Task_1(void)
{
	// Toggle the state of pin 13 (PB5) to turn the LED on/off
	a++;
	if(a&0x01)
	PORTB |= 0x01;
	else
	PORTB &= 0xFE;
}


void Init_tasks()
{
	int i;
	DDRB=0xff;
	DDRB |= (1 << 5);
	for(i=0;i<TASK_MAX;i++)
	Task_list[i].task_number = 0;	// empty slots
	
	Add_task(0, TASK_1, 1, 9); // Start task 1 after 1 ticks
								// and then every second
}


void dispatcher()
{
	int i;
	for(i=0;i<TASK_MAX;i++) {                // check every task
		if(Task_list[i].task_number != 0) {  // not an empty slot
			if(Task_list[i].run > 0) {	     // time to run
				// Call the task function. This can be done more elegantly using function pointers
				if(Task_list[i].task_number == TASK_1) Task_1();
				Task_list[i].run -= 1;      // decrease run request
			}
		}
	}
}


int main(void)
{
	Init_tasks();	// create 3 tasks
	Init_update();	// start timer and enable OVF interrupt
	while (1)
	{
		dispatcher();	// run tasks that are ready for execution
		//#define POWER_SAVE
		#ifdef POWER_SAVE
		set_sleep_mode(SLEEP_MODE_IDLE); // set the sleep-mode you want
		cli(); // disable interrupts
		sleep_enable(); // set SE-bit
		sei(); // enable interrupts
		sleep_cpu(); // SLEEP-instruction
		// entry-point after wake-up
		sleep_disable(); // reset SE-bit
		sei(); // enable interrupts
		#endif
	}
}



#include "stm32f30x.h"                  // Device header
#include "cadc_stm32f3.h"
#include "ctimers_stm32f3.h"

int main(){
	adc_timer2_init(10000);
	timer15_init(1000,1);
	timer15_enableIRQ();
	timer15_start();
	
	timer16_init(2000,1);
	timer16_enableIRQ();
	timer16_start();
	
	timer17_init(4000,1);
	timer17_enableIRQ();
	timer17_start();
	
	while(1){
	}
}

int timer15_counter = 0;
void timer15_callback(void){
	timer15_counter++;
}

int timer16_counter = 0;
void timer16_callback(void){
	timer16_counter++;
}
int timer17_counter = 0;
void timer17_callback(void){
	timer17_counter++;
}

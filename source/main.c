#include "stm32f30x.h"                  // Device header
#include "cadc_stm32f3.h"
#include "ctimers_stm32f3.h"

int main(){
	adc_timer2_init(10000);
	timer15_init(1000,1);
	timer15_enableIRQ();
	timer15_start();
	while(1){
	}
}

int timer15_counter = 0;
void timer15_callback(void){
	timer15_counter++;
}

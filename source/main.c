#include "stm32f30x.h"                  // Device header
#include "cadc_stm32f3.h"
#include "ctimers_stm32f3.h"

int main(){
	timer15_init(100,PERIOD_IN_MICROSECONDS);
	adc_injected(ADC_TRIGGER_TIMER15);
	timer15_start();
	
	while(1){
	}
}

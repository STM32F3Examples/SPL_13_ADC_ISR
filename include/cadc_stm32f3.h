#ifndef _CADC_STM32F3_H
#define _CADC_STM32F3_H

#define ADC_TRIGGER_NONE 0
#define ADC_TRIGGER_TIMER2 1
#define ADC_TRIGGER_TIMER6 2
#define ADC_TRIGGER_TIMER15 3
#define ADC_TRIGGER_TIMER16 4

#define DONT_USE_TRIGGER 0
#define USE_TRIGGER 1

#ifdef __cplusplus
extern "C" {
#endif
	
	void adc_injected(int adc_trigger);
	
#ifdef __cplusplus
}
#endif

#endif//_CADC_STM32F3_H

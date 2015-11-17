#ifndef _CADC_STM32F3_H
#define _CADC_STM32F3_H

#ifdef __cplusplus
extern "C" {
#endif
	
	void adc_timer2_init(int ts_in_us);
	void adc_timer2_start(void);
	void adc_timer2_stop(void);
	
	
	
#ifdef __cplusplus
}
#endif

#endif//_CADC_STM32F3_H

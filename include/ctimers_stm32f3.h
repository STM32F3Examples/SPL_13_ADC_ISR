#ifndef _CTIMERS_STM32F3_H
#define _CTIMERS_STM32F3_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	void timer15_init(uint16_t time, int use_us);
	void timer15_enableIRQ(void);
	void timer15_start(void);
	void timer15_stop(void);
	void timer15_callback(void);//To be implemeted by user
	
#ifdef __cplusplus
}
#endif

#endif//_CTIMERS_STM32F3_H

#ifndef _CTIMERS_STM32F3_H
#define _CTIMERS_STM32F3_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	void timer2_init(int time, int use_us);
	void timer2_enableIRQ(void);
	void timer2_start(void);
	void timer2_stop(void);
	void timer2_callback(void);//To be implemeted by user
	
	void timer15_init(uint16_t time, int use_us);
	void timer15_enableIRQ(void);
	void timer15_start(void);
	void timer15_stop(void);
	void timer15_callback(void);//To be implemeted by user
	
	void timer16_init(uint16_t time, int use_us);
	void timer16_enableIRQ(void);
	void timer16_start(void);
	void timer16_stop(void);
	void timer16_callback(void);//To be implemeted by user
	
	void timer17_init(uint16_t time, int use_us);
	void timer17_enableIRQ(void);
	void timer17_start(void);
	void timer17_stop(void);
	void timer17_callback(void);//To be implemeted by user
	
#ifdef __cplusplus
}
#endif

#endif//_CTIMERS_STM32F3_H

#include "ctimers_stm32f3.h"
#include "stm32f30x.h"                  // Device header

void timer2_init(int period,int use_us){
	int prescaler = 0;
	SystemCoreClockUpdate();
	
	prescaler = use_us ? ((SystemCoreClock/1000000)-1) : ((SystemCoreClock/1000)-1); //CountFreq=(CoreClock/Prescaler+1)
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_TimeBaseInitTypeDef myTimer;
	TIM_TimeBaseStructInit(&myTimer);
	myTimer.TIM_CounterMode=TIM_CounterMode_Down;
	myTimer.TIM_Prescaler=prescaler;
	myTimer.TIM_ClockDivision=TIM_CKD_DIV1;
	myTimer.TIM_Period=period;
	TIM_TimeBaseInit(TIM2,&myTimer);
	
	TIM_InternalClockConfig(TIM2);
}

void timer2_enableIRQ(void){		
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	NVIC_EnableIRQ(TIM2_IRQn);
}

void timer2_start(void){
	TIM_Cmd(TIM2,ENABLE);
}

void timer2_stop(void){
	TIM_Cmd(TIM2,DISABLE);
}

void __attribute__((weak)) timer2_callback(void){}

#ifdef __cplusplus
extern "C" {
#endif
void TIM2_IRQHandler(void){
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		timer2_callback();
	}
#ifdef __cplusplus
}
#endif


void timer15_init(uint16_t period,int use_us){
	int prescaler = 0;
	SystemCoreClockUpdate();
	
	prescaler = use_us ? ((SystemCoreClock/1000000)-1) : ((SystemCoreClock/1000)-1); //CountFreq=(CoreClock/Prescaler+1)
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15,ENABLE);
	TIM_TimeBaseInitTypeDef myTimer;
	TIM_TimeBaseStructInit(&myTimer);
	myTimer.TIM_CounterMode=TIM_CounterMode_Up;//TIM15 can only be up timer
	myTimer.TIM_Prescaler=prescaler;
	myTimer.TIM_ClockDivision=TIM_CKD_DIV1;
	myTimer.TIM_Period=(period-1);
	TIM_TimeBaseInit(TIM15,&myTimer);
	TIM_InternalClockConfig(TIM15);
}

void timer15_enableIRQ(void){		
	TIM_ITConfig(TIM15,TIM_IT_Update,ENABLE);
	NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
}
void timer15_start(void){
	TIM_Cmd(TIM15,ENABLE);
}

void timer15_stop(void){
	TIM_Cmd(TIM15,DISABLE);
}

void __attribute__((weak)) timer15_callback(void){}

#ifdef __cplusplus
extern "C" {
#endif
void TIM1_BRK_TIM15_IRQHandler(void){
		TIM_ClearITPendingBit(TIM15,TIM_IT_Update);
		timer15_callback();
	}
#ifdef __cplusplus
}
#endif

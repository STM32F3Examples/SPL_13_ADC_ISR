#include "stm32f30x.h"                  // Device header
/*Led PB13, Button PC13*/

void delay_ms(int delay_time);
void adc_init_injected(void);
void timer2_init(int period,int use_us);
void timer2_start(void);
void timer2_stop(void);

unsigned int adc_val[4];

int main(){
	SystemCoreClockUpdate();
	timer2_init(1000,1);
	adc_init_injected();
	ADC_StartInjectedConversion(ADC1);
	timer2_start();
	while(1){
	}
}

int adc_count = 0;
void ADC1_IRQHandler(void){
	ADC_ClearITPendingBit(ADC1, ADC_IT_JEOS);
	adc_val[0]=ADC_GetInjectedConversionValue(ADC1,ADC_InjectedSequence_1);
	adc_val[1]=ADC_GetInjectedConversionValue(ADC1,ADC_InjectedSequence_2);
	adc_count++;
}

void adc_init_injected(void){
	//Confiure pins PC0[AN6], PC1[AN7] for analog input operation
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
	GPIO_InitTypeDef myGPIO;
	GPIO_StructInit(&myGPIO);
	myGPIO.GPIO_Pin=(GPIO_Pin_1|GPIO_Pin_0);
	myGPIO.GPIO_Mode=GPIO_Mode_AN;
	GPIO_Init(GPIOC,&myGPIO);
	//Configure ADC
	RCC_ADCCLKConfig(RCC_ADC12PLLCLK_OFF);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_ADC12EN,ENABLE); 
	delay_ms(0xFFF);
	ADC_CommonInitTypeDef myADC_Comm;
	ADC_CommonStructInit(&myADC_Comm);
	myADC_Comm.ADC_Clock=ADC_Clock_SynClkModeDiv1;
	ADC_CommonInit(ADC1,&myADC_Comm);
	ADC_VoltageRegulatorCmd(ADC1,ENABLE);
	delay_ms(0xFFF);
	
	/*Initial calibration*/
	ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) != RESET);
	ADC_GetCalibrationValue(ADC1);
	
	ADC_InjectedInitTypeDef myADC;
	ADC_InjectedStructInit(&myADC);
	myADC.ADC_ExternalTrigInjecEventEdge=ADC_ExternalTrigInjecEventEdge_RisingEdge;
	myADC.ADC_ExternalTrigInjecConvEvent=ADC_ExternalTrigInjecConvEvent_2;//Start convertion on TIM2_OTRIG
	myADC.ADC_NbrOfInjecChannel=1;
	myADC.ADC_InjecSequence1=ADC_InjectedChannel_6;
	myADC.ADC_InjecSequence2=ADC_InjectedChannel_7;
	myADC.ADC_InjecSequence3=ADC_InjectedChannel_6;
	myADC.ADC_InjecSequence4=ADC_InjectedChannel_7;
	ADC_InjectedInit(ADC1,&myADC);
	ADC_InjectedChannelSampleTimeConfig(ADC1,ADC_InjectedChannel_6,ADC_SampleTime_7Cycles5);
	ADC_InjectedChannelSampleTimeConfig(ADC1,ADC_InjectedChannel_7,ADC_SampleTime_7Cycles5);
	
	//
	ADC_ITConfig(ADC1, ADC_IT_JEOS, ENABLE);
	NVIC_EnableIRQ(ADC1_IRQn);
	/* wait for ADRDY */
	ADC_Cmd(ADC1,ENABLE);
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));
}

void delay_ms(int delay_time){
	for(int i=0; i<delay_time; i++);
}

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
	TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_Update);//OTRIG : Update
	
	//TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	//NVIC_EnableIRQ(TIM2_IRQn);
}

void timer2_start(void){
	TIM_Cmd(TIM2,ENABLE);
}

void timer2_stop(void){
	TIM_Cmd(TIM2,DISABLE);
}

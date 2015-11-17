#include "cadc_stm32f3.h"
#include "ctimers_stm32f3.h"
#include "stm32f30x.h"                  // Device header

void adc_init_injected(int use_trigger,int trigger);

void adc_injected(int adc_trigger){
	uint32_t injected_trigger = 0;
	if(adc_trigger == ADC_TRIGGER_NONE){
		adc_init_injected(DONT_USE_TRIGGER,injected_trigger);
	}else{
		if(adc_trigger == ADC_TRIGGER_TIMER2){
			injected_trigger = ADC_ExternalTrigInjecConvEvent_2;
			TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_Update);//OTRIG : Update
		}else if(adc_trigger == ADC_TRIGGER_TIMER6){
			injected_trigger = ADC_ExternalTrigInjecConvEvent_14;
			TIM_SelectOutputTrigger(TIM6,TIM_TRGOSource_Update);//OTRIG : Update
		}else if(adc_trigger == ADC_TRIGGER_TIMER15){
			injected_trigger = ADC_ExternalTrigInjecConvEvent_15;
			TIM_SelectOutputTrigger(TIM15,TIM_TRGOSource_Update);//OTRIG : Update
		}
		adc_init_injected(USE_TRIGGER,injected_trigger);
	}
	ADC_StartInjectedConversion(ADC1);
}

void adc_init_injected(int use_trigger, int trigger){
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
	ADC_CommonInitTypeDef myADC_Comm;
	ADC_CommonStructInit(&myADC_Comm);
	myADC_Comm.ADC_Clock=ADC_Clock_SynClkModeDiv1;
	ADC_CommonInit(ADC1,&myADC_Comm);
	ADC_VoltageRegulatorCmd(ADC1,ENABLE);
	
	/*Initial calibration*/
	ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) != RESET);
	ADC_GetCalibrationValue(ADC1);
	
	ADC_InjectedInitTypeDef myADC;
	ADC_InjectedStructInit(&myADC);

	myADC.ADC_ExternalTrigInjecEventEdge= (use_trigger) ? ADC_ExternalTrigInjecEventEdge_RisingEdge : ADC_ExternalTrigInjecEventEdge_None; 
	//Connect timer with adc
	myADC.ADC_ExternalTrigInjecConvEvent=trigger;//Start convertion on TIM2_OTRIG
	
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

int adc_count = 0;
unsigned int adc_val[4];

void ADC1_IRQHandler(void){
	ADC_ClearITPendingBit(ADC1, ADC_IT_JEOS);
	adc_val[0]=ADC_GetInjectedConversionValue(ADC1,ADC_InjectedSequence_1);
	adc_val[1]=ADC_GetInjectedConversionValue(ADC1,ADC_InjectedSequence_2);
	adc_count++;
}

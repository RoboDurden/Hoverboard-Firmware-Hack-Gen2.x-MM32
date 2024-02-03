#include "mm32_device.h"                // Device header
#include "RTE_Components.h"             // Component selection
#include "hal_gpio.c"
#include "hal_gpio.h"
#include "hal_rcc.h"
#include "delay.h"
#include "pinout.h"


//#define HALL2LED

s32 main(void){
	//enable gpio clock
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOC, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOD, ENABLE);
	//LED output
	GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin = LEDRPIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(LEDRPORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = LEDGPIN;
	GPIO_Init(LEDGPORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = LEDBPIN;
	GPIO_Init(LEDBPORT, &GPIO_InitStruct);
	//latch output
	GPIO_InitStruct.GPIO_Pin = LATCHPIN;
	GPIO_Init(LATCHPORT, &GPIO_InitStruct);
	//bz output
	GPIO_InitStruct.GPIO_Pin = BZPIN;
	GPIO_Init(BZPORT, &GPIO_InitStruct);
	//hall input
	GPIO_InitStruct.GPIO_Pin = HALLAPIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(HALLAPORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = HALLBPIN;
	GPIO_Init(HALLBPORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = HALLCPIN;
	GPIO_Init(HALLCPORT, &GPIO_InitStruct);
	//button input
	GPIO_InitStruct.GPIO_Pin = BTNPIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_FLOATING;
	GPIO_Init(BTNPORT, &GPIO_InitStruct);
	//systick config
	DELAY_Init();
	//latch on power
	GPIO_WriteBit(LATCHPORT, LATCHPIN, 1);
	//wait while release button
	while(GPIO_ReadInputDataBit(BTNPORT, BTNPIN)) {
		GPIO_WriteBit(BZPORT, BZPIN, 1);
		DELAY_Ms(1);
		GPIO_WriteBit(BZPORT, BZPIN, 0);
		DELAY_Ms(1);
	}
	//prevent turning back off imidiately
	DELAY_Ms(5);	
  while(1) {
		#ifdef HALL2LED
		//rotating led
    GPIO_WriteBit(LEDRPORT, LEDRPIN, GPIO_ReadInputDataBit(HALLAPORT, HALLAPIN));
		GPIO_WriteBit(LEDGPORT, LEDGPIN, GPIO_ReadInputDataBit(HALLBPORT, HALLBPIN));
		GPIO_WriteBit(LEDBPORT, LEDBPIN, GPIO_ReadInputDataBit(HALLCPORT, HALLCPIN));
		DELAY_Ms(5);		
		#else
		//blink in sequence
		GPIO_WriteBit(LEDRPORT, LEDRPIN, 1);
		GPIO_WriteBit(LEDBPORT, LEDBPIN, 0);
		DELAY_Ms(500);
		GPIO_WriteBit(LEDGPORT, LEDGPIN, 1);
		GPIO_WriteBit(LEDRPORT, LEDRPIN, 0);
		DELAY_Ms(500);
		GPIO_WriteBit(LEDBPORT, LEDBPIN, 1);
		GPIO_WriteBit(LEDGPORT, LEDGPIN, 0);
		DELAY_Ms(500);
		#endif
		//button press for shutdown
		if(GPIO_ReadInputDataBit(BTNPORT, BTNPIN)){
			//wait for release
			for(int i=0;i<3;i++){
			GPIO_WriteBit(BZPORT, BZPIN, 1);
		  DELAY_Ms(50);
		  GPIO_WriteBit(BZPORT, BZPIN, 0);
		  DELAY_Ms(50);
			}
			while(GPIO_ReadInputDataBit(BTNPORT, BTNPIN)) {
		    __nop();
	    }
			//last line to ever be executed
			GPIO_WriteBit(LATCHPORT, LATCHPIN, 0);
		}
  }
	
}


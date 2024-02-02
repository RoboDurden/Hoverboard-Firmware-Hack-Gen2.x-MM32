#include "mm32_device.h"                // Device header
#include "RTE_Components.h"             // Component selection
#include "hal_gpio.c"
#include "hal_gpio.h"
#include "hal_rcc.h"
#include "delay.h"

#define LEDRPIN GPIO_Pin_12
#define LEDGPIN GPIO_Pin_2
#define LEDBPIN GPIO_Pin_3
#define LEDRPORT GPIOA
#define LEDGPORT GPIOD
#define LEDBPORT GPIOD

#define HALLAPIN GPIO_Pin_13
#define HALLBPIN GPIO_Pin_14
#define HALLCPIN GPIO_Pin_15
#define HALLAPORT GPIOC
#define HALLBPORT GPIOC
#define HALLCPORT GPIOC

#define HALL2LED

s32 main(void){
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOC, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin = LEDRPIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(LEDRPORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = LEDGPIN;
	GPIO_Init(LEDGPORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = LEDBPIN;
	GPIO_Init(LEDBPORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = HALLAPIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(HALLAPORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = HALLBPIN;
	GPIO_Init(HALLBPORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = HALLCPIN;
	GPIO_Init(HALLCPORT, &GPIO_InitStruct);
	
	DELAY_Init();
  while(1) {
		#ifdef HALL2LED
    GPIO_WriteBit(LEDRPORT, LEDRPIN, GPIO_ReadInputDataBit(HALLAPORT, HALLAPIN));
		GPIO_WriteBit(LEDGPORT, LEDGPIN, GPIO_ReadInputDataBit(HALLBPORT, HALLBPIN));
		GPIO_WriteBit(LEDBPORT, LEDBPIN, GPIO_ReadInputDataBit(HALLCPORT, HALLCPIN));
		DELAY_Ms(2);		
		#else
		GPIO_WriteBit(LEDRPORT, LEDRPIN, 1);
		GPIO_WriteBit(LEDBPORT, LEDBPIN, 0);
		DELAY_Ms(200);
		GPIO_WriteBit(LEDGPORT, LEDGPIN, 1);
		GPIO_WriteBit(LEDRPORT, LEDRPIN, 0);
		DELAY_Ms(200);
		GPIO_WriteBit(LEDBPORT, LEDBPIN, 1);
		GPIO_WriteBit(LEDGPORT, LEDGPIN, 0);
		DELAY_Ms(200);
		#endif
  }
	
}


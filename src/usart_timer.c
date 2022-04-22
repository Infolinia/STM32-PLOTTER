#include "stm32f0xx.h"
#include "stm32f0xx_nucleo.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <ctype.h>
#include "define.h"

void Pin_AxisXConfig() {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = axisXstep | axisXdir;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(axisXpin, &gpio);
}

void Pin_AxisYConfig() {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = axisYstep | axisYdir;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(axisYpin, &gpio);
}

void Pin_Config() {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = GPIO_Pin_5;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);
}

void Nvic_Timer2_Config() {
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = TIM2_IRQn;
	nvic.NVIC_IRQChannelPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

void Nvic_Timer3_Config() {
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = TIM3_IRQn;
	nvic.NVIC_IRQChannelPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

void Timer2_Config() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef tim;
	TIM_TimeBaseStructInit(&tim);
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_Prescaler = 48 - 1;
	tim.TIM_Period = 1000 - 1;
	TIM_TimeBaseInit(TIM2, &tim);
	TIM_Cmd(TIM2, ENABLE);
}

void Timer3_Config() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitTypeDef tim;
	TIM_TimeBaseStructInit(&tim);
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_Prescaler = 48 - 1;
	tim.TIM_Period = 1000 - 1;
	TIM_TimeBaseInit(TIM3, &tim);
	TIM_Cmd(TIM3, ENABLE);
}
void USART2_Init(void) {
	USART_InitTypeDef usart;
	GPIO_InitTypeDef gpio;
	NVIC_InitTypeDef nvic;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);

	gpio.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_10;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_3;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &gpio);

	nvic.NVIC_IRQChannel = USART2_IRQn;
	nvic.NVIC_IRQChannelPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	usart.USART_BaudRate = 9600;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2, &usart);

	USART_Cmd(USART2, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

}

int parametr(char tab[]) {
	char t[10];
	char t2[10];
	sscanf(tab, "%s" "%s" "%s", t, t2, t);
	for(int i = 0; i< strlen(t2); i++){
		if(!isdigit(t2[i]))
			return -1;
	}
	memset(tab, 0, strlen(tab));
	return atoi(t2);
}

char *toString(int x) {
	char tab[10];
	char *str = &tab[0];
	sprintf(str, "%d", x);
	return str;
}

char *conkat(char *a, char *b, char *c){
	char str[120];
	char *p = &str[0];
	sprintf(str,"%s %s %s", a, b, c);
	return  p;
}
char *floatToString(float x){
	char str[33];
	char * p = &str[0];
	sprintf(str, "%0.5f", x);
	return p;
}



#ifndef USART_TIMER_H_
#define USART_TIMER_H_
#include "define.h"

void sendString(char tmp[]);
void checkCommand(char *tablica);
void usartGetValue(void);
void USART2_Init(void);
void USART2_IRQHandler(void);

char *conkat(char *a, char *b, char *c);
int parametr(char tab[]);
void *toString(int x);
char *floatToString(float x);

void axisXmove(int stepsX, float vXMin, float vXMax,int accelXTime, int decelXTime,  int direction);
void axisYmove(int stepsX, float vXMin, float vXMax,int accelXTime, int decelXTime,  int direction);
void Pin_AxisXConfig();
void Pin_AxisYConfig();
void Pin_Config();
void Nvic_Timer2_Config();
void Nvic_Timer3_Config();
void Timer2_Config();
void Timer3_Config();

#endif

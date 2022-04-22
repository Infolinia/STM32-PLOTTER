#include "stm32f0xx.h"
#include "stm32f0xx_nucleo.h"
#include "usart_timer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// usart bufor rx / tx //
char odbieranie[128];
int od_poczatek = 0;
int od_wolny = 0;

char wysylanie[512];
int wy_poczatek = 0;
int wy_wolny = 0;
// koniec usart bufor rx / tx //


// oœ X //
volatile int times_X_ms = 0;
volatile int stepsX = 100;
float vXMin = 1.0;
float vXMax = 10.0;

int accelXTime = 3;
int decelXTime = 3;

double accelerationX = 0;
double decelerationX = 0;

int stepsXAccel = 0;
int stepsXDecel = 0;
int middleXSteps = 0;

volatile float currentXSpeed = 1.0;
volatile int stepAxisX = 1;
volatile int axisXEnabled = OFF;
// koniec oœ X //

// oœ Y //
volatile int times_Y_ms = 0;
volatile int stepsY = 100;
float vYMin = 1.0;
float vYMax = 10.0;

int accelYTime = 2;
int decelYTime = 2;

double accelerationY = 0.0;
double decelerationY = 0.0;

int stepsYAccel = 0;
int stepsYDecel = 0;
int middleYSteps = 0;

volatile float currentYSpeed = 0.0;
volatile int stepAxisY = 1;
volatile int axisYEnabled = OFF;
// koniec oœ Y //


volatile uint32_t timer_ms = 0;

void SysTick_Handler()
{
    if (timer_ms) {
        timer_ms--;
    }
}

void delay_ms(int time)
{
    timer_ms = time;
    while(timer_ms > 0){};
}

void sendString(char tmp[]) {
	volatile int index = wy_wolny;
	for (int i = 0; i < strlen(tmp); i++) {
		wysylanie[index] = tmp[i];
		index++;
		if (index >= 512)
			index = 0;
	}
	__disable_irq();
	if ((wy_poczatek == wy_wolny) && (USART_GetFlagStatus(USART2,USART_FLAG_TXE) == SET)) {
		wy_wolny = index;
		USART_SendData(USART2, wysylanie[wy_poczatek]);
		wy_poczatek++;
		if (wy_poczatek >= 512)
			wy_poczatek = 0;
		USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	} else {
		wy_wolny = index;
	}
	__enable_irq();
}

void checkCommand(char *tablica) {
	if (strncmp(tablica, "setXSteps ", 10) == 0) {
			if ((parametr(tablica) >= minSteps && parametr(tablica) <= maxSteps) && axisXEnabled == OFF) {
				stepsX = parametr(tablica);
				sendString(conkat("INFO: Axis X, steps = ", toString(stepsX), "\r\n"));
			} else
				sendString("ERROR: Invalid steps range <10:230>,\r\n or engine X is currently enabled.\r\n");
	} else if (strncmp(tablica, "setXAccel ", 10) == 0) {
		if ((parametr(tablica) >= minAccelTime && parametr(tablica) <= maxAccelTime) && axisXEnabled == OFF) {
			accelXTime = parametr(tablica);
			sendString(conkat("INFO: Axis X, accel time = ", toString(accelXTime), "\r\n"));
		} else
			sendString("ERROR: Invalid accel time range <1:5>,\r\n or engine X is currently enabled.\r\n");
	} else if (strncmp(tablica, "setXDecel ", 10) == 0) {
		if ((parametr(tablica) >= minDecelTime && parametr(tablica) <= maxDecelTime) && axisXEnabled == OFF) {
			decelXTime = parametr(tablica);
			sendString(conkat("INFO: Axis X, decel time = ", toString(decelXTime), "\r\n"));
		} else
			sendString("ERROR: Invalid decel time range <1:5>,\r\n or engine X is currently enabled.\r\n");
	} else if (strncmp(tablica, "setXSpeedMin ", 13) == 0) {
		if ((parametr(tablica) >= speedMin && parametr(tablica) <= speedMax) && axisXEnabled == OFF) {
			vXMin = parametr(tablica);
			sendString(conkat("INFO: Axis X, speed min = ", toString(vXMin), "\r\n"));
		} else
			sendString("ERROR: Invalid speed min range <1:10>,\r\n or engine X is currently enabled.\r\n");
	} else if (strncmp(tablica, "setXSpeedMax ", 13) == 0) {
		if ((parametr(tablica) >= speedMin && parametr(tablica) <= speedMax) && axisXEnabled == OFF) {
			vXMax = parametr(tablica);
			sendString(conkat("INFO: Axis X, speed max = ", toString(vXMax), "\r\n"));
		} else
			sendString("ERROR: Invalid speed max range <1:10>,\r\n or engine X is currently enabled.\r\n");
	}else if (strncmp(tablica, "setYSteps ", 10) == 0) {
		if ((parametr(tablica) >= minSteps && parametr(tablica) <= maxSteps) && axisYEnabled == OFF) {
			stepsY = parametr(tablica);
			sendString(conkat("INFO: Axis Y, steps = ", toString(stepsY), "\r\n"));
		} else
			sendString("ERROR: Invalid steps range <10:230>,\r\n or engine Y is currently enabled.\r\n");
	} else if (strncmp(tablica, "setYAccel ", 10) == 0) {
		if ((parametr(tablica) >= minAccelTime && parametr(tablica) <= maxAccelTime) && axisYEnabled == OFF) {
			accelYTime = parametr(tablica);
			sendString(conkat("INFO: Axis Y, accel time = ", toString(accelYTime), "\r\n"));
		} else
			sendString("ERROR: Invalid accel time range <1:5>,\r\n or engine Y is currently enabled.\r\n");
	} else if (strncmp(tablica, "setYDecel ", 10) == 0) {
		if ((parametr(tablica) >= minDecelTime && parametr(tablica) <= maxDecelTime) && axisYEnabled == OFF) {
			decelYTime = parametr(tablica);
			sendString(conkat("INFO: Axis Y, decel time = ", toString(decelYTime), "\r\n"));
		} else
			sendString("ERROR: Invalid decel time range <1:5>,\r\n or engine Y is currently enabled.\r\n");
		} else if (strncmp(tablica, "setYSpeedMin ", 13) == 0) {
			if ((parametr(tablica) >= speedMin && parametr(tablica) <= speedMax) && axisYEnabled == OFF) {
				vYMin = parametr(tablica);
				sendString(conkat("INFO: Axis Y, speed min = ", toString(vYMin), "\r\n"));
			} else
				sendString("ERROR: Invalid speed min range <1:10>,\r\n or engine Y is currently enabled.\r\n");
		} else if (strncmp(tablica, "setYSpeedMax ", 13) == 0) {
			if ((parametr(tablica) >= speedMin && parametr(tablica) <= speedMax) && axisYEnabled == OFF) {
				vYMax = parametr(tablica);
				sendString(conkat("INFO: Axis Y, speed max = ", toString(vYMax), "\r\n"));
		} else
			sendString("ERROR: Invalid speed max range <1:10>,\r\n or engine Y is currently enabled.\r\n");
	} else if (strcmp(tablica, "axisXmoveRight") == 0) {
		axisXmove(stepsX, vXMin,vXMax,accelXTime,decelXTime, RIGHT);
	} else if (strcmp(tablica, "axisXmoveLeft") == 0) {
		axisXmove(stepsX, vXMin,vXMax,accelXTime,decelXTime, LEFT);
	}else if (strcmp(tablica, "axisYmoveRight") == 0) {
		axisYmove(stepsY, vYMin, vYMax,accelYTime,decelYTime, RIGHT);
	} else if (strcmp(tablica, "axisYmoveLeft") == 0) {
		axisYmove(stepsY, vYMin, vYMax,accelYTime,decelYTime, LEFT);
	}else if (strcmp(tablica, "drawSquare") == 0) {
		sendString("Draws a square. Please wait.\r\n");
		stepsX = 100; stepsY = 100;
		axisYmove(100, 1.0, 10.0, 2,2, LEFT);
		delay_ms(13000);
		axisXmove(100, 1.0, 10.0, 2,2, LEFT);
		delay_ms(13000);
		axisYmove(100, 1.0, 10.0, 2,2, RIGHT);
		delay_ms(13000);
		axisXmove(100, 1.0, 10.0, 2,2, RIGHT);
	}else if (strcmp(tablica, "showCommands") == 0) {
		sendString("\r\nINFORMACJE \r\nsetSteps - ilosc krokow do wykonania.\r\n");
		sendString("setSpeedMin - predkosc minimalna.\r\n");
		sendString("setSpeedMax - predkosc maksymalna.\r\n");
		sendString("setAccel - czas rozpedzania.\r\n");
		sendString("setDecel - czas hamowania.\r\n");
		sendString("showCommands - dostepne polecenia.\r\n");
		sendString("axisXmoveL - ruch os 'x' kierunek lewo.\r\n");
		sendString("axisXmoveR - ruch os 'x' kierunek prawo.\r\n");
		sendString("axisYmoveL - ruch os 'y' kierunek lewo.\r\n");
		sendString("axisYmoveR - ruch os 'y' kierunek prawo.\r\n");
	} else
		sendString("ERROR: Wrong command.\r\n");

	memset(tablica, 0, strlen(tablica));
}

void usartGetValue() {
	if (od_poczatek != od_wolny) {
		static int index = 0;
		static char tab[128];
			if (odbieranie[od_poczatek] != '\n' && odbieranie[od_poczatek] != '\r') {
				if (odbieranie[od_poczatek] != ';') {
					tab[index] = odbieranie[od_poczatek];
					index++;
					if (index >= 128)
						index = 0;
				} else {
					checkCommand(tab);
					index = 0;
				}
			}
			od_poczatek++;
			if (od_poczatek >= 128)
				od_poczatek = 0;
	}
}

void basicXSettings(){
	sendString("BASIC X SETTINGS\r\n");
	sendString(conkat("Axis X, speed min = ",toString(vXMin),"\r\n"));
	sendString(conkat("Axis X, speed max = ",toString(vXMax),"\r\n"));
	sendString(conkat("Axis X, steps = ",toString(stepsX),"\r\n"));
	sendString(conkat("Axis X, accel = ",toString(accelXTime),"\r\n"));
	sendString(conkat("Axis X, decel = ",toString(decelXTime),"\r\n"));
}

void basicYSettings(){
	sendString("\r\nBASIC Y SETTINGS\r\n");
	sendString(conkat("Axis Y, speed min = ",toString(vYMin),"\r\n"));
	sendString(conkat("Axis Y, speed max = ",toString(vYMax),"\r\n"));
	sendString(conkat("Axis Y, steps = ",toString(stepsY),"\r\n"));
	sendString(conkat("Axis Y, accel = ",toString(accelYTime),"\r\n"));
	sendString(conkat("Axis Y, decel = ",toString(decelYTime),"\r\n"));
}

void USART2_IRQHandler(void) {
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		odbieranie[od_wolny] = USART_ReceiveData(USART2);
		od_wolny++;
		if (od_wolny >= 128)
			od_wolny = 0;
	}

	if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET) {
		if (wy_poczatek != wy_wolny) {
			USART_SendData(USART2, wysylanie[wy_poczatek]);
			wy_poczatek++;
			if (wy_poczatek >= 512)
				wy_poczatek = 0;
		} else
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
	}
}

void axisXmove(int stepsX, float vXMin, float vXMax,int accelXTime, int decelXTime,  int direction) {
	if (axisXEnabled == OFF) {
		accelerationX = (vXMax - vXMin) / (accelXTime * 1000.0);
		decelerationX = (vXMax - vXMin) / (decelXTime * 1000.0);

		stepsXAccel = ceil(((vXMin * accelXTime) + ((accelerationX * 1000)* (accelXTime * accelXTime)) / 2));
		stepsXDecel = ceil(((vXMin * decelXTime) + ((decelerationX * 1000)* (decelXTime * decelXTime)) / 2));
		middleXSteps = stepsX - stepsXAccel - stepsXDecel;

		if (stepsX >= stepsXAccel + stepsXDecel) {
			if (direction == LEFT){
				sendString("Engine X running. - left\r\n");
				GPIO_SetBits(axisXpin, axisXdir);
			}
			else{
				sendString("Engine X running. - right\r\n");
				GPIO_ResetBits(axisXpin, axisXdir);
			}

			axisXEnabled = ON;
			currentXSpeed = vXMin;
			stepAxisX = 1;
			times_X_ms = 0;
			TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		} else
			sendString(conkat("ERROR: Minimum ",toString(stepsXAccel + stepsXDecel)," steps.\r\n"));
	} else
		sendString("ERROR: Engine X is currently enabled.\r\n");
}


void axisYmove(int stepsY, float vYMin, float vYMax,int accelYTime, int decelYTime,  int direction) {
	if (axisYEnabled == OFF) {
		accelerationY = (vYMax - vYMin) / (accelYTime * 1000.0);
		decelerationY = (vYMax - vYMin) / (decelYTime * 1000.0);

		stepsYAccel = ceil(((vYMin * accelYTime) + ((accelerationY * 1000)* (accelYTime * accelYTime)) / 2));
		stepsYDecel = ceil(((vYMin * decelYTime) + ((decelerationY * 1000)* (decelYTime * decelYTime)) / 2));
		middleYSteps = stepsY - stepsYAccel - stepsYDecel;

		if (stepsY >= stepsYAccel + stepsYDecel) {
			if (direction == LEFT){
				sendString("Engine Y running. - left\r\n");
				GPIO_SetBits(axisYpin, axisYdir);
			}
			else{
				sendString("Engine Y running. - right\r\n");
				GPIO_ResetBits(axisYpin, axisYdir);
			}

			axisYEnabled = ON;
			currentYSpeed = vYMin;
			stepAxisY = 1;
			times_Y_ms = 0;
			TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
		} else
			sendString(conkat("ERROR: Minimum ",toString(stepsYAccel + stepsYDecel)," steps.\r\n"));
	} else
		sendString("ERROR: Engine Y is currently enabled.\r\n");
}

volatile int time2= 0;
void TIM2_IRQHandler() {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		times_X_ms++;
		time2++;
		if (stepAxisX <= stepsX) {
			if(times_X_ms == 1){
				sendString(conkat("Speed: ",floatToString(currentXSpeed),".\r\n"));
				sendString(conkat("Time: ",floatToString(time2),".\r\n"));
			}
			if (times_X_ms > 0 && times_X_ms <= 50) {
				GPIO_SetBits(axisXpin, axisXstep);
			} else {
				GPIO_ResetBits(axisXpin, axisXstep);
			}

				if (stepAxisX < stepsXAccel)
					currentXSpeed += accelerationX;
				else if (stepAxisX <= (stepsXAccel + middleXSteps))
					currentXSpeed = 10;
				else {
					if (currentXSpeed <= 1)
						currentXSpeed = 1;
					else
						currentXSpeed -= decelerationX;
				}

			if (times_X_ms >= (1000 / currentXSpeed)) {
				times_X_ms = 0;
				stepAxisX++;
			}
		} else {
			axisXEnabled = OFF;
			TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

void TIM3_IRQHandler() {
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		times_Y_ms++;
		if (stepAxisY <= stepsY) {

			if (times_Y_ms > 0 && times_Y_ms <= 50) {
				GPIO_SetBits(axisYpin, axisYstep);
			} else {
				GPIO_ResetBits(axisYpin, axisYstep);
			}

			if (stepAxisY < stepsYAccel)
				currentYSpeed += accelerationY;
			else if (stepAxisY <= (stepsYAccel + middleYSteps))
				currentYSpeed = 10;
			else {
				if (currentYSpeed <= 1)
					currentYSpeed = 1;
				else
					currentYSpeed -= decelerationY;
			}

			if (times_Y_ms >= (1000 / currentYSpeed)) {
				times_Y_ms = 0;
				stepAxisY++;
			}
		} else {
			axisYEnabled = OFF;
			TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

int main(void) {
	USART2_Init();
	Pin_AxisXConfig();
	Pin_AxisYConfig();
	Pin_Config();
	Nvic_Timer2_Config();
	Nvic_Timer3_Config();
	Timer2_Config();
	Timer3_Config();
	basicXSettings();
	basicYSettings();
	SysTick_Config(SystemCoreClock / 1000);

	while (1) {
		usartGetValue();
	}
}

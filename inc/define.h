#ifndef DEFINE_H_
#define DEFINE_H_

#define axisXpin GPIOC
#define axisXstep GPIO_Pin_1
#define axisXdir GPIO_Pin_0

#define axisYpin GPIOC
#define axisYstep GPIO_Pin_2
#define axisYdir GPIO_Pin_3

#define LEFT 0
#define RIGHT 1

#define OFF 0
#define ON 1

#define minSteps 10
#define maxSteps 230

#define minAccelTime 1
#define maxAccelTime 5

#define minDecelTime 1
#define maxDecelTime 5

#define speedMin 1
#define speedMax 10


#endif

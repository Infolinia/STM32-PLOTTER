#include <iostream>
#include <cmath>

using namespace std;
/*
	S(t) = V0 * t + a*t^2/2
	V(t) = V0 + a * t => t = (V(t) - V0) / a
	V(s) = ?
	
	S = V0*((V(t) - V0) / a) + 1/2*a*((V(t) - V0) / a) ^ 2 = 
	= (2*V0(V(t) - V0) + ((V(t) - V0)^2)) / 2 * a =
	= (2*V0*V(t) - 2V0^2 + V^2(t) - 2*V(t)*V0 + V0^2) / 2 * a = 
	= (V^2(t) - V0^2) / 2*a = S
	
	V^2(t) = 2*a*s + V0^2
	
	V(krok) = sqrt(2*acceleration*krok + Vmin^2)
	
*/
int main()
{
    int vMin = 1; // prędkość minimalna
    int vMax = 10; // prędkość maksymalna

    int accelTime = 3; // czas rozpędzania
    int decelTime = 3; // czas hamowania

    int currentStep = 0; // krok początkowy
    int totalSteps = 40; // łączna ilość kroków

    float acceleration = (vMax - vMin) / accelTime; // przyspieszenie
    float deceleration = (vMax - vMin) / decelTime; // hamowanie

	float stepsAccel = ((vMin * accelTime) + (acceleration * (accelTime * accelTime)) / 2); // kroki rozpędzania
	float stepsDecel = ((vMin * decelTime) + (deceleration  * (decelTime * decelTime)) / 2); // kroki hamowania

    cout << "vMin = " << vMin << endl;
    cout << "vMax = " << vMax << endl << endl;
    cout << "accelTime = " << accelTime << endl;
    cout << "decelTime = " << decelTime << endl << endl;
    cout << "acceleration = " << acceleration << endl;
    cout << "deceleration = " << deceleration << endl << endl;
    cout << "stepsXAccel = " << ceil(stepsAccel) << endl;
    cout << "stepsXDecel = " << ceil(stepsDecel) << endl << endl;

    if(totalSteps > ceil(stepsAccel) + ceil(stepsDecel)){
    float v;
        while(currentStep < totalSteps){
            if(currentStep < ceil(stepsAccel)){ // rozpędzanie
                v = sqrt((2*acceleration*currentStep) + pow(vMin, 2));
            }else if(currentStep < totalSteps-ceil(stepsDecel)){ //prędkość maksymalna
                v = vMax;
            }else{
                v = sqrt(2*(-deceleration)*(currentStep-totalSteps+stepsDecel+1) + pow(vMax, 2)); // hamowanie
            }

            cout << "Predkosc w kroku: " << currentStep << " wynosi: " << v << " okres T wynosi: " << 1000 / v <<  endl;
            currentStep++;
        }
    }else
        cout << "Aby prawidlowo wykonac ruch potrzeba min. " << ceil(stepsAccel) + ceil(stepsDecel) + 1 << " kroków. " <<  endl;

    return 0;
}

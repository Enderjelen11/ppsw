#include "led.h"
#include "keyboard.h"

void Delay(int iTimeMillis) {
	unsigned int uiCounter;
	unsigned int uiTime = iTimeMillis * 2399 * 2;

	for (uiCounter = 0; uiCounter < uiTime; uiCounter++) {}
}

enum LedState{LED_STOP, LED_RIGHT};

int main() {
	enum LedState eLedState = LED_RIGHT;
	
	LedInit();
	
	while(1){
	switch(eLedState){
	case LED_STOP:
		if (ReadButton()==BUTTON_1){
            eLedState = LED_RIGHT;
        }
        break;
	
	case LED_RIGHT:
		if (ReadButton()==BUTTON_0){
            eLedState = LED_STOP;
		}else{
            LedStepRight();
        }
		break;
	
	}
	Delay(250);
	}
}

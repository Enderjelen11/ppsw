#include "led.h"
#include "keyboard.h"

void Delay(int iTimeMillis) {
	unsigned int uiCounter;
	unsigned int uiTime = iTimeMillis * 2399 * 2;

	for (uiCounter = 0; uiCounter < uiTime; uiCounter++) {}
}

enum LedState{LED_LEFT, LED_RIGHT};

int main() {
	enum LedState eLedState = LED_LEFT;
    unsigned int uiCount = 0;
	
	LedInit();
	
	while(1){
	switch(eLedState){
	case LED_LEFT:
		if (uiCount%3==0){
			eLedState = LED_RIGHT;
		}else{
			LedStepLeft();
            uiCount++;
		}
		break;
	
	case LED_RIGHT:
		if (uiCount%3==0){
			eLedState = LED_LEFT;
		}else{
			LedStepRight();
            uiCount++;
		}
		break;
	
	}
	Delay(250);
	}
}

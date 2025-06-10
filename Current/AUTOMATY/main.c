#include "led.h"
#include "keyboard.h"

void Delay(int iTimeMillis) {
	unsigned int uiCounter;
	unsigned int uiTime = iTimeMillis * 2399 * 2;

	for (uiCounter = 0; uiCounter < uiTime; uiCounter++) {}
}

enum LedState{LED_STOP, LED_LEFT, LED_RIGHT, LED_BLINK};

int main() {
	enum LedState eLedState = LED_STOP;
	unsigned char ucBlinkCount = 0;
	
	LedInit();
	
	while(1){
	switch(eLedState){
		
	case LED_STOP:
		if (ReadButton()==BUTTON_0){
			eLedState = LED_LEFT;
		}else if (ReadButton()==BUTTON_2){
			eLedState = LED_RIGHT;
		}
		break;
		
	case LED_LEFT:
		if (ReadButton()==BUTTON_1){
			eLedState = LED_STOP;
		//stan lewy do prawy i mryganie w miedzy czasie
		}else if(ReadButton()==BUTTON_3){
			eLedState = LED_BLINK;
		}else{
			LedStepLeft();
		}
		break;
	
	case LED_RIGHT:
		if (ReadButton()==BUTTON_1){
			eLedState = LED_STOP;
		}else{
			LedStepRight();
		}
		break;
	case LED_BLINK:
		if (ucBlinkCount==6){
			eLedState = LED_RIGHT;
			ucBlinkCount = 0;
		}else{
			if(ucBlinkCount%2==0){
				LedOn(1);
			}if(ucBlinkCount%2==1){
				LedOn(5);
			}
			ucBlinkCount++;
		}
		break;
	
	}
	Delay(250);
	}
}

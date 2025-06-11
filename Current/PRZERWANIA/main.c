#include "led.h"
#include "timer1Interrupts.h"
#include "keyboard.h"

void Automat(void) {
	enum LedState { LED_RIGHT, STOP, LED_LEFT };
	static enum LedState eLedState = STOP;

	switch(eLedState) {
		case STOP:
			if(ReadButton() == BUTTON_0) {
				eLedState = LED_LEFT;
			} else if(ReadButton() == BUTTON_2) {
				eLedState = LED_RIGHT;
			} else {
				eLedState = STOP;
			}
			break;

		case LED_LEFT:
			LedStepLeft();
			if(ReadButton() == BUTTON_1) {
				eLedState = STOP;
			} else {
				eLedState = LED_LEFT;
			}
			break;

		case LED_RIGHT:
			LedStepRight();
			if(ReadButton() == BUTTON_1) {
				eLedState = STOP;
			} else {
				eLedState = LED_RIGHT;
			}
			break;

		default:
			break;
	}
}

int main() {
	unsigned int iMainLoopCtr;
	LedInit();
	KeyboardInit();
	Timer0Interrupts_Init(20000, &Automat);
	while(1) {
		iMainLoopCtr++;
	}
}

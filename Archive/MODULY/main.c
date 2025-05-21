#include "led.h"
#include "keyboard.h"

void Delay(int iTimeMillis) {
	unsigned int uiCounter;
	unsigned int uiTime = iTimeMillis * 2399 * 2;

	for (uiCounter = 0; uiCounter < uiTime; uiCounter++) {}
}

int main() {
	LedInit();
	KeyboardInit();

	while (1) {
		switch (ReadButton()) {
			case BUTTON_1:
				LedStepRight();
				break;
			case BUTTON_2:
				LedStepLeft();
				break;
			default:
				break;
		}

		Delay(300);
	}
}

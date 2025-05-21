#include <LPC21xx.H>

#define LED0_bm 0x10000
#define LED1_bm 0x20000
#define LED2_bm 0x40000
#define LED3_bm 0x80000

#define BUTTON0_bm 0x10
#define BUTTON1_bm 0x20
#define BUTTON2_bm 0x40
#define BUTTON3_bm 0x80


void Delay(int iTimeMillis) {
	unsigned int uiCounter;
	unsigned int uiTime = iTimeMillis * 2399 * 2;

	for (uiCounter = 0; uiCounter < uiTime; uiCounter++) {}
}

void LedInit() {
	IO1DIR = IO1DIR | LED0_bm | LED1_bm | LED2_bm | LED3_bm; //0xF0000

	IO1SET = LED0_bm;
}

void LedOn(unsigned char ucLedIndex) {
	IO1CLR = LED0_bm | LED1_bm | LED2_bm | LED3_bm;

	switch (ucLedIndex) {
		case 0:
			IO1SET = LED0_bm;
			break;
		case 1:
			IO1SET = LED1_bm;
			break;
		case 2:
			IO1SET = LED2_bm;
			break;
		case 3:
			IO1SET = LED3_bm;
			break;
	}
}

void KeyboardInit() {
	IO0DIR = IO0DIR & ~(BUTTON0_bm | BUTTON1_bm | BUTTON2_bm | BUTTON3_bm);
}

enum KeyboardState { RELASED, BUTTON_0, BUTTON_1, BUTTON_2, BUTTON_3 };

enum KeyboardState ReadButton() {
	if ((IO0PIN & BUTTON0_bm) == 0) {
		return BUTTON_0;
	} else if ((IO0PIN & BUTTON1_bm) == 0) {	
		return BUTTON_1;
	} else if ((IO0PIN & BUTTON2_bm) == 0) {	//	0x40 ; 0x00
		return BUTTON_2;
	} else if ((IO0PIN & BUTTON3_bm) == 0) {	//	0x80 ; 0x00
		return BUTTON_3;
	} else {
		return RELASED;
	}
}

enum StepDir { RIGHT, LEFT };

void LedStep(enum StepDir eDirection) {
	static unsigned int uiCurrentLed = 0;

	switch (eDirection) {
		case LEFT:
			uiCurrentLed = (uiCurrentLed + 1) % 4;
			break;
		case RIGHT:
			uiCurrentLed = (uiCurrentLed - 1) % 4;
			break;
	}

	LedOn((unsigned char)uiCurrentLed);
}

void LedStepLeft(void) {
	LedStep(LEFT);
}

void LedStepRight(void) {
	LedStep(RIGHT);
}

int main() {
	LedInit();
	KeyboardInit();

	while (1) {
		switch (ReadButton()) {
			case BUTTON_1:
				LedStep(RIGHT);
				break;
			case BUTTON_2:
				LedStep(LEFT);
				break;
			default:
				break;
		}

		Delay(300);
	}
}

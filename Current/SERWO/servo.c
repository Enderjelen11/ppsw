#include <LPC21xx.H>
#include "led.h"
#include "timer1Interrupts.h"

#define DETECTOR_bm (1<<10)

enum ServoState {CALLIB, IDLE, IN_PROGRESS};

struct Servo {
	enum ServoState eState;
	unsigned int uiCurrentPosition;
	unsigned int uiDesiredPosition;
};

struct Servo sServo;

void DetectorInit() {
	IO0DIR = IO0DIR & (~DETECTOR_bm);
}

enum DetectorState {ACTIVE, INACTIVE};

enum DetectorState eReadDetector() {
	if ((IO0PIN & DETECTOR_bm) == 0) {
		return ACTIVE;
	} else {
		return INACTIVE;
	}
}

void Automat(void) {
	switch (sServo.eState) {
		case CALLIB:
			if (eReadDetector() == INACTIVE) {
				LedStepRight();
				sServo.eState = CALLIB;
			} else {
				sServo.uiCurrentPosition = 0;
				sServo.uiDesiredPosition = 0;
				sServo.eState = IDLE;
			}
			break;
		case IDLE:
			if (sServo.uiCurrentPosition != sServo.uiDesiredPosition) {
				sServo.eState = IN_PROGRESS;
			} else {
				sServo.eState = IDLE;
			}
			break;
		case IN_PROGRESS:
			if (sServo.uiCurrentPosition < sServo.uiDesiredPosition) {
				LedStepRight();
				sServo.uiCurrentPosition++;
				sServo.eState = IN_PROGRESS;
			} else if (sServo.uiCurrentPosition > sServo.uiDesiredPosition) {
				LedStepLeft();
				sServo.uiCurrentPosition--;
				sServo.eState = IN_PROGRESS;
			} else {
				sServo.eState = IDLE;
			}
			break;
		default:
			break;
	}
}

void ServoInit(unsigned int uiServoFrequency) {
	sServo.eState = CALLIB;
	LedInit();
	Timer0Interrupts_Init((1000000 / uiServoFrequency), &Automat);
}

void ServoCallib(void) {
	sServo.eState = CALLIB;
}

void ServoGoTo(unsigned int uiPosition) {
	sServo.uiDesiredPosition = uiPosition;
}

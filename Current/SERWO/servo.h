#ifndef SERVO_H
#define SERVO_H

void ServoInit(unsigned int uiServoFrequency);
void ServoCallib(void);
void ServoGoTo(unsigned int uiPosition);

#endif
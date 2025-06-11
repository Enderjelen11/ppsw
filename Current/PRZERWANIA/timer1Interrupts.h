#ifndef TIMER1_INTERRUPTS_H
#define TIMER1_INTERRUPTS_H

void Timer1Interrupts_Init(unsigned int uiPeriod, void (*ptrInterruptFunction)(void));

#endif
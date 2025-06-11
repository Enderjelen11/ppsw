#include <LPC21xx.H>
#include "timer.h"

#define mCOUNTER_ENABLE (1<<0)
#define mCOUNTER_RESET (1<<1)
#define mRESET_ON_MR0 (1<<1)
#define mINTERRUPT_ON_MR0 (1<<0)
#define mINTERRUPT_FLAG_MR0 (1<<0)

void InitTimer0(void){
	T0TCR = mCOUNTER_ENABLE; //wlaczam timer
}

void WaitOnTimer0(unsigned int uiTime){
	T0TCR |= mCOUNTER_RESET; //ustawiam bit resetu timer control, timer przechodzi w reset
	T0TCR &= (~mCOUNTER_RESET); //ustawiam bit resetu na 0, przez to wylaczam resem i uruchamiam timer
	while(T0TC < (uiTime * 15)){} //pusta petla while dopoki TC jest mniejsze od oczekiwanego czasu czekania. Zmiejszamy timer, zmieniamy pomiar czasu na milisekundy
}

void InitTimer0Match0(unsigned int uiDelayTime){
	T0MR0 = uiDelayTime * 15; //ustawiam match na oczekiwany czas
	T0MCR |= (mRESET_ON_MR0 | mINTERRUPT_ON_MR0); //ustawiam bity 0 i 1 MCR na 1 gdyz to spowoduje ze w przypadku zrownania sie TC i MR uruchomi sie reset oraz interrupt
	T0TCR |= mCOUNTER_RESET; //ustawiam reset
	T0TCR &= ~mCOUNTER_RESET; //wylaczam reset
	T0TCR |= mCOUNTER_ENABLE; //uruchamiam timer
}

void WaitOnTimer0Match0(){
	while((T0IR & mINTERRUPT_FLAG_MR0) == 0){} //czekaj dopoki brak flagi interuptu MT
	T0IR = mINTERRUPT_FLAG_MR0; //zerowanie flagi MR0
	T0IR |= mINTERRUPT_FLAG_MR0; //nie zrobi tego czego chcemy poniewaz wpisanie jedynki na dany bit powoduje zerowanie tej flagi
}

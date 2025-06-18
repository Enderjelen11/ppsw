#include "uart.h"
#include "led.h"
#include "servo.h"

int main(void) {
		LedInit();
		
		ServoInit(100);
		ServoCallib();
		
    UART_InitWithInt(9600);

    while (1) {
			switch(cOdebranyZnak){
				/*
				case '1':
					ServoGoTo(13);
					break;
				case '2':
					ServoGoTo(26);
					break;
				case '3':
					ServoGoTo(39);
					break;
				case '4':
					LedOn(3);
					break;
				*/
				case '1':
					ServoShift(12);
					cOdebranyZnak = 0x00;
					break;
				case 'c':
					ServoCallib();
					cOdebranyZnak = 0x00;
					break;
			}
			
    }

    return 0;
}

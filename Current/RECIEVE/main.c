#include "uart.h"
#include "led.h"
#include "servo.h"

enum CompResult {DIFFERENT, EQUAL};

enum CompResult eCompareString(char pcStr1[], char pcStr2[]){
    unsigned char ucCharCounter;

    for (ucCharCounter = 0; (('\0' != pcStr1[ucCharCounter]) || (pcStr2[ucCharCounter] != '\0')); ucCharCounter++) {
        if(pcStr1[ucCharCounter] != pcStr2[ucCharCounter]){
            return DIFFERENT;
        }
    }

    return EQUAL;
}

int main(void) {
		unsigned char ucResult[254];
	
		LedInit();
		
		ServoInit(100);
		ServoCallib();
		
    UART_InitWithInt(9600);

    while (1) {
			if(eReciever_GetStatus()==READY){
				Reciever_GetStringCopy((char *)ucResult);
			}
			if(eCompareString(ucResult, 'callib')){

			}else if(eCompareString(ucResult, 'left')){

			}else if(eCompareString(ucResult,'right')){

			}
			
    }

    return 0;
}

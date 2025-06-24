#include "uart.h"
#include "servo.h"
#include "led.h"

#define NULL '\0'

char cString[RECIEVER_SIZE];
char *apcTokens[2];

enum KeywordCode {CALLIB, GOTO, UNKNOWN};

enum CompResult {DIFFERENT, EQUAL};

enum CompResult eCompareString(char pcStr1[], char pcStr2[]){
	unsigned char ucCharCounter;
	for (ucCharCounter = 0; ((pcStr1[ucCharCounter] != NULL) || (pcStr2[ucCharCounter] != NULL)); ucCharCounter++){
		if(pcStr1[ucCharCounter] != pcStr2[ucCharCounter]){
			return DIFFERENT;
		}
	}
	return EQUAL;
}

unsigned char ucStringToTokens(char *pcString, char *apcTokens[]){
	unsigned char ucTokenNr = 0;
	while (*pcString != DELIMETER){
		if (*pcString != ' '){
			apcTokens[ucTokenNr] = pcString;
            ucTokenNr++;
			while (*pcString != ' ' && *pcString != DELIMETER){
				pcString++;
			}
			if (*pcString == DELIMETER) break;
			*pcString = DELIMETER;
			pcString++;
		}else{
			pcString++;
		}
	}
	return ucTokenNr;
}

enum KeywordCode eDecodeCmd(char *pcToken){
	if (eCompareString(pcToken, "callib") == EQUAL) return CALLIB;
	if (eCompareString(pcToken, "goto") == EQUAL) return GOTO;
	return UNKNOWN;
}

enum Result { OK, ERROR };

enum Result eHexStringToUInt(char pcStr[], unsigned int *puiValue) {
    unsigned char ucCharCounter;
    unsigned char ucCurrentChar;
	
    if (pcStr[0] != '0' || pcStr[1] != 'x' || pcStr[2] == NULL) {
        return ERROR;
    }

    *puiValue = 0;

    for (ucCharCounter  = 2; ucCharCounter  < 6 && pcStr[ucCharCounter ] != '\0'; ucCharCounter++) {
        *puiValue = *puiValue << 4;

        ucCurrentChar = pcStr[ucCharCounter];

        if (ucCurrentChar >= '0' && ucCurrentChar <= '9') {
            *puiValue = *puiValue | (ucCurrentChar - '0');
        } else if (ucCurrentChar >= 'A' && ucCurrentChar <= 'F') {
            *puiValue = *puiValue | (ucCurrentChar - 'A' + 10);
        } else {
            return ERROR;
        }
    }

    return OK;
}

int main(){
    unsigned int uiServoDest;
		unsigned char ucTokenCount;

	UART_InitWithInt(9600);
	while(1){
		if (eReciever_GetStatus() == READY){
			Reciever_GetStringCopy(cString);
			ucTokenCount = ucStringToTokens(cString, apcTokens);
			if (ucTokenCount > 0){
				switch(eDecodeCmd(apcTokens[0])){
					case CALLIB:
						ServoCallib();
						break;
					case GOTO:
						if (ucTokenCount >= 2){
							eHexStringToUInt(apcTokens[1], &uiServoDest);
							ServoGoTo(uiServoDest);
						}
						break;
					default:
						break;
				}
			}
		}
	}
}

#define NULL '\0'
#define HEX_bm 0x000F

void UIntToHexStr(unsigned int uiValue, char pcStr[]) {
    unsigned char ucCharCounter;
	
    pcStr[0] = '0';
    pcStr[1] = 'x';

    for (ucCharCounter = 0; ucCharCounter  < 4; ucCharCounter++) {
        unsigned char ucNibble = (uiValue >> ((3 - ucCharCounter ) * 4)) & HEX_bm;

        if(ucNibble < 10){
            pcStr[ucCharCounter + 2] = '0' + ucNibble;
        }else {
            pcStr[ucCharCounter + 2] = 'A' + ucNibble - 10;
        }
    }

    pcStr[6] = '\0';
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


void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[]){
    unsigned char ucCharacterCounter;

    for(ucCharacterCounter=0; pcDestinationStr[ucCharacterCounter]!=NULL; ucCharacterCounter++){}

    UIntToHexStr(uiValue, pcDestinationStr+ucCharacterCounter);
}

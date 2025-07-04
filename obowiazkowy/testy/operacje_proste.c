#define NULL '\0'

#define NOTEQUAL DIFFERENT
#define EQUEL EQUAL
enum CompResult {DIFFERENT, EQUAL};

void CopyString(char pcSource[], char pcDestination[]){
    unsigned char ucCharCounter;

    for ( ucCharCounter = 0; NULL != pcSource[ucCharCounter]; ucCharCounter++) {
        pcDestination[ucCharCounter] = pcSource[ucCharCounter];
    }

    pcDestination[ucCharCounter] = NULL;
}



enum CompResult eCompareString(char pcStr1[], char pcStr2[]){
    unsigned char ucCharCounter;

    for (ucCharCounter = 0; ((NULL != pcStr1[ucCharCounter]) || (pcStr2[ucCharCounter] != NULL)); ucCharCounter++) {
        if(pcStr1[ucCharCounter] != pcStr2[ucCharCounter]){
            return NOTEQUAL;
        }
    }

    return EQUEL;
}



void AppendString(char pcSourceStr[], char pcDestinationStr[]) {
    unsigned char ucCharCounter;

    for (ucCharCounter = 0; NULL != pcDestinationStr[ucCharCounter]; ucCharCounter++) {};

    CopyString(pcSourceStr, pcDestinationStr + ucCharCounter); 
}



void ReplaceCharactersInString(char pcString[],char cOldChar,char cNewChar) {
    unsigned char ucCharCounter;

    for (ucCharCounter = 0; NULL != pcString[ucCharCounter]; ucCharCounter++){
        if(pcString[ucCharCounter] == cOldChar){
            pcString[ucCharCounter] = cNewChar;
        }
    }
}

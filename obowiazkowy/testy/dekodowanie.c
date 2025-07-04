#include "stdio.h"
#include <string.h>
#define MAX_TOKEN_NR 3
#define NULL 0
#define MAX_KEYWORD_NR 3
#define MAX_KEYWORD_STRING_LTH 10

enum KeywordCode {LD, ST, RST};

union TokenValue{
	enum KeywordCode eKeyword;
	unsigned int uiNumber;
	char* pcString;
};

enum TokenType {KEYWORD, NUMBER, STRING};

struct Token{
	enum TokenType eType;
	union TokenValue uValue;
};

struct Token asToken[MAX_TOKEN_NR];

struct Keyword{
	enum KeywordCode eCode;
	char cString[MAX_KEYWORD_STRING_LTH + 1];
};

struct Keyword asKeywordList[MAX_KEYWORD_NR]={
	{RST,"reset"},
	{LD, "load"},
	{ST, "store"}
};

unsigned char ucTokenCount;

enum Mode{TOKEN, DELIMITER};

unsigned char ucTokenNr;

unsigned char ucFindTokensInString(char *pcString){
	enum Mode eMode=DELIMITER;
	unsigned char ucTokenCount=0;
	unsigned char ucArrayIndex;
	unsigned char ucCurrentChar;

	for(ucArrayIndex=0;;ucArrayIndex++){
		ucCurrentChar=pcString[ucArrayIndex];
		switch(eMode){
		case TOKEN:
            if(ucTokenCount==MAX_TOKEN_NR){
                return ucTokenCount;
            }
            else if(ucCurrentChar==NULL){
                return ucTokenCount;
            }
            else if(ucCurrentChar==' '){
                eMode=DELIMITER;
            }
            else{
                eMode=TOKEN;
            }
            break;
		case DELIMITER:
            if(ucCurrentChar==NULL){
                return ucTokenCount;
            }
            else if(ucCurrentChar==' '){
                eMode=DELIMITER;
            }
            else{
                eMode=TOKEN;
                asToken[ucTokenCount].uValue.pcString=pcString+ucArrayIndex;
                ucTokenCount++;
            }
            break;
		}
	}
}


enum CompResult {DIFFERENT, EQUAL};


enum Result { OK, ERROR };
enum Result eStringToKeyword(char pcStr[], enum KeywordCode *peKeywordCode) {
	unsigned char ucArrayIndex;
	for (ucArrayIndex = 0; ucArrayIndex < MAX_KEYWORD_NR; ucArrayIndex++) {
		if (EQUAL == eCompareString(asKeywordList[ucArrayIndex].cString, pcStr)) {
			*peKeywordCode = asKeywordList[ucArrayIndex].eCode;
			return OK;
		}
	}
	return ERROR;
}


void DecodeTokens(void) {
	unsigned char ucArrayIndex;
	struct Token *psCurrentToken;
	unsigned int uiTokenValue;
	enum KeywordCode eTokenCode;

	for (ucArrayIndex = 0; ucArrayIndex < ucTokenNr; ucArrayIndex++) {
		psCurrentToken = &asToken[ucArrayIndex];

		if (OK == eHexStringToUInt(psCurrentToken->uValue.pcString, &uiTokenValue)) {
			psCurrentToken->eType = NUMBER;
			psCurrentToken->uValue.uiNumber = uiTokenValue;
		}
		else if (OK == eStringToKeyword(psCurrentToken->uValue.pcString, &eTokenCode)) {
			psCurrentToken->eType = KEYWORD;
			psCurrentToken->uValue.eKeyword = eTokenCode;
		}
		else {
			psCurrentToken->eType = STRING;
		}
	}
}


void DecodeMsg(char *pcString){
	ucTokenNr = ucFindTokensInString(pcString);
	ReplaceCharactersInString(pcString, ' ', NULL);
	DecodeTokens();
}


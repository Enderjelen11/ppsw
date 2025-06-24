#include "uart.h"

#define mDIVISOR_LATCH_ACCES_BIT                   0x00000080
#define m8BIT_UART_WORD_LENGTH                     0x00000003

#define mRX_DATA_AVALIABLE_INTERRUPT_ENABLE        0x00000001
#define mTHRE_INTERRUPT_ENABLE                     0x00000002

#define mINTERRUPT_PENDING_IDETIFICATION_BITFIELD  0x0000000F
#define mTHRE_INTERRUPT_PENDING                    0x00000002
#define mRX_DATA_AVALIABLE_INTERRUPT_PENDING       0x00000004

#define VIC_UART0_CHANNEL_NR  6
#define VIC_UART1_CHANNEL_NR  7
#define mIRQ_SLOT_ENABLE                           0x00000020
#define UART_PIN_bm (1<<2)

char cOdebranyZnak;

struct RecieverBuffer gRecieverBuffer = {
	{ '\0' },
	0,
	EMPTY
};

void Reciever_PutCharacterToBuffer(char cCharacter) {
	if (gRecieverBuffer.eStatus == READY || gRecieverBuffer.eStatus == OVERFLOW) {
		return;
	}

	if (cCharacter == '\0' || cCharacter == '\n' || cCharacter == '\r') {
		if (gRecieverBuffer.ucCharCtr < RECIEVER_SIZE) {
			gRecieverBuffer.cData[gRecieverBuffer.ucCharCtr] = '\0';
			gRecieverBuffer.eStatus = READY;
		} else {
			gRecieverBuffer.eStatus = OVERFLOW;
		}
	} else {
		if (gRecieverBuffer.ucCharCtr < RECIEVER_SIZE - 1) {
			gRecieverBuffer.cData[gRecieverBuffer.ucCharCtr++] = cCharacter;
		} else {
			gRecieverBuffer.eStatus = OVERFLOW;
		}
	}
}

enum eRecieverStatus eReciever_GetStatus(void) {
	return gRecieverBuffer.eStatus;
}

void Reciever_GetStringCopy(char * ucDestination) {
	if (gRecieverBuffer.eStatus == READY) {
		unsigned char i = 0;
		while (gRecieverBuffer.cData[i] != DELIMETER) {
			ucDestination[i] = gRecieverBuffer.cData[i];
			i++;
		}
		ucDestination[i] = DELIMETER;
	} else {
		ucDestination[0] = '\0';
	}
	gRecieverBuffer.ucCharCtr = 0;
	gRecieverBuffer.eStatus = EMPTY;
}

__irq void UART0_Interrupt(void) {
	unsigned int uiCopyOfU0IIR = U0IIR;

	if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mRX_DATA_AVALIABLE_INTERRUPT_PENDING) {
		cOdebranyZnak = U0RBR;
		Reciever_PutCharacterToBuffer(U0RBR);
	}

	if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mTHRE_INTERRUPT_PENDING) {
		// Nadajnik pusty, nic nie robimy
	}

	VICVectAddr = 0;
}

void UART_InitWithInt(unsigned int uiBaudRate) {
	PINSEL0 = PINSEL0 | UART_PIN_bm;
	U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCES_BIT;
	U0DLL   = ((15000000) / 16) / uiBaudRate;
	U0LCR  &= (~mDIVISOR_LATCH_ACCES_BIT);
	U0IER  |= mRX_DATA_AVALIABLE_INTERRUPT_ENABLE;

	VICVectAddr1  = (unsigned long) UART0_Interrupt;
	VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;
	VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);
}

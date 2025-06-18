#include <LPC210X.H>

/************ UART ************/
// U0LCR Line Control Register
#define mDIVISOR_LATCH_ACCES_BIT                   0x00000080
#define m8BIT_UART_WORD_LENGTH                     0x00000003

// UxIER Interrupt Enable Register
#define mRX_DATA_AVALIABLE_INTERRUPT_ENABLE        0x00000001
#define mTHRE_INTERRUPT_ENABLE                     0x00000002

// UxIIR Pending Interrupt Identification Register
#define mINTERRUPT_PENDING_IDETIFICATION_BITFIELD  0x0000000F
#define mTHRE_INTERRUPT_PENDING                    0x00000002
#define mRX_DATA_AVALIABLE_INTERRUPT_PENDING       0x00000004

/************ Interrupts **********/
// VIC (Vector Interrupt Controller) channels
#define VIC_UART0_CHANNEL_NR  6
#define VIC_UART1_CHANNEL_NR  7

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE                           0x00000020

#define UART_PIN_bm (1<<2)

#define RECIEVER_SIZE 10

#define DELIMETER '\0'

////////////// Zmienne globalne ////////////
char cOdebranyZnak;


enum eRecieverStatus {EMPTY, READY, OVERFLOW};
struct RecieverBuffer{ char cData[RECIEVER_SIZE];
	unsigned char ucCharCtr;
	enum eRecieverStatus eStatus;

};

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

void CopyString(char pcSource[], char pcDestination[]){
    unsigned char ucCharCounter;

    for ( ucCharCounter = 0; DELIMETER != pcSource[ucCharCounter]; ucCharCounter++) {
        pcDestination[ucCharCounter] = pcSource[ucCharCounter];
    }

    pcDestination[ucCharCounter] = DELIMETER;
}

void Reciever_GetStringCopy(char * ucDestination) {
    if (gRecieverBuffer.eStatus == READY) {
        CopyString(ucDestination, gRecieverBuffer.cData);
    } else {
        ucDestination[0] = '\0';
    }
    gRecieverBuffer.ucCharCtr = 0;
    gRecieverBuffer.eStatus = EMPTY;
}


///////////////////////////////////////////
__irq void UART0_Interrupt (void) {
   // jesli przerwanie z odbiornika (Rx)
   
   unsigned int uiCopyOfU0IIR=U0IIR; // odczyt U0IIR powoduje jego kasowanie wiec lepiej pracowac na kopii

   if      ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mRX_DATA_AVALIABLE_INTERRUPT_PENDING) // odebrano znak
   {
      cOdebranyZnak = U0RBR;
			Reciever_PutCharacterToBuffer(U0RBR);
   } 
   
   if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mTHRE_INTERRUPT_PENDING)              // wyslano znak - nadajnik pusty 
   {
      // narazie nic nie wysylamy
   }

   VICVectAddr = 0; // Acknowledge Interrupt
}

////////////////////////////////////////////
void UART_InitWithInt(unsigned int uiBaudRate){

   // UART0
   PINSEL0 = PINSEL0 | UART_PIN_bm;                                // ustawic pina na odbiornik uart0
   U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCES_BIT; // dlugosc slowa, DLAB = 1
   U0DLL   = ((15000000)/16)/uiBaudRate;                      // predkosc transmisji
   U0LCR  &= (~mDIVISOR_LATCH_ACCES_BIT);                       // DLAB = 0
   U0IER  |= mRX_DATA_AVALIABLE_INTERRUPT_ENABLE;               // ??? co tu robinmy

   // INT
   VICVectAddr1  = (unsigned long) UART0_Interrupt;             // set interrupt service routine address
   VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;     // use it for UART 0 Interrupt
   VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);               // Enable UART 0 Interrupt Channel
}




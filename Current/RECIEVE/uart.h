#ifndef UART0_DRIVER_H
#define UART0_DRIVER_H

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

/************ Global Variables ************/
extern char cOdebranyZnak;

typedef enum {
    EMPTY,
    READY,
    OVERFLOW
} eRecieverStatus;

typedef struct {
    char cData[RECIEVER_SIZE];
    unsigned char ucCharCtr;
    eRecieverStatus eStatus;
} RecieverBuffer;

extern RecieverBuffer gRecieverBuffer;

/************ Functions ************/
void Reciever_PutCharacterToBuffer(char cCharacter);
eRecieverStatus eReciever_GetStatus(void);
void CopyString(char pcSource[], char pcDestination[]);
void Reciever_GetStringCopy(char * ucDestination);

__irq void UART0_Interrupt(void);

void UART_InitWithInt(unsigned int uiBaudRate);

#endif // UART0_DRIVER_H

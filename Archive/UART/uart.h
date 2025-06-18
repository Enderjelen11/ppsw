#ifndef UART_H
#define UART_H

/************ UART DEFINES ************/
#define mDIVISOR_LATCH_ACCES_BIT                   0x00000080
#define m8BIT_UART_WORD_LENGTH                     0x00000003

#define mRX_DATA_AVALIABLE_INTERRUPT_ENABLE        0x00000001
#define mTHRE_INTERRUPT_ENABLE                     0x00000002

#define mINTERRUPT_PENDING_IDETIFICATION_BITFIELD  0x0000000F
#define mTHRE_INTERRUPT_PENDING                    0x00000002
#define mRX_DATA_AVALIABLE_INTERRUPT_PENDING       0x00000004

#define VIC_UART0_CHANNEL_NR                       6
#define VIC_UART1_CHANNEL_NR                       7

#define mIRQ_SLOT_ENABLE                           0x00000020

#define UART_RX_PINSEL_MASK                        (1 << 4)
#define UART_TX_PINSEL_MASK                        (1 << 6)

/************ API ************/
void UART_InitWithInt(unsigned int uiBaudRate);

/************ Global Variables ************/
extern char cOdebranyZnak;

#endif /* UART_H */

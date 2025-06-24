#ifndef UART_H
#define UART_H

#include <LPC210X.H>

#define RECIEVER_SIZE 10
#define DELIMETER '\0'

// UART0 interrupt handler
__irq void UART0_Interrupt(void);

// Initialize UART with interrupt
void UART_InitWithInt(unsigned int uiBaudRate);

// Get current receiver status
enum eRecieverStatus eReciever_GetStatus(void);

// Get copy of received string
void Reciever_GetStringCopy(char * ucDestination);

// Enum for receiver buffer status
enum eRecieverStatus {
	EMPTY,
	READY,
	OVERFLOW
};

// Receiver buffer structure
struct RecieverBuffer {
	char cData[RECIEVER_SIZE];
	unsigned char ucCharCtr;
	enum eRecieverStatus eStatus;
};

// Put character to buffer
void Reciever_PutCharacterToBuffer(char cCharacter);

#endif

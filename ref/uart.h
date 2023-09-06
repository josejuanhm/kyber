#ifndef __UART
#define __UART

#include <string.h>
#include "fpau_switches.h"

extern char str[21];
extern char* pbuf;

void itoa(char *buf, unsigned int d, int base);
void print_runtime(unsigned int cycle_start, unsigned int cycle_end);

#ifdef STEEL
// ADDRESS MAP
extern char* UART_TX_ADDRESS;
extern char* UART_RX_ADDRESS_DATA;
extern char* UART_RX_ADDRESS_READY;

void uart_send_string(const char* str);

#define isTransmitBusy(BASE_ADDRESS)  \
  ((*(volatile unsigned int *)((unsigned int)BASE_ADDRESS)) != 1)

#else

#include <stdint.h>

#define BASE_ADDRESS_U (0xE0001000)

#define PS7_UART_CONTROL_REGISTER   0x00
#define ARTY_UART_CONTROL_REGISTER  0x0C

#define UART_CR_RESET_TX_RX         0x3

#define PS7_UART_DATA_OUT_REGISTER  0x30
#define ARTY_UART_DATA_OUT_REGISTER 0x04

#define PS7_UART_STATUS_REGISTER    0x2C
#define ARTY_UART_STATUS_REGISTER   0x08

#define PS7_UART_STATUS_FULL  0x00000010
#define ARTY_UART_STATUS_FULL 0x00000008

#define X_mWriteReg(BASE_ADDRESS, RegOffset, data)  \
  *(unsigned int *)(BASE_ADDRESS + RegOffset) = ((unsigned int) data);
#define X_mReadReg(BASE_ADDRESS, RegOffset) \
  *(unsigned int *)(BASE_ADDRESS + RegOffset);
#define XUartChanged_IsTransmitFull(BASE_ADDRESS)  \
  ((*(volatile unsigned int *)((unsigned int)BASE_ADDRESS + PS7_UART_STATUS_REGISTER) & PS7_UART_STATUS_FULL) != 0)

void XUARTChanged_SendByte(uint32_t BaseAddress, uint8_t Data);
void uart_send_string(char *ptr);
void XUART_Reset(void);

#endif // STEEL

#endif
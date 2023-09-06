#include "uart.h"

char str[21] = {0};
char* pbuf = &str[0];

void itoa(char *buf, unsigned int d, int base)
{
	int div = 1;
	while (d/div >= base)
		div *= base;

	while (div != 0)
	{
		int num = d/div;
		d = d%div;
		div /= base;
		if (num > 9)
			*(buf++) = (num-10) + 'A';
		else
			*(buf++) = num + '0';
	}
  *(buf++) = '\0';
}

void print_runtime(unsigned int cycle_start, unsigned int cycle_end)
{
  unsigned int clock_cycles = cycle_end - cycle_start;

  itoa(pbuf, cycle_start, 10);
  uart_send_string(" Start cycle: ");
  uart_send_string(str);

  itoa(pbuf, cycle_end, 10);
  uart_send_string(" End cycle: ");
  uart_send_string(str);

  itoa(pbuf, clock_cycles, 10);
  uart_send_string(" Total clock cycles: ");
  uart_send_string(str);
}

#ifdef STEEL
// ADDRESS MAP
char* UART_TX_ADDRESS = (char*)0x00010000;
char* UART_RX_ADDRESS_DATA = (char*)0x00020000;
char* UART_RX_ADDRESS_READY = (char*)0x00020004;

void uart_send_string(const char* str)
{
  for(int i = 0; i < strlen(str); i++)
  {
    while(isTransmitBusy(UART_TX_ADDRESS)); 
    (*UART_TX_ADDRESS) = str[i];
  }
}

#else
void XUARTChanged_SendByte(uint32_t BaseAddress, uint8_t Data) {
  while (XUartChanged_IsTransmitFull(BaseAddress));
  X_mWriteReg(BaseAddress, PS7_UART_DATA_OUT_REGISTER, Data);
}

void uart_send_string(char *ptr) {
  while (*ptr) {
    while (XUartChanged_IsTransmitFull(BASE_ADDRESS_U));
    X_mWriteReg(BASE_ADDRESS_U, PS7_UART_DATA_OUT_REGISTER, *ptr++);
  }
}

void XUART_Reset(void)
{
  X_mWriteReg(BASE_ADDRESS_U, PS7_UART_CONTROL_REGISTER, UART_CR_RESET_TX_RX); // Ctrl_reg disable interrupts, reset FIFOs
}
#endif


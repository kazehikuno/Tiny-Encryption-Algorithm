/* 	Christopher Hays
	Encryption practice in C using the DE2 board
*/

#include "tea.h"

#define JTAG_ADDR 0x10001000

__attribute__ ((section (".text")));
  
/*** function declarations ***/
void put_jtag(int volatile *, char);  		// print one character to the jtag

const uint32_t my_key[4] = { 0x95a8882c, 0x9d2cc113, 0x815aa0cd, 0xa1c489f7 };		// the key


int main(void){
	int volatile *jtag_uart_addr = (int*)JTAG_ADDR;
	
	int i;
	char input[] = "\nThis is the test string printed from allocated memory.\n> ";		// uart test string
	
	int data_len = strlen(input);
	char *bytes = (char*)malloc(data_len+15);
	for (i = 0; i < data_len; i++){
		bytes[i] = input[i];
	}
	
													// create the arguments for the encrypt function
	uint8_t *data = (uint8_t*)bytes;				// create an array pointer to the data and cast it in unsigned 8-bit chunks
	uint32_t *len = (uint32_t*)&data_len;		// create a pointer to the length of the data, cast it as 32-bit number
	uint32_t *key = (uint32_t*)my_key;				// create a pointer to the key, cast it as 32-bit number
	
	encryptData(data, len, key);						// encrypt
	printf("\n\nEncrypted data: %d bytes.\n",*len);		// print encrypted data
	for (i = 0; i < *len; i++){
		put_jtag(jtag_uart_addr, data[i]);
	}
	
	free(bytes);
	return 0;
}


// write a char to jtag uart
void put_jtag(int volatile *jtag_uart_addr, char c){
	int control;
	control = *(jtag_uart_addr + 1); 		// read the jtag uart control register
	if (control & 0xFFFF0000) 				// if space, then print the character
		*(jtag_uart_addr) = c;				// else, ignore
}

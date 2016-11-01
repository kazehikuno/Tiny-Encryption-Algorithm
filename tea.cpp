#include "tea.h"

void encrypt64(uint32_t* v, uint32_t* k);
void decrypt64(uint32_t* v, uint32_t* k);

void encryptData(uint8_t * data, uint32_t * len, uint32_t * key)
{
	uint32_t blocks, i;						// make sure we are using unsigned 32-bit integers
	uint32_t *data32;						// pointer to data
	data32 = (uint32_t*)data;				// cast the data as unsigned 32-bit integers
	blocks = (((*len) + 7) / 8) + 1;		// determine the number of 8-byte (64 bit) blocks in the data, 
											// adding 7 bytes allows us to round up to the next grouping of 8 bytes
											// then add 1 block to store the original data length 
											// 392 length gives us 49+1=50 blocks, this is the reason we allocate extra space in the main program
											// there are 50 64-bit blocks, so that means 100 32-bit blocks since the data is cast as 32-bit

	data32[(blocks * 2) - 1] = *len;		// use the newly created last block to hold the original data length
	*len = blocks * 8;						// new encrypted data length in bytes (400 bytes in this case)
	for (i = 0; i< blocks; i++)				// loop for the number of 64-bit blocks (50)
	{
		encrypt64(&data32[i * 2], key);		// call encrypt, sending 64 bits at a time (each index is a 32-bit chunk)
	}
}


void decryptData(uint8_t * data, uint32_t * len, uint32_t * key)
{
	uint32_t blocks, i;						// initialize 32-bit unsigned integers
	uint32_t * data32;						// pointer to data
	data32 = (uint32_t *)data;				// cast the data to 32-bit unsinged integers
	blocks = (*len) / 8;					// find the number of 8-byte (64-bit) blocks
	for (i = 0; i< blocks; i++)				// loop for the number of 64-bit blocks
	{
		decrypt64(&data32[i * 2], key);		// call decrypt, sending 64 bits at a time
	}
	*len = data32[(blocks * 2) - 1];		// restore the original length of the data
}


void encrypt64(uint32_t* v, uint32_t* k) {							// v points to 64 bits of data as a 2 elements of 32 bits, k points to the key array
	uint32_t v0 = v[0], v1 = v[1], sum = 0, i;						// v[0] is the first 32 bits, v[1] the second
	uint32_t delta = 0x9e3779b9;									// key schedule constant as determined by the authors
	uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];			// cache the key
	for (i = 0; i < 32; i++) {										// start the 32-round cycle
		sum += delta;												// sum constantly increments by delta
		v0 += ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);		// v0 += (v1 shift left by 4 + key0) XOR (v1+sum) XOR (v1 shift right by 5 + key1)
		v1 += ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);		// v1 += (v0 shift left by 4 + key2) XOR (v0+sum) XOR (v0 shift right by 5 + key3)
	}																// end cycle
	v[0] = v0; v[1] = v1;											// write encrypted data to memory
}


void decrypt64(uint32_t* v, uint32_t* k) {							// v points to 64 bits of data as a 2 elements of 32 bits, k points to the key array
	uint32_t v0 = v[0], v1 = v[1], sum = 0xC6EF3720, i;				// v[0] is the first 32 bits, v[1] the second
	uint32_t delta = 0x9e3779b9;									// key schedule constant as determined by the authors
	uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];			// chache the key
	for (i = 0; i<32; i++) {										// start the 32-round cycle
		v1 -= ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);		// v1 -= (v0 shift left by 4 + key2) XOR (v0+sum) XOR (v0 shift right by 5 + key3)
		v0 -= ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);		// v0 -= (v1 shift left by 4 + key0) XOR (v1+sum) XOR (v1 shift right by 5 + key1)
		sum -= delta;												// sum decrements by delta
	}																// end cycle
	v[0] = v0; v[1] = v1;											// write decrypted data to memory
}
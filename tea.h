#ifndef __TEA_H__
#define __TEA_H__

#include <stdint.h>

void encryptData(uint8_t * data, uint32_t * len, uint32_t * key);
void decryptData(uint8_t * data, uint32_t * len, uint32_t * key);

#endif

/* encryptBlock
*   Encrypts byte array data of length len with key key using TEA
* Arguments:
*   data - pointer to 8 bit data array to be encrypted - SEE NOTES
*   len - length of array
*   key - Pointer to four integer array (16 bytes) holding TEA key
* Returns:
*   data - encrypted data held here
*   len - size of the new data array
* Side effects:
*   Modifies data and len
* NOTES:
* data size must be equal to or larger than ((len + 7) / 8) * 8 + 8
* TEA encrypts in 8 byte blocks, so it must include enough space to
* hold the entire data to pad out to an 8 byte boundary, plus another
* 8 bytes at the end to give the length to the decrypt algorithm.
*
*  - Shortcut - make sure that data is at least len + 15 bytes in size.
*/

/* decryptBlock
*   Decrypts byte array data of length len with key key using TEA
* Arguments:
*   data - pointer to 8 bit data array to be decrypted - SEE NOTES
*   len - length of array
*   key - Pointer to four integer array (16 bytes) holding TEA key
* Returns:
*   data - decrypted data held here
*   len - size of the new data array
* Side effects:
*   Modifies data and len
* NOTES:
*   None
*/

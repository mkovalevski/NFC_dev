#ifndef INC_PN532_H_
#define INC_PN532_H_
#include "main.h"
//PN532 chip

#define __NFC_I2C I2C1
#define __NFC_DMA DMA1_Stream0
#define __DMA_ENABLE DMA_SxCR_EN

#define __PN532_FRAME_MAX_LENGTH	263 //255 + 8 service bytes
#define __PN532_TIMEOUT				1000


#define __PN532_PREAMBLE                      (0x00)
#define __PN532_STARTCODE1                    (0x00)
#define __PN532_STARTCODE2                    (0xFF)
#define __PN532_POSTAMBLE                     (0x00)

#define __PN532_ADDR 0x48
#define __PN532_COMMAND_GETFIRMWAREVERSION (0x02)
#define __PN532_COMMAND_DIAGNOSE  (0x00)

#define __I2C_READ 1
#define __I2C_WRITE 0

uint8_t wait_ready(void);
void read_pn532_data(uint8_t * pData, uint16_t len);

#endif /* INC_PN532_H_ */

#ifndef INC_NFC_CONFIG_H_
#define INC_NFC_CONFIG_H_

//PN532 chip

#define __NFC_I2C I2C1
#define __NFC_DMA DMA1_Stream0
#define __DMA_ENABLE DMA_SxCR_EN

#define __PN532_ADDR 0x48

#define __I2C_READ 1
#define __I2C_WRITE 0


#endif /* INC_NFC_CONFIG_H_ */

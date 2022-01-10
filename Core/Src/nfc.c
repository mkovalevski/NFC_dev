#include "main.h"
#include "nfc_config.h"

extern uint8_t nfc_rx_data[8];
extern uint8_t nfc_tx_data[8];

uint32_t i2c_timeout;

void read_nfc_data(void){
	__NFC_I2C->CR1 |= I2C_CR1_PE;

	__NFC_DMA->CR &=~ __DMA_ENABLE;
	__NFC_DMA->NDTR = 8;
	__NFC_DMA->M0AR = (uint32_t)nfc_rx_data;
	__NFC_DMA->PAR = (uint32_t)&(I2C1->RXDR);
	__NFC_DMA->CR |= __DMA_ENABLE;

	__NFC_I2C->CR1 |= I2C_CR1_RXDMAEN;


	__NFC_I2C->CR2 = (8<<I2C_CR2_NBYTES_Pos) | (__I2C_READ<<I2C_CR2_RD_WRN_Pos) | (__PN532_ADDR<<1) | I2C_CR2_START | I2C_CR2_AUTOEND;
	i2c_timeout = 0x1FFFF;
	while (i2c_timeout) {
		i2c_timeout--;
		if (__NFC_I2C->ISR & (I2C_ISR_BUSY)) break;
	}

	__NFC_I2C->ISR = 0;

	DMA1->LIFCR = 0x3F;
	__NFC_DMA->CR &=~ __DMA_ENABLE;
}

void write_nfc_data(void){

}


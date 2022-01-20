#include "pn532.h"

uint8_t matched;
uint32_t i2c_timeout;

const uint8_t PN532_ACK[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};

extern I2C_HandleTypeDef hi2c1;
extern uint8_t nfc_rx_data[64];
extern uint8_t nfc_tx_data[64];

void get_firmware_version(void){
	uint8_t frame[__PN532_FRAME_MAX_LENGTH];
	uint8_t checksum = 0;
	frame[0] = __PN532_PREAMBLE;
	frame[1] = __PN532_STARTCODE1;
	frame[2] = __PN532_STARTCODE2;
	frame[3] = 1; //LEN length of command (1 byte)
	frame[4] = (~frame[3] + 1); //LCS length of CRC. Lower byte of [LEN + LCS] = 0x00
	frame[5] = 0xD4;
	frame[6] = __PN532_COMMAND_GETFIRMWAREVERSION;
	for (uint8_t i = 0; i < 7; i++) {
		checksum += frame[i];
	}
	frame[7] = ~checksum & 0xFF;
	frame[8] = __PN532_POSTAMBLE;
	//send command
	if (HAL_I2C_Master_Transmit(&hi2c1, __PN532_ADDR, frame, frame[3]+8, __PN532_TIMEOUT) != HAL_OK){
//		Error_Handler();
	}
	//wait for RDY status frame
	if (!wait_ready()){
//		Error_Handler();
	}
	//wait for ACK frame
	//TODO

	//wait for data frame
	//TODO
}


void read_pn532_data(uint8_t * pData, uint16_t len){
	HAL_I2C_Master_Receive(&hi2c1, 0x49, pData, len, __PN532_TIMEOUT);
}

uint8_t wait_ready(void){
//    uint8_t status[] = {0x00, 0x00};
    uint32_t tickstart = HAL_GetTick();
    while (HAL_GetTick() - tickstart < __PN532_TIMEOUT*2) {
    	read_pn532_data(nfc_rx_data, sizeof(nfc_rx_data));
//        if (nfc_rx_data[0] == 0x01 || nfc_rx_data[1]) {
//            return 1;
//        } else {
            HAL_Delay(5);
//        }
    }
    return 0;
}

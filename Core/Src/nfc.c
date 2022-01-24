#include "pn532.h"

uint32_t i2c_timeout;

const uint8_t PN532_ACK[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};

extern I2C_HandleTypeDef hi2c1;
extern uint8_t pn532_fw_version[6];

uint8_t get_firmware_version(void){
	uint8_t txframe[__PN532_FRAME_MAX_LENGTH] = {0};
	uint8_t rxframe[__PN532_FRAME_MAX_LENGTH] = {0};
	uint8_t checksum = 0;
	txframe[0] = __PN532_PREAMBLE;
	txframe[1] = __PN532_STARTCODE1;
	txframe[2] = __PN532_STARTCODE2;
	txframe[3] = 2; //LEN length of command (1 byte of data + TFI)
	txframe[4] = (~txframe[3] + 1); //LCS length of CRC. Lower byte of [LEN + LCS] = 0x00
	txframe[5] = 0xD4;
	txframe[6] = __PN532_COMMAND_GetFWVersion;
	for (uint8_t i = 0; i < 7; i++) {
		checksum += txframe[i];
	}
	txframe[7] = ~checksum & 0xFF;
	txframe[8] = __PN532_POSTAMBLE;
	//send command
	write_pn532_data(txframe, sizeof(txframe));

	//wait for RDY status frame
	if (!wait_ready()){
		Error_Handler(); //no RDY answer
	}
	//read received frame
	read_pn532_data(rxframe, sizeof(rxframe));

	for (uint8_t i=0; i<sizeof(PN532_ACK); i++){
		if (rxframe[i+1] != PN532_ACK[i]){
			Error_Handler();
		}
	}

	for (uint8_t i=0; i<sizeof(pn532_fw_version); i++){
		pn532_fw_version[i] = rxframe[i+7];
	}
	return rxframe[7]; //size of rx data
}

uint8_t wait_ready(void){
	uint8_t status[] = {0x00};
	uint32_t tickstart = HAL_GetTick();
	while (HAL_GetTick() - tickstart < __PN532_TIMEOUT) {
		read_pn532_data(status, sizeof(status));
		if (status[0] == 0x01) {
			return 1;
		}
		else{
			HAL_Delay(5);
		}
	}
	return 0;
}
//TODO: add MIFARE standard
int8_t read_passive_target(uint8_t * pData, uint16_t len){
	uint8_t txframe[__PN532_FRAME_MAX_LENGTH] = {0};
	uint8_t rxframe[__PN532_FRAME_MAX_LENGTH] = {0};
	uint8_t offset;
	uint8_t checksum = 0;
	txframe[0] = __PN532_PREAMBLE;
	txframe[1] = __PN532_STARTCODE1;
	txframe[2] = __PN532_STARTCODE2;
	txframe[3] = 3; //LEN length of command (1 byte of data + TFI)
	txframe[4] = (~txframe[3] + 1); //LCS length of CRC. Lower byte of [LEN + LCS] = 0x00
	txframe[5] = 0xD4;
	txframe[6] = __PN532_COMMAND_InListPassivTargets;
	txframe[7] = 0x01; //num of cards to initialize
	for (uint8_t i = 0; i < 8; i++) {
		checksum += txframe[i];
	}
	txframe[7] = ~checksum & 0xFF;
	txframe[8] = __PN532_POSTAMBLE;

	//send command
	write_pn532_data(txframe, sizeof(txframe));

	//wait for RDY status frame
	if (!wait_ready()){
		Error_Handler(); //no RDY answer
	}
	//read received frame
	read_pn532_data(rxframe, sizeof(rxframe));

	for (uint8_t i=0; i<sizeof(PN532_ACK); i++){
		if (rxframe[i+1] != PN532_ACK[i]){
			Error_Handler();
		}
	}
	offset = 0;
	//find data
	while (rxframe[offset] == 0x00){
		offset++;
		if (offset >= sizeof(rxframe))
			return -1;
	}
	return rxframe[offset]; //return size of rx data
}

void write_pn532_data(uint8_t * pData, uint16_t len){
	//TODO: change HAL by CMSIS
	HAL_I2C_Master_Transmit(&hi2c1, __PN532_ADDR, pData, len, __PN532_TIMEOUT);
}

void read_pn532_data(uint8_t * pData, uint16_t len){
	//TODO: change HAL by CMSIS
	HAL_I2C_Master_Receive(&hi2c1, __PN532_ADDR, pData, len, __PN532_TIMEOUT);
}

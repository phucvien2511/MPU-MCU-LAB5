/*
 * fsm.c
 *
 *  Created on: Dec 1, 2022
 *      Author: Phuc
 */

#include "fsm.h"
ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;
int uart_status = UART_INIT, cmd_status = CMD_INIT;
int cmd_index = 0, ADC_value = -1;
char str[50], str2[50];
uint8_t cmd[50];
uint8_t buffer_bytes = 0, buffer_flag = 0;
uint8_t buffer[MAX_BUFFER_SIZE] = {0};
int validateCmd(int code) {
	if (code == RST) {
		if (cmd[0] == 'R' && cmd[1] == 'S' && cmd[2] == 'T') {
			return 1;
		}
		return 0;
	}
	else if (code == OK) {
		if (cmd[0] == 'O' && cmd[1] == 'K') {
			return 1;
		}
		return 0;
	}
	return 0;
}
void command_parser_fsm() {
	switch (cmd_status) {
	case CMD_INIT:
		if (buffer_bytes == '!') {	//First byte is valid
			cmd_status = CMD_READ;
		}
		break;
	case CMD_READ:
		if (buffer_bytes != '#' && buffer_bytes != '#') {
			cmd[cmd_index++] = buffer_bytes;
		}
		if (buffer_bytes == '#') {
			cmd_status = CMD_STOP;	//Stop reading command
			cmd_index = 0;
			buffer_bytes = 0;
		}
		break;
	case CMD_STOP:
		if (validateCmd(RST)) {
			uart_status = UART_RST;
		}
		else if (validateCmd(OK)) {
			uart_status = UART_OK;
		}
		cmd_status = CMD_INIT;
		break;
	default:
		break;
	}
}
void uart_communication_fsm() {
	switch(uart_status) {
	case UART_INIT:
		break;
	case UART_RST:
		ADC_value = HAL_ADC_GetValue(&hadc1);
		HAL_UART_Transmit(&huart2, (void *)str, sprintf(str, "!ADC=%d#\r\n", ADC_value), 50);
		uart_status = UART_WAIT;
		setTimer2(300);
		break;
	case UART_WAIT:		//Wait for OK command
		if (validateCmd(OK)) {
			uart_status = UART_OK;
			ADC_value = -1;
		}
		else {
			if (timer2_flag == 1) {	//Every 3 seconds while waiting, print previous sensor value
				HAL_UART_Transmit(&huart2, (void *)str, sprintf(str, "!ADC=%d#\r\n", ADC_value), 50);
				setTimer2(300);
			}
		}
		break;
	case UART_OK:
		//Wait for next packet
		uart_status = UART_INIT;
		timer2_flag = 0;
		timer2_counter = 0;
		break;
	default:
		break;
	}
}



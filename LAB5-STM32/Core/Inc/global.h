/*
 * global.h
 *
 *  Created on: Dec 6, 2022
 *      Author: Phuc
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_
#include "main.h"
#include "software_timer.h"

//#include "scheduler.h"
//#include "software_timer.h"


#define MAX_BUFFER_SIZE		30
#define UART_INIT	10
#define UART_RST	11
#define UART_OK		12
#define UART_WAIT	13
#define UART_ERR	14
#define CMD_INIT 	15
#define CMD_READ	16
#define CMD_STOP	17
#define CMD_ERR		18
#define RST			0
#define OK			1
extern uint8_t buffer_bytes;
extern uint8_t buffer_flag;
extern uint8_t buffer[MAX_BUFFER_SIZE];
#endif /* INC_GLOBAL_H_ */

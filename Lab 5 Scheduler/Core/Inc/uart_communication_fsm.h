/*
 * uart_communication_fsm.h
 *
 *  Created on: Nov 16, 2023
 *      Author: HP
 */

#ifndef INC_UART_COMMUNICATION_FSM_H_
#define INC_UART_COMMUNICATION_FSM_H_
#include "main.h"

#define OK				  4
#define REQUEST			  5
#define END_COMMUNICATION	8
#define NORMAL				1
#define SEND_ADC			9
#define WAITING				3
#define ERROR_COMMAND		10
#define IDLE				11
#define RECEIVE				12
#define MAX_BUFFER_SIZE 30
#define COMMAND_WATING		7

extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;

extern uint8_t temp;
extern uint8_t buffer [MAX_BUFFER_SIZE];
extern uint8_t buffer_flag;
extern int command_flag;
extern int command_state;
extern int status_UART;
extern uint8_t command_data [MAX_BUFFER_SIZE];
void uart_communication_fsm ();
void command_parser_fsm();

#endif /* INC_UART_COMMUNICATION_FSM_H_ */

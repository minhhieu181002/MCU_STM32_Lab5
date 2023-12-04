/*
 * uart_communication_fsm.c
 *
 *  Created on: Nov 16, 2023
 *      Author: HP
 */
#include"uart_communication_fsm.h"
#include"software_timer.h"
#include"main.h"
#include"global.h"
#include <stdio.h>
#include <string.h>
#include<scheduler.h>



uint8_t temp = 0;
uint8_t buffer [MAX_BUFFER_SIZE] ={0};
uint8_t buffer_flag = 0;

int command_state = 0;
const char userRequest[] = "!RST#";
const char userEnd[] = "!OK#";

char str[100];
uint8_t command_data[MAX_BUFFER_SIZE]={0};
int status_UART = 0;
int cnt_ADC_value = 0;
int command_flag = 0;
int idx_command_data = 0;
void normal_mode() {
	HAL_SuspendTick();
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	HAL_ResumeTick();
}
void clearBuffer() {
	for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
		buffer[i] = 0;
	}
	index_buffer = 0;
}
void clearCommand() {
	for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
			command_data[i] = 0;
	}
	idx_command_data = 0;
}

int end_command(){
	if(command_data[0]=='O' && command_data[1]=='K' && command_data[2]==0) return 1;
	return 0;

}

int request_command(){
	if(command_data[0]=='R'&&command_data[1]=='S'&& command_data[2]=='T'&& command_data[3]==0) return 1;
	return 0;

}

void check_command_data () {
	if(request_command()==1){
		ADC_value = HAL_ADC_GetValue(&hadc1);
		//command_flag = 1;
		status_UART = SEND_ADC;
		clearCommand();
	}else if(end_command()==1){
		status_UART = END_COMMUNICATION;
		clearCommand();
	}else {
		clearCommand();
	}
}
void command_parser_fsm () {
	switch(command_state) {
	case IDLE:
		if(temp == '!') {
			command_state = RECEIVE;
			status_UART = COMMAND_WATING;
			idx_command_data = 0;
		}
		break;
	case RECEIVE:
		if(temp=='#'){
			check_command_data();
			command_state = IDLE;
		}
		else if(temp != '#') {
			command_data[idx_command_data]=temp;
			idx_command_data++;
		}
		else if(temp=='!'){
			clearCommand();
			idx_command_data=0;
		}
		break;
	default:
			break;
	}
	//HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
}
int cnt = 0;
void taskWaitForOK () {
	cnt++;
	if(cnt == 30){
		status_UART = SEND_ADC;
		cnt = 0;
	}
}
void uart_communication_fsm () {
	switch(status_UART) {
			case COMMAND_WATING:
				if(command_flag == 1){
					status_UART = SEND_ADC;
					command_flag = 0;
				}

				break;
			case NORMAL:
				normal_mode();
				break;
			case SEND_ADC:
				HAL_UART_Transmit(&huart2, (uint8_t*)str, sprintf(str,"\r\n!ADC=%lu#\r\n", ADC_value), 1000);

				status_UART = WAITING;
				break;
			case WAITING:
				taskWaitForOK();
				break;

			case END_COMMUNICATION:
				HAL_UART_Transmit(&huart2, (uint8_t*)str,
						sprintf(str, "%s","\r\nCommunication is end\r\n"), 1000);
				status_UART = COMMAND_WATING;
				break;
//
	}
	//HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
}

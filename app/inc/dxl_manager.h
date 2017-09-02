// Muthanna A. Attyah
// Created on: Jul 29, 2017
// please feel free to copy this code and use it

//------------------------------------------------------------------------------------------------

// defines
//------------------------------------------------------------------------------------------------

#ifndef APP_INC_DXL_MANAGER_H_
#define APP_INC_DXL_MANAGER_H_

#define UTILITY_VER		"1.0"
#define UTILITY_NAME	"Dynamixel Manager Utility"
#define UTILITY_FILE	"./dxlman"

#define PROTOCOL_V1		1.0				// Dynamixel Protocol Version 1.0
#define PROTOCOL_V2		2.0				// Dynamixel Protocol Version 2.0

#define PBSTR "############################################################" 	// progress bar string
#define PBWIDTH 60																// progress bar length

#define MAX_CLINE_LEN	128				// Maximum command line length
#define MAX_CMD_LEN		15				// Maximum command length
#define MAX_PARAM_COUNT	20				// Maximum number of parameters per command
#define MAX_PARAM_LEN	30				// Maximum length of a parameter
#define MAX_MNAME_LEN 	15				// Maximum length of Dynamixel Model Name
#define MAX_PNAME_LEN 	30				// Maximum length of Port Device Name
#define MAX_PBAUD_LEN	30				// Maximum length of Port Baud

#define COMPORTNAME     "/dev/ttyACM0"	// Default communication port name
#define COMPORTBAUD		"1000000"		// Default communication port baud rate

//------------------------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------------------------

#include <stdio.h>			// Standard input/output definitions
#include <unistd.h>			// UNIX standard function definition
#include <stdlib.h>			// General functions, macros, and variable types
#include <string.h>			// Manipulating arrays of characters

#include "dynamixel_sdk.h"	// Dynamixel SDK

//------------------------------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------------------------------

char    cmd[MAX_CMD_LEN];	// to store received command

//------------------------------------------------------------------------------------------------
// Command Functions
//------------------------------------------------------------------------------------------------

int getCmd(void);

int cmdHelp(void);
int cmdInfo(void);
int cmdClear(void);

int cmdBaud(void);
int cmdPort(void);

int cmdPing(void);
int cmdScan(void);

int cmdWrB1(void);
int cmdWrB2(void);
int cmdWrW1(void);
int cmdWrW2(void);
int cmdWrD2(void);

int cmdRdB1(void);
int cmdRdB2(void);
int cmdRdW1(void);
int cmdRdW2(void);
int cmdRdD2(void);

int cmdReset1(void);
int cmdReset2(void);
int cmdReboot2(void);

void cmdExit(void);
void cmdBad(void);

//------------------------------------------------------------------------------------------------
// Helper Functions
//------------------------------------------------------------------------------------------------

void startBanner(void);
void getOptions(int argc, char* argv[]);
void usage(void);
void printProgress (double percentage);
void portInfo(void);

int portOpen(void);
int portBaud(void);

void writeNByteTxRx(int protocol_version, uint8_t id, uint16_t addr, uint16_t length, uint32_t value);
void readNByteTxRx(int protocol_version, uint8_t id, uint16_t addr, uint16_t length);

void modelName(int dxl_model_num, char* model_name);

//------------------------------------------------------------------------------------------------

#endif /* APP_INC_DXL_MANAGER_H_ */

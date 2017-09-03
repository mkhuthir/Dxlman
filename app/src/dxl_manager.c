// Muthanna A. Attyah
// Created on: Jul 29, 2017
// please feel free to copy this code and use it

//------------------------------------------------------------------------------------------------

#include "dxl_manager.h"

//------------------------------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------------------------------

char    paramVal[MAX_PARAM_COUNT][MAX_PARAM_LEN];	// to store command parameters
int     paramCount;									// to store command parameters count

int 	comPortNum;									// used to store communication port number
char 	comPortName[MAX_PNAME_LEN] = COMPORTNAME;	// use COMPORTNAME if there was no command line parameter given.
char	comPortBaud[MAX_PBAUD_LEN] = COMPORTBAUD;	// use COMPORTBAUD if there was no command line parameter given.

//------------------------------------------------------------------------------------------------
// Command Functions
//------------------------------------------------------------------------------------------------

int getCmd(void)
{

	char    input[MAX_CLINE_LEN];
	char*   token;

	printf(">");

	if (fgets(input, MAX_CLINE_LEN, stdin)==NULL)
		{
			printf("Error reading input!!");
			return -1;
		}

	char *p;
	if ((p = strchr(input, '\n'))!= NULL) *p = '\0';
	fflush(stdin);

	if (strlen(input) == 0)	return -1;

	token = strtok(input, " ");
	if (token == 0)	return -1;

	strcpy(cmd, token);

	token = strtok(0, " ");
	paramCount = 0;

	while(token != 0)
	{
		strcpy(paramVal[paramCount], token);
		paramCount=paramCount+1;
		token = strtok(0, " ");
	}

	return 0;

}
//------------------------------------------------------------------------------------------------

int cmdHelp(void)
{
	printf("\n");

	printf(" DXL Manager Command List\n\n");

	printf(" ==== Common Commands for Dynamixel Protocol 1.0 & 2.0 =====\n");

	printf(" h|?                :Displays help\n");
	printf(" info               :Prints utility info.\n");
	printf(" clear              :Clears screen\n");
	printf(" exit               :Exits this program\n");
	printf(" \n");

	printf(" port [PName]       :Displays/Changes port name\n");
	printf(" baud [BAUD]        :Displays/Changes baudrate\n");
	printf(" ping [ID] [ID]...  :Pings Dynamixel\n");
	printf(" scan               :Scans all Dynamixel\n");
	printf(" \n");

	printf(" =========== Commands for Dynamixel Protocol 1.0 ===========\n");

	printf(" rb1 [ID] [ADR]     :Reads byte\n");
	printf(" rw1 [ID] [ADR]     :Reads word\n");
	printf(" \n");

	printf(" wb1 [ID] [ADR] [B] :Writes byte\n");
	printf(" ww1 [ID] [ADR] [B] :Writes word\n");
	printf(" \n");

	printf(" rst1 [ID]          :Factory resets Dynamixel\n");
	printf(" \n");

	printf(" ============ Commands for Dynamixel Protocol 2.0 ==========\n");

	printf(" rb2 [ID] [ADR]     :Reads byte\n");
	printf(" rw2 [ID] [ADR]     :Reads word\n");
	printf(" rd2 [ID] [ADR]     :Reads double word\n");
	printf(" \n");

	printf(" wb2 [ID] [ADR] [B] :Writes byte\n");
	printf(" ww2 [ID] [ADR] [W] :Writes word\n");
	printf(" wd2 [ID] [ADR] [D] :Writes double word\n");
	printf(" \n");

	printf(" rst2 [ID]          :Factory resets Dynamixel\n");
	printf(" rbt2 [ID]          :Reboots Dynamixel\n");
	printf("\n");

	return 0;
}
//------------------------------------------------------------------------------------------------

int cmdInfo(void)
{
	printf("\n%s Version %s\n\n",UTILITY_NAME,UTILITY_VER);
	portInfo();
	return 0;
}
//------------------------------------------------------------------------------------------------

int cmdClear(void)
{
	return system("clear");
}
//------------------------------------------------------------------------------------------------

int cmdBaud(void)
{
	char tmpPortBaud[MAX_PBAUD_LEN];
	if (paramCount==0)
	{
		printf("Current Baudrate: %d\n", getBaudRate(comPortNum));
	}
	else if (paramCount == 1)
	{
		strcpy(tmpPortBaud,comPortBaud); // Save old baud
		strcpy(comPortBaud,paramVal[0]); // Copy new baud

		if(portBaud()==0)
			return 0;
		else
		{
			strcpy(comPortBaud,tmpPortBaud); // Restore old port
			return -1;
		}

	}
	else
	{
		fprintf(stderr, "Invalid parameters! \n");
		return -1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------

int cmdPort(void)
{
	int tmpPortNum;
	char tmpPortName[MAX_PNAME_LEN];

	switch(paramCount)
	{
		case 0:									// list existing ports information.
			portInfo();
			return 0;

		case 1:									// Open a new port
			strcpy(tmpPortName,comPortName); 	// Save old port name
			tmpPortNum=comPortNum;			 	// Save old port no.
			strcpy(comPortName,paramVal[0]); 	// Copy new port
			if(portOpen()==0)
			{
				portBaud();
				return 0;
			}
			else
			{
				strcpy(comPortName,tmpPortName);// Restore old port name
				comPortNum=tmpPortNum;			// Restore old port no.
				portOpen();
				return -1;
			}
		case 2:									// Open a new port and set its baud rate.
			return 0;

		default:
			fprintf(stderr, "Invalid parameters! \n");
			return -1;
	}

}
//------------------------------------------------------------------------------------------------

int cmdPing(void)
{
	int i;
	uint16_t dxl_model_num;
	int dxl_comm_result;

	if (paramCount == 0)
	{
		printf(" Invalid parameters! \n");
		return -1;
	}

	printf("\nping using both protocols version 1.0 & 2.0\n");

	for (i = 0; i < paramCount; i++)
	{
		dxl_model_num = pingGetModelNum(comPortNum, PROTOCOL_V1, atoi(paramVal[i]));

		if ((dxl_comm_result = getLastTxRxResult(comPortNum, PROTOCOL_V1)) == COMM_SUCCESS)
			printf("Success...[ID:%.3d] ModelNo:[%.5d] Protocol Ver 1.0\n", atoi(paramVal[i]), dxl_model_num);
		else
		{
			dxl_model_num = pingGetModelNum(comPortNum, PROTOCOL_V2, atoi(paramVal[i]));

			if ((dxl_comm_result = getLastTxRxResult(comPortNum, PROTOCOL_V1)) == COMM_SUCCESS)
				printf("Success...[ID:%.3d] ModelNo:[%.5d] Protocol Ver 2.0\n", atoi(paramVal[i]), dxl_model_num);
			else
				printf("Failed....[ID:%.3d] !!!! \n", atoi(paramVal[i]));
		}
	}

	printf("\n");
	return 0;
}
//------------------------------------------------------------------------------------------------

int cmdScan(void)
{
	int id;
	int count=0;

	char model_name[MAX_MNAME_LEN];

	uint16_t dxl_model_num;
	int dxl_comm_result;


	if (paramCount!=0)
	{
		printf("Invalid parameters! \n");
		return -1;
	}

	printf("\nScanning device %s at buadrate %d ....\n\n",comPortName,getBaudRate(comPortNum));
	printf("ID#  Prot.  Model  Name           \n");
	printf("----------------------------------\n");

	for (id = 1; id <= MAX_ID; id++)
	{
		dxl_model_num = pingGetModelNum(comPortNum, PROTOCOL_V1, id);

		if ((dxl_comm_result = getLastTxRxResult(comPortNum, PROTOCOL_V1)) == COMM_SUCCESS)
		{
			modelName(dxl_model_num, model_name);
			printf("\r                                                                   ");
			printf("\r%.3d  1.0    %.5d  %-*s \n", id, dxl_model_num,MAX_MNAME_LEN,model_name);
			count++;
		}
		else
		{
			dxl_model_num = pingGetModelNum(comPortNum, PROTOCOL_V2, id);
			if ((dxl_comm_result = getLastTxRxResult(comPortNum, PROTOCOL_V2)) == COMM_SUCCESS)
			{
				modelName(dxl_model_num, model_name);
				printf("\r                                                                   ");
				printf("\r%.3d  2.0    %.5d  %-*s \n", id, dxl_model_num,MAX_MNAME_LEN,model_name);
				count++;
			}
			else
			{
			printProgress((double)id/MAX_ID);
			}
		}

	}
	printf("\r                                                                   ");
	printf("\nScan is completed; Found %d devices.\n\n",count);
	return 0;
}
//------------------------------------------------------------------------------------------------

int cmdWrB1(void)
{
	if (paramCount == 3)
	{
		writeNByteTxRx(PROTOCOL_V1, atoi(paramVal[0]), atoi(paramVal[1]), 1, atoi(paramVal[2]));
	}
	else
	{
		printf("Invalid parameters! \n");
		return -1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------

int cmdWrB2(void)
{
	if (paramCount == 3)
	{
		writeNByteTxRx(PROTOCOL_V2, atoi(paramVal[0]), atoi(paramVal[1]), 1, atoi(paramVal[2]));
	}
	else
	{
		printf("Invalid parameters! \n");
		return -1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------

int cmdWrW1(void)
{
	if (paramCount == 3)
	{
		writeNByteTxRx(PROTOCOL_V1, atoi(paramVal[0]), atoi(paramVal[1]), 2, atoi(paramVal[2]));
	}
	else
	{
		printf("Invalid parameters! \n");
		return -1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------

int cmdWrW2(void)
{
	if (paramCount == 3)
	{
		writeNByteTxRx(PROTOCOL_V2, atoi(paramVal[0]), atoi(paramVal[1]), 2, atoi(paramVal[2]));
	}
	else
	{
		printf("Invalid parameters! \n");
		return -1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------

int cmdWrD2(void)
{
	if (paramCount == 3)
	{
		writeNByteTxRx(PROTOCOL_V2, atoi(paramVal[0]), atoi(paramVal[1]), 4, atoi(paramVal[2]));
	}
	else
	{
		printf("Invalid parameters! \n");
		return -1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------

int cmdRdB1(void)
{
	if (paramCount == 2)
	{
		readNByteTxRx(PROTOCOL_V1, atoi(paramVal[0]), atoi(paramVal[1]), 1);
	}
	else
	{
		printf("Invalid parameters! \n");
		return -1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------

int cmdRdB2(void)
{
	if (paramCount == 2)
	{
		readNByteTxRx(PROTOCOL_V2, atoi(paramVal[0]), atoi(paramVal[1]), 1);
	}
	else
	{
		printf("Invalid parameters! \n");
		return -1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------

int cmdRdW1(void)
{
	if (paramCount == 2)
	{
		readNByteTxRx(PROTOCOL_V1, atoi(paramVal[0]), atoi(paramVal[1]), 2);
	}
	else
	{
		printf("Invalid parameters! \n");
		return -1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------

int cmdRdW2(void)
{
	if (paramCount == 2)
	{
		readNByteTxRx(PROTOCOL_V2, atoi(paramVal[0]), atoi(paramVal[1]), 2);
	}
	else
	{
		printf("Invalid parameters! \n");
		return -1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------

int cmdRdD2(void)
{
	if (paramCount == 2)
	{
		readNByteTxRx(PROTOCOL_V2, atoi(paramVal[0]), atoi(paramVal[1]), 4);
	}
	else
	{
		printf("Invalid parameters! \n");
		return -1;
	}
	return 0;
}

//------------------------------------------------------------------------------------------------

int cmdReset1(void)
{
	uint8_t dxl_error;
	if (paramCount == 1)
	{
		int dxl_comm_result;
		factoryReset(comPortNum, PROTOCOL_V1, atoi(paramVal[0]), 0x00);
		if ((dxl_comm_result = getLastTxRxResult(comPortNum, PROTOCOL_V1)) == COMM_SUCCESS)
		{
			if ((dxl_error = getLastRxPacketError(comPortNum, PROTOCOL_V1)) != 0)
				printRxPacketError(PROTOCOL_V1, dxl_error);
			printf("\nSuccess to reset! \n\n");
		}
		else
		{
			printTxRxResult(PROTOCOL_V1, dxl_comm_result);
			printf("\nFail to reset! \n\n");
			return -1;
		}
	}
	else
	{
		printf("Invalid parameters! \n");
		return -1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------

int cmdReset2(void)
{
	uint8_t dxl_error;
	if (paramCount == 1)
	{
		int dxl_comm_result;
		factoryReset(comPortNum, PROTOCOL_V2, atoi(paramVal[0]), 0x00);
		if ((dxl_comm_result = getLastTxRxResult(comPortNum, PROTOCOL_V2)) == COMM_SUCCESS)
		{
			if ((dxl_error = getLastRxPacketError(comPortNum, PROTOCOL_V2)) != 0)
				printRxPacketError(PROTOCOL_V2, dxl_error);
			printf("\nSuccess to reset! \n\n");
		}
		else
		{
			printTxRxResult(PROTOCOL_V2, dxl_comm_result);
			printf("\nFail to reset! \n\n");
			return -1;
		}
	}
	else
	{
		printf("Invalid parameters! \n");
		return -1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------

int cmdReboot2(void)
{
	uint8_t dxl_error;
	if (paramCount == 1)
	{
		int dxl_comm_result;
		reboot(comPortNum, PROTOCOL_V2, atoi(paramVal[0]));
		if ((dxl_comm_result = getLastTxRxResult(comPortNum, PROTOCOL_V2)) == COMM_SUCCESS)
		{
			if ((dxl_error = getLastRxPacketError(comPortNum, PROTOCOL_V2)) != 0)
				printRxPacketError(PROTOCOL_V2, dxl_error);
			printf("\nSuccess to reboot! \n\n");
		}
		else
		{
			printTxRxResult(PROTOCOL_V2, dxl_comm_result);
			printf("\nFail to reboot! \n\n");
			return -1;
		}
	}
	else
	{
		printf("Invalid parameters! \n");
		return -1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------

void cmdExit(void)
{
	closePort(comPortNum);
	printf("\n... Thanks for using Dynamixel Manager ...\n\n");
	exit(EXIT_SUCCESS);
}
//------------------------------------------------------------------------------------------------

void cmdBad(void)
{
	printf("Bad command! Please type 'Help'.\n");
}

//------------------------------------------------------------------------------------------------
// Helper Functions
//------------------------------------------------------------------------------------------------

void startBanner(void)
{

	printf("\n%s Version %s\n\n",UTILITY_NAME,UTILITY_VER);

}
//------------------------------------------------------------------------------------------------

void getOptions(int argc, char* argv[])
{
	int c;
	opterr = 0;	// Disable getopt errors

	while ((c = getopt (argc, argv, "hp:b:")) != -1)
	    switch (c)
	      {

	      case 'h':
	    	  usage();
	    	  exit(EXIT_SUCCESS);

	      case 'p':
	    	  strcpy(comPortName,optarg);
	    	  break;

	      case 'b':
	      	  strcpy(comPortBaud,optarg);
	      	  strcpy(paramVal[0],comPortBaud);
	      	  paramCount=1;
	      	  break;

	      case '?':
	    	  if (optopt == 'd')
	    		  printf ("Option -%c requires an argument.\n", optopt);
	    	  else
	    		  printf ("Unknown option `-%c'\n", optopt);
	    	  	  usage();
	    	  	  exit(EXIT_FAILURE);

	      default:
	    	  usage();
	    	  exit(EXIT_SUCCESS);
	      }
}
//------------------------------------------------------------------------------------------------

void usage(void)
{

	printf("\nUsage: %s [-h][-d devicename][-b baudrate]\n\n", UTILITY_FILE);
	printf(" None :Opens default port %s\n",COMPORTNAME);
	printf(" -p   :Opens serial port devicename\n");
	printf(" -b   :Sets baud rate to baudrate\n");
	printf(" -h   :Displays this help\n\n");
	exit(EXIT_SUCCESS);

}
//------------------------------------------------------------------------------------------------

void printProgress (double percentage)
{
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf ("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush (stdout);
}
//------------------------------------------------------------------------------------------------

void portInfo(void)
{
	printf("Current port no. is %d\n",comPortNum);
	printf("List of ports:-\n");
	for(int port=0; port<g_used_port_num;port++)
	{
		printf("   No. : %d\n"  , port);
		printf("   Name: %s\n"  , getPortName(port));
		printf("   Baud: %d\n\n", getBaudRate(port));
	}
	printf("Total Open ports: %d\n",g_used_port_num);

}
//------------------------------------------------------------------------------------------------

int portOpen(void)
{

	comPortNum = portHandler(comPortName);

	if (openPort(comPortNum))
	{
		printf("Opened serial port %s\n", comPortName);
		packetHandler();
		return 0;
	}
	else
	{
		printf("Failed to open port %s !\n", comPortName);
		return -1;
	}
}
//------------------------------------------------------------------------------------------------

int portBaud(void)
{
	if (setBaudRate(comPortNum, atoi(comPortBaud)))
	{
		printf("Changed baud rate to %s\n", comPortBaud);
		return 0;
	}
	else
	{
		printf("Failed to change baud rate to %s !\n", comPortBaud);
		return -1;
	}

}
//------------------------------------------------------------------------------------------------

void writeNByteTxRx(int protocol_version, uint8_t id, uint16_t addr, uint16_t length, uint32_t value)
{
	uint8_t dxl_error = 0;
	int dxl_comm_result = COMM_TX_FAIL;

	if (length == 1)
	{
		write1ByteTxRx(comPortNum, protocol_version, id, addr, (uint8_t)value);
	}
	else if (length == 2)
	{
		write2ByteTxRx(comPortNum, protocol_version, id, addr, (uint16_t)value);
	}
	else if (length == 4)
	{
		write4ByteTxRx(comPortNum, protocol_version, id, addr, (uint32_t)value);
	}
	if ((dxl_comm_result = getLastTxRxResult(comPortNum, protocol_version)) == COMM_SUCCESS)
	{
		if ((dxl_error = getLastRxPacketError(comPortNum, protocol_version)) != 0)
			printRxPacketError(protocol_version, dxl_error);
		printf("\nSuccess to write\n\n");
	}
	else
	{
		printTxRxResult(protocol_version, dxl_comm_result);
		printf("\nFail to write! \n\n");
	}
}
//------------------------------------------------------------------------------------------------

void readNByteTxRx(int protocol_version, uint8_t id, uint16_t addr, uint16_t length)
{
	uint8_t dxl_error = 0;
	int     dxl_comm_result = COMM_TX_FAIL;

	int8_t  value8    = 0;
	int16_t value16   = 0;
	int32_t value32   = 0;

	if (length == 1)
	{
		value8 = read1ByteTxRx(comPortNum, protocol_version, id, addr);
	}
	else if (length == 2)
	{
		value16 = read2ByteTxRx(comPortNum, protocol_version, id, addr);
	}
	else if (length == 4)
	{
		value32 = read4ByteTxRx(comPortNum, protocol_version, id, addr);
	}

	if ((dxl_comm_result = getLastTxRxResult(comPortNum, protocol_version)) == COMM_SUCCESS)
	{
		if ((dxl_error = getLastRxPacketError(comPortNum, protocol_version)) != 0)
			printRxPacketError(protocol_version, dxl_error);

		if (length == 1)
		{
			printf("\nID:%d > Addr:0x%02x > Data:0x%02x=0d%d\n",id,addr,value8,value8);
		}
		else if (length == 2)
		{
			printf("\nID:%d > Addr:0x%02x > Data:0x%04x=0d%d\n",id,addr,value16,value16);
		}
		else if (length == 4)
		{
			printf("\nID:%d > Addr:0x%02x > Data:0x%08x=0d%d\n",id,addr,value32,value32);
		}
	}
	else
	{
		printTxRxResult(protocol_version, dxl_comm_result);
		printf("\nFail to write!\n\n");
	}
}
//------------------------------------------------------------------------------------------------

void modelName(int dxl_model_num, char* model_name)
{
	switch(dxl_model_num)
	{


	case 10:
		strcpy(model_name,"RX-10");
		break;

	case 12:
		strcpy(model_name,"AX-12/12A/12+");
		break;

	case 13:
		strcpy(model_name,"AX-S1");
		break;

	case 18:
		strcpy(model_name,"AX-18A/18F)");
		break;

	case 24:
		strcpy(model_name,"RX-24F");
		break;

	case 28:
		strcpy(model_name,"RX-28");
		break;

	case 29:
		strcpy(model_name,"MX-28T/R/AT/AR");
		break;

	case 30:
		strcpy(model_name,"MX-28");
		break;

	case 44:
		strcpy(model_name,"AX-12W");
		break;

	case 54:
		strcpy(model_name,"MX-64T/R/AT/AR");
		break;

	case 64:
		strcpy(model_name,"RX-64/MX-106T/R");
		break;

	case 104:
		strcpy(model_name,"MX-12W");
		break;

	case 107:
		strcpy(model_name,"EX-106+");
		break;

	case 113:
		strcpy(model_name,"DX-113");
		break;

	case 116:
		strcpy(model_name,"DX-116");
		break;

	case 117:
		strcpy(model_name,"DX-117");
		break;

	case 311:
		strcpy(model_name,"MX-64");
		break;

	case 321:
		strcpy(model_name,"MX-106");
		break;

	case 350:
		strcpy(model_name,"XL320");
		break;

	case 360:
		strcpy(model_name,"MX-12W");
		break;

	case 1000:
		strcpy(model_name,"XH430-W350");
		break;

	case 1010:
		strcpy(model_name,"XH430-W210");
		break;

	case 1020:
		strcpy(model_name,"XM430-W350");
		break;

	case 1030:
		strcpy(model_name,"XM430-W210");
		break;

	case 1040:
		strcpy(model_name,"XH430-V350");
		break;

	case 1050:
		strcpy(model_name,"XH430-V210");
		break;

	case 35072:
		strcpy(model_name,"L42-10-S300-R");
		break;

	case 37896:
		strcpy(model_name,"L54-30-S500-R");
		break;

	case 37928:
		strcpy(model_name,"L54-30-S400-R");
		break;

	case 38152:
		strcpy(model_name,"L54-50-S500-R");
		break;

	case 38176:
		strcpy(model_name,"L54-50-S290-R");
		break;

	case 43288:
		strcpy(model_name,"M42-10-S260-R");
		break;

	case 46096:
		strcpy(model_name,"M54-40-S250-R");
		break;

	case 46352:
		strcpy(model_name,"M54-60-S250-R");
		break;

	case 51200:
		strcpy(model_name,"H42-20-S300-R");
		break;

	case 53768:
		strcpy(model_name,"H54-100-S500-R");
		break;

	case 54024:
		strcpy(model_name,"H54-200-S500-R");
		break;

	default :strcpy(model_name,"Unknown");
	}
}
//------------------------------------------------------------------------------------------------

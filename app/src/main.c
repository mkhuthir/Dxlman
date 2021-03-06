// Muthanna A. Attyah
// Created on: Jul 29, 2017
// please feel free to copy this code and use it

//------------------------------------------------------------------------------------------------

#include "dxl_manager.h"

//------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{

	startBanner();							// Print startup Banner
	getOptions(argc,argv);					// Parse command line parameters

	if (portOpen()!=0) exit(EXIT_FAILURE);	// Open port
	if (portBaud()!=0) exit(EXIT_FAILURE);	// Set port baud rate


// Main loop

	printf("\nType h or ? for help on commands.\n\n");
	while(1)
	{

		if (getCmd()!=0) continue;

		if (strcmp(cmd, "h")==0) cmdHelp();
		else if (strcmp(cmd,"?")==0) cmdHelp();
		else if (strcmp(cmd,"info")==0) cmdInfo();
		else if (strcmp(cmd,"clear")==0) cmdClear();
		else if (strcmp(cmd,"baud")==0) cmdBaud();
		else if (strcmp(cmd,"port")==0) cmdPort();
		else if (strcmp(cmd,"ping")==0) cmdPing();
		else if (strcmp(cmd,"scan")==0) cmdScan();

		else if (strcmp(cmd,"wb1")==0) cmdWrB1();
		else if (strcmp(cmd,"wb2")==0) cmdWrB2();
		else if (strcmp(cmd,"ww1")==0) cmdWrW1();
		else if (strcmp(cmd,"ww2")==0) cmdWrW2();
		else if (strcmp(cmd,"wd2")==0) cmdWrD2();
		else if (strcmp(cmd,"rb1")==0) cmdRdB1();
		else if (strcmp(cmd,"rb2")==0) cmdRdB2();
		else if (strcmp(cmd,"rw1")==0) cmdRdW1();
		else if (strcmp(cmd,"rw2")==0) cmdRdW2();
		else if (strcmp(cmd,"rd2")==0) cmdRdD2();

		else if (strcmp(cmd,"rbt2")==0) cmdReboot2();
		else if (strcmp(cmd,"rst1")==0) cmdReset1();
		else if (strcmp(cmd,"rst2")==0) cmdReset2();

		else if (strcmp(cmd,"exit")==0) cmdExit();
		else cmdBad();
	}
}

/*
*	FILE - MAIN.CPP
*	PURPORSE - Set-up all necessary components of program and wait for user input
* 
*	AUTHOR - Dennis Fogerty
*	DATE - 3/14/2016
*/

#include <iostream>
#include <stdio.h>
#include "defs.h"
#include "board.h"
#include "hash.h"
#include "io_handler.h"

int main() {

	std::cout << ("\n");
	std::cout << ENGINE_NAME << std::endl;
	std::cout << ("RPI / SD&D / SPRING 2016\n");
	std::cout << ("\n");
	std::cout << ("Type \"help\" or \"?\" to display a list of commands.\n");
	std::cout << ("\n");

	InitAll();
	Board b1;
	Hash HashGenerator;
	InputHandler IO_Handler;

	b1.ParseFEN(START_FEN, HashGenerator);
	IO_Handler.IO_Loop(b1);

	return 0;
}
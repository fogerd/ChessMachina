/*
*	FILE - IO_HANDLER.H
*	PURPORSE - Object for handling input commands
 * 
*	AUTHOR - Dennis Fogerty
*	DATE - 4/3/2016
*/

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H


class InputHandler {
	public:
		InputHandler() {};
		int IO_Loop(Board GameBoard);
		std::pair<int,int> ParseMove(char *move_input, Board GameBoard);
		char input[256];
};

#endif
#define _CRT_SECURE_NO_WARNINGS

#include "InputHandler.h"
#include "Board.h"
#include "NmkEngine.h"
#include <assert.h>
#include "Player.h"
#include <iostream>

#include <chrono>
using namespace std::chrono;
#include <fstream>

#define MAX_INPUT_LENGTH 50
#define GENERATE_COMMAND "GEN_ALL_POS_MOV"
#define GENERATE_CUT_COMMAND "GEN_ALL_POS_MOV_CUT_IF_GAME_OVER"
#define SOLVE_COMMAND "SOLVE_GAME_STATE"

void InputHandler::handle() {
	char input[MAX_INPUT_LENGTH];
	std::ofstream myfile;
	myfile.open("result2.txt");
	//myfile << "Writing this to a file.\n";
	while (true) {
		scanf("%s", input);
		if (feof(stdin) != 0) {
			break;
		}
		//auto t1 = high_resolution_clock::now();
		int height;
		int width;
		int minToWin;
		int playerNum;
		scanf("%d %d %d %d", &height, &width, &minToWin, &playerNum);
		Player player = Player(playerNum);
		Board board = Board(width, height);
		board.read();
		NmkEngine engine = NmkEngine(board, minToWin, player);
		if (strcmp(input, GENERATE_COMMAND) == 0) {
			engine.generate(false);
		} else if (strcmp(input, GENERATE_CUT_COMMAND) == 0) {
			engine.generate(true);
		} else if (strcmp(input, SOLVE_COMMAND) == 0) {
			engine.solve(myfile);
		} else {
			printf("Invalid command: %s\n", input);
		}
		//auto t2 = high_resolution_clock::now();
		//duration<double, std::milli> ms_double = t2 - t1;

		//std::cout << ms_double.count() << "ms\n";
	}
	myfile.close();
}

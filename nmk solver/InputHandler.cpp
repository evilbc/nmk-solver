#define _CRT_SECURE_NO_WARNINGS

#include "InputHandler.h"
#include "Board.h"
#include "NmkEngine.h"
#include <assert.h>
#include "Player.h"
#include <iostream>

#define MAX_INPUT_LENGTH 50
#define GENERATE_COMMAND "GEN_ALL_POS_MOV"
#define GENERATE_CUT_COMMAND "GEN_ALL_POS_MOV_CUT_IF_GAME_OVER"
#define SOLVE_COMMAND "SOLVE_GAME_STATE"
int n = 0;

void InputHandler::handle() {
	char input[MAX_INPUT_LENGTH];
	while (true) {
		scanf("%s", input);
		if (feof(stdin) != 0) {
			break;
		}
		n++;
		int height;
		int width;
		int k;
		int p;
		scanf("%d %d %d %d", &height, &width, &k, &p);
		Player player = Player(p);
		Board board = Board(width, height);
		board.read();
		NmkEngine engine = NmkEngine(board, k, player);
		if (strcmp(input, GENERATE_COMMAND) == 0) {
			engine.generate(false);
		} else if (strcmp(input, GENERATE_CUT_COMMAND) == 0) {
			engine.generate(true);
		} else if (strcmp(input, SOLVE_COMMAND) == 0) {
			engine.solve();
		} else {
			printf("Invalid command: %s\n", input);
		}
	}
}

#include "InputHandler.h"
#include <iostream>
#include "Board.h"
#include "NmkEngine.h"
#include <assert.h>
#include "Player.h"

#define MAX_INPUT_LENGTH 50
#define GENERATE_COMMAND "GEN_ALL_POS_MOV"
#define GENERATE_CUT_COMMAND "GEN_ALL_POS_MOV_CUT_IF_GAME_OVER"
#define SOLVE_COMMAND "SOLVE_GAME_STATE"

InputHandler::InputHandler() {
	//std::ios_base::sync_with_stdio(false);
	//std::cin.tie(NULL);
	//std::cout.tie(NULL);
}

void InputHandler::handle() {
	char input[MAX_INPUT_LENGTH];
	while (std::cin >> input) {
		int n;
		int m;
		int k;
		//Player player;
		scanf("%d %d %d", &n, &m, &k);
		Player player = Player::read();
		//std::cin >> n;
		//std::cin >> m;
		//std::cin >> k;
		//std::cin >> player;
		Board board = Board(n, m);
		std::cin >> board;
		NmkEngine engine = NmkEngine(board, k, player);
		if (strcmp(input, GENERATE_COMMAND) == 0) {
			engine.generate(false);
		} else if (strcmp(input, GENERATE_CUT_COMMAND) == 0) {
			engine.generate(true);
		} else if (strcmp(input, SOLVE_COMMAND) == 0) {
			engine.solve();
		} else {
			//std::cout << "Invalid command: " << input;
		}
	}
}

#pragma once

#include "Board.h"
#include "LinkedBoardList.h"
#include "Player.h"

#define MESSAGE_TIE "BOTH_PLAYERS_TIE\n"
#define MESSAGE_P1 "FIRST_PLAYER_WINS\n"
#define MESSAGE_P2 "SECOND_PLAYER_WINS\n"

#define UNKNOWN_MOVE -1
#define WIN 1
#define LOSS -1
#define TIE 0

class Board;
class LinkedBoardList;
class Player;

class NmkEngine {
public:
	NmkEngine(Board& board, int k, Player player);
	void generate(bool shouldCut);
	void solve();
private:
	Board& board;
	int minToWin;
	Player player;
	void printSolutions(LinkedBoardList& solutions, bool shouldCut);
	bool isWinning(const Board& currBoard, const Player& currPlayer);
	bool isWinning(const Board& currBoard, int x, int y);
	bool isWinning(const Board& currBoard, int startX, int startY, int dx, int dy);
	int howManyInDirection(const Board& currBoard, int startX, int startY, int dx, int dy);
	int minimax(Board& currBoard, Player currPlayer);
	int evaluate(Board& currBoard);
	LinkedBoardList* generatePossibleMoves(const Board& currBoard, const Player& currPlayer, int lastX = UNKNOWN_MOVE, int lastY = UNKNOWN_MOVE);
	bool isOver(const Board& currBoard, const Player& currPlayer, int lastX = UNKNOWN_MOVE, int lastY = UNKNOWN_MOVE);
	//bool isOver(Board& currBoard);
};

#pragma once

#include "Board.h"
#include "LinkedMoveList.h"
#include "Player.h"
#include "Utils.h"

#define MESSAGE_TIE "BOTH_PLAYERS_TIE\n"
#define MESSAGE_P1 "FIRST_PLAYER_WINS\n"
#define MESSAGE_P2 "SECOND_PLAYER_WINS\n"

#define P1_WIN 1
#define P1_LOSS -1
#define TIE 0

class Board;
class LinkedMoveList;
class Player;
struct Move;


#include <iostream> // TODO

class NmkEngine {
public:
	NmkEngine(Board& board, int k, Player player);
	void generate(bool shouldCut);
	void solve();
private:
	Board& board;
	int minToWin;
	Player player;
	void printSolutions(LinkedMoveList& solutions, bool shouldCut);
	bool isWinning(const Player& currPlayer);
	bool isWinning(int x, int y);
	bool isWinning(int startX, int startY, int dx, int dy);
	int howManyInDirection(int startX, int startY, int dx, int dy);
	int minimax(Move& currMove, Player currPlayer, LinkedMoveList& threats);
	void addThreats (Move& currMove, LinkedMoveList& threats);
	void addThreats(Move& currMove, LinkedMoveList& threats, int dx, int dy);
	int evaluate(Move& currMove, Player currPlayer, LinkedMoveList& threats);
	LinkedMoveList* generatePossibleMoves(const Player& currPlayer);
	bool isOver(const Player& currPlayer, int lastX = UNKNOWN_MOVE, int lastY = UNKNOWN_MOVE);
	bool hasWinningThreat(Move& currMove);
	bool isOpenEndedThreat(int x, int y, int dx, int dy);
};

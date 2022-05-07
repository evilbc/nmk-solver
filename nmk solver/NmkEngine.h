#pragma once

#include "Board.h"
#include "LinkedMoveList.h"
#include "Player.h"

#define MESSAGE_TIE "BOTH_PLAYERS_TIE\n"
#define MESSAGE_P1 "FIRST_PLAYER_WINS\n"
#define MESSAGE_P2 "SECOND_PLAYER_WINS\n"

#define UNKNOWN_MOVE -1
#define WIN 1
#define LOSS -1
#define TIE 0

class Board;
class LinkedMoveList;
class Player;

struct Move {
	explicit Move(Board* board, int lastMoveX = UNKNOWN_MOVE, int lastMoveY = UNKNOWN_MOVE) : board(board), lastMoveX(lastMoveX), lastMoveY(lastMoveY) {
	}
	~Move() {
		delete board;
	}
	Board* board;
	int lastMoveX;
	int lastMoveY;
	bool moveIsKnown() const {
		return lastMoveX != UNKNOWN_MOVE && lastMoveY != UNKNOWN_MOVE;
	}
};

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
	bool isWinning(const Board& currBoard, const Player& currPlayer);
	bool isWinning(const Board& currBoard, int x, int y);
	bool isWinning(const Board& currBoard, int startX, int startY, int dx, int dy);
	static int howManyInDirection(const Board& currBoard, int startX, int startY, int dx, int dy);
	int minimax(Move& currMove, Player currPlayer);
	int evaluate(Move& currMove);
	LinkedMoveList* generatePossibleMoves(const Board& currBoard, const Player& currPlayer);
	bool isOver(const Board& currBoard, const Player& currPlayer, int lastX = UNKNOWN_MOVE, int lastY = UNKNOWN_MOVE);
	bool hasWinningThreat(Move& currMove);
	bool isOpenEndedThreat(const Board& currBoard, int x, int y, int dx, int dy);
};

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

#include <fstream>

class NmkEngine {
public:
	NmkEngine(Board& board, int k, Player player);
	void generate(bool shouldCut);
	void solve(std::ostream& file);
private:
	Board& board;
	int minToWin;
	Player player;
	void printSolutions(LinkedMoveList& solutions, bool shouldCut);
	bool isWinning(const Player& currPlayer) const;
	bool isWinning(int x, int y) const;
	bool isWinning(int startX, int startY, int dx, int dy) const;
	int howManyInDirection(int startX, int startY, int dx, int dy) const;
	int minimax(Move& currMove, Player playerToMove, LinkedMoveList& threats);
	void addThreats (Move& currMove, LinkedMoveList& threats) const;
	void addThreats(Move& currMove, LinkedMoveList& threats, int dx, int dy) const;
	Move* howManyInDirectionWithSkip(Move& currMove, int dx, int dy, int& counter, int& skipCounter) const;
	int evaluate(Move& currMove, Player playerToMove, LinkedMoveList& threats) const;
	LinkedMoveList* generatePossibleMoves(const Player& currPlayer);
	bool isOver(const Player& currPlayer, int lastX = UNKNOWN_MOVE, int lastY = UNKNOWN_MOVE) const;
	void removeBlockedThreats(Move& currMove, LinkedMoveList& threats) const;
	static bool moveWasWinning(Move& currMove, LinkedMoveList& threats);
	static int resultByPlayer(Player pl);
	void fillThreatsAtStart(LinkedMoveList& threats) const;
};

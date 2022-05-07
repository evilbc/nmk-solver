#include "NmkEngine.h"
#include <algorithm>

NmkEngine::NmkEngine(Board& board, int k, Player player) : board(board), minToWin(k), player(player) {
}

LinkedMoveList* NmkEngine::generatePossibleMoves(const Board& currBoard, const Player& currPlayer) {
	LinkedMoveList* solutions = new LinkedMoveList();
	//if (isOver(currBoard, currPlayer, lastX, lastY)) {
	//	return solutions;
	//}
	for (int y = 0; y < currBoard.getHeight(); y++) {
		for (int x = 0; x < currBoard.getWidth(); x++) {
			if (currBoard.getPlayer(x, y) == Player::NONE) {
				Board* solution = new Board(currBoard);
				solution->setPlayer(x, y, currPlayer);
				solutions->push(new Move(solution, x, y));
			}
		}
	}
	return solutions;
}

bool NmkEngine::isOver(const Board& currBoard, const Player& currPlayer, int lastX, int lastY) {
	if (currBoard.isFull()) {
		return true;
	}
	if (lastX == UNKNOWN_MOVE || lastY == UNKNOWN_MOVE) {
		return isWinning(currBoard, currPlayer.getOpponent()) || isWinning(currBoard, currPlayer);
	} else {
		return isWinning(currBoard, lastX, lastY);
	}
}

void NmkEngine::generate(bool shouldCut) {
	if (isOver(board, player)) {
		printf("0\n");
		return;
	}
	LinkedMoveList* solutions = generatePossibleMoves(board, player);
	printSolutions(*solutions, shouldCut);
	delete solutions;
}

void NmkEngine::printSolutions(LinkedMoveList& solutions, bool shouldCut) {
	if (shouldCut) {
		for (LinkedMoveList::Iterator it = solutions.start(); it.hasNext(); it.next()) {
			if (isWinning(*(it.get().board), player)) {
				printf("1\n");
				it.get().board->write();
				return;
			}
		}
	}
	printf("%u\n", solutions.getSize());
	for (LinkedMoveList::Iterator it = solutions.start(); it.hasNext(); it.next()) {
		it.get().board->write();
		printf("\n");
	}
}

bool NmkEngine::isWinning(const Board& currBoard, const Player& currPlayer) {
	for (int y = 0; y < currBoard.getHeight(); y++) {
		for (int x = 0; x < currBoard.getWidth(); x++) {
			if (currPlayer != currBoard.getPlayer(x, y)) {
				continue;
			}
			if (isWinning(currBoard, x, y)) {
				return true;
			}
		}
	}
	return false;
}

bool NmkEngine::isWinning(const Board& currBoard, int x, int y) {
	return isWinning(currBoard, x, y, 1, 0)
		|| isWinning(currBoard, x, y, 0, 1)
		|| isWinning(currBoard, x, y, 1, 1)
		|| isWinning(currBoard, x, y, 1, -1);
}

bool NmkEngine::isWinning(const Board& currBoard, int startX, int startY, int dx, int dy) {
	int counter = 1 + howManyInDirection(currBoard, startX, startY, dx, dy) + howManyInDirection(currBoard, startX, startY, -dx, -dy);
	return counter >= minToWin;
}

int NmkEngine::howManyInDirection(const Board& currBoard, int startX, int startY, int dx, int dy) {
	Player currPlayer = currBoard.getPlayer(startX, startY);
	int counter = 0;
	int x = startX + dx;
	int y = startY + dy;
	while (currBoard.withinBounds(x, y)) {
		if (currBoard.getPlayer(x, y) != currPlayer) {
			break;
		}
		counter++;
		x += dx;
		y += dy;
	}
	return counter;
}

int NmkEngine::evaluate(Move& currMove) {
	Board& currBoard = *(currMove.board);
	if (currMove.moveIsKnown()) {
		//if (isWinning(currBoard, currMove.lastMoveX, currMove.lastMoveY) || hasWinningThreat(currMove)) {
		//	return currBoard.getPlayer(currMove.lastMoveX, currMove.lastMoveY) == Player::FIRST ? WIN : LOSS;
		//} else {
		//	return TIE;
		//}
		//if (isWinning(currBoard, currMove.lastMoveX, currMove.lastMoveY)) {
		//	return currBoard.getPlayer(currMove.lastMoveX, currMove.lastMoveY) == Player::FIRST ? WIN : LOSS;
		//} else {
		//	return TIE;
		//}
		if (hasWinningThreat(currMove)) {
			return currBoard.getPlayer(currMove.lastMoveX, currMove.lastMoveY) == Player::FIRST ? WIN : LOSS;
		} else {
			return TIE;
		}
	}
	if (isWinning(currBoard, Player::FIRST)) {
		return WIN;
	} else if (isWinning(currBoard, Player::SECOND)) {
		return LOSS;
	}
	return TIE;
}

int NmkEngine::minimax(Move& currMove, Player currPlayer) {
	Board& currBoard = *(currMove.board);
	int score = evaluate(currMove);
	if (score == WIN || score == LOSS || currBoard.isFull()) {
		return score;
	}
	int best;
	LinkedMoveList* possibleMoves = generatePossibleMoves(currBoard, currPlayer);
	if (currPlayer == Player::FIRST) {
		best = LOSS;
		for (LinkedMoveList::Iterator it = possibleMoves->start(); it.hasNext(); it.next()) {
			best = std::max(best, minimax(it.get(), currPlayer.getOpponent()));
			if (best == WIN) {
				break;
			}
		}
	} else {
		best = WIN;
		for (LinkedMoveList::Iterator it = possibleMoves->start(); it.hasNext(); it.next()) {
			best = std::min(best, minimax(it.get(), currPlayer.getOpponent()));
			if (best == LOSS) {
				break;
			}
		}
	}
	delete possibleMoves;
	return best;
}

void NmkEngine::solve() {
	Move move = Move(new Board(board));
	int res = minimax(move, player);
	if (res == TIE) {
		printf(MESSAGE_TIE);
		return;
	}
	printf(res == WIN ? MESSAGE_P1 : MESSAGE_P2);
}

bool NmkEngine::hasWinningThreat(Move& currMove) {
	//if (!currMove.moveIsKnown()) {
	//	return false;
	//}
	Board& currBoard = *(currMove.board);
	int x = currMove.lastMoveX;
	int y = currMove.lastMoveY;
	return isOpenEndedThreat(currBoard, x, y, 1, 0)
		|| isOpenEndedThreat(currBoard, x, y, 0, 1)
		|| isOpenEndedThreat(currBoard, x, y, 1, 1)
		|| isOpenEndedThreat(currBoard, x, y, 1, -1);
}

bool NmkEngine::isOpenEndedThreat(const Board& currBoard, int x, int y, int dx, int dy) {
	int howManyNormal = howManyInDirection(currBoard, x, y, dx, dy);
	int howManyReversed = howManyInDirection(currBoard, x, y, -dx, -dy);
	//return howManyNormal + howManyReversed == minToWin - 1
	//	&& currBoard.withinBounds(x + dx * (1 + howManyNormal), y + dy * (1 + howManyNormal))
	//	&& currBoard.withinBounds(x - dx * (1 + howManyReversed), y - dy * (1 + howManyReversed));
	if (howManyNormal + howManyReversed >= minToWin - 1) {
		return true;
	} else if (howManyNormal + howManyReversed == minToWin - 2) {
		int xNormal = x + (dx * ++howManyNormal);
		int yNormal = y + (dy * howManyNormal);
		int xRev = x - (dx * ++howManyReversed);
		int yRev = y - (dy * howManyReversed);
		return
			currBoard.withinBounds(xNormal, yNormal) && currBoard.getPlayer(xNormal, yNormal) == Player::NONE
			&& currBoard.withinBounds(xRev, yRev) && currBoard.getPlayer(xRev, yRev) == Player::NONE;
	}
	return false;
	//bool test = howManyNormal + howManyReversed == minToWin - 2
	//	&& currBoard.withinBounds(x + dx * (1 + howManyNormal), y + dy * (1 + howManyNormal))
	//	&& currBoard.withinBounds(x - dx * (1 + howManyReversed), y - dy * (1 + howManyReversed));
	//if (test) {
	//	int a = 0;
	//}
	//return test;


	//int xNormal = x + (dx * ++howManyNormal);
	//int yNormal = y + (dy * howManyNormal);
	//int xRev = x - (dx * ++howManyReversed);
	//int yRev = y - (dy * howManyReversed);
	//return howManyNormal + howManyReversed == minToWin - 2
	//	&& currBoard.withinBounds(xNormal, yNormal) && currBoard.getPlayer(xNormal, yNormal) == Player::NONE
	//	&& currBoard.withinBounds(xRev, yRev) && currBoard.getPlayer(xRev, yRev) == Player::NONE;
}

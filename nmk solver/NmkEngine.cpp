#include "NmkEngine.h"
#include <algorithm>

NmkEngine::NmkEngine(Board& board, int k, Player player) : board(board), minToWin(k), player(player) {
}

LinkedMoveList* NmkEngine::generatePossibleMoves(const Player& currPlayer) {
	LinkedMoveList* solutions = new LinkedMoveList();
	for (int y = 0; y < board.getHeight(); y++) {
		for (int x = 0; x < board.getWidth(); x++) {
			if (board.getPlayer(x, y) == Player::NONE) {
				solutions->push(new Move(currPlayer, x, y));
			}
		}
	}
	return solutions;
}

bool NmkEngine::isOver(const Player& currPlayer, int lastX, int lastY) {
	if (board.isFull()) {
		return true;
	}
	if (lastX == UNKNOWN_MOVE || lastY == UNKNOWN_MOVE) {
		return isWinning(currPlayer.getOpponent()) || isWinning(currPlayer);
	} else {
		return isWinning(lastX, lastY);
	}
}

void NmkEngine::generate(bool shouldCut) {
	if (isOver(player)) {
		printf("0\n");
		return;
	}
	LinkedMoveList* solutions = generatePossibleMoves(player);
	printSolutions(*solutions, shouldCut);
	delete solutions;
}

void NmkEngine::printSolutions(LinkedMoveList& solutions, bool shouldCut) {
	if (shouldCut) {
		for (LinkedMoveList::Iterator it = solutions.start(); it.hasNext(); it.next()) {
			board.setPlayer(it.get().x, it.get().y, player);
			if (isWinning(player)) {
				printf("1\n");
				board.write();
				return;
			}
			board.setPlayer(it.get().x, it.get().y, Player::NONE);
		}
	}
	printf("%lu\n", solutions.getSize());
	for (LinkedMoveList::Iterator it = solutions.start(); it.hasNext(); it.next()) {
		board.setPlayer(it.get().x, it.get().y, player);
		board.write();
		printf("\n");
		board.setPlayer(it.get().x, it.get().y, Player::NONE);
	}
}

bool NmkEngine::isWinning(const Player& currPlayer) {
	for (int y = 0; y < board.getHeight(); y++) {
		for (int x = 0; x < board.getWidth(); x++) {
			if (currPlayer != board.getPlayer(x, y)) {
				continue;
			}
			if (isWinning(x, y)) {
				return true;
			}
		}
	}
	return false;
}

bool NmkEngine::isWinning(int x, int y) {
	return isWinning(x, y, 1, 0)
		|| isWinning(x, y, 0, 1)
		|| isWinning(x, y, 1, 1)
		|| isWinning(x, y, 1, -1);
}

bool NmkEngine::isWinning(int startX, int startY, int dx, int dy) {
	int counter = 1 + howManyInDirection(startX, startY, dx, dy) + howManyInDirection(startX, startY, -dx, -dy);
	return counter >= minToWin;
}

int NmkEngine::howManyInDirection(int startX, int startY, int dx, int dy) {
	Player currPlayer = board.getPlayer(startX, startY);
	int counter = 0;
	int x = startX + dx;
	int y = startY + dy;
	while (board.withinBounds(x, y)) {
		if (board.getPlayer(x, y) != currPlayer) {
			break;
		}
		counter++;
		x += dx;
		y += dy;
	}
	return counter;
}

int NmkEngine::evaluate(Move& currMove, Player currPlayer, LinkedMoveList& threats) {
	int result = TIE;
	if (currMove.moveIsKnown()) {
		//if (isWinning(currBoard, currMove.lastMoveX, currMove.lastMoveY) || hasWinningThreat(currMove)) {
		//	return currBoard.getPlayer(currMove.lastMoveX, currMove.lastMoveY) == Player::FIRST ? P1_WIN : P1_LOSS;
		//} else {
		//	return TIE;
		//}
		//if (isWinning(currMove.x, currMove.y)) {
		//	result = currPlayer != Player::FIRST ? P1_WIN : P1_LOSS;
		//}
		//if (hasWinningThreat(currMove)) {
		//	return result = currPlayer != Player::FIRST ? P1_WIN : P1_LOSS;
		//}
		if (isWinning(currMove.x, currMove.y)) {
			result = currPlayer != Player::FIRST ? P1_WIN : P1_LOSS;
		} else {
			addThreats(currMove, threats);
			if (threats.sizeByPlayer(Player::FIRST) >= 2) {
				return P1_WIN;
			}
			if (threats.sizeByPlayer(Player::SECOND) >= 2) {
				return P1_LOSS;
			}
		}
	} else {
		if (isWinning(Player::FIRST)) {
			result = P1_WIN;
		} else if (isWinning(Player::SECOND)) {
			result = P1_LOSS;
		}
	}
	return result;
}

int NmkEngine::minimax(Move& currMove, Player currPlayer, LinkedMoveList& threats) {
	for (LinkedMoveList::Iterator it = threats.start(); it.hasNext(); it.next()) {
		if (it.get().x == currMove.x && it.get().y == currMove.y) {
			if (it.get().player == currMove.player) {
				return currMove.player == Player::FIRST ? P1_WIN : P1_LOSS;
			} else {
				it.remove();
				break;
			}
		}
	}
	int score = evaluate(currMove, currPlayer, threats);
	if (score == P1_WIN || score == P1_LOSS || board.isFull()) {
		return score;
	}
	int best;
	LinkedMoveList* possibleMoves = generatePossibleMoves(currPlayer);
	if (currPlayer == Player::FIRST) {
		best = P1_LOSS;
		for (LinkedMoveList::Iterator it = possibleMoves->start(); it.hasNext(); it.next()) {
			board.setPlayer(it.get().x, it.get().y, currPlayer);
			LinkedMoveList t = LinkedMoveList(threats);
			best = std::max(best, minimax(it.get(), currPlayer.getOpponent(), t));
			board.setPlayer(it.get().x, it.get().y, Player::NONE);
			if (best == P1_WIN) {
				break;
			}
		}
	} else {
		best = P1_WIN;
		for (LinkedMoveList::Iterator it = possibleMoves->start(); it.hasNext(); it.next()) {
			board.setPlayer(it.get().x, it.get().y, currPlayer);
			LinkedMoveList t = LinkedMoveList(threats);
			best = std::min(best, minimax(it.get(), currPlayer.getOpponent(), t));
			board.setPlayer(it.get().x, it.get().y, Player::NONE);
			if (best == P1_LOSS) {
				break;
			}
		}
	}
	delete possibleMoves;
	return best;
}

void NmkEngine::addThreats(Move& currMove, LinkedMoveList& threats) {
	addThreats(currMove, threats, 1, 0);
	addThreats(currMove, threats, 0, 1);
	addThreats(currMove, threats, 1, 1);
	addThreats(currMove, threats, 1, -1);
}


#include <iostream>
void NmkEngine::addThreats(Move& currMove, LinkedMoveList& threats, int dx, int dy) {
	int startX = currMove.x;
	int startY = currMove.y;
	//int howManyNormal = howManyInDirection(x, y, dx, dy);
	//int howManyReversed = howManyInDirection(x, y, -dx, -dy);
	Move* skip = nullptr;
	Player currPlayer = currMove.player;
	Player opponent = currPlayer.getOpponent();
	int counter = 0;
	bool skippedOne = false;
	int x = startX + dx;
	int y = startY + dy;
	int lX = -1;
	int rX = -1;
	int lY = -1;
	int rY = -1;
	while (board.withinBounds(x, y)) {
		Player pl = board.getPlayer(x, y);
		if (pl == opponent) {
			break;
		}
		if (pl == Player::NONE) {
			if (!skippedOne && board.withinBounds(x + dx, y + dy) && board.getPlayer(x + dx, y + dy) == currPlayer) {
				skippedOne = true;
				skip = new Move(currPlayer, x, y);
				x += dx;
				y += dy;
				continue;
			} else {
				break;
			}
		}
		counter++;
		x += dx;
		y += dy;
		lX = x;
		lY = y;
	}
	dx *= -1;
	dy *= -1;
	x = startX + dx;
	y = startY + dy;
	while (board.withinBounds(x, y)) {
		Player pl = board.getPlayer(x, y);
		if (pl == opponent) {
			break;
		}
		if (pl == Player::NONE) {
			if (!skippedOne && board.withinBounds(x + dx, y + dy) && board.getPlayer(x + dx, y + dy) == currPlayer) {
				skippedOne = true;
				skip = new Move(currPlayer, x, y);
				x += dx;
				y += dy;
				continue;
			} else {
				break;
			}
		}
		counter++;
		x += dx;
		y += dy;
		rX = x;
		rY = y;
	}
	if (counter + 2 >= minToWin) {
		if (skip != nullptr && !threats.contains(skip->player, skip->x, skip->y)) {
			//std::cout << board.asString() << '\n';
			threats.push(skip);
		}
		if (board.withinBounds(lX, lY) && board.getPlayer(lX, lY) == Player::NONE && !threats.contains(currPlayer, lX, lY)) {
			//std::cout << board.asString() << '\n';
			threats.push(new Move(currPlayer, lX, lY));
		}
		if (board.withinBounds(rX, rY) && board.getPlayer(rX, rY) == Player::NONE && !threats.contains(currPlayer, rX, rY)) {
			//std::cout << board.asString() << '\n';
			threats.push(new Move(currPlayer, rX, rY));
		}
	}
	//if (howManyNormal + howManyReversed != minToWin - 2) {
	//	return;
	//	int xNormal = x + (dx * ++howManyNormal);
	//	int yNormal = y + (dy * howManyNormal);
	//	int xRev = x - (dx * ++howManyReversed);
	//	int yRev = y - (dy * howManyReversed);
	//	return
	//		board.withinBounds(xNormal, yNormal) && board.getPlayer(xNormal, yNormal) == Player::NONE
	//		&& board.withinBounds(xRev, yRev) && board.getPlayer(xRev, yRev) == Player::NONE;
	//}
}

void NmkEngine::solve() {
	Move move = Move(player.getOpponent(), UNKNOWN_MOVE, UNKNOWN_MOVE);
	LinkedMoveList threats;
	int res = minimax(move, player, threats);
	if (res == TIE) {
		printf(MESSAGE_TIE);
		return;
	}
	printf(res == P1_WIN ? MESSAGE_P1 : MESSAGE_P2);
}

bool NmkEngine::hasWinningThreat(Move& currMove) {
	int x = currMove.x;
	int y = currMove.y;
	return isOpenEndedThreat(x, y, 1, 0)
		|| isOpenEndedThreat(x, y, 0, 1)
		|| isOpenEndedThreat(x, y, 1, 1)
		|| isOpenEndedThreat(x, y, 1, -1);
}

bool NmkEngine::isOpenEndedThreat(int x, int y, int dx, int dy) {
	int howManyNormal = howManyInDirection(x, y, dx, dy);
	int howManyReversed = howManyInDirection(x, y, -dx, -dy);
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
			board.withinBounds(xNormal, yNormal) && board.getPlayer(xNormal, yNormal) == Player::NONE
			&& board.withinBounds(xRev, yRev) && board.getPlayer(xRev, yRev) == Player::NONE;
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

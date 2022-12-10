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

bool NmkEngine::isOver(const Player& currPlayer, int lastX, int lastY) const {
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

bool NmkEngine::isWinning(const Player& currPlayer) const {
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

bool NmkEngine::isWinning(int x, int y) const {
	return isWinning(x, y, 1, 0)
		|| isWinning(x, y, 0, 1)
		|| isWinning(x, y, 1, 1)
		|| isWinning(x, y, 1, -1);
}

bool NmkEngine::isWinning(int startX, int startY, int dx, int dy) const {
	int counter = 1 + howManyInDirection(startX, startY, dx, dy) + howManyInDirection(startX, startY, -dx, -dy);
	return counter >= minToWin;
}

int NmkEngine::howManyInDirection(int startX, int startY, int dx, int dy) const {
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

int NmkEngine::resultByPlayer(Player pl) {
	return pl == Player::FIRST ? P1_WIN : P1_LOSS;
}

int NmkEngine::evaluate(Move& currMove, Player playerToMove, LinkedMoveList& threats) const {
	if (moveWasWinning(currMove, threats)) {
		return resultByPlayer(currMove.player);
	}
	if (threats.sizeByPlayer(playerToMove) >= 1) {
		return resultByPlayer(playerToMove);
	}
	addThreats(currMove, threats);
	if (threats.sizeByPlayer(Player::FIRST) >= 2) {
		return P1_WIN;
	}
	if (threats.sizeByPlayer(Player::SECOND) >= 2) {
		return P1_LOSS;
	}
	return TIE;
}

int NmkEngine::minimax(Move& currMove, Player playerToMove, LinkedMoveList& threats) {
	removeBlockedThreats(currMove, threats);
	int score = evaluate(currMove, playerToMove, threats);
	if (score == P1_WIN || score == P1_LOSS || board.isFull()) {
		return score;
	}
	int best;
	LinkedMoveList* possibleMoves = generatePossibleMoves(playerToMove);
	if (playerToMove == Player::FIRST) {
		best = P1_LOSS;
		for (LinkedMoveList::Iterator it = possibleMoves->start(); it.hasNext(); it.next()) {
			board.setPlayer(it.get().x, it.get().y, playerToMove);
			LinkedMoveList threatsCopy = LinkedMoveList(threats);
			best = std::max(best, minimax(it.get(), playerToMove.getOpponent(), threatsCopy));
			board.setPlayer(it.get().x, it.get().y, Player::NONE);
			if (best == P1_WIN) {
				break;
			}
		}
	} else {
		best = P1_WIN;
		for (LinkedMoveList::Iterator it = possibleMoves->start(); it.hasNext(); it.next()) {
			board.setPlayer(it.get().x, it.get().y, playerToMove);
			LinkedMoveList threatsCopy = LinkedMoveList(threats);
			best = std::min(best, minimax(it.get(), playerToMove.getOpponent(), threatsCopy));
			board.setPlayer(it.get().x, it.get().y, Player::NONE);
			if (best == P1_LOSS) {
				break;
			}
		}
	}
	delete possibleMoves;
	return best;
}

void NmkEngine::removeBlockedThreats(Move& currMove, LinkedMoveList& threats) const {
	for (LinkedMoveList::Iterator it = threats.start(); it.hasNext(); it.next()) {
		if (it.get().x == currMove.x && it.get().y == currMove.y && it.get().player != currMove.player) {
			it.remove();
		}
	}
}

bool NmkEngine::moveWasWinning(Move& currMove, LinkedMoveList& threats) {
	return threats.contains(currMove.player, currMove.x, currMove.y);
}

void NmkEngine::addThreats(Move& currMove, LinkedMoveList& threats) const {
	if (!currMove.moveIsKnown()) {
		return;
	}
	addThreats(currMove, threats, 1, 0);
	addThreats(currMove, threats, 0, 1);
	addThreats(currMove, threats, 1, 1);
	addThreats(currMove, threats, 1, -1);
}

void NmkEngine::addThreats(Move& currMove, LinkedMoveList& threats, int dx, int dy) const {
	int counterNormal = 0;
	int counterReversed = 0;
	int skipCounterNormal = 0;
	int skipCounterReversed = 0;
	Move* skipNormal = howManyInDirectionWithSkip(currMove, dx, dy, counterNormal, skipCounterNormal);
	Move* skipReversed = howManyInDirectionWithSkip(currMove, -dx, -dy, counterReversed, skipCounterReversed);
	int counter = counterNormal + counterReversed;
	if (skipNormal != nullptr) {
		if (counter + skipCounterNormal + 2 >= minToWin && !threats.contains(skipNormal->player, skipNormal->x, skipNormal->y)) {
			threats.push(skipNormal);
		} else {
			delete skipNormal;
		}
	}
	if (skipReversed != nullptr) {
		if (counter + skipCounterReversed + 2 >= minToWin && !threats.contains(skipReversed->player, skipReversed->x, skipReversed->y)) {
			threats.push(skipReversed);
		} else {
			delete skipReversed;
		}
	}
	if (counter + 2 >= minToWin) {
		int startX = currMove.x;
		int startY = currMove.y;
		int normalX = startX + dx * (counterNormal + 1);
		int normalY = startY + dy * (counterNormal + 1);
		int reversedX = startX - dx * (counterReversed + 1);
		int reversedY = startY - dy * (counterReversed + 1);
		Player currPlayer = currMove.player;
		if (board.withinBounds(normalX, normalY) && board.getPlayer(normalX, normalY) == Player::NONE && !threats.contains(currPlayer, normalX, normalY)) {
			threats.push(new Move(currPlayer, normalX, normalY));
		}
		if (board.withinBounds(reversedX, reversedY) && board.getPlayer(reversedX, reversedY) == Player::NONE && !threats.contains(currPlayer, reversedX, reversedY)) {
			threats.push(new Move(currPlayer, reversedX, reversedY));
		}
	}
}

Move* NmkEngine::howManyInDirectionWithSkip(Move& currMove, int dx, int dy, int& counter, int& skipCounter) const {
	Move* skip = nullptr;
	int x = currMove.x + dx;
	int y = currMove.y + dy;
	Player currPlayer = currMove.player;
	Player opponent = currPlayer.getOpponent();
	while (board.withinBounds(x, y)) {
		Player pl = board.getPlayer(x, y);
		if (pl == opponent) {
			break;
		}
		if (pl == Player::NONE) {
			if (!(board.withinBounds(x + dx, y + dy) && board.getPlayer(x + dx, y + dy) == currPlayer)) {
				break;
			}
			skip = new Move(currPlayer, x, y);
			x += dx;
			y += dy;
			while (board.withinBounds(x, y) && board.getPlayer(x, y) == currPlayer) {
				skipCounter++;
				x += dx;
				y += dy;
			}
			break;
		}
		counter++;
		x += dx;
		y += dy;
	}
	return skip;
}

void NmkEngine::solve(std::ostream& file) {
	//if (isWinning(Player::FIRST)) {
	// if (isWinning(player.getOpponent())) {
	//	//printf(MESSAGE_P1);
	//	//file << MESSAGE_P1;
	//	 if (player != Player::FIRST) {
	//		 file << MESSAGE_P1;
	//	 } else {
	//		 file << MESSAGE_P2;
	//	 }
	//	return;
	////} else if (isWinning(Player::SECOND)) {
	// } else if (isWinning(player)) {
	//	//printf(MESSAGE_P2);
	//	//file << MESSAGE_P2;
	//	 if (player == Player::FIRST) {
	//		 file << MESSAGE_P1;
	//	 } else {
	//		 file << MESSAGE_P2;
	//	 }
	//	return;
	//}
	for (int y = 0; y < board.getHeight(); y++) {
		for (int x = 0; x < board.getWidth(); x++) {
			if (Player::NONE == board.getPlayer(x, y)) {
				continue;
			}
			if (isWinning(x, y)) {
				if (board.getPlayer(x, y) == Player::FIRST) {
					file << MESSAGE_P1;
					return;
				} else {
					file << MESSAGE_P2;
					return;
				}
			}
		}
	}
	LinkedMoveList threatsAtStart;
	fillThreatsAtStart(threatsAtStart);
	Move move = Move(player.getOpponent(), UNKNOWN_MOVE, UNKNOWN_MOVE);
	int result = minimax(move, player, threatsAtStart);
	if (result == TIE) {
		//printf(MESSAGE_TIE);
		file << MESSAGE_TIE;
		return;
	}
	//printf(result == P1_WIN ? MESSAGE_P1 : MESSAGE_P2);
	if (result == P1_WIN) {
		file << MESSAGE_P1;
	} else {
		file << MESSAGE_P2;
	}
	//file << (result == P1_WIN) ? MESSAGE_P1 : MESSAGE_P2;
}

void NmkEngine::fillThreatsAtStart(LinkedMoveList& threats) const {
	for (int y = 0; y < board.getHeight(); y++) {
		for (int x = 0; x < board.getWidth(); x++) {
			if (board.getPlayer(x, y) != Player::NONE) {
				Move move(board.getPlayer(x, y), x, y);
				addThreats(move, threats);
			}
		}
	}
}

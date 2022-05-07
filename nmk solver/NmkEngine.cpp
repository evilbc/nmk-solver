#include "NmkEngine.h"
#include <algorithm>

NmkEngine::NmkEngine(Board& board, int k, Player player) : board(board), minToWin(k), player(player) {
}

LinkedBoardList* NmkEngine::generatePossibleMoves(const Board& currBoard, const Player& currPlayer, int lastX, int lastY) {
	LinkedBoardList* solutions = new LinkedBoardList();
	if (isOver(currBoard, currPlayer, lastX, lastY)) {
		return solutions;
	}
	for (int i = 0; i < board.getWidth(); i++) {
		for (int j = 0; j < board.getHeight(); j++) {
			if (board.getPlayer(i, j) == Player::NONE) {
				Board* solution = new Board(board);
				solution->setPlayer(i, j, currPlayer);
				solutions->push(solution);
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
	//LinkedBoardList solutions;
	//if (isWinning(board, player.getOpponent())) {
	//	printSolutions(solutions, shouldCut);
	//	return;
	//}
	//for (int i = 0; i < board.getWidth(); i++) {
	//	for (int j = 0; j < board.getHeight(); j++) {
	//		if (board.player(i, j) == Player::NONE) {
	//			Board* solution = new Board(board);
	//			solution->player(i, j) = player;
	//			solutions.push(solution);
	//		}
	//	}
	//}
	LinkedBoardList* solutions = generatePossibleMoves(board, player);
	printSolutions(*solutions, shouldCut);
	delete solutions;
}

void NmkEngine::printSolutions(LinkedBoardList& solutions, bool shouldCut) {
	if (shouldCut) {
		for (LinkedBoardList::Iterator it = solutions.start(); it.hasNext(); it.next()) {
			if (isWinning(it.get(), player)) {
				//std::cout << "1\n" << it.get();
				printf("1\n");
				it.get().write();
				return;
			}
		}
	}
	//std::cout << solutions.getSize() << "\n";
	printf("%d\n", solutions.getSize());
	//if (solutions.isEmpty()) {
	//	return;
	//}
	for (LinkedBoardList::Iterator it = solutions.start(); it.hasNext(); it.next()) {
		//std::cout << it.get() << "\n";
		it.get().write();
		printf("\n");
	}
}

//bool NmkEngine::isWinning(LinkedBoardList& boardList, int currPlayer) {
//	for (LinkedBoardList::Iterator it = boardList.start(); it.hasNext(); it.next()) {
//		if (isWinning(it.get(), currPlayer)) {
//			return true;
//		}
//	}
//	return false;
//}

bool NmkEngine::isWinning(const Board& currBoard, const Player& currPlayer) {
	for (int i = 0; i < currBoard.getWidth(); i++) {
		for (int j = 0; j < currBoard.getHeight(); j++) {
			if (currPlayer != currBoard.getPlayer(i, j)) {
				continue;
			}
			if (isWinning(currBoard, i, j)) {
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

int NmkEngine::evaluate(Board& currBoard) {
	if (isWinning(currBoard, player)) {
		return WIN;
	} else if (isWinning(currBoard, player.getOpponent())) {
		return LOSS;
	}
	return TIE;
}

//int NmkEngine::findBestMove(Board& currBoard) {
//	int bestMove = INT_MIN;
//	
//}

int NmkEngine::minimax(Board& currBoard, Player currPlayer) {
	int score = evaluate(currBoard);
	if (score == WIN || score == LOSS || currBoard.isFull()) {
		return score;
	}
	int best;
	LinkedBoardList* possibleMoves = generatePossibleMoves(currBoard, currPlayer);
	if (currPlayer == player) {
		best = LOSS;
		for (LinkedBoardList::Iterator it = possibleMoves->start(); it.hasNext(); it.next()) {
			best = std::max(best, minimax(it.get(), currPlayer.getOpponent()));
			if (best == WIN) {
				break;
			}
		}
	} else {
		best = WIN;
		for (LinkedBoardList::Iterator it = possibleMoves->start(); it.hasNext(); it.next()) {
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
	int res = minimax(board, player);
	if (res == TIE) {
		//std::cout << MESSAGE_TIE;
		printf(MESSAGE_TIE);
		return;
	}
	Player winner;
	if (res == WIN) {
		winner = player;
	} else {
		winner = player.getOpponent();
	}
	if (winner == Player::FIRST) {
		std::cout << MESSAGE_P1;
	} else {
		std::cout << MESSAGE_P2;
	}
}

//int Minmax(G gameState, Player activePlayer)
//int score = gameState.Evaluate(activePlayer)
//if (gameState.State == GameOver) {
//	return score
//}
//allPossibleMoves = gameState.GeneratePossibleMoves(activePlayer)
//if (activePlayer == Player::first)
//forall(possibleMove in allPossibleMoves)
//best = maximum(best, Minmax(possibleMove, activePlayer.GetOponent()))
//return best
//else
//forall(possibleMove in allPossibleMoves) {
//	best = minimum(best, Minmax(possibleMove, activePlayer.GetOponent()))
//		return best
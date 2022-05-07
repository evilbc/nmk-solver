#include "Board.h"
#include <assert.h>

Board::Board(int width, int height) : width(width), height(height), numOfEmptyFields(height* width) {
	board = new Player * [width];
	for (int i = 0; i < width; i++) {
		board[i] = new Player[height];
		//for (int j = 0; j < height; j++) {
		//	board[i][j] = Player::NONE;
		//}
	}
}

Board::Board(const Board& other) : width(other.width), height(other.height), numOfEmptyFields(other.numOfEmptyFields) {
	board = new Player * [width];
	for (int i = 0; i < width; i++) {
		board[i] = new Player[height];
		for (int j = 0; j < height; j++) {
			board[i][j] = other.board[i][j];
		}
	}
}

Board::~Board() {
	for (int i = 0; i < width; i++) {
		delete[] board[i];
	}
	delete[] board;
}

bool Board::withinBounds(int x, int y) const {
	return x >= 0 && x < width&& y >= 0 && y < height;
}

//Player& Board::player(int x, int y) {
//	assert(withinBounds(x, y));
//	return board[x][y];
//}

const Player& Board::getPlayer(int x, int y) const {
	assert(withinBounds(x, y));
	return board[x][y];
}

void Board::setPlayer(int x, int y, Player player) {
	assert(withinBounds(x, y));
	Player& p = board[x][y];
	if (p == Player::NONE && player != Player::NONE) {
		numOfEmptyFields++;
	} else if (p != Player::NONE && player == Player::NONE) {
		numOfEmptyFields--;
	}
	board[x][y] = player;
}

int Board::getWidth() const {
	return width;
}

int Board::getHeight() const {
	return height;
}

bool Board::isFull() const {
	//for (int i = 0; i < width; i++) {
	//	for (int j = 0; j < height; j++) {
	//		if (board[i][j] == Player::NONE) {
	//			return false;
	//		}
	//	}
	//}
	//return true;
	return numOfEmptyFields <= 0;
}

std::ostream& operator<< (std::ostream& os, const Board& board) {
	for (int i = 0; i < board.width; i++) {
		for (int j = 0; j < board.height; j++) {
			os << board.board[i][j] << " ";
		}
		os << "\n";
	}
	return os;
}

std::istream& operator>> (std::istream& is, Board& board) {
	board.numOfEmptyFields = 0;
	for (int i = 0; i < board.width; i++) {
		for (int j = 0; j < board.height; j++) {
			is >> board.board[i][j];
			if (board.board[i][j] == Player::NONE) {
				board.numOfEmptyFields++;
			}
		}
	}
	return is;
}

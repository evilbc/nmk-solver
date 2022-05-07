#pragma once

#include <iostream>
#include "Player.h"

class Player;

class Board {
public:
	Board(int width, int height);
	Board(const Board& other);
	~Board();
	//Player& player(int x, int y);
	const Player& getPlayer(int x, int y) const;
	void setPlayer(int x, int y, Player player);
	int getWidth() const;
	int getHeight() const;
	bool withinBounds(int x, int y) const;
	bool isFull() const;

	friend std::ostream& operator<< (std::ostream& os, const Board& board);
	friend std::istream& operator>> (std::istream& is, Board& board);
	//friend std::istream;

	void read();
	void write();
private:
	Player** board;
	int width;
	int height;
	int numOfEmptyFields;
};

#pragma once

#include <iostream>
#include "Player.h"

class Player;

class Board {
public:
	Board(int width, int height);
	Board(const Board& other);
	~Board();
	const Player& getPlayer(int x, int y) const;
	void setPlayer(int x, int y, Player player);
	int getWidth() const;
	int getHeight() const;
	bool withinBounds(int x, int y) const;
	bool isFull() const;

	void read();
	void write();
private:
	Player** board;
	int width;
	int height;
	int numOfEmptyFields;
};

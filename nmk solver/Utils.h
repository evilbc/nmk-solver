#pragma once

#include "Player.h"

#define UNKNOWN_MOVE -1

class Player;

struct Move {
	explicit Move(Player player, int x, int y) : player(player), x(x), y(y) {
	}
	Player player;
	int x;
	int y;
	bool moveIsKnown() const {
		return x != UNKNOWN_MOVE && y != UNKNOWN_MOVE;
	}
};
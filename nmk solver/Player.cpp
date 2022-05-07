#include "Player.h"
#include <assert.h>

Player::Player() : value(Value::NONE) {
}

Player::Player(Value player) : value(player) {
}

Player::Player(int val) : value(getVal(val)) {
}

Player Player::getOpponent() const {
	assert(value != Player::NONE);
	return value == Player::FIRST ? Player::SECOND : Player::FIRST;
}

std::istream& operator>>(std::istream& is, Player& player) {
	int val;
	is >> val;
	player.value = player.getVal(val);
	return is;
}

Player::Value Player::getVal(int val) {
	assert(val == 1 || val == 2 || val == 0);
	if (val == 0) {
		return Player::NONE;
	}
	return val == 1 ? Player::FIRST : Player::SECOND;
}

int Player::valueAsInt() const {
	switch (value) {
	case Player::NONE:
		return 0;
	case Player::FIRST:
		return 1;
	case Player::SECOND:
		return 2;
	}
	return 0;
}

//std::ostream& operator<<(std::ostream& os, Player player) {
//	os << player.valueAsInt();
//	return os;
//}

std::ostream& operator<<(std::ostream& os, const Player& player) {
	os << player.valueAsInt();
	return os;
}

//bool operator==(Player left, Player::Value right) {
//	return left.value == right;
//}
//
//bool operator==(Player left, Player right) {
//	return left.value == right.value;
//}
//
//bool operator!=(Player left, Player right) {
//	return left.value != right.value;
//}
//
//bool operator!=(Player left, Player::Value right) {
//	return left.value != right;
//}

bool operator==(const Player& left, const Player::Value& right) {
	return left.value == right;
}

bool operator==(const Player& left, const Player& right) {
	return left.value == right.value;
}

bool operator!=(const Player& left, const Player& right) {
	return left.value != right.value;
}

bool operator!=(const Player& left, const Player::Value& right) {
	return left.value != right;
}

Player Player::read() {
	Player res;
	int v;
	scanf("%d", &v);
	res.value = getVal(v);
	return res;
}

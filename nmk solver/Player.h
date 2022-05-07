#pragma once

#include <iostream>

class Player {
public:
	enum Value {
		FIRST, SECOND, NONE
	};
	Player();
	Player(Value player);
	Player(int val);
	Player getOpponent() const;

	friend std::istream& operator>>(std::istream& is, Player& player);
	//friend std::ostream& operator<<(std::ostream& os, Player player);
	friend std::ostream& operator<<(std::ostream& os, const Player& player);
	//friend bool operator==(Player left, Player::Value right);
	//friend bool operator==(Player left, Player right);
	//friend bool operator!=(Player left, Player right);
	//friend bool operator!=(Player left, Player::Value right);
	friend bool operator==(const Player& left, const Player::Value& right);
	friend bool operator==(const Player& left, const Player& right);
	friend bool operator!=(const Player& left, const Player& right);
	friend bool operator!=(const Player& left, const Player::Value& right);

	int valueAsInt() const;
	static Player read();
private:
	Value value;
	static Value getVal(int val);
};


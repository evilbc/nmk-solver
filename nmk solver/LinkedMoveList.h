#pragma once

#include "Board.h"
#include "NmkEngine.h"
#include <cstddef>
#include "Player.h"

class Board;
struct Move;
class Player;

class LinkedMoveList {
	struct Node {
		explicit Node(Move* move);
		~Node();
		Node* next;
		Node* previous;
		Move* move;
	};
public:
	struct Iterator {
	public:
		explicit Iterator(Node* current, LinkedMoveList& list);
		Move& get();
		void next();
		bool hasNext() const;
		void remove();

		friend class LinkedMoveList;
	private:
		Node* current;
		LinkedMoveList& list;
		bool removed;
	};
	LinkedMoveList();
	LinkedMoveList(const LinkedMoveList& other);
	~LinkedMoveList();
	std::size_t getSize() const;
	void push(Move* move);
	bool isEmpty() const;
	Iterator start();
	bool contains(Player player, int x, int y) const;
	size_t sizeByPlayer(Player player) const;
private:
	Node* head;
	Node* tail;
	std::size_t size;
	void remove(Iterator& it);
};


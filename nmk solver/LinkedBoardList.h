#pragma once

#include "Board.h"

class Board;

class LinkedBoardList {
		struct Node {
			explicit Node(Board* board);
			~Node();
			Node* next;
			Node* previous;
			Board* board;
		};
	public:
		struct Iterator {
		public:
			Iterator(Node* current, LinkedBoardList& list);
			Board& get();
			void next();
			bool hasNext() const;
			void remove();

			friend class LinkedBoardList;
		private:
			Node* current;
			LinkedBoardList& list;
			bool removed;
		};
		LinkedBoardList();
		~LinkedBoardList();
		std::size_t getSize() const;
		void push(Board* player);
		bool isEmpty() const;
		Iterator start();
	private:
		Node* head;
		Node* tail;
		std::size_t size;
		void remove(Iterator& it);
};


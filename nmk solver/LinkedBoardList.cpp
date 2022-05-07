#include "LinkedBoardList.h"
#include <assert.h>

LinkedBoardList::LinkedBoardList() : head(nullptr), tail(nullptr), size(0) {
}

LinkedBoardList::~LinkedBoardList() {
	Node* current = head;
	while (current != nullptr) {
		Node* prev = current;
		current = current->next;
		delete prev;
	}
}

size_t LinkedBoardList::getSize() const {
	return size;
}

void LinkedBoardList::push(Board* player) {
	Node* last = tail;
	tail = new Node(player);
	if (head == nullptr) {
		head = tail;
	} else {
		last->next = tail;
		tail->previous = last;
	}
	size++;
}

bool LinkedBoardList::isEmpty() const {
	return size <= 0;
}

LinkedBoardList::Iterator LinkedBoardList::start() {
	return Iterator(head, *this);
}

void LinkedBoardList::remove(Iterator& it) {
	Node* prev = it.current->previous;
	Node* next = it.current->next;
	if (prev != nullptr) {
		prev->next = next;
	} else {
		head = head->next;
		if (head != nullptr) {
			head->previous = nullptr;
		}
	}
	if (next != nullptr) {
		next->previous = prev;
	} else {
		tail = tail->previous;
		if (tail != nullptr) {
			tail->next = nullptr;
		}
	}
	delete it.current;
	it.current = next;

	size--;
}

LinkedBoardList::Node::Node(Board* board) : next(nullptr), previous(nullptr), board(board) {
}

LinkedBoardList::Node::~Node() {
	delete board;
}

LinkedBoardList::Iterator::Iterator(Node* current, LinkedBoardList& list) : current(current), list(list), removed(false) {
}

Board& LinkedBoardList::Iterator::get() {
	return *(current->board);
}

void LinkedBoardList::Iterator::next() {
	current = removed ? current : current->next;
	removed = false;
}

bool LinkedBoardList::Iterator::hasNext() const {
	return current != nullptr;
}

void LinkedBoardList::Iterator::remove() {
	list.remove(*this);
	removed = true;
}

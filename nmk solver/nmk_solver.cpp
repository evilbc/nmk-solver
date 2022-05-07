#include "InputHandler.h"
#include <chrono>
#include <iostream>

int main() {
	auto start = std::chrono::system_clock::now();
	InputHandler handler;
	handler.handle();
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> timePassed = end - start;
	std::cout << "Time passed: " << timePassed.count() << "\n";
	return 0;
}
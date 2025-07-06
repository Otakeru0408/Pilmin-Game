#pragma once
#include <random>

class RandomValue {
private:

	std::random_device rd;
public:

	int GetRandomVal() {
		std::mt19937_64 mt(rd());
		std::uniform_int_distribution<> rand100(0, 100);
		return rand100(mt);
	}
	int GetRandomVal(int min, int max) {
		std::mt19937_64 mt(rd());
		std::uniform_int_distribution<> rand100(min,max);
		return rand100(mt);
	}
};
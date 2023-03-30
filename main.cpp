#include "main.h"


int main() {

	const auto processor_count = std::thread::hardware_concurrency();

	for (auto i = 0; i < processor_count; i++) {

		Competitor newComp;
		competitors.push_back(newComp);

		threads.push_back(std::thread(&Competitor::update, competitors[i]));

	}



	for (auto& thread : threads) {
		thread.join();
	}

	std::cout << counter;

}
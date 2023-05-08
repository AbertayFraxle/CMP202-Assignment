#include "main.h"


int main() {
	competitorCount = 10000;

	srand(time(NULL));
	//get the amount of physical threads on the machine
	const auto processor_count = std::thread::hardware_concurrency();
	std::barrier turnTick(processor_count);
	std::cout <<"  _____ _                    ___      __        _ \n |_   _| |_  _ _ ___ __ _ __| \\ \\    / /_ _ _ _| |\n   | | | ' \\| '_/ -_) _` / _` |\\ \\/\\/ / _` | '_|_|\n   |_| |_||_|_| \\___\\__,_\\__,_| \\_/\\_/\\__,_|_| (_)\n" << std::endl;

	compIndex = 0;
	for (auto i = 0; i < processor_count; i++) {
		threads.push_back(std::thread([&]() {
			srand(time(NULL)+i);
			while (true) {
				std::unique_lock<std::mutex> lock(mutex);
				
				if (competitors.size() >= competitorCount) { return; };
				Competitor* newComp = new Competitor(&competitors, compIndex, &competitorCount);
				newComp->setSync(&mutex);
				newComp->setBar(&turnTick);
				competitors.push_back(newComp);
				turnQueue.push(newComp);
				++compIndex;
			}

			}));
	}

	for (auto& thread : threads) {
		thread.join();
	}


	for (auto i = 0; i < processor_count; i++) {
		threads[i] = std::thread([&]() {
		while (true) {
			std::unique_lock<std::mutex> lock(mutex);

			if (turnQueue.size() == 1) { return; };
			Competitor* current = turnQueue.front();
			turnQueue.pop();

			current->update();
			if (current->getHealth() > 0) {
				turnQueue.push(current);
			}

		}
		});
	}

	

	for (auto& thread : threads) {
		thread.join();
	}

	turnQueue.front()->hasWon();

}


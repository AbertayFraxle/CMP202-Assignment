#include "main.h"


int main() {
	competitorCount = 10000;

	ready = false;
	int roundCount = 0;
	srand(time(NULL));
	//get the amount of physical threads on the machine
	const auto processor_count = std::thread::hardware_concurrency();

	std::cout <<"  _____ _                    ___      __        _ \n |_   _| |_  _ _ ___ __ _ __| \\ \\    / /_ _ _ _| |\n   | | | ' \\| '_/ -_) _` / _` |\\ \\/\\/ / _` | '_|_|\n   |_| |_||_|_| \\___\\__,_\\__,_| \\_/\\_/\\__,_|_| (_)\n" << std::endl;

	compIndex = 0;
	for (auto i = 0; i < processor_count; i++) {
		threads.push_back(std::thread([&]() {
			srand(time(NULL)+i);
			while (true) {
				std::unique_lock<std::mutex> lock(mutex);
				
				if (competitors.size() >= competitorCount) { return; };
				Competitor* newComp = new Competitor(&competitors, compIndex, &competitorCount);
				competitors.push_back(newComp);
				nTurnQueue.push(newComp);
				++compIndex;
			}

			}));
	}

	for (auto& thread : threads) {
		thread.join();
	}

	threads[0] = std::thread([&roundCount]() {

		while (true) {
			std::unique_lock<std::mutex> lock(updatex);
			while (!ready) {
				update.wait(lock);
			}
			if (nTurnQueue.size() == 1) { return; };
			ready = false;
			std::swap(turnQueue, nTurnQueue);
			roundCount++;
			update.notify_one();
		}

		});

	for (auto i = 1; i < processor_count; i++) {
		threads[i] = std::thread([&]() {
			while (true) {
				std::unique_lock<std::mutex> nlock(mutex);
				
				if (turnQueue.size() == 0) {
					
					
					std::unique_lock<std::mutex> lock(updatex);
					ready = true;
					
					update.notify_one();
					if (nTurnQueue.size() == 1) { return; };


					update.wait(lock);

				}
				else {

					Competitor* current = turnQueue.front();
					turnQueue.pop();

					current->update();
					if (current->getHealth() > 0) {
						nTurnQueue.push(current);
					}
				};
			}
			});
	}


	

	for (auto& thread : threads) {
		thread.join();
	}

	
	


	nTurnQueue.front()->hasWon();

	std::cout << "\n this battle took " << roundCount << " rounds of combat";

}


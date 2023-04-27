#include "main.h"


int main() {
	srand(time(NULL));
	//get the amount of physical threads on the machine
	const auto processor_count = std::thread::hardware_concurrency();
	std::barrier turnTick(processor_count);
	std::cout <<"  _____ _                    ___      __        _ \n |_   _| |_  _ _ ___ __ _ __| \\ \\    / /_ _ _ _| |\n   | | | ' \\| '_/ -_) _` / _` |\\ \\/\\/ / _` | '_|_|\n   |_| |_||_|_| \\___\\__,_\\__,_| \\_/\\_/\\__,_|_| (_)\n" << std::endl;

	for (auto i = 0; i < processor_count; i++) {

		Competitor newComp;
		newComp.setSync(&mutex);
		newComp.setBar(&turnTick);
		competitors.push_back(newComp);

		threads.push_back(std::thread(&Competitor::update, competitors[i]));

	}



	for (auto& thread : threads) {
		thread.join();
	}


}
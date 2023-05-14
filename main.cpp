#include "main.h"



int main() {

	arriveCount = 0;
	ready = false;
	won = false;
	canCV = true;
	

	//get the amount of physical threads on the machine
	const auto max_threads = std::thread::hardware_concurrency();

	//ascii art goes brr
	std::cout << "  _____ _                    ___      __        _ \n |_   _| |_  _ _ ___ __ _ __| \\ \\    / /_ _ _ _| |\n   | | | ' \\| '_/ -_) _` / _` |\\ \\/\\/ / _` | '_|_|\n   |_| |_||_|_| \\___\\__,_\\__,_| \\_/\\_/\\__,_|_| (_)\n" << std::endl;


	//get the amount of competitors to fight and threads to use from user, also toggle death log

	competitorCount = 1;
	while (competitorCount <2) {
		std::cout << "How many competitors should fight? (more than 1): ";
		std::cin >> competitorCount;

		if (competitorCount < 2) {
			std::cout << "Please enter a valid number!\n";
		}
	}
	int processor_count = -1;

	while (processor_count < 3 || processor_count > max_threads) {
		std::cout << "How many threads would you like to use? (3-" << max_threads << "): ";
		std::cin >> processor_count;

		if (processor_count < 3 || processor_count > max_threads) {
			std::cout << "Please enter a valid number!\n";
		}
	}
	 bar = new std::barrier<>(processor_count-1);

	 int input = 0;
	 while (input < 1 || input >2) {
		 std::cout << "Enable death log? (hinders performance)(1 = yes, 2 = no): ";
		 std::cin >> input;
		 if (input < 1 || input >2) {
			 std::cout << "Please enter a valid number!\n";
		 }
	 }

	 if (input == 1) {
		 killLog = true;
	 }
	 else {
		 killLog = false;
	 }

	int count = processor_count - 1;

	nTurnQueue.resize(processor_count);

	//create a task farm to efficiently add competitors to the Competitors vector (this sucks)
	compIndex = 0;
	for (auto i = 0; i < processor_count; i++) {
		threads.push_back(std::thread([&]() {
			srand(time(NULL) + i);
		while (true) {
			mutex.lock();
			if (competitors.size() >= competitorCount) { mutex.unlock();  return; };
			Competitor* newComp = new Competitor(&competitors, compIndex, &competitorCount);
			competitors.push_back(newComp);
			++compIndex;
			mutex.unlock();
		}

			}));
	}

	//join the threads
	for (auto& thread : threads) {
		thread.join();
	}

	//initialise how many queue positions each thread should manage to begin with
	int step = competitorCount / (processor_count-1);

	int remainder = competitorCount-(step * (processor_count-1));

	//assign each competitor to a queue turn position to be used by each different thread, means next to no common resources, no mutex slowdown
	for (int i = 1; i < processor_count; i++) {
		for (int j = 0; j < step; j++) {
			nTurnQueue[i].push(competitors[(step * (i - 1)) + j]);
		}
		if ((i == (processor_count - 1) ) && remainder !=0) {
			for (int j = 0; j < remainder; j++) {
				nTurnQueue[i].push(competitors[(step*i)+j]);
			}
		}
	}

	//this thread times each round of combat
	threads[0] = std::thread([&]() {
		bool first = true;
	the_clock::time_point start, end;

		while (true) {

			//wait for this to be ready
			std::unique_lock<std::mutex> lock(updatex);
			while (!ready) {
				update.wait(lock);
			}



			//for the first time, start the clock, every other time, get a timing and start again
			if (first) {
				 start = the_clock::now();
				 first = false;
			}
			else {

				end = the_clock::now();

				auto time_taken = duration_cast<microseconds>(end - start).count();
				times.push_back(time_taken);
				start = the_clock::now();

			}
			
			

			//check to see if the win condition has been met, exit if so
			if (won) {
				update.notify_one(); return;
			};
			

			//notify waiting threads and set to unready again
			update.notify_one();
			ready = false;
		}
	
		});
	

	//std::swap(turnQueue, nTurnQueue);

	//run every player that is alive's turns
	for (auto i = 1; i < processor_count; i++) {
		threads[i] = std::thread(runTurn, std::ref(nTurnQueue[i]),std::ref(count),bar);
	}

	for (auto& thread : threads) {
		thread.join();
	}
	
	
	
	for (int i = 1; i < processor_count; i++) {
		if (!nTurnQueue[i].empty()) {
			nTurnQueue[i].front()->hasWon();
		}
	}

	//display all round times and average time, also output to file for graph making
	std::ofstream outFile;
	float avg = 0;
	for (int i = 0; i < times.size(); i++) {
		std::cout << "\n round " <<i+1 << " took " << times[i]/1000 << " ms\n";
		outFile.open("output.csv", std::ofstream::app);
		outFile << std::to_string(i + 1) + "," + std::to_string(times[i]/1000) << "\n";
		outFile.close();
		avg += times[i];
	}
	avg = avg / times.size();
	std::cout << "\n The average round took " << avg << "ms\n";

}


//run the turns
void runTurn(std::queue<Competitor*>& turns, int& counter, std::barrier<>* barrier) {

	//read in pre-treated queue of competitors
	std::queue<Competitor*> tQueue, ntQueue;
	std::swap(ntQueue, turns);
	
	//loop until breaking
	while (true) {
		
		//run competitor's turn
		if (tQueue.size() != 0) {

			Competitor* current = tQueue.front();

			tQueue.pop();

			current->update();
			if (current->getHealth() > 0) {

				ntQueue.push(current);

			}
			else {
				if (killLog) { std::cout << current->getEulogy(); };
			}
		}

		//if the queue is empty
		if (tQueue.size() == 0) {
			
				
			
			//if no previous competitor lived, exit this thread
			if (ntQueue.size() == 0) {
				mutex.lock();
				std::swap(ntQueue, turns);
				counter--;
				barrier->arrive_and_drop();
				mutex.unlock();
				return;
			}
			else if (ntQueue.size() == 1 && counter == 1) {
				//this is the win condition
				mutex.lock();
				won = true;
				std::swap(ntQueue, turns);
				ready = true;
				update.notify_one();
				barrier->arrive_and_drop();
				mutex.unlock();
				return;
			}
			

			//notify the stopwatch thread to update
			mutex.lock();
			if (canCV) {
				canCV = false;
				std::unique_lock<std::mutex> lock(updatex);
				ready = true;
				update.notify_one();
				update.wait(lock);
			}
			mutex.unlock();
			barrier->arrive_and_wait();
			mutex.lock();
			if (!canCV) {
				canCV = true;
			}
			swap(tQueue, ntQueue);
			mutex.unlock();
			barrier->arrive_and_wait();
			//this also synchronises all the taskfarm threads, keeps rounds consistent.
			
			
		}

	}
}
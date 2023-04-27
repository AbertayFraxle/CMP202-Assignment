#pragma once
#include <string>
#include <vector>
#include <random>
#include <iostream>
#include <mutex>
using std::string;

class Competitor
{
public:
	Competitor();
	void update();
	void generateName();
	void setSync(std::mutex*);
	void attack();
	int getHealth();

private:
	int health, strength, armor;
	string firstName, lastName;
	std::mutex* syncMutex;

};


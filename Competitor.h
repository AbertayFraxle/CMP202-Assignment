#pragma once
#include <string>
#include <vector>
#include <random>
#include <iostream>
#include <mutex>
#include <barrier>
#include "Weapon.h"

using std::string;

enum state {
	deciding,
	looting,
	attacking,
	defending,
	moving
};

class Competitor
{
public:
	Competitor();
	void update();
	void generateName();
	void setSync(std::mutex*);
	void setBar(std::barrier<std::_No_completion_function>*);
	void attack();
	int getHealth();
	int getState();

private:
	int health, strength, armor;
	string firstName, lastName;
	std::mutex* syncMutex;
	Weapon* wielding;
	state cState;
	std::barrier<std::_No_completion_function>* bar;
	
};


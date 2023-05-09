#pragma once
#include <string>
#include <vector>
#include <random>
#include <iostream>
#include <mutex>
#include <barrier>
#include "Weapon.h"
#include <queue>

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
	Competitor(std::vector<Competitor*>* nComp, int, int*);
	void update();
	void generateName();
	void attack();
	int getHealth();
	int getState();
	int getArmor();
	void hasWon();
	void reduceHealth(int, string, string);
	void findTarget();


private:
	int health, strength, armor, index,targetI;
	string firstName, lastName;
	std::mutex* healthMutex;
	Weapon* wielding;
	state cState;
	std::vector<Competitor*>* competitors;
	
	bool dead;

	int* competitorCount;
	
};


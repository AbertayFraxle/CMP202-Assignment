#pragma once
#include <string>
#include <vector>
#include <random>
#include <iostream>
#include <mutex>
#include <barrier>
#include "Weapon.h"
#include "Armor.h"
#include <string>

using std::string;

enum state {
	deciding,
	looting,
	attacking,
	defending,
	moving,
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
	void reduceHealth(int, string, string, int);
	void findTarget();
	bool isDefending();
	std::string getEulogy();



private:
	int maxHealth,health, strength, AC, index,targetI;
	string firstName, lastName;
	std::mutex* healthMutex;
	Weapon* wielding;
	Armor* wearing;
	state cState;
	std::vector<Competitor*>* competitors;
	std::string eulogy;
	
	bool blocking;
	int turnsTaken;

	int* competitorCount;
	
};


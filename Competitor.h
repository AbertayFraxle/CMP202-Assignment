#pragma once
#include <string>
#include <vector>
#include <random>
#include <iostream>
using std::string;

class Competitor
{
public:
	Competitor();
	void update();
	void generateName();

private:
	int health, strength, armor;
	string firstName, lastName;

};


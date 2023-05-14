#pragma once
#include <random>
enum ArmorType {
	Ring,
	Chain,
	Splint
};

class Armor
{
public:
	Armor();
	int getAC();
private:
	int AC;
	int type;
};


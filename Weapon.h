#pragma once
#include <random>
enum weaponType {
	Scimitar,
	Glaive,
	Halberd,
	Greataxe,
	Greatsword,
	Whip
};

class Weapon
{
public:
	Weapon();
	int RollDamage();
private:
	int type;
};


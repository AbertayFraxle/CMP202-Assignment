#include "Weapon.h"

Weapon::Weapon() { 
	type = rand() % 6 + 1;
}

int Weapon::RollDamage() {
	switch (type) {
	case Scimitar:
		//d6 damage
		return rand() % 6 + 1;
		break;
	case Glaive:
		//d10 damage
		return rand() % 10 + 1;
		break;
	case Greataxe:
		//d12 damage
		return rand() % 12 + 1;
		break;
	case Greatsword:
		//2d6 damage
		return (rand() % 6 + 1)+ (rand() % 6 + 1);
		break;
	case Whip:
		//d4 damage
		return (rand() % 4 + 1);
		break;
	}
}
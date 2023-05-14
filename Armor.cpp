#include "Armor.h"
Armor::Armor() {
	type = rand() % 4 + 1;
	AC = 0;

	switch (type) {
	case Ring:
		//Armor class is 14
		AC = 14;
		break;
	case Chain:
		//Armor class is 16
		AC = 16;
		break;
	case Splint:
		//Armor class is 17
		AC = 17;
		break;
	}
}

int Armor::getAC() {
	return AC;
}
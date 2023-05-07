#include "Competitor.h"

Competitor::Competitor(std::vector<Competitor>* nComp, int idx, int* compCount) {
	generateName();
	wielding = NULL;
	//generate health
	health = 12;
	for (int i = 0; i < 5; i++) {
		int nAddHealth = (rand() % 8 + 1);
		health += nAddHealth + 2;
	}

	competitors = nComp;
	index = idx;
	cState = deciding;

	healthMutex = new std::mutex;

	armor = 10;

	competitorCount = compCount;
}

void Competitor::generateName() {
	std::vector<string> firstNames{"Legira", "Yei","K'lani", "Gagea", "Jal", "Jah", "Mavena", "Latem", "Kehsi", "Runo", "Jangar", "Leth", "Banut", "Eoman", "Maxith", "Waul", "Drakuseth"};
	std::vector<string> lastNames{ "Keensplitter","Serpentbeard","Fallentide", "Stoutwood","Fusereaver","Horsegrove","Cindergazer","Ambershout","Fogbrooke","Strongglow","Bronzepunch","Gorewoods","Dirgepunch", "Freebreath", "Boulderdreamer","Hardbrand","Cloudbringer","Fogbrow","Holyleaf","Mystan"};

	int fGen = rand() % firstNames.size();
	int lGen = rand() % lastNames.size();

	firstName = firstNames[fGen];
	lastName = lastNames[lGen];
}

void Competitor::update() {

	syncMutex->lock();
	std::cout << ">" << firstName << " " << lastName << " has entered the arena!" << std::endl;
	syncMutex->unlock();

	while (health > 0) {
		
		switch (cState) {
		case deciding:

			if (!wielding) {
				cState = looting;
			}
			else {
				cState = attacking;
			}

			break;
		case looting:

			//TODO add proper looting, for now:
			wielding = new Weapon;

			cState = deciding;

			break;
		case attacking:

			//need to choose a target
			attack();

			cState = deciding;


			break;
		case defending:


			
			break;
		case moving:
			break;
		}

		if ((*competitorCount) == 1 && health > 0) {
			std::cout << ">" << firstName << " " << lastName << " has won the battle!" << std::endl;
		}

		bar->arrive_and_wait();
	
	}
	bar->arrive_and_wait();
}

void Competitor::setSync(std::mutex* nSync) {
	syncMutex = nSync;
}

void Competitor::attack(){

	bool foundValid = false;
	int targetI;
		while (!foundValid) {

			targetI = rand() % (*competitors).size();

			if (targetI != index) {
				if ((*competitors)[targetI].getHealth() > 0) {
					foundValid = true;
				}
			}
		}


	Competitor* target = &(*competitors)[targetI];

	int attackRoll = rand() % 20 + 1;

	if (attackRoll != 1) {
		if (attackRoll + strength + 3 >= target->getArmor()) {

			target->reduceHealth(wielding->RollDamage());
		}
	}

}

int Competitor::getHealth() {
	return health;
}
void Competitor::setBar(std::barrier<std::_No_completion_function>* nBar) {
	bar = nBar;
}

int Competitor::getArmor() {

	return armor;
}

void Competitor::reduceHealth(int reduction) {
	healthMutex->lock();
	if (health > 0) {
		health -= reduction;
		if (health <= 0) {
			(* competitorCount)--;
		}
	}
	healthMutex->unlock();
}
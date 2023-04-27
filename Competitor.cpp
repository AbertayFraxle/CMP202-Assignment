#include "Competitor.h"

Competitor::Competitor() {
	generateName();
	wielding = NULL;
	//generate health
	health = 12;
	for (int i = 0; i < 5; i++) {
		int nAddHealth = (rand() % 8 + 1);
		health += nAddHealth + 2;
	}
	
	cState = deciding;

	armor = 10;
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
	std::cout << ">" << firstName << " " << lastName << " has entered the arena!"  << std::endl;
	syncMutex->unlock();

	while (health > 0) {
		
		switch (cState) {
		case deciding:

			if (!wielding) {
				cState = looting;
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

		bar->arrive_and_wait();
	}
}

void Competitor::setSync(std::mutex* nSync) {
	syncMutex = nSync;
}

void Competitor::attack(){}
int Competitor::getHealth() {
	return health;
}
void Competitor::setBar(std::barrier<std::_No_completion_function>* nBar) {
	bar = nBar;
}
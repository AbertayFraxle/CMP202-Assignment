#include "Competitor.h"

Competitor::Competitor(std::vector<Competitor*>* nComp, int idx, int* compCount) {
	generateName();
	wielding = NULL;
	//generate health
	maxHealth = 12;
	for (int i = 0; i < 5; i++) {
		int nAddHealth = (rand() % 8 + 1);
		maxHealth += nAddHealth + 2;
	}
	health = maxHealth;

	competitors = nComp;
	index = idx;
	cState = deciding;

	healthMutex = new std::mutex;

//	std::cout << ">" << firstName << " " << lastName << " has entered the arena!" << std::endl;

	armor = 10;

	competitorCount = compCount;

	targetI = -1;
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

	switch (cState) {
		case deciding:

			if (!wielding) {
				cState = looting;
			}
			else {
				cState = attacking;
			}

			if (health <= (maxHealth / 2)) {
				
				int defenseRoll = rand() % 100 + 1;

				if (defenseRoll > 20 + (health * 2)) {
					cState = defending;
				}


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

			blocking = true;
			cState = deciding;
		
			break;
		case moving:
			break;
	}

}


void Competitor::hasWon() {
	std::cout << firstName << " " << lastName << " has won the battle!";
}

void Competitor::findTarget() {

	bool foundValid = false;
	int flip = rand() % 2 + 1;
	int dir;
	switch (flip) {
	case 1:
		dir = -1;
		break;
	case 2:
		dir = 1;
		break;
	}

	int attempt = 1;

	while (!foundValid) {

		int check = index + (dir * attempt);

		if (check > (competitors->size() - 1) || check < 0) {
			attempt = 1;
			dir = -dir;
		}
		else {

			if ((*competitors)[check]->getHealth() > 0) {
				targetI = check;
				foundValid = true;
			}
			else {
				attempt++;
			}
		}

	}
}

void Competitor::attack(){
	if (targetI == -1) {
		findTarget();
	}
	if ((*competitors)[targetI]->getHealth() <= 0) {
		findTarget();
	}


	
		Competitor* target = (*competitors)[targetI];

		int attackRoll;

		if (!target->isDefending()) {
			attackRoll = rand() % 20 + 1;
		}
		else {
			int roll1 = rand() % 20 + 1;
			int roll2 = rand() % 20 + 1;

			//if the target is defending, roll the attack twice and take the lowest one
			if (roll1 <= roll2) {
				attackRoll = roll1;
			}
			else if (roll2 < roll1) {
				attackRoll = roll2;
			}
		}


		if (attackRoll != 1) {
			if (attackRoll + strength + 3 >= target->getArmor()) {

				if (health > 0) {
					target->reduceHealth(wielding->RollDamage(),firstName,lastName);
				}
			}
		}
	
}

int Competitor::getHealth() {
	return health;
}

int Competitor::getArmor() {

	return armor;
}

void Competitor::reduceHealth(int reduction, string fName, string sName) {
	healthMutex->lock();
	if (health > 0) {
		health -= reduction;
		if (health <= 0) {
			(* competitorCount)--;

			//std::cout << ">"<< fName <<" "<< sName << " has killed " << firstName << " " << lastName << std::endl;
		}
	}
	healthMutex->unlock();
}

bool Competitor::isDefending() {
	return blocking;
}
#include "Competitor.h"

Competitor::Competitor(std::vector<Competitor*>* nComp, int idx, int* compCount) {
	generateName();
	wielding = NULL;
	wearing = NULL;
	//generate health
	maxHealth = 12;
	for (int i = 0; i < 5; i++) {
		int nAddHealth = (rand() % 8 + 1);
		maxHealth += nAddHealth + 2;
	}
	health = maxHealth;

	strength = rand() % 4 + 1;

	competitors = nComp;
	index = idx;
	healthMutex = new std::mutex;

	turnsTaken = 0;

	//std::cout << ">" << firstName << " " << lastName << " has entered the arena!" << std::endl;

	

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
	turnsTaken++;
	int roll = rand() % 100 + 1;

	if (!wielding) {
		if (roll < 90) {
			cState = looting;
		}
		else {
			cState = defending;
		}
	}
	else {
		if (!wearing && roll < 90) {
			
			cState = looting;
		}
		else {
			cState = attacking;
		}
	}

	if (health <= (maxHealth / 2)) {

		if (roll > 40 + (health * 2)) {
			cState = defending;
		}
	}

	if (health > 0) {
		switch (cState) {
		case looting:
			//TODO add proper looting, for now:
			if (!wielding) {
				wielding = new Weapon;
			}
			else {
				wearing = new Armor;
			}
			break;
		case attacking:
			//need to choose a target
			attack();

			break;
		case defending:
			blocking = true;

			break;
		case moving:
			break;
		}
	}

}


void Competitor::hasWon() {
	std::cout << firstName << " " << lastName << " (" << index << ") has won the battle in " << turnsTaken << " turns!\n";
}

void Competitor::findTarget() {

	bool foundValid = false;
	int flip = rand() % 2 + 1;
	int dir = 1;

	//decide whether to check positive from position or negative
	switch (flip) {
	case 1:
		dir = -1;
		break;
	case 2:
		dir = 1;
		break;
	}

	//count the attempts taken
	int attempt = 1;

	//count if checked the whole way along the arena
	int fullWay = 0;
	
	//find the next alive competitor along the chosen direction
	while (!foundValid) {

		if (fullWay >=2) {

			targetI = -1;

			break; }

		int check = index + (dir * attempt);

		if (check > (competitors->size() - 1) || check < 0) {
			attempt = 1;
			dir = -dir;
			fullWay++;
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

	//if find a target if current one is dead or not assigned
	if (targetI == -1) {
		findTarget();
		if (targetI == -1) {
			return;
		}
	}
	if ((*competitors)[targetI]->getHealth() <= 0) {
		findTarget();
		if (targetI == -1) {
			return;
		}
	}

	
	
		Competitor* target = (*competitors)[targetI];

		int attackRoll = 0;

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

			if (attackRoll ==20){
				if (health > 0) {
					target->reduceHealth(wielding->RollDamage()+ wielding->RollDamage() +strength, firstName, lastName,index);
				}

			}else if (attackRoll + strength + 3 >= target->getArmor()) {

				if (health > 0) {
					target->reduceHealth(wielding->RollDamage()+strength,firstName,lastName,index);
				}
			}
		}
	
}

int Competitor::getHealth() {
	healthMutex->lock();
	int rHealth = health;
	healthMutex->unlock();
	return rHealth;
}

int Competitor::getArmor() {

	if (wearing) {
		return wearing->getAC();
	}
	else {
		return 10;
	}
}

void Competitor::reduceHealth(int reduction, string fName, string sName, int eIndex) {
	if (health > 0) {
		health -= reduction;
		if (health <= 0) {
			(* competitorCount)--;

			eulogy =  "> " + fName + " " + sName  +" (" + std::to_string(eIndex) + ") has killed " + firstName + " " + lastName + " (" + std::to_string(index) + ")" + "\n";
		}
	}
}

bool Competitor::isDefending() {
	return blocking;
}

std::string Competitor::getEulogy() {
	return eulogy;
}
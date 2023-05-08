#pragma once
#define _SILENCE_AMP_DEPRECATION_WARNINGS
#include <iostream>
#include <fstream>
#include <thread>
#include <amp.h>
#include <vector>
#include <math.h>
#include <mutex>
#include <barrier>
#include "Competitor.h"


std::vector<std::thread> threads;
std::mutex mutex;
std::vector<Competitor*> competitors;
std::queue<Competitor*> turnQueue;


int competitorCount;
int compIndex;


void addCompetitor(int i);

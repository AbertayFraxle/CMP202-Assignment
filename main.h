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
#include <condition_variable>


std::vector<std::thread> threads;

std::mutex mutex, updatex;

std::condition_variable update;

std::vector<Competitor*> competitors;
std::queue<Competitor*> turnQueue;
std::queue<Competitor*> nTurnQueue;

bool ready;

int competitorCount;
int compIndex;


void addCompetitor(int i);

#pragma once
#define _SILENCE_AMP_DEPRECATION_WARNINGS
#include <iostream>
#include <fstream>
#include <thread>
#include <amp.h>
#include <vector>
#include <math.h>
#include <mutex>
#include "Competitor.h"
#include <condition_variable>
#include <chrono>
#include <queue>
#include <barrier>
#include <fstream>

using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::milliseconds;
typedef std::chrono::steady_clock the_clock;

std::vector<std::thread> threads;

std::mutex mutex, updatex, newtex;

std::barrier<>* bar;

std::condition_variable update;

std::vector<Competitor*> competitors;
std::vector<std::queue<Competitor*>> nTurnQueue;

bool won;
int arriveCount;

std::vector<float> times;

bool ready, canCV,killLog;

int competitorCount;
int compIndex;

void runTurn(std::queue<Competitor*>& turns, int& counter,std::barrier<>*);
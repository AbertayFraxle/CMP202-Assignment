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

int counter[1920];
std::vector<std::thread> threads;
std::mutex mutex;
std::vector<Competitor> competitors;

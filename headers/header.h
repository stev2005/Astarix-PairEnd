#pragma once

//#define NDEBUG
#include <bits/stdc++.h>
using namespace std;
char base[4] = {'A', 'C', 'G', 'T'};

typedef int cost_t;
cost_t inf = (1<<30);
cost_t maximum_edit_cost;

int nindel = 0;///can be taken as ndel and nins if deletion cost ==  insertion cost
string heuristic;

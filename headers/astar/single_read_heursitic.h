#pragma once

#include "../header.h"
#include <bits/stdc++.h>
#include "statesstruct.h"
using namespace std;

function<cost_t(int, Node, int, vector<int>&, crumbs_t&)> chosen_heuristic;

cost_t seed_heuristic(int qpos, Node p, int k, vector<int> &seeds, crumbs_t & crumbs){
    cost_t hvalue = 0;
    int i = (qpos % k)? qpos / k + 1: qpos / k;
    for (; i < seeds.size(); ++i)
        if (crumbs[p][i] == false){
            ++hvalue;
        }
    return hvalue;
}

inline void select_heuristic(string &heuristic){
    if (heuristic == "seed_heuristic")
        chosen_heuristic = &seed_heuristic;
    else if (heuristic == "dijkstra_heuristic")
        chosen_heuristic = [](int qpos, Node p, int k, vector<int> &seeds, crumbs_t &crumbs) -> cost_t{return 0;};
    else{
        cerr << "The choosen heuristic is unavailable.\n";
        cerr << "Choosen heuristic: " << heuristic << "\n";
        cerr << "Program is going to abort.\n";
        assert(false);
    }
}

Statesr createStatesr(int qpos, Node p, cost_t g, int k, vector<int> &seeds, crumbs_t &crumbs){
    return Statesr(qpos, p, g, chosen_heuristic(qpos, p, k, seeds, crumbs));
}

Statesr createStatesr(int qpos, int rpos, cost_t g, int k, vector<int> &seeds, crumbs_t &crumbs){
    return createStatesr(qpos, Node(rpos), g, k, seeds, crumbs);
}

Statesr createStatesr(int qpos, Trie *u, cost_t g, int k, vector<int> &seeds, crumbs_t &crumbs){
    return createStatesr(qpos, Node(u), g, k, seeds, crumbs);
}
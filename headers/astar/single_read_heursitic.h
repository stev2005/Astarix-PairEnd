#pragma once

#include "../header.h"
#include <bits/stdc++.h>
#include "statesstruct.h"
using namespace std;

cost_t seed_heuristic(int qpos, Node p, int k, vector<int> &seeds, crumbs_t & crumbs){
    cost_t hvalue = 0;
    int i = (qpos % k)? qpos / k + 1: qpos / k;
    for (; i < seeds.size(); ++i)
        if (crumbs[p][i] == false)
            ++hvalue;
    return hvalue;
}

cost_t choose_heuristic(int qpos, Node p, int k, vector<int> &seeds, crumbs_t &crumbs){
    if (heuristic == "seed_heuristic")
        return seed_heuristic(qpos, p, k, seeds, crumbs);
    else if (heuristic == "dijkstra_heuristic")
        return 0;
    else{
        cerr << "The choosen heuristic is unavailable.\n";
        cerr << "Choosen heuristic: " << heuristic << "\n";
        cerr << "Program is going to abort.\n";
        assert(false);
    }
}

Statesr createStatesr(int qpos, Node p, cost_t g, int k, vector<int> &seeds, crumbs_t &crumbs){
    return Statesr(qpos, p, g, choose_heuristic(qpos, p, k, seeds, crumbs));
}

Statesr createStatesr(int qpos, int rpos, cost_t g, int k, vector<int> &seeds, crumbs_t &crumbs){
    return createStatesr(qpos, Node(rpos), g, k, seeds, crumbs);
}

Statesr createStatesr(int qpos, Trie *u, cost_t g, int k, vector<int> &seeds, crumbs_t &crumbs){
    return createStatesr(qpos, Node(u), g, k, seeds, crumbs);
}
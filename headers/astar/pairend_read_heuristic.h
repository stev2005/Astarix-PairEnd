#pragma once

#include "../header.h"
#include <bits/stdc++.h>
#include "single_read_heursitic.h"
#include "statesstruct.h"
using namespace std;

cost_t pairend_heuristic(int qpos, Node p1, Node p2, int k, MatchingKmers &info){
    if (heuristic == "seed_heuristic"){
        cost_t h1 = seed_heuristic(qpos, p1, k, info.seeds1, info.crumbs1);
        cost_t h2 = seed_heuristic(qpos, p2, k, info.seeds2, info.crumbs2);
        return h1 + h2;
    }
    else if (heuristic == "dijkstra_heuristic")
        return 0;
    else{
        cerr << "The choosen heuristic is unavailable.\n";
        cerr << "Choosen heuristic: " << heuristic << "\n";
        cerr << "Program is going to abort.\n";
        assert(false);
    }
}


Statepr createStatepr(int qpos, Node p1, Node p2, cost_t g, cost_t h){
    return Statepr(qpos, p1, p2, g, h);
}

Statepr createStatepr(int qpos, int rpos1, int rpos2, cost_t g, cost_t h){
    return createStatepr(qpos, Node(rpos1), Node(rpos2), g, h);
}

Statepr createStatepr(int qpos, int rpos1, Trie* u2, cost_t g, cost_t h){
    return createStatepr(qpos, Node(rpos1), Node(u2), g, h);
}

Statepr createStatepr(int qpos, int rpos1, Node p2, cost_t g, cost_t h){
    return createStatepr(qpos, Node(rpos1), p2, g, h);
}

Statepr createStatepr(int qpos, Trie* u1, int rpos2, cost_t g, cost_t h){
    return createStatepr(qpos, Node(u1), Node(rpos2), g, h);
}

Statepr createStatepr(int qpos, Trie* u1, Trie* u2, cost_t g, cost_t h){
    return createStatepr(qpos, Node(u1), Node(u2), g, h);
}

Statepr createStatepr(int qpos, Trie* u1, Node p2, cost_t g, cost_t h){
    return createStatepr(qpos, Node(u1), p2, g, h);
}

Statepr createStatepr(int qpos, Node p1, int rpos2, cost_t g, cost_t h){
    return createStatepr(qpos, p1, Node(rpos2), g, h);
}

Statepr createStatepr(int qpos, Node p1, Trie* u2, cost_t g, cost_t h){
    return createStatepr(qpos, p1, Node(u2), g, h);
}

/*Statepr createStatepr(int qpos, Node p1, Node p2, cost_t g, int k, MatchingKmers &info){
    return Statepr(qpos, p1, p2, g, pairend_heuristic(qpos, p1, p2, k, info));
}

Statepr createStatepr(int qpos, int rpos1, int rpos2, cost_t g, int k, MatchingKmers &info){
    return createStatepr(qpos, Node(rpos1), Node(rpos2), g, k, info);
}

Statepr createStatepr(int qpos, int rpos1, Trie* u2, cost_t g, int k, MatchingKmers &info){
    return createStatepr(qpos, Node(rpos1), Node(u2), g, k, info);
}

Statepr createStatepr(int qpos, int rpos1, Node p2, cost_t g, int k, MatchingKmers &info){
    return createStatepr(qpos, Node(rpos1), p2, g, k, info);
}

Statepr createStatepr(int qpos, Trie* u1, int rpos2, cost_t g, int k, MatchingKmers &info){
    return createStatepr(qpos, Node(u1), Node(rpos2), g, k, info);
}

Statepr createStatepr(int qpos, Trie* u1, Trie* u2, cost_t g, int k, MatchingKmers &info){
    return createStatepr(qpos, Node(u1), Node(u2), g, k, info);
}

Statepr createStatepr(int qpos, Trie* u1, Node p2, cost_t g, int k, MatchingKmers &info){
    return createStatepr(qpos, Node(u1), p2, g, k, info);
}

Statepr createStatepr(int qpos, Node p1, int rpos2, cost_t g, int k, MatchingKmers &info){
    return createStatepr(qpos, p1, Node(rpos2), g, k, info);
}

Statepr createStatepr(int qpos, Node p1, Trie* u2, cost_t g, int k, MatchingKmers &info){
    return createStatepr(qpos, p1, Node(u2), g, k, info);
}*/
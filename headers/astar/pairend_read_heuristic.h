#pragma once

#include "../header.h"
#include <bits/stdc++.h>
#include "single_read_heursitic.h"
#include "statesstruct.h"
using namespace std;

cost_t pairend_heuristic(int qpos, Node p1, Node p2, int k, vector<int> &seeds1, vector<int> &seeds2, crumbs_t &crumbs1, crumbs_t &crumbs2){
    return chosen_heuristic(qpos, p1, k, seeds1, crumbs1) + chosen_heuristic(qpos, p2, k, seeds2, crumbs2);
}

cost_t pairend4D_heuristic(int qpos1, int qpos2, Node p1, Node p2, int k, vector<int> &seeds1, vector<int> &seeds2, crumbs_t &crumbs1, crumbs_t &crumbs2){
    return chosen_heuristic(qpos1, p1, k, seeds1, crumbs1) + chosen_heuristic(qpos2, p2, k, seeds2, crumbs2);
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

Statepr4D createStatepr4D(int qpos1, int qpos2, Node p1, Node p2, cost_t g, cost_t h){
    return Statepr4D(qpos1, qpos2, p1, p2, g, h);
}

Statepr4D createStatepr4D(int qpos1, int qpos2, int rpos1, int rpos2, cost_t g, cost_t h){
    return createStatepr4D(qpos1, qpos2, Node(rpos1), Node(rpos2), g, h);
}

Statepr4D createStatepr4D(int qpos1, int qpos2, int rpos1, Trie* u2, cost_t g, cost_t h){
    return createStatepr4D(qpos1, qpos2, Node(rpos1), Node(u2), g, h);
}

Statepr4D createStatepr4D(int qpos1, int qpos2, int rpos1, Node p2, cost_t g, cost_t h){
    return createStatepr4D(qpos1, qpos2, Node(rpos1), p2, g, h);
}

Statepr4D createStatepr4D(int qpos1, int qpos2, Trie* u1, int rpos2, cost_t g, cost_t h){
    return createStatepr4D(qpos1, qpos2, Node(u1), Node(rpos2), g, h);
}

Statepr4D createStatepr4D(int qpos1, int qpos2, Trie* u1, Trie* u2, cost_t g, cost_t h){
    return createStatepr4D(qpos1, qpos2, Node(u1), Node(u2), g, h);
}

Statepr4D createStatepr4D(int qpos1, int qpos2, Trie* u1, Node p2, cost_t g, cost_t h){
    return createStatepr4D(qpos1, qpos2, Node(u1), p2, g, h);
}

Statepr4D createStatepr4D(int qpos1, int qpos2, Node p1, int rpos2, cost_t g, cost_t h){
    return createStatepr4D(qpos1, qpos2, p1, Node(rpos2), g, h);
}

Statepr4D createStatepr4D(int qpos1, int qpos2, Node p1, Trie* u2, cost_t g, cost_t h){
    return createStatepr4D(qpos1, qpos2, p1, Node(u2), g, h);
}

#pragma once

#include "astar_pair-end.h"
using namespace std;

inline void push_init_Statepr4D_in_q(priority_queue<Statepr4D> &q, Node &&p1, Node &&p2, int k, vector<int> &seeds1, vector<int> &seeds2,
                                   crumbs_t &crumbs1, crumbs_t &crumbs2, bool negative){
    cost_t h = pairend4D_heuristic(0, 0, p1, p2, k, seeds1, seeds2, crumbs1, crumbs2);
    auto cur = createStatepr4D(0, 0, p1, p2, 0, h);
    cur.negative = negative;
    q.push(cur);
}

inline void push_first_Statepr4D_in_q_starnd(priority_queue<Statepr4D> &q, int m, Trie *root, int d, int k,vector<int> &seeds1,
                                            vector<int> &seeds2, crumbs_t &crumbs1, crumbs_t &crumbs2, bool negative){
    Node Nroot(root);
    push_init_Statepr4D_in_q(q, move(Nroot), move(Nroot), k, seeds1, seeds2, crumbs1, crumbs2, negative);
    for (int i = m - d + 1; i <= m; ++i){
        push_init_Statepr4D_in_q(q, move(Nroot), Node(i), k, seeds1, seeds2, crumbs1, crumbs2, negative);
        push_init_Statepr4D_in_q(q, Node(i), move(Nroot), k, seeds1, seeds2, crumbs1, crumbs2, negative);
    }
    for (int i = m - d + 1; i <= m; ++i)
        for (int j = i; j <= m; ++j)
            push_init_Statepr4D_in_q(q, Node(i), Node(i), k, seeds1, seeds2, crumbs1, crumbs2, negative);

}

inline void push_first_Statepr4D_in_q(priority_queue<Statepr4D> &q, int m, Trie *root, int d, int k, MatchingKmers &info){
    vector<int> *curseeds1 = &info.seeds1, *curseeds2 = &info.nseeds2;
    crumbs_t *curcrumbs1 = &info.crumbs1, *curcrumbs2 = &info.ncrumbs2;
    push_first_Statepr4D_in_q_starnd(q, m, root, d, k, *curseeds1, *curseeds2, *curcrumbs1, *curcrumbs2, false);
    curseeds1 = &info.seeds2;
    curseeds2 = &info.nseeds1;
    curcrumbs1 = &info.crumbs2;
    curcrumbs2 = &info.ncrumbs1;
    push_first_Statepr4D_in_q_starnd(q, m, root, d, k, *curseeds1, *curseeds2, *curcrumbs1, *curcrumbs2, true);
}

/*Statepr4D*/ void astar_pairend_read4D_alignment(pair<string, string> &queryp, pair<string, string> &nqueryp, string &ref, int d, int k, Trie *root, MatchingKmers &info){
    int n = queryp.first.size();
    int m = ref.size();
    priority_queue<Statepr4D> q;
    push_first_Statepr4D_in_q(q, m, root, d, k, info);
    Statepr cur;
    punishl = readdist - drange;
    punishr = readdist + drange;
    
}
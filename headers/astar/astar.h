#pragma once

#include"../header.h"
#include<bits/stdc++.h>
#include"../trie.h"
using namespace std;

struct State
{
    int qpos, rpos;///qpos: position in query; rpos: position in reference
    cost_t cost;///edit distance
    cost_t h;///value of heuristic function
    State(){}
    State(int _qpos, int _rpos, cost_t _cost, cost_t _h){
        qpos = _qpos;
        rpos = _rpos;
        cost = _cost;
        h = _h;
    }
    bool operator<(const State &state)const{
        return cost+h > state.cost+state.h;
    }
};

auto SetStateCmp =[](State c, State d){
    if (c.qpos != d.qpos) return c.qpos < d.qpos;
    return c.rpos < d.rpos;
};

int seed_heuristic(int qpos, int rpos,const int k, const vector<int> &seeds,const vector<int> &last){
    int h = 0;
    for (int i = qpos / k + 1; i < seeds.size(); ++i){///i: the index of next seed of the seed in which is qpos
        if (seeds[i] != -1){
            int lastbeg = (last[seeds[i]]/k)*k;///the start position of the last occurance of kmer
            if (rpos > lastbeg)///no exact match in the leaving reference
                ++h;
        }
        else ++h;
    } 
    return h;
}

cost_t align_astar(string &query, string &ref, vector<int> &seeds, vector<int> &last){
    priority_queue<State>q;
    set<State, decltype(SetStateCmp)> visited(SetStateCmp);
    State cur;
    int n = query.size();
    int m = ref.size();
    int k = log2(m)/2;
    for (int i = 0; i <= m; ++i)
        q.push(State(0, i, 0, seed_heuristic(0, i, k, seeds, last)));
    while (!q.empty()){
        cur = q.top();
        q.pop();
        if (cur.qpos == n) break;
        if (visited.find(cur) == visited.end()){
            visited.insert(cur);
            if (cur.rpos < m){
                if (query[cur.qpos] == ref[cur.rpos]){
                    q.push(State(cur.qpos+1, cur.rpos+1, cur.cost, seed_heuristic(cur.qpos+1, cur.rpos+1, k, seeds, last)));
                    continue;
                }
                else q.push(State(cur.qpos+1, cur.rpos+1, cur.cost+1, seed_heuristic(cur.qpos+1, cur.rpos+1, k, seeds, last)));
                q.push(State(cur.qpos, cur.rpos+1, cur.cost+1, seed_heuristic(cur.qpos, cur.rpos+1, k, seeds, last)));
            }
            q.push(State(cur.qpos+1, cur.rpos, cur.cost+1, seed_heuristic(cur.qpos+1, cur.rpos, k, seeds, last)));
        }
    }
    return cur.cost;
}

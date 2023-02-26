#pragma once

#include"../header.h"
#include<bits/stdc++.h>
#include"../trie.h"
using namespace std;

struct State
{
    int qpos, rpos;
    cost_t cost, h;
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

int heuristic(int qpos, int rpos, int k, vector<int> &seeds, vector<int> &last){
    int h = 0;
    ///for (auto i:seeds)
    for (int i = (qpos-1) / k + 1; i < seeds.size(); ++i)
        if (seeds[i] != -1){
            int lastbeg = (last[seeds[i]]/k)*k;
            if (rpos > lastbeg)
                ++h;
        }
    return h;
}

cost_t edit_distance_AstarixSH(string &query, string &ref, vector<int> &seeds, vector<int> &last){
    priority_queue<State>q;
    set<State, decltype(SetStateCmp)> visited(SetStateCmp);
    State cur;
    int n = query.size();
    int m = ref.size();
    for (int i = 0; i <= m; ++i)
        q.push(State(0, i, 0, heuristic(0, i, log2(m)/2, seeds, last)));
    while (!q.empty()){
        cur = q.top();
        q.pop();
        if (cur.qpos == n) break;
        if (visited.find(cur) == visited.end()){
            visited.insert(cur);
            if (cur.rpos < m){
                if (query[cur.qpos] == ref[cur.rpos]){
                    q.push(State(cur.qpos+1, cur.rpos+1, cur.cost, heuristic(cur.qpos, cur.rpos, log2(m)/2, seeds, last)));
                    continue;
                }
                else q.push(State(cur.qpos+1, cur.rpos+1, cur.cost+1, heuristic(cur.qpos, cur.rpos, log2(m)/2, seeds, last)));
                q.push(State(cur.qpos, cur.rpos+1, cur.cost+1, heuristic(cur.qpos, cur.rpos+1, log2(m)/2, seeds, last)));
            }
            q.push(State(cur.qpos+1, cur.rpos, cur.cost+1, heuristic(cur.qpos+1, cur.rpos, log2(m)/2, seeds, last)));
        }
    }
    return cur.cost;
}

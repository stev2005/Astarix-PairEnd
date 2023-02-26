#pragma once

#include "../header.h"
#include <bits/stdc++.h>
#include "../trie.h"
using namespace std;
struct Node{
    int rpos;///position of the linier reference
    Trie *u;///position of the Trie tree

    Node(){}
    Node(int _rpos, Trie *_u){
        rpos = _rpos;
        u = _u;
    }
    Node(int _rpos){
        rpos = _rpos;
        u = nullptr;
    }
    Node(Trie *_u){
        rpos = -1;
        u = _u;
    }

    bool is_in_trie(){
        return (u == nullptr)? false: true;
    }
    
    bool operator<(const Node &p)const{
        if (u != p.u) return u < p.u;
        return rpos < p.rpos;
    }

    bool operator==(const Node &p)const{
        return (rpos == p.rpos && u == p.u)?true:false;
    }
};

struct State{
    int qpos;///position of the query
    Node p;///node of the reference alignment
    cost_t cost;///edit distance
    cost_t h;///heuristic() of the state
    State(){}
    State(int _qpos, Node _p, cost_t _cost, cost_t _h){
        qpos = _qpos;
        p = _p;
        cost = _cost;
        h = _h;
    }
    State(int _qpos, Node _p){
        qpos =  _qpos;
        p = _p;
    }
    bool operator<(const State &state)const{
        return cost+h > state.cost+state.h;
    }
};

auto SetStateCmp = [](State c, State d){
    if (c.qpos != d.qpos) return c.qpos < d.qpos;
    return c.p < d.p;
};

int heuristic_int(int qpos, int rpos, int k, vector<int> &seeds, vector<int> &last){
    int h = 0;
    for (int i = (qpos-1) / k + 1; i < seeds.size(); ++i)
        if (seeds[i] != -1){
            int lastbeg = (last[seeds[i]]/k)*k;
            if (rpos > lastbeg)
                ++h;
        }
    return h;
}


int heuristic(int qpos, Node p, int k, vector<int> &seeds, vector<int> &last){
    if (p.is_in_trie())return last.size();///worst case scenario
    return heuristic_int(qpos, p.rpos, k, seeds, last);
}

cost_t edit_distance_AstarixSH_TrieStart(string &query, string &ref, Trie *root, vector<int> &seeds, vector<int> &last, vector<int> &prevpos){
    priority_queue<State>q;
    set <State, decltype(SetStateCmp)> visited(SetStateCmp);
    int n = query.size();
    int m = ref.size();
    int k = log2(m)/2;
    State cur(0, Node(root), 0, last.size());
    //q.push(cur);
    for (int i = 0; i <= m; ++i)
        q.push(State(0, Node(i), 0, heuristic(0, Node(i), log2(m)/2, seeds, last)));
    while (!q.empty()){
        cur =  q.top();
        q.pop();
        if (cur.qpos == n) break;
        if (visited.find(cur) == visited.end()){
            visited.insert(cur);
            if (cur.p.is_in_trie()){
                if (cur.p.u->is_leaf()){
                    for (int i = last[cur.p.u->num]; i != -1; i = prevpos[i])
                        q.push(State(cur.qpos, Node(i+1), cur.cost, heuristic(cur.qpos, Node(i+1), k, seeds, last)));
                    continue;
                }
                if (cur.qpos == 0 && cur.cost == 0){
                    for (int i = 0; i < 4; ++i)
                        if (cur.p.u->child[i] != nullptr)
                            q.push(State(cur.qpos, Node(cur.p.u->child[i]), cur.cost, heuristic(cur.qpos, Node(cur.p.u->child[i]), k, seeds, last)));
                }
                q.push(State(cur.qpos+1, cur.p, cur.cost+1, heuristic(cur.qpos+1, cur.p, k, seeds, last)));
                for (int i = 0; i < 4; ++i)
                    if (cur.p.u->child[i] != nullptr){
                        if (base[i] == query[cur.qpos])
                            q.push(State(cur.qpos+1, Node(cur.p.u->child[i]), cur.cost, heuristic(cur.qpos+1, Node(cur.p.u->child[i]), k, seeds, last)));
                        else q.push(State(cur.qpos+1, Node(cur.p.u->child[i]), cur.cost+1, heuristic(cur.qpos+1, Node(cur.p.u->child[i]), k, seeds, last)));
                        q.push(State(cur.qpos, Node(cur.p.u->child[i]), cur.cost+1, heuristic(cur.qpos+1, Node(cur.p.u->child[i]), k, seeds, last)));
                    }
            }
            else{
                if (cur.p.rpos < m){
                    if (query[cur.qpos] == ref[cur.p.rpos]){
                        q.push(State(cur.qpos+1, Node(cur.p.rpos+1), cur.cost, heuristic(cur.qpos+1, Node(cur.p.rpos+1), k, seeds, last)));
                        continue;
                    }
                    else q.push(State(cur.qpos+1, Node(cur.p.rpos+1), cur.cost+1, heuristic(cur.qpos+1, Node(cur.p.rpos+1), k, seeds, last)));
                    q.push(State(cur.qpos, Node(cur.p.rpos+1), cur.cost+1, heuristic(cur.qpos, Node(cur.p.rpos+1), k, seeds, last)));
                }
                q.push(State(cur.qpos+1, cur.p, cur.cost+1, heuristic(cur.qpos+1, cur.p, k, seeds, last)));
            }
        }
    }
    return cur.cost;
}
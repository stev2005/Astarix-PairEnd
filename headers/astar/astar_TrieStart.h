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

int seed_heuristic_int(int qpos, int rpos, int k,const vector<int> &seeds,const vector<int> &last){
    int h = 0;
    for (int i = qpos / k + 1; i < seeds.size(); ++i){///i: index of neighboor right seed of the seed in which is qpos
        if (seeds[i] != -1){
            int lastbeg = (last[seeds[i]]/k)*k;///the start position of the last occurance of kmer
            if (rpos > lastbeg)///no exact match in the leaving reference
                ++h;
        }
        else ++h;
    } 
    return h;
}

int seed_heuristic(State cur, int k,const vector<int> &seeds,const vector<int> &last){
    if (cur.p.is_in_trie()) return seed_heuristic_int(cur.qpos, cur.p.u->firstapp+1, k, seeds, last);
    return seed_heuristic_int(cur.qpos, cur.p.rpos, k, seeds, last);
}

vector<State> getNextStates(State cur, char c, int k,const string &ref, const vector<int> &seeds, const vector<int> &last, const vector<int> &prevpos){
    vector<State>ineritors;
    if (cur.p.is_in_trie()){
        if (cur.p.u->is_leaf()){
            for (int i = last[cur.p.u->num]; i != -1; i = prevpos[i])
                inheritors.push_back(State(cur.qpos, Node(i+1), cur.cost, cur.h));
            return inheritors;
        }
        inheritors.push_back(State(cur.qpos+1, cur.p, cur.cost+1, seed_heuristic(State(cur.qpos+1, cur.p), k, seeds, last)))
        for (int i = 0; i < 4; ++i)
            if (cur.p->[i] != nullptr){
                inheritors.push_back(State(cur.qpos+1, Node(cur.p.u->child), cur.cost, seed_heuristic(State(cur.qpos+1, cur.p.u->child[i]), k, seeds, last)));
                if (base[i] != c)inheritors.back().cost++;
                inheritors.push_back(State(cur.qpos, Node(cur.p.u->child), cur.cost+1, seed_heuristic(State(cur.qpos, cur.p.u->child[i]), k, seeds, last)));
            }
    }
    else{
        if (cur.p.rpos < ref.size()){
            inheritors.push_back(State(cur.qpos+1, Node(cur.p.rpos+1), cur.cost, seed_heuristic(State(cur.qpos+1, Node(cur.p.rpos+1)), k, seeds, last));
            if (c == ref[cur.p.rpos])
                return inheritors;
            else inhetiros.back().cost++;
            inheritors.push_back(State.cur.qpos, Node(cur.p.rpos+1), cur.cost, seed_heuristic(State(cur.qpos, Node(cur.p.rpos)), k, seeds, last));
        }
        inheritors.push_back(State(cur.qpos+1, cur.p, cur.cost+1, seed_heuristic(State(cur.qpos+1, cur.p), k, seeds, last)));
    }
    return inheritorts;
}

cost_t align_astar_TrieStart(string &query, string &ref, Trie *root, const vector<int> &seeds, const vector<int> &last, const vector<int> &prevpos){
    priority_queue<State>q;
    set <State, decltype(SetStateCmp)> visited(SetStateCmp);
    int n = query.size();
    int m = ref.size();
    int k = log2(m)/2;
    State cur(0, Node(root), 0, last.size());
    q.push(cur);
    while (!q.empty()){
        cur =  q.top();
        q.pop();
        if (cur.qpos == n) break;
        if (visited.find(cur) == visited.end()){
            visited.insert(cur);
            vector<State> next = getNextStates(cur, query[cur.qpos], k, ref, seeds, last, prevpos);
            for (auto i:next)
                q.push(i);
        }
    }
    return cur.cost;
}
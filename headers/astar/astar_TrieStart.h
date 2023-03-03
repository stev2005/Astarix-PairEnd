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
        cost = 0;
        h = 0;
    }
    bool operator<(const State &state)const{
        return cost+h > state.cost+state.h;
    }
    /*void print(){
        cout<<qpos<<" ";
        if (p.is_in_trie())
            cout<<
    }*/
};

auto SetStateCmp = [](State c, State d){
    if (c.qpos != d.qpos) return c.qpos < d.qpos;
    return c.p < d.p;
};

int seed_heuristic_int(int qpos, int rpos, int k,const MatchingKmers &info){
    int h = 0;
    for (int i = qpos / k + 1; i < info.seeds.size(); ++i){///i: index of neighboor right seed of the seed in which is qpos
        if (info.seeds[i] != -1){
            int lastbeg = (info.last[info.seeds[i]]/k)*k;///the start position of the last occurance of kmer
            if (rpos > lastbeg)///no exact match in the leaving reference
                ++h;
        }
        else ++h;
    } 
    return h;
}

int seed_heuristic(State cur, int k, const MatchingKmers &info){
    if (cur.p.is_in_trie()) return seed_heuristic_int(cur.qpos, cur.p.u->firstapp+1, k, info);
    return seed_heuristic_int(cur.qpos, cur.p.rpos, k, info);
}

int heuristic(State cur, int k, const MatchingKmers &info, char *heuristic_method){
    if (strcmp(heuristic_method, "dijkstra_heuristic") == 0) return 0;///there isn't any
    if (strcmp(heuristic_method, "seed_heuristic") == 0) return seed_heuristic(cur, k, info);
}

State PushState(State cur, int k, const MatchingKmers &info, char *heuristic_method){
    return State(cur.qpos, cur.p, 1, heuristic(cur, k, info, heuristic_method));
}

vector<State> getNextStates(State cur, char c, int k,const string &ref,const MatchingKmers &info, char *heuristic_method){
    vector<State>inheritors;
    if (cur.p.is_in_trie()){
        if (cur.p.u->is_leaf()){
            for (int i = info.last[cur.p.u->num]; i != -1; i = info.prevpos[i]){
                inheritors.push_back(PushState(State(cur.qpos, Node(i+1)), k, info, heuristic_method));
                inheritors.back().cost--;
            }   
            return inheritors;
        }
        /*if (cur.qpos == 0 && cur.cost == 0)
            for (int i = 0; i < 4; ++i)
                if (cur.p.u->child[i] != nullptr)
                    inheritors.push_back(State(cur.qpos, Node(cur.p.u->child[i]), cur.cost, seed_heuristic(State(cur.qpos, cur.p.u->child[i]), k, seeds, last)));*/
        inheritors.push_back(PushState(State(cur.qpos+1, cur.p), k, info, heuristic_method));
        for (int i = 0; i < 4; ++i)
            if (cur.p.u->child[i] != nullptr){
                inheritors.push_back(PushState(State(cur.qpos+1, Node(cur.p.u->child[i])), k, info, heuristic_method));
                if (base[i] == c)inheritors.back().cost--;
                inheritors.push_back(PushState(State(cur.qpos, Node(cur.p.u->child[i])), k, info, heuristic_method));
            }
    }
    else{
        if (cur.p.rpos < ref.size()){
            inheritors.push_back(PushState(State(cur.qpos+1, Node(cur.p.rpos+1)), k, info, heuristic_method));
            if (c == ref[cur.p.rpos]){
                inheritors.back().cost--;
                return inheritors;
            }
            inheritors.push_back(PushState(State(cur.qpos, Node(cur.p.rpos+1)), k, info, heuristic_method));
        }
        inheritors.push_back(PushState(State(cur.qpos+1, cur.p), k, info, heuristic_method));
    }
    return inheritors;
}

cost_t align_astar_TrieStart(string &query, string &ref, int k, Trie *root, const MatchingKmers info, char *heuristic_method){
    priority_queue<State>q;
    set <State, decltype(SetStateCmp)> visited(SetStateCmp);
    int n = query.size();
    int m = ref.size();
    State cur=PushState(State(0, Node(root)), k, info, heuristic_method);
    cur.cost--;///PushState returns cost=1, when it is needed, reducing it to 0
    q.push(cur);
    for (int i = m; i >= m-k+1; --i){
        cur = PushState(State(0, Node(i)), k, info, heuristic_method);
        cur.cost--;
        q.push(State());  
    }
    while (!q.empty()){
        cur =  q.top();
        q.pop();
        //assert(cout<<"")
        if (cur.qpos == n) break;
        if (visited.find(cur) == visited.end()){
            visited.insert(cur);
            vector<State> next = getNextStates(cur, query[cur.qpos], k, ref, info, heuristic_method);
            for (auto i:next){
                i.cost+=cur.cost;
                q.push(i);
            }
                
        }
    }
    //cout<<"Count of Explored states: "<<visited.size()+1<<"\n";/// +1 last state not inserted
    return cur.cost;
}
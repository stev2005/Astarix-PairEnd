#pragma once

#include"../header.h"
#include<bits/stdc++.h>
#include"../trie.h"

using namespace std;

struct Node{
    int rpos;
    Trie *u;

    Node(){}
    Node(int _rpos){
        rpos = _rpos;
        u = nullptr;
    }
    Node(Trie *_u){
        rpos = -1;
        u = _u;
    }

    bool is_in_trie(){
        return (u!=nullptr)?true:false;
    }

    bool operator==(const Node &other)const{
        return (rpos == other.rpos && u == other.u)?true:false;
    }

    bool operator<(const Node &other)const{
        if (rpos != other.rpos)return rpos < other.rpos;
        return u < other.u;
    }
};


struct  Statesr{
    ///State single read
    int qpos;
    Node p;
    cost_t g;///edit distance
    cost_t h;///heuristic
    Statesr(){}
    Statesr(int _qpos, Node _p){
        qpos = _qpos;
        p = _p;
        g = 0;
        h = 0;
    }
    Statesr(int _qpos, Node _p, cost_t _g, cost_t _h){
        qpos = _qpos;
        p = _p;
        g = _g;
        h = _h;
    }
    bool operator<(const Statesr &other)const{
        return g + h > other.g + other.h;
    }
    bool print(){
        cout << "Statesr: "<< qpos<< " "; 
        if (p.is_in_trie())
            cout << p.u ;
        else cout << p.rpos;
        cout<< " " << g << " " << h << endl;
        return true;
    }
};

struct explstatesr{
    int qpos;
    Node p;
    explstatesr(){}
    explstatesr(Statesr cur){
        qpos = cur.qpos;
        p = cur.p;
    }
    bool operator<(const explstatesr &other)const{
        if (qpos != other.qpos) return qpos < other.qpos;
        return p < other.p;
    }
};

auto setStatesrCmp = [](Statesr c, Statesr d){
    if (c.qpos != d.qpos)return c.qpos < d.qpos;
    return c.p < d.p;
};

cost_t seed_heuristic_int(int qpos, int rpos, int k, MatchingKmers &info){
    int h = 0;
    vector<int> seeds = info.seeds, last = info.last;
    for (int i = qpos / k + 1; i < seeds.size(); ++i){
        if (seeds[i] == -1) h++;
        else{
            int lastbeg = (last[seeds[i]]/k)*k + 1;
            /*start position of the last occurance of the seed[i]
              using that division last[seed[i]]/k rounds to lower int. not to upper one */
            if (lastbeg <= rpos)h++;
        }
    }
    return h;
}

cost_t seed_heuristic(Statesr cur, int k, MatchingKmers &info){
    if (cur.p.is_in_trie())return seed_heuristic_int(cur.qpos, cur.p.u->firstapp, k, info);
    else return seed_heuristic_int(cur.qpos, cur.p.rpos, k, info);
}

cost_t heuristic(Statesr cur, int k, MatchingKmers &info, char *heuristic_method){
    if (strcmp(heuristic_method, "dijkstra_heuristic") == 0)return 0;
    if (strcmp(heuristic_method, "seed_heuristic") == 0) return seed_heuristic(cur, k, info);
}

Statesr PushStatesr(Statesr cur, int k, MatchingKmers &info, char *heuristic_method){
    return Statesr(cur.qpos, cur.p, 1, heuristic(cur, k, info, heuristic_method));
}

vector <Statesr> NextStatesr(Statesr cur, char curqbp, const string &ref, int k, MatchingKmers &info, char *heuristic_method){
    vector <Statesr> next;
    if (cur.p.is_in_trie()){
        if (cur.p.u->is_leaf()){
            vector <int> last = info.last;
            vector <int> prevpos = info.prevpos;
            for (int i = last[cur.p.u->num]; i != -1; i = prevpos[i]){
                next.push_back(PushStatesr(Statesr(cur.qpos, Node(i+1)), k, info, heuristic_method));
                next.back().g--;
            }
            return next;
        }
        next.push_back(PushStatesr(Statesr(cur.qpos+1, cur.p), k, info, heuristic_method));
        for (int i = 0; i < 4; ++i)
            if (cur.p.u->child[i] != nullptr){
                next.push_back(PushStatesr(Statesr(cur.qpos+1, Node(cur.p.u->child[i])), k, info, heuristic_method));
                if (base[i] == curqbp)
                    next.back().g--;
                next.push_back(PushStatesr(Statesr(cur.qpos, Node(cur.p.u->child[i])), k, info, heuristic_method));
            }
    }
    else{
        if (cur.p.rpos < ref.size()){
            next.push_back(PushStatesr(Statesr(cur.qpos+1, Node(cur.p.rpos+1)), k, info, heuristic_method));
            if (ref[cur.p.rpos] == curqbp){
                next.back().g--;
                return next;
            }
            next.push_back(PushStatesr(Statesr(cur.qpos, Node(cur.p.rpos+1)), k, info, heuristic_method));
        }
        next.push_back(PushStatesr(Statesr(cur.qpos+1, cur.p), k, info, heuristic_method));
    }
    return next;
}

cost_t astar_single_read_alignment(string &query, string &ref, int k, Trie *root, MatchingKmers info, char *heuristic_method, char *showcntexplstates, char *triestart){
    int n = query.size();
    int m = ref.size();
    priority_queue<Statesr> q;
    set<explstatesr>visited;
    Statesr cur;
    if (strcmp(triestart, "Yes") == 0){
        cur = PushStatesr(Statesr(0, Node(root)), k, info, heuristic_method);
        cur.g--;
        q.push(cur);
        for (int i = m - k + 1; i <= m; ++i){
            cur = PushStatesr(Statesr(0, Node(i)), k, info, heuristic_method);
            cur.g--;
            q.push(cur);
        }
    }
    else{
        for (int i = 0; i <= m; ++i){
            cur = PushStatesr(Statesr(0, Node(i)), k, info, heuristic_method);
            cur.g--;
            q.push(cur);
        }
    }
    while(!q.empty()){
        cur = q.top();
        q.pop();
        assert(cur.print());
        if (cur.qpos == n)
            break;
        if (visited.find(explstatesr(cur)) == visited.end()){
            visited.insert(explstatesr(cur));
            vector <Statesr> next = NextStatesr(cur, query[cur.qpos], ref, k, info, heuristic_method);
            assert(cout << "size of next == "<< next.size() << endl);
            for (auto i:next){
                assert(cout<<"Inheritor: ");
                assert(i.print());
                i.g += cur.g;
                q.push(i);
            }
        }
    }
    if (strcmp(showcntexplstates, "Yes") == 0)
        cout << "Explored states == " << visited.size() << " ";
    return cur.g;
}
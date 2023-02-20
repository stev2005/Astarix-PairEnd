#pragma once

#include "../header.h"
#include "../trie.h"
#include <bits/stdc++.h>

using namespace std;

struct  RefTrieptr{
    int rpos;
    ///rpos - points to the linier reference
    Trie *u;
    ///u - points to the Trie tree
    RefTrieptr(){}
    RefTrieptr(int _rpos, Trie *_u){
        rpos = _rpos;
        u = _u;
    }
    
    bool is_in_trie(){
        return (u == nullptr)? false: true;
    }
    
    bool operator<(const RefTrieptr &p)const{
        if (u == nullptr){
            if (p.u == nullptr)
                return rpos < p.rpos;
            return true;
        }
        else if (p.u == nullptr)
            return false;
        else return rpos < p.rpos;
    }

    bool operator==(const RefTrieptr &p)const{
        if (rpos == p.rpos && u == p.u)
            return true;
        return false;
    }

};

struct nextstate{
    RefTrieptr state;
    cost_t add;
    int qpos;
    ///next - the next pointer to the graph
    ///add - the additional cost
    nextstate(){}
    nextstate(int _qpos, RefTrieptr _state, cost_t _add){
        qpos = _qpos;
        state = _state;
        add = _add;
    }
    void print(){
        cout<<state.rpos<<" "<<state.u<<" "<<add<<" "<<qpos<<"\n";
    }
};

vector <nextstate> getnextstate(char c,string &ref, RefTrieptr par, int qpos){
    vector <nextstate> v;
    nextstate cur;
    cur = nextstate(qpos, par, 0);
    assert(cout<<"The same state as the one from which we go to others\n");
    v.push_back(cur);
    cur = nextstate(qpos+1, par, 1);
    v.push_back(cur);
    ///indel
    if (par.is_in_trie()){
        for (int i = 0; i < 4; ++i)
            if (par.u->child[i] != nullptr){
                cur = nextstate(qpos, RefTrieptr(-1, par.u->child[i]), 1);
                v.push_back(cur);
                ///indel
                cur = nextstate(qpos+1, RefTrieptr(-1, par.u->child[i]), 0);
                if (c != base[i])
                    cur.add++;
                v.push_back(cur);
                ///match/subtitution
            }
    }
    else if(par.rpos < ref.size()){
        cur = nextstate(qpos, RefTrieptr(par.rpos+1, nullptr), 1);
        v.push_back(cur);
        ///indel
        cur = nextstate(qpos+1, RefTrieptr(par.rpos+1, nullptr), 0);
        if (c != ref[par.rpos])
            cur.add++;
        v.push_back(cur);
        ///match/substition
    }
    return v;
}

struct Nodeptr{
    RefTrieptr p1, p2;
    /// p1 - the left alignment
    /// p2 - the right alignment
    int qpos;
    cost_t cost;
    Nodeptr(){}
    Nodeptr(int _qpos, RefTrieptr _p1, RefTrieptr _p2, cost_t _cost){
        qpos = _qpos;
        p1 = _p1;
        p2 = _p2;
        cost = _cost;
    }
    
    bool operator<(Nodeptr const &state)const{
        return cost > state.cost;
    }

    void print(){
        cout<<"State: qpos== "<<qpos<<" p1.u== "<<p1.u<<" p1.rpos== "<<p1.rpos<<" p2.u == "<< p2.u<<" p2.rpos== "<<p2.rpos<<" cost=="<<cost<<endl;
    }

};

auto setNodeptrcmp = [](Nodeptr c, Nodeptr d){
    if (c.qpos == d.qpos){
        if (c.p1 == d.p1)
            return c.p2 < d.p2;
        return c.p1 < d.p1;
    }
    return c.qpos < d.qpos;
};

int edit_distance_dijkstra_paired_end_trie(pair<string, string> &query, string &ref, Trie *T, vector<int>&last, vector<int>&prevpos){
    string left = query.first;
    string right = query.second;
    int n = left.size();
    int m = ref.size();
    priority_queue<Nodeptr> q;
    set <Nodeptr, decltype(setNodeptrcmp)> visited(setNodeptrcmp);
    Nodeptr w, nb;
    w = Nodeptr(0, RefTrieptr(-1, T), RefTrieptr(-1, T), 0);
    q.push(w);
    bool flag=true;
    ///next1 - states from the left alignment
    ///next2 - states from the right alignment
    while (!q.empty()){
        w = q.top();
        q.pop();
        //w.print();
        if (visited.find(w) == visited.end())
            visited.insert(w);
        else{
            assert(cout<<"Previously poped out state\n");
            /*auto it=visited.find(w);
            nb=*it;
            nb.print();*/
            continue;
        }
        if (w.qpos == n)
            break;
        vector<nextstate> next1, next2;
        next1 = getnextstate(left[w.qpos], ref, w.p1, w.qpos);
        next2 = getnextstate(right[w.qpos], ref, w.p2, w.qpos);
        if (!w.p1.is_in_trie() && !w.p2.is_in_trie()){
            if (w.p1.rpos < m && w.p2.rpos < m){
                if (left[w.qpos] == ref[w.p1.rpos] &&
                    right[w.qpos] == ref[w.p2.rpos]){
                        nb = Nodeptr(w.qpos+1, RefTrieptr(w.p1.rpos+1, nullptr), RefTrieptr(w.p2.rpos+1, nullptr), w.cost);
                        q.push(nb);
                        continue;
                    }
            }
        }
        if (w.p1.is_in_trie() && w.p1.u->num!=-1){
            for (int i = last[w.p1.u->num]; i != -1; i = prevpos[i]){
                nb = Nodeptr(w.qpos, RefTrieptr(i+1, nullptr), w.p2, w.cost);
                q.push(nb);
            }
            /*if (w.p1.u->is_leaf())
                continue;*/
            ///if the optimizated build of the trie is used one of the k-mers has lenght less than k
        }

        if (w.p2.is_in_trie() && w.p2.u->num!=-1){
            for (int i = last[w.p2.u->num]; i != -1; i = prevpos[i]){
                nb = Nodeptr(w.qpos, w.p1, RefTrieptr(i+1, nullptr), w.cost);
                q.push(nb);
            }
            /*if (w.p2.u->is_leaf())
                continue;*/
        }

        for (auto i:next1)
            for (auto j:next2)
                if (i.qpos == j.qpos){
                    nb = Nodeptr(i.qpos, i.state, j.state, w.cost);
                    nb.cost += i.add + j.add;
                    q.push(nb);
                }
    }
    //w.print();
    return w.cost;
}
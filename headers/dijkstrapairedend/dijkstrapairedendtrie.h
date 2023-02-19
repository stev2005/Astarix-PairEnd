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
    vector <pair<RefTrieptr, int> > getnextstates(char c,string &ref){
        vector<pair<RefTrieptr, int> > v;
        ///first - the next state
        ///second - the additional cost at matching=0 or substitution=1
        if(rpos!=-1){
            if(rpos < ref.size()){
                v.push_back({RefTrieptr(rpos+1, nullptr),0});
                if(ref[rpos] != c)
                   v[0].second++;
            }
        }
        else{
            for(int i = 0; i < 4; ++i)
                if(u->child[i] != nullptr){
                    v.push_back({RefTrieptr(-1, u->child[i]),0});
                    if (base[i] != c)
                        v.back().second++;
                        //v[v.size()-1]++;
                }
        }
        return v;
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

struct Nodeptr{
    RefTrieptr p1, p2;
    /// p1 - the left alignment
    /// p2 - the right alignment
    int qpos, cost;
    Nodeptr(){}
    Nodeptr(int _qpos, RefTrieptr _p1, RefTrieptr _p2, int _cost){
        qpos = _qpos;
        p1 = _p1;
        p2 = _p2;
        cost = _cost;
    }
    bool operator<(Nodeptr const &state)const{
        return cost>state.cost;
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
    vector<pair<RefTrieptr, int> > next1, next2;
    ///next1 - states from the left alignment
    ///next2 - states from the right alignment
    while (!q.empty()){
        w = q.top();
        q.pop();
        if (visited.find(w) == visited.end())
            visited.insert(w);
        else{
            assert(cout<<"Previously poped out state\n");
            continue;
        }
        if (w.qpos == n)
            break;
        next1.clear();
        next2.clear();
        next1 = w.p1.getnextstates(left[w.qpos], ref);
        next2 = w.p2.getnextstates(right[w.qpos], ref);
        if (w.p1.u == nullptr && w.p2.u == nullptr){
            if(next1.size() && next2.size()){
                if (next1[0].second + next2[0].second == 0){
                    nb = Nodeptr(w.qpos+1, next1[0].first, next2[0].first, w.cost);
                    q.push(nb);
                    continue;
                }
            }
        }
        if (w.p1.u != nullptr && w.p1.u->num!=-1){
            for (int i = last[w.p1.u->num]; i != -1; i = prevpos[i]){
                nb = Nodeptr(w.qpos, RefTrieptr(i+1, nullptr), w.p2, w.cost);
                q.push(nb);
            }
            if (w.p1.u->is_leaf())
                continue;
            ///if the optimizated build of the trie is used one of the k-mers has lenght less than k
        }

        if (w.p2.u != nullptr && w.p2.u->num!=-1){
            for (int i = last[w.p2.u->num]; i != -1; i = prevpos[i]){
                nb = Nodeptr(w.qpos, w.p1, RefTrieptr(i+1, nullptr), w.cost);
                q.push(nb);
            }
            if (w.p2.u->is_leaf())
                continue;
        }

        nb = Nodeptr (w.qpos+1, w.p1, w.p2, w.cost+2);
        q.push(nb);
        for (auto i:next1){
            for (auto j:next2){
                nb = Nodeptr (w.qpos+1, i.first, j.first, w.cost);
                nb.cost += (i.second + j.second);
                q.push(nb);

                nb = Nodeptr (w.qpos, i.first, j.first, w.cost+2);
                q.push(nb);

                /*nb = Nodeptr (w.qpos, w.p1, j.first, w.cost+1);
                q.push(nb);

                nb = Nodeptr (w.qpos+1, w.p1, j.first, w.cost+1);
                nb.cost += j.second;
                q.push(nb);*/
            }

            nb = Nodeptr (w.qpos, i.first, w.p2, w.cost+1);
            q.push(nb);

            nb = Nodeptr(w.qpos+1, i.first, w.p2, w.cost+1);
            nb.cost += i.second;
            q.push(nb);
        }
        for (auto j:next2){
            nb = Nodeptr (w.qpos, w.p1, j.first, w.cost+1);
                q.push(nb);

                nb = Nodeptr (w.qpos+1, w.p1, j.first, w.cost+1);
                nb.cost += j.second;
                q.push(nb);
        }
    }
    return w.cost;
}
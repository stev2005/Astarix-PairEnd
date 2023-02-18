#pragma once

#include "../header.h"
#include <bits/stdc++.h>
#include "../trie.h"
using namespace std;

struct Nodetr{
    int querypos, refpos, cost;
    Trie *u;
    bool not_in_trei;

    Nodetr(){}
    Nodetr(int _querypos, int _refpos, int _cost){
        querypos=_querypos;
        refpos=_refpos;
        cost=_cost;
        u=nullptr;
        not_in_trei=true;
    }
    Nodetr(int _querypos, Trie *_u, int _cost){
        querypos=_querypos;
        u=_u;
        refpos=-1;
        cost=_cost;
        not_in_trei=false;
    }

    bool operator<(const Nodetr &state)const{
        return cost>state.cost;
    }
};

struct Expanded{
    int querypos, refpos, cost;
    Trie *u;
    //bool not_in_trei;

    Expanded(){}
    Expanded(int _querypos, int _refpos){
        querypos=_querypos;
        refpos=_refpos;
        u=nullptr;
        //not_in_trei=true;
    }
    Expanded(int _querypos, Trie *_u){
        querypos=_querypos;
        u=_u;
        refpos=-1;
        //not_in_trei=false;
    }

    bool operator<(const Expanded &state)const{
        if (querypos == state.querypos){
            if (u == state.u)///both are NULL
                return refpos<state.refpos;
            return u<state.u;
        }
        return querypos < state.querypos;
    }
};

int edit_distance_dijkstratrienew(string &query, string &ref, Trie *T, vector<int>&last, vector<int>&prevpos){
    priority_queue<Nodetr>q;
    set <Expanded> visited;
    Expanded cur_state;
    Nodetr w(0, T, 0), nb;
    q.push(w);
    int n=query.size(), m=ref.size();
    char c1, c2;
    while(!q.empty()){
        w=q.top();
        q.pop();
        if(w.querypos==n)break;
        if(w.u==nullptr){
            cur_state=Expanded(w.querypos,w.refpos);
            if(visited.find(cur_state)==visited.end())
                visited.insert(cur_state);
            else continue;
            if (w.querypos<n&&w.refpos<m){
                c1 = ref[w.refpos] ;
                c2 = query[w.querypos] ;
                nb = Nodetr(w.querypos+1,  w.refpos+1,  w.cost);
                if (c1 ==c2 ){
                    q.push(nb);
                    continue;
                }
                ++nb.cost;
                q.push(nb);
            }
            if (w.refpos<m){
                nb=Nodetr(w.querypos, w.refpos+1, w.cost+1);
                q.push(nb);
            }
            if (w.querypos<n){
                nb=Nodetr(w.querypos+1, w.refpos, w.cost+1);
                q.push(nb);
            }
        }
        else{
            cur_state=Expanded(w.querypos,w.u);
            if(visited.find(cur_state)==visited.end())
                visited.insert(cur_state);
            else continue;
            bool leaf=true;
            for(int i=0; i<4; ++i){
                if(w.u->child[i]==nullptr)
                    continue;
                leaf=false;
                if(w.querypos==0){
                    nb=Nodetr(w.querypos, w.u->child[i], w.cost);
                    q.push(nb);
                    ///moving the start position
                }
                if(base[i]==query[w.querypos]){
                    nb=Nodetr(w.querypos+1, w.u->child[i], w.cost);
                    q.push(nb);
                    //continue;
                }   
                else nb=Nodetr(w.querypos+1, w.u->child[i], w.cost+1);
                q.push(nb);
                nb=Nodetr(w.querypos, w.u->child[i], w.cost+1);
                q.push(nb);
                nb=Nodetr(w.querypos+1, w.u, w.cost+1);
                q.push(nb);
            }
            if(leaf){
                for(int i=last[w.u->num];i!=-1;i=prevpos[i]){
                    nb=Nodetr(w.querypos, i+1, w.cost);
                    q.push(nb);
                }
            }
        }
    }
    return w.cost;
}
#pragma once

#include "header.h"
#include <bits/stdc++.h>
#include "trie.h"
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

int minimum_edit_distance_dijkstra_tri(string &query, string &ref, Trie *T, vector<int>&last, vector<int>&prevpos){
    priority_queue<Nodetr>q;
    Nodetr w(0, T, 0), nb;
    q.push(w);
    int n=query.size(), m=ref.size();
    char c1, c2;
    while(!q.empty()){
        w=q.top();
        q.pop();
        if(w.querypos==n)break;
        if(w.not_in_trei){
            if (w.refpos<m){
                nb=Nodetr(w.querypos, w.refpos+1, w.cost+1);
                q.push(nb);
            }
            if (w.querypos<n&&w.refpos<m){
                c1 = ref[w.refpos] ;
                c2 = query[w.querypos] ;
                nb = Nodetr(w.querypos+1,  w.refpos+1,  w.cost);
                if (c1!=c2) ++nb.cost;
                q.push(nb);
            }
            if (w.querypos<n){
                nb=Nodetr(w.querypos+1, w.refpos, w.cost+1);
                q.push(nb);
            }
        }
        else{
            bool leaf=true;
            for(int i=0;i<4;++i){
                if(w.u->child[i]==nullptr)
                    continue;
                leaf=false;
                if(base[i]==query[w.querypos])
                    nb=Nodetr(w.querypos+1, w.u->child[i], w.cost);
                else nb=Nodetr(w.querypos+1, w.u->child[i], w.cost+1);
                q.push(nb);
                nb=Nodetr(w.querypos, w.u->child[i], w.cost+1);
                q.push(nb);
                nb=Nodetr(w.querypos+1, w.u, w.cost+1);
                q.push(nb);
                if(w.querypos==0){
                    nb=Nodetr(w.querypos, w.u->child[i], w.cost);
                    q.push(nb);
                    ///moving the start position
                }
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
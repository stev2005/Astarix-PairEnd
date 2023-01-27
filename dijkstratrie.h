#pragma once

#include <bits/stdc++.h>
#include "trie.h"
using namespace std;

struct Nodetr{
    int querypos,refpos,cost;
    Trie *u;
    bool flag;
    /// the second dimension to be Trie* (false) or int (true)

    Nodetr(){}
    Nodetr(int _querypos,int _refpos,int _cost){
        querypos=_querypos;
        refpos=_refpos;
        cost=_cost;
        u=nullptr;
        flag=true;
    }
    Nodetr(int _querypos,Trie *_u,int _cost){
        querypos=_querypos;
        u=_u;
        refpos=-1;
        cost=_cost;
        flag=false;
    }

    bool operator<(const Nodetr &state)const{
        return cost>state.cost;
    }
};

struct Expanded{
    int querypos,refpos;
    Trie *u;
    bool flag;

    Expanded(){}
    Expanded(int _querypos,int _refpos){
        querypos=_querypos;
        refpos=_refpos;
        flag=true;
        u=nullptr;
    }
    Expanded(int _querypos,Trie *_u){
        querypos=_querypos;
        u=_u;
        refpos=-1;
        flag=false;
    }

    bool operator<(const Expanded &state)const{
        if(querypos==state.querypos){
            if(flag==state.flag){
                if(flag)
                    return refpos<state.refpos;
                else return u<state.u;
            }
            return flag<state.flag;
        }
        return querypos<state.querypos;
    }

};

inline void init_seeds(string &ref,Trie *&T,vector<int>&last,vector<int>&prevpos){
    int m=ref.size();
    int len=log2(m);
    int sz;
    prevpos.resize(m,-1);
    last.resize(m,-1);
    int cntseed=0,prevcnt;
    string w;
    for(int i=0;i<m;i+=len){
        w=ref.substr(i,len);
        sz=w.size();
        prevcnt=cntseed;
        insert_seed(T,w,0,cntseed);
        prevpos[i+sz-1]=last[cntseed];
        last[cntseed]=i+sz-1;
        if(prevcnt==cntseed)
            ++cntseed;
        else cntseed=prevcnt;
    }
}

inline void print_out_last_prevpos(vector<int>&last,vector<int>&prevpos){
    for(auto cur:last)
        cout<<cur<<" ";
    cout<<"\n";
    for(auto cur:prevpos)
        cout<<cur<<" ";
    cout<<"\n";
}

int minimum_edit_distance_dijkstra_tri(string &query,string &ref,Trie *T,vector<int>&last,vector<int>&prevpos){
    set<Expanded>visited;
    Expanded hstate;
    priority_queue<Nodetr>q;
    Nodetr w(0,T,0),nb;
    q.push(w);
    int n=query.size(),m=ref.size();
    char base[4]={'A','C','G','T'},c1,c2;
    while(!q.empty()){
        w=q.top();
        q.pop();
        if(w.querypos==n)break;
        if(w.flag){
            hstate=Expanded(w.querypos,w.refpos);
            if(visited.find(hstate)==visited.end())
                visited.insert(hstate);
            else continue;
            if (w.refpos<m){
                nb=Nodetr(w.querypos,w.refpos+1,w.cost+1);
                q.push(nb);
            }
            if (w.querypos<n&&w.refpos<m){
                c1 = ref [ w.refpos ] ;
                c2 = query [ w.querypos ] ;
                nb=Nodetr(w.querypos+1,w.refpos+1,w.cost);
                if(c1!=c2)++nb.cost;
                q.push(nb);
            }
            if (w.querypos<n){
                nb=Nodetr(w.querypos+1,w.refpos,w.cost+1);
                q.push(nb);
            }
        }
        else{
            hstate=Expanded(w.querypos,w.u);
            if(visited.find(hstate)==visited.end())
                visited.insert(hstate);
            else continue;
            bool leaf=true;
            for(int i=0;i<4;++i){
                if(w.u->child[i]==nullptr)
                    continue;
                leaf=false;
                if(base[i]==query[w.querypos])
                    nb=Nodetr(w.querypos+1,w.u->child[i],w.cost);
                else nb=Nodetr(w.querypos+1,w.u->child[i],w.cost+1);
                q.push(nb);
                nb=Nodetr(w.querypos,w.u->child[i],w.cost+1);
                q.push(nb);
                nb=Nodetr(w.querypos+1,w.u,w.cost+1);
                q.push(nb);
                if(w.querypos==0){
                    nb=Nodetr(w.querypos,w.u->child[i],w.cost);
                    q.push(nb);
                    ///moving the start position
                }
            }
            if(leaf){
                for(int i=last[w.u->num];i!=-1;i=prevpos[i]){
                    nb=Nodetr(w.querypos,i+1,w.cost);
                    q.push(nb);
                }
            }
        }
    }
    cout << "Expanded states: "<<visited.size()<<"\n";
    return w.cost;
}
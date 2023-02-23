#pragma once

#include "../header.h"
#include<bits/stdc++.h>
using namespace std;

struct Nodep{
    int qpos, lpos, rpos, cost;

    Nodep(){
        lpos= -1;
        rpos= -1;
        qpos= -1;
        cost= -1;
    }

    Nodep(int _qpos, int _lpos, int _rpos, int _cost){
        qpos= _qpos;
        lpos= _lpos;
        rpos= _rpos;
        cost= _cost;
    }

    bool operator<(const Nodep &state)const{
        return cost > state.cost;
    }
};

struct Expanded{
    int qpos, lpos, rpos;
    
    Expanded(){
        qpos= -1;
        lpos= -1;
        rpos= -1;
    }

    Expanded(int _qpos, int _lpos, int _rpos){
        qpos= _qpos;
        lpos= _lpos;
        rpos= _rpos;
    }

    bool operator<(const Expanded &state)const{
        if(qpos == state.qpos){
            if(lpos == state.lpos)
                return rpos<state.rpos;
            return lpos<state.lpos;
        }
        return qpos<state.qpos;
    }
};

int edit_distance_pairedend(pair< string, string> &query, string &ref){
    string left = query.first;
    string right = query.second;
    priority_queue <Nodep> q;
    set <Expanded> visited;
    Nodep w, nb;
    Expanded state;
    int n= left.size();
    int m= ref.size();
    for (int i=0;i<=m;++i)
        for (int j=0;j<=m;++j)
            q.push(Nodep(0,i,j,0));
    while (!q.empty()){
        w= q.top();
        q.pop();
        state= Expanded(w.qpos,w.lpos,w.rpos);
        if(visited.find(state) == visited.end())
            visited.insert(state);
        else continue;
        if(w.qpos == n)
            break;
        ///first case
        if(w.lpos<m && w.rpos<m){
            nb=Nodep(w.qpos+1, w.lpos+1, w.rpos+1, w.cost);
            if(left[w.qpos] != ref[w.lpos])
                ++nb.cost;
            if(right[w.qpos] != ref[w.rpos])
                ++nb.cost;
            q.push(nb);
            if(w.cost == nb.cost)
                continue;
        }
        ///second case
        nb= Nodep(w.qpos+1, w.lpos, w.rpos, w.cost+2);
        q.push(nb);
        if(w.rpos<m){
            ///third case
            nb= Nodep(w.qpos, w.lpos, w.rpos+1, w.cost+1);
            q.push(nb);
            ///fifth case
            nb= Nodep(w.qpos+1, w.lpos, w.rpos+1, w.cost+1);
            if(right[w.qpos] != ref[w.rpos])
                ++nb.cost;
            q.push(nb);
        }
        if(w.lpos<m){
            ///fourth case
            nb= Nodep(w.qpos, w.lpos+1, w.rpos, w.cost+1);
            q.push(nb);
            ///sixth case
            nb= Nodep(w.qpos+1, w.lpos+1, w.rpos, w.cost+1);
            if(left[w.qpos] != ref[w.lpos])
                ++nb.cost;
            q.push(nb);
        }
        if(w.lpos<m && w.rpos<m){
            nb= Nodep(w.qpos, w.lpos+1, w.rpos+1, w.cost+2);
            q.push(nb);
        }
    }
    //cout<<"end of the dijkstra\n";
    //cout<<w.qpos<<" "<<w.lpos<<" "<<w.rpos<<" "<<w.cost<<endl;
    //cout<<"Expanded states == "<<visited.size()<<" "; 
    return w.cost;
}
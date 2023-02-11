#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include <bits/stdc++.h>
//#include "DNA.h"
using namespace std;

struct Node{
    int querypos, refpos, cost;
    Node(){}
    Node(int _querypos, int _refpos, int _cost){
        querypos=_querypos;
        refpos=_refpos;
        cost=_cost;
    }
    void output (){
        cout << "querypos==" << querypos << " refpos==" << refpos << " cost==" << cost << "\n" ;
    }
    bool operator< (const Node& cell) const{
        return cost>cell.cost;
    }
};

int minimum_edit_distance_dijkstra(string &query,string &Ref){
    set < pair < int , int > > visited ;
    //set < pair < int , int > > expanded;
    int n, m;
    n=query.size();
    m=Ref.size();
    priority_queue<Node>q;
    for (int i=0; i<=m; ++i){
        q.push(Node(0,i,0));
        //expanded.insert({0,i});
    }
    Node w,nb;
    pair < int , int > curcell;
    char c1,c2;
    while (!q.empty()){
        w=q.top();
        q.pop();
        curcell = { w.querypos , w . refpos } ;
        if ( visited . find ( curcell ) == visited . end () )
            visited . insert ( curcell ) ;
        else continue;
        if (w.querypos==n)break;
        if (w.refpos<m){
            nb=Node(w.querypos,w.refpos+1,w.cost+1);
            q.push(nb);
            //expanded.insert({nb.querypos,nb.refpos});
        }
        if (w.querypos<n&&w.refpos<m){
            c1 = Ref [ w.refpos ] ;
            c2 = query [ w.querypos ] ;
            nb=Node(w.querypos+1,w.refpos+1,w.cost);
            if(c1!=c2)++nb.cost;
            q.push(nb);
            //expanded.insert({nb.querypos,nb.refpos});
        }
        if (w.querypos<n){
            nb=Node(w.querypos+1,w.refpos,w.cost+1);
            q.push(nb);
            //expanded.insert({nb.querypos,nb.refpos});
        }
    }
    while ( ! q . empty () )
        q . pop () ;
    //cout << "Explored states dijkstra: " << visited.size() << "\n" ;
    return w.cost;
}

#endif // DIJKSTRA_h
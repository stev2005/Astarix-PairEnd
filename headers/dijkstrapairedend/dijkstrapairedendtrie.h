#pragma once

#include "../header.h"
#include "../trie.h"
#include <bits/stdc++.h>

using namespace std;

struct  Node{
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

    bool operator!=(const Node &p)const{
        return (rpos != p.rpos || u != p.u)?true:false;
    }

};

struct NodeWithCost{
    Node node;///pointer to a vertex of the graph
    cost_t edit_cost;///the additional edit_cost
    int qpos;///pointer to the query

    NodeWithCost(){}

    NodeWithCost(int _qpos, Node _node, cost_t _edit_cost){
        qpos = _qpos;
        node = _node;
        edit_cost = _edit_cost;
    }

    void print(){
        cout<<node.rpos<<" "<<node.u<<" "<<edit_cost<<" "<<qpos<<"\n";
    }

};

struct State{
    Node p1;///node of the left alignment
    Node p2;///node of the right alignment
    int qpos;
    cost_t cost;
    State(){}
    State(int _qpos, Node _p1, Node _p2, cost_t _cost){
        qpos = _qpos;
        p1 = _p1;
        p2 = _p2;
        cost = _cost;
    }
    
    bool operator<(State const &state)const{
        return cost > state.cost;
    }

    void print(){
        cout<<"State: qpos== "<<qpos<<" p1.u== "<<p1.u<<" p1.rpos== "<<p1.rpos<<" p2.u == "<< p2.u<<" p2.rpos== "<<p2.rpos<<" cost=="<<cost<<endl;
    }

};

vector <NodeWithCost> getNextNodeWithCost(char c,string &ref, Node ver, int qpos, vector<int> &last, vector<int> &prevpos){
    ///c - value of query[qpos] (no matter wheather left or right one)
    ///ref - the reference 
    ///ver - vertex of the graph
    ///qpos - pointer to the query
    ///last and prevpos: help arrays
    vector <NodeWithCost> inheritors;
    inheritors.push_back(NodeWithCost(qpos, ver, 0));
    inheritors.push_back(NodeWithCost(qpos+1, ver, 1));///insertion
    if (ver.is_in_trie()){
        if (ver.u->num != -1)
            for (int i = last[ver.u->num]; i != -1; i = prevpos[i])
                inheritors.push_back(NodeWithCost(qpos, Node(i+1), 0));
        for (int i = 0; i < 4; ++i)
            if (ver.u->child[i] != nullptr){
                inheritors.push_back(NodeWithCost(qpos, Node(ver.u->child[i]), 1));///deletion
                inheritors.push_back(NodeWithCost(qpos+1, Node(ver.u->child[i]), 0));///match/substitution
                if (base[i] != c) inheritors.back().edit_cost++;
            }
    }
    else if (ver.rpos < ref.size()){
        inheritors.push_back(NodeWithCost(qpos, Node(ver.rpos+1), 1));///deletion
        inheritors.push_back(NodeWithCost(qpos+1, Node(ver.rpos+1), 0));///match/substitution
        if (ref[ver.rpos] != c) inheritors.back().edit_cost++;
    }
    return inheritors;
}

auto setStatecmp = [](State c, State d){
    if (c.qpos != d.qpos) return c.qpos < d.qpos;
    if (!(c.p1 == d.p1)) return c.p1 < d.p1;
    return c.p2 < d.p2;
};

int edit_distance_dijkstrapairedend_trie(pair<string, string> &query, string &ref, Trie *root, vector<int>&last, vector<int>&prevpos){
    ///root - root of the trie tree
    ///last and prevpos: help arrays
    string q1 = query.first;///left query alignment
    string q2 = query.second;///right query alignment
    int n = q1.size();
    int m = ref.size();
    priority_queue<State> q;
    set <State, decltype(setStatecmp)> visited(setStatecmp);
    State cur(0, Node(root), Node(root), 0);
    q.push(cur);
    //set<int>reachedqpos;
    while (!q.empty()){
        cur = q.top();
        q.pop();
        //cur.print();
        if (visited.find(cur) == visited.end()){
            visited.insert(cur);
            if (cur.qpos == n)
            break;
            if (!cur.p1.is_in_trie() && !cur.p2.is_in_trie()){
                if (cur.p1.rpos < m && cur.p2.rpos < m){
                    if (q1[cur.qpos] == ref[cur.p1.rpos] &&
                        q2[cur.qpos] == ref[cur.p2.rpos]){
                            q.push(State(cur.qpos+1, Node(cur.p1.rpos+1), Node(cur.p2.rpos+1), cur.cost));
                            continue;
                        }
                }
            }
            vector<NodeWithCost>next1;///following nodes from the left alignment node
            vector<NodeWithCost>next2;///following nodes from the right alignment node
            next1 = getNextNodeWithCost(q1[cur.qpos], ref, cur.p1, cur.qpos, last, prevpos);
            //assert(cout<<"got the left alignment nodes\n");
            next2 = getNextNodeWithCost(q2[cur.qpos], ref, cur.p2, cur.qpos, last, prevpos);
            //assert(cout<<"got the right alignment nodes\n");
            for (auto i1:next1)
                for (auto i2:next2)
                    if (i1.qpos == i2.qpos)
                        q.push(State(i1.qpos, i1.node, i2.node, cur.cost+i1.edit_cost+i2.edit_cost));
        }
    }
    return cur.cost;
}
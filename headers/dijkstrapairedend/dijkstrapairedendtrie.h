#pragma once

#include "../header.h"
#include "../trie.h"
#include <bits/stdc++.h>

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

    void print(){
        cout<<"        is_in_trie() == "<<is_in_trie()<<"\n";
        if (is_in_trie())
            cout<<"         u->bp == "<<u->bp<<"\n";
        else cout<<"           rpos == "<<rpos<<"\n";
    }

};

struct NodeWithCost{
    int qpos;///pointer to the query
    Node node;///pointer to a vertex of the graph
    cost_t edit_cost;///the additional edit_cost
    
    NodeWithCost(){}

    NodeWithCost(int _qpos, Node _node, cost_t _edit_cost){
        qpos = _qpos;
        node = _node;
        edit_cost = _edit_cost;
    }

    void print(){
        cout<<"This NodeWithCost is: qpos == "<<qpos<<"\n";
        cout<<"Node:\n";
        node.print();
        cout<<"edit_cost == "<<edit_cost<<"\n";
    }

};

struct State{
    int qpos;///position of the query
    Node p1;///node of the left alignment
    Node p2;///node of the right alignment
    cost_t cost;///edit_distance of the alignment

    State(){}
    State(int _qpos, Node _p1, Node _p2,cost_t _cost){
        qpos = _qpos;
        p1 = _p1;
        p2 = _p2;
        cost = _cost;
    }

    bool operator<(const State &state)const{
        return cost>state.cost;
    }

    void print(){
        cout<<"The current state:\n";
        cout<<"    qpos == "<<qpos<<"\n";
        cout<<"    p1:\n";
        p1.print();
        cout<<"    p2:\n";
        p2.print();
        cout<<"    cost == "<<cost<<"\n";
    }
};

auto setStatecmp = [](State c, State d){
    if (c.qpos != d.qpos) return c.qpos < d.qpos;
    if (!(c.p1 == d.p1)) return c.p1 < d.p1;
    return c.p2 < d.p2;
};


vector <NodeWithCost> getNextNodeWithCost(char c,string &ref, Node ver, int qpos/*, cost_t cost*/, vector<int> &last, vector<int> &prevpos){
    ///c - value of query[qpos] (no matter wheather left or right one)
    ///ref - the reference 
    ///ver - vertex of the graph
    ///qpos - pointer to the query
    ///last and prevpos: help arrays
    vector <NodeWithCost> inheritors;
    inheritors.push_back(NodeWithCost(qpos, ver, 0));
    inheritors.push_back(NodeWithCost(qpos+1, ver, 1));///insertion
    if (ver.is_in_trie()){
        assert(cout<<"it is in trie\n");
        /*if (qpos == 0 && cost == 0){///moving the start position
            for (int i = 0; i < 4; ++i)
                if(ver.u->child[i] != nullptr)
                	inheritors.push_back(NodeWithCost(qpos, Node(ver.u->child[i]), 0));
        }*/
        if (ver.u->num != -1){
            for (int i = last[ver.u->num]; i !=-1 ; i = prevpos[i])
                inheritors.push_back(NodeWithCost(qpos, Node(i+1), 0));
        }
        for (int i = 0; i < 4; ++i)
            if (ver.u->child[i] != nullptr){
                inheritors.push_back(NodeWithCost(qpos, Node(ver.u->child[i]), 1));
                /*inheritors.push_back(NodeWithCost(qpos+1, Node(ver.u->child[i]), 0));
                if (base[i] != c)
                    inheritors.back().edit_cost++;*/
                if (base[i] == c) inheritors.push_back(NodeWithCost(qpos+1, Node(ver.u->child[i]), 0));
                else inheritors.push_back(NodeWithCost(qpos+1, Node(ver.u->child[i]), 1));
            }
    }
    else if (ver.rpos < ref.size()){
        inheritors.push_back(NodeWithCost(qpos, Node(ver.rpos+1), 1));///deletion
        /*inheritors.push_back(NodeWithCost(qpos+1, Node(ver.rpos+1), 0));
        if (ref[ver.rpos] != c)
            inheritors.back().edit_cost++;*/
        ///mathc/substitution
        if ( ref[ver.rpos] == c) inheritors.push_back(NodeWithCost(qpos+1, Node(ver.rpos+1), 0));
        else inheritors.push_back(NodeWithCost(qpos+1, Node(ver.rpos+1), 1));
    }
    return inheritors;
}

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
    /*for (int i=0;i<=m;++i)
        for (int j=0;j<=m;++j)
            q.push(State(0,Node(i),Node(j),0));*/
    while (!q.empty()){
        //assert(cout<<"queue size == "<<q.size()<<" ");
        cur = q.top();
        //assert(cout<<"cur.cost=="<<cur.cost<<"\n");
        q.pop();
        static int cntshowtimestates=0;
        /*if(cur.qpos>8 && cntshowtimestates<10000){
            cur.print();
            cntshowtimestates++;
        }*/
        if (cur.qpos == n)
            break;
        if (visited.find(cur) == visited.end()){
            visited.insert(cur);
            if (!cur.p1.is_in_trie() && !cur.p2.is_in_trie()){
                if (cur.p1.rpos < m && cur.p2.rpos < m){
                    if (q1[cur.qpos] == ref[cur.p1.rpos]&&
                        q2[cur.qpos] == ref[cur.p2.rpos]){
                            q.push(State(cur.qpos+1, Node(cur.p1.rpos+1), Node(cur.p2.rpos+1), cur.cost));
                            continue;
                        }
                }
            }
            vector <NodeWithCost> next1;///following nodes from the left alignment node
            vector <NodeWithCost> next2;///following nodes from the right alignment node
            next1 = getNextNodeWithCost(q1[cur.qpos], ref, cur.p1, cur.qpos/*, cur.cost*/, last, prevpos);
            next2 = getNextNodeWithCost(q2[cur.qpos], ref, cur.p2, cur.qpos/*, cur.cost*/, last, prevpos);
            static int showcnt=0;
            /*if (showcnt < 1){
                cout<<"left nide:\n";
                cur.p1.print();
                for (auto i1:next1){
                    i1.print();
                    cout<<"\n";
                }
                cout<<"right node:\n";
                cur.p1.print();
                for (auto i2:next2){
                    i2.print();
                    cout<<"\n";
                }
                showcnt++;
            }*/
            for (auto i1:next1)
                for (auto i2:next2)
                    if (i1.qpos == i2.qpos)
                        q.push(State(i1.qpos, i1.node, i2.node, cur.cost + i1.edit_cost + i2.edit_cost));
        }
    }
    //assert(cout<<"queusize==0\n");
    //cout<<"Expanded states == "<<visited.size()<<" ";
    return cur.cost;
}




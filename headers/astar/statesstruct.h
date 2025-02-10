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

typedef map<Node, bitset<64> > crumbs_t;

struct MatchingKmers{///fast and convinient way to pass a lot of data structures as parameters to functions
    vector <int> seeds, seeds1, seeds2;
    /*seeds: does (seed[i]>=0) or doesn't(seed[i]==-1) the ith seed match a kmer;
    1 for the first read, 2 for the second read*/
    vector <int> nseeds, nseeds1, nseeds2;//nseeds: seeds but from the negative strand
    vector <int> last;///last: the end position of a last occurance of a dmer in the reference
    vector <int> prevpos;///prevpos: end positions of previous occurances of a dmer in the reference
    vector <Trie*> backtotrieconnection;///backtotrieconnection: pointer to trie leaf from which a given bp of the ref is accessed (for dmers)
    vector <int> lastkmer;///same definition as last but for kmers instead of dmers
    vector <int> prevposkmer; ///same definition as prevpos but for kmers instead of dmers
    vector <Trie*> backtotrieconnectionkmer;///same definition as backtotrieconnection but for kmers instead of dmers
    crumbs_t crumbs, crumbs1, crumbs2;
    crumbs_t ncrumbs, ncrumbs1, ncrumbs2;//negative crumbs
    void clearquerydata(){
        seeds.clear();
        nseeds.clear();
        seeds1.clear();
        seeds2.clear();
        nseeds1.clear();
        nseeds2.clear();
        crumbs.clear();
        ncrumbs.clear();
        crumbs1.clear();
        crumbs2.clear();
        ncrumbs1.clear();
        ncrumbs2.clear();
    }

    bool operator==(const MatchingKmers &other)const{
        if (seeds != other.seeds) return false;
        if (nseeds != other.nseeds) return false;
        if (seeds1 != other.seeds1) return false;
        if (seeds2 != other.seeds2) return false;
        if (crumbs != other.crumbs) return false;
        if (ncrumbs != other.ncrumbs) return false;
        if (crumbs1 != other.crumbs1) return false;
        if (crumbs2 != other.crumbs2) return false;
        if (lastkmer != other.lastkmer) return false;
        if (prevposkmer != other.prevposkmer) return false;
        if (backtotrieconnectionkmer != other.backtotrieconnectionkmer) return false;
        if (last != other.last) return false;
        if (prevpos != other.prevpos) return false;
        return true;
    }
};

struct  Statesr{
    ///State single read
    Node p;
    cost_t g;///edit distance of alignment of using qpos and p
    cost_t h;///value of heuristic function of using qpos and p
    int qpos;
    bool negative;//is the state for the positive or negative strand
    Statesr(){}
    Statesr(int _qpos, Node _p){
        qpos = _qpos;
        p = _p;
        g = 0;
        h = 0;
        negative = false;
    }
    Statesr(int _qpos, Node _p, cost_t _g, cost_t _h){
        qpos = _qpos;
        p = _p;
        g = _g;
        h = _h;
        negative = false;
    }
    bool operator<(const Statesr &other)const{
        return g + h > other.g + other.h;
    }
    void print(){
        cout << "Statesr: "<< qpos<< " "; 
        if (p.is_in_trie())
            cout << p.u ;
        else cout << p.rpos;
        cout<< " " << g << " " << h << endl;
    }
};

struct Statepr{
    int qpos;
    Node p1;
    Node p2;
    cost_t g;
    cost_t h;
    bool negative;
    Statepr(){}
    Statepr(int _qpos, Node _p1, Node _p2): qpos(_qpos), p1(_p1), p2(_p2), g(0), h(0), negative(false){}
    Statepr(int _qpos, Node _p1, Node _p2, cost_t _g, cost_t _h, bool _negative = false):
            qpos(_qpos), p1(_p1), p2(_p2), g(_g), h(_h), negative(_negative){}
    bool operator<(const Statepr &other) const{//only for priority_queue
        return g + h >  other.g + other.h;
    }
};

struct Statepr4D{
    int qpos1;
    int qpos2;
    Node p1;
    Node p2;
    cost_t g;
    cost_t h;
    bool negative;
    Statepr4D(){}
    Statepr4D(int _qpos1, int _qpos2, Node _p1, Node _p2): qpos1(_qpos1), qpos2(_qpos2), p1(_p1), p2(_p2), g(0), h(0), negative(false){}
    Statepr4D(int _qpos1, int _qpos2, Node _p1, Node _p2, cost_t _g, cost_t _h, bool _negative = false):
            qpos1(_qpos1), qpos2(_qpos2), p1(_p1), p2(_p2), g(_g), h(_h), negative(_negative){}
    bool operator<(const Statepr4D &other) const{//only for priority_queue
        return g + h >  other.g + other.h;
    }
};

class BucketQueueSE{
    private:
        unordered_map<cost_t, queue<Statesr> > prq;///the priority-bucket queue;
        using Comparator = std::function<bool(cost_t, cost_t)>;
        Comparator cmpprior = [] (cost_t a, cost_t b){return a > b;};
        priority_queue<cost_t, vector<cost_t>, Comparator> priorities{cmpprior};
        int size;
    public:
        BucketQueueSE(){
            size = 0;
        }

        ~BucketQueueSE(){
            prq.clear();
            //priorities.clear();
            while (!priorities.empty())
                priorities.pop();
        }

        void Push(Statesr cur){
            cost_t pri = cur.g + cur.h;
            if (prq.find(pri) == prq.end())
                priorities.push(pri);
            prq[pri].push(cur);
            size++;
        }
        
        bool Empty(){
            return priorities.empty();
        }
        
        Statesr Top(){
            if (priorities.empty())
                assert(false);
            cost_t pri = priorities.top();
            Statesr ver = prq[pri].front();
            return ver;
        }

        void Pop(){
            if (priorities.empty())
                assert(false);
            cost_t pri = priorities.top();
            prq[pri].pop();
            if (prq[pri].empty()){
                auto it = prq.find(pri);
                prq.erase(it);
                priorities.pop();
            }
            size--;
        }

        int Size(){
            return size;
        }
};

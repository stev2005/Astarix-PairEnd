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

struct MatchingKmers{
    vector <int> seeds, last, prevpos;
    vector <Trie*> connection;
    int qsize;
    map<Node, bitset<64> > crumbs;
    ///last: the end position of a last occurance of a kmer in the reference
    ///prevpos: end positions of previous occurances of a kmer in the reference
    ///seeds: does (seed[i]>=0) or doesn't(seed[i]==-1) the ith seed match a kmer
    /*seeds: with which kmere a given seed has a mathc.
    if seeds[i] == -1 there isn't such a kmer, otherwise seeds[i] = to the corresponding kmer*/
    ///connection: pointer to trie leaf from which a given bp of the ref is accessed  
};

struct  Statesr{
    ///State single read
    int qpos;
    Node p;
    cost_t g;///edit distance of alignment of using qpos and p
    cost_t h;///value of heuristic function of using qpos and p
    cost_t stepcost;///what to add to the already achived cost of the previous of state <qpos, p>
    Statesr(){}
    Statesr(int _qpos, Node _p){
        qpos = _qpos;
        p = _p;
        g = 0;
        h = 0;
        stepcost = 0;
    }
    Statesr(int _qpos, Node _p, cost_t _stepcost){
        qpos = _qpos;
        p = _p;
        g = 0;
        h = 0;
        stepcost = _stepcost;
    }
    Statesr(int _qpos, Node _p, cost_t _g, cost_t _h){
        qpos = _qpos;
        p = _p;
        g = _g;
        h = _h;
        stepcost = 0;
    }
    Statesr(int _qpos, Node _p, cost_t _g, cost_t _h, cost_t _stepcost){
        qpos = _qpos;
        p = _p;
        g = _g;
        h = _h;
        stepcost = _stepcost;
    }
    bool operator<(const Statesr &other)const{
        return g + h > other.g + other.h;
    }
    bool print(){
        cout << "Statesr: "<< qpos<< " "; 
        if (p.is_in_trie())
            cout << p.u ;
        else cout << p.rpos;
        cout<< " " << g << " " << h << endl;
        return true;
    }
};

struct explstatesr{
    int qpos;
    Node p;
    explstatesr(){}
    explstatesr(Statesr cur){
        qpos = cur.qpos;
        p = cur.p;
    }
    bool operator<(const explstatesr &other)const{
        if (qpos != other.qpos) return qpos < other.qpos;
        return p < other.p;
    }
};

map <Node, bitset<64> > getcrumbs(const string &ref, int k, const MatchingKmers &info){
    map <Node, bitset<64> >crumbs;
    vector <int> seeds = info.seeds;
    vector <int> last = info.last;
    vector <int> prevpos = info.prevpos;
    vector <Trie*> connection = info.connection;
    cout << "checking if (seeds.size() <= 64) : ";
    if (seeds.size() <= 64)
        cout << "yes\n";
    else cout << "no\n";
    assert(seeds.size() <= 64);
    for (int i = 0; i < seeds.size(); ++i){
        if (seeds[i] >= 0){
            int qpos = i * k ;///the start position of the current seed
            for (int j = last[seeds[i]]; j != -1; prevpos[j]){
                for (int x = 0; x < qpos; ++x){
                    int st = j - k + 1; ///start of the kmer matched with the current seed
                    int rpos = st - x; ///position where to add crumb;
                    crumbs[Node(rpos)] |= (1 << i);
                    Trie *cur = connection[rpos];
                    while (cur != nullptr){
                        if (crumbs[Node(cur)][i])
                            break;
                        crumbs[Node(cur)] |= (1 << i);
                        cur = cur->parent;
                    }
                }
            }
        }
    }
    return crumbs;
}

cost_t seed_heuristic(Statesr cur, int k, MatchingKmers &info){
    int remaining_seeds = info.seeds.size() - ceil((double)cur.qpos/k);
    if (remaining_seeds < 0) return 0; ///almost nothing more to align, only bps' count < k
    int rez = remaining_seeds - info.crumbs[cur.p].count();
    if (rez < 0) rez = 0;
    return rez;
    /*int misses = info.seeds.size() - info.crumbs[cur.p].count();
    int rez = info.qsize - cur.qpos + misses;
    if (rez < 0) rez = 0;
    return rez;*/
}

cost_t heuristic(Statesr cur, int k, MatchingKmers &info, char *heuristic_method){
    if (strcmp(heuristic_method, "dijkstra_heuristic") == 0)return 0;
    if (strcmp(heuristic_method, "seed_heuristic") == 0) return seed_heuristic(cur, k, info);
}

Statesr CreateStatesr(Statesr cur, int k, MatchingKmers &info, char *heuristic_method, cost_t stepcost){
    return Statesr(cur.qpos, cur.p, 0, heuristic(cur, k, info, heuristic_method), stepcost);
}

vector <Statesr> NextStatesr(Statesr cur, char curqbp, const string &ref, int k, MatchingKmers &info, char *heuristic_method){
    vector <Statesr> next;
    if (cur.p.is_in_trie()){
        if (cur.p.u->is_leaf()){
            vector <int> last = info.last;
            vector <int> prevpos = info.prevpos;
            for (int i = last[cur.p.u->num]; i != -1; i = prevpos[i]){
                next.push_back(CreateStatesr(Statesr(cur.qpos, Node(i+1)), k, info, heuristic_method, 0));
                //next.back().g--;
            }
        }
        else{
            next.push_back(CreateStatesr(Statesr(cur.qpos+1, cur.p), k, info, heuristic_method, 1));
            for (int i = 0; i < 4; ++i)
                if (cur.p.u->child[i] != nullptr){
                    next.push_back(CreateStatesr(Statesr(cur.qpos+1, Node(cur.p.u->child[i])), k, info, heuristic_method, 1));
                    if (base[i] == curqbp)
                        next.back().stepcost--;
                    next.push_back(CreateStatesr(Statesr(cur.qpos, Node(cur.p.u->child[i])), k, info, heuristic_method, 1));
                }
        }
    }
    else{
        if (cur.p.rpos < ref.size() && ref[cur.p.rpos] == curqbp){
            next.push_back(CreateStatesr(Statesr(cur.qpos+1, Node(cur.p.rpos+1)), k, info, heuristic_method, 0));
            //next.back().g--;
        }
        else{
            if (cur.p.rpos < ref.size()){
                next.push_back(CreateStatesr(Statesr(cur.qpos+1, Node(cur.p.rpos+1)), k, info, heuristic_method, 1));
                next.push_back(CreateStatesr(Statesr(cur.qpos, Node(cur.p.rpos+1)), k, info, heuristic_method, 1));
            }
            next.push_back(CreateStatesr(Statesr(cur.qpos+1, cur.p), k, info, heuristic_method, 1));
        }
    }
    return next;
}

/*auto setStatesrCmp = [](Statesr c, Statesr d){
    if (c.qpos != d.qpos)return c.qpos < d.qpos;
    return c.p < d.p;
};*/

cost_t astar_single_read_alignment(string &query, string &ref, int k, Trie *root, MatchingKmers &info, char *heuristic_method, char *showcntexplstates, char *triestart){
    int n = query.size();
    int m = ref.size();
    priority_queue<Statesr> q;
    set<explstatesr>visited;
    //set<Statesr, decltype(setStatesrCmp)> visited;
    Statesr cur;
    if (strcmp(triestart, "Yes") == 0){
        cur = CreateStatesr(Statesr(0, Node(root)), k, info, heuristic_method, 0);
        //cur.g--;
        q.push(cur);
        for (int i = m - k + 1; i <= m; ++i){
            cur = CreateStatesr(Statesr(0, Node(i)), k, info, heuristic_method, 0);
            //cur.g--;
            q.push(cur);
        }
    }
    else{
        for (int i = 0; i <= m; ++i){
            cur = CreateStatesr(Statesr(0, Node(i)), k, info, heuristic_method, 0);
            //cur.g--;
            q.push(cur);
        }
    }
    while(!q.empty()){
        cur = q.top();
        q.pop();
        //assert(cur.print());
        if (cur.qpos == n)
            break;
        if (visited.find(explstatesr(cur)) == visited.end()){
            visited.insert(explstatesr(cur));
        /*if (visited.find(cur) == visited.end()){
            visited.insert(cur);*/
            vector <Statesr> next = NextStatesr(cur, query[cur.qpos], ref, k, info, heuristic_method);
            //assert(cout << "size of next == "<< next.size() << endl);
            for (auto i:next){
                //assert(cout<<"Inheritor: ");
                //assert(i.print());
                i.g = cur.g + i.stepcost;
                q.push(i);
            }
        }
    }
    if (strcmp(showcntexplstates, "Yes") == 0)
        cout << "Explored states == " << visited.size() << " ";
    return cur.g;
}
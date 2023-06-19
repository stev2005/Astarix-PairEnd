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
    vector <int> seeds1, seeds2;
    /*seeds: does (seed[i]>=0) or doesn't(seed[i]==-1) the ith seed match a kmer;
    1 for the first alignment, 2 for the second alignment*/
    vector <int> last;///last: the end position of a last occurance of a dmer in the reference
    vector <int> prevpos;///prevpos: end positions of previous occurances of a dmer in the reference
    vector <Trie*> backtotrieconnection;///backtotrieconnection: pointer to trie leaf from which a given bp of the ref is accessed (for dmers)
    vector <int> lastkmer;///same definition as last but for kmers instead of dmers
    vector <int> prevposkmer; ///same definition as prevpos but for kmers instead of dmers
    vector <Trie*> backtotrieconnectionkmer;///same definition as backtotrieconnection but for kmers instead of dmers
    map<Node, bitset<64> > crumbs1, crumbs2;
    vector<unordered_set<int> > seedsph1;
    vector<unordered_set<int> > seedsph2;
    void clearquerydata(){
        seeds1.clear();
        seeds2.clear();
        crumbs1.clear();
        crumbs2.clear();
        seedsph1.clear();
        seedsph2.clear();
    }

    
    
    /*seeds: with which kmere a given seed has a mathc.
    if seeds[i] == -1 there isn't such a kmer, otherwise seeds[i] = to the corresponding kmer*/
    
    /*seedsph[i]: is there such a pair <l, r> so that |r-l|<=10000 where l is one of the possible occurance possitions of
    seed[i] of the first alignment and r is one of the possible occurance positions of seed[i] of the second alignment*/
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

bool is_available_to_crumb(int alignment, MatchingKmers &info, int num, int pos){
    if (alignment == 0)
        return true;
    else if (alignment == 1){
        if (info.seedsph1[num].find(pos) != info.seedsph1[num].end())
            return true;
        else return false;
    }
    else if (alignment == 2){
        if (info.seedsph2[num].find(pos) != info.seedsph2[num].end())
            return true;
        else return false;
    }
    assert(false);
}

void /*map <Node, bitset<64> >*/ getcrumbs(const string &ref, int k, MatchingKmers &info, int alignment){
    ///alignment: 0 for single reads; 1 or 2 for pair-ends
    map <Node, bitset<64> > & crumbs = (alignment == 0 || alignment == 1)?info.crumbs1: info.crumbs2;
    const vector<int> & seeds = (alignment == 0 || alignment == 1)? info.seeds1 : info.seeds2;
    const vector<unordered_set<int> > & seedsph = (alignment == 0 || alignment == 1)? info.seedsph1 : info.seedsph2;
    const vector<int> & last = info.last;
    const vector<int> & prevpos = info.prevpos;
    const vector<Trie*> & backtotrieconnection = info.backtotrieconnection;
    //cout << "seeds.size == " <<seeds.size()<<endl;
    //cout << "last.size == "<<last.size()<<endl;
    queue<tuple<Trie*, int, int>> q;
    for (int i = 0; i < seeds.size(); ++i){
        //cout << "i == "<< i << " " << seeds[i] << endl;
        if (seeds[i]>=0){
            //cout <<"A match\n";
            int seedpos = i * k;///start of the seed
            for (int j = last[seeds[i]]; j != -1; j = prevpos[j]){
                if (is_available_to_crumb(alignment, info, i, j)){
                    for (int back = 0; back < seedpos; ++back){
                        int rpos = j - k - back;
                        if (rpos >= 0){
                            crumbs[Node(rpos)][i] = true;
                            int climb = seedpos - back;
                            Trie *cur = backtotrieconnection[rpos];
                            tuple<Trie*, int, int> topushq = make_tuple(cur, climb, i);
                            q.push(topushq);
                        }
                    }
                }
            }      
        }
    }
    while (!q.empty()){
        tuple<Trie*, int, int> current = q.front();
        q.pop();
        Trie *cur = get<0>(current);
        int climb = get<1>(current);
        int seed = get<2>(current);
        while (climb > 0 && cur != nullptr){
            crumbs[Node(cur)][seed] = true;
            cur = cur->parent;
            climb--;
        }
    }
}

cost_t seed_heuristic(Statesr cur, int k, MatchingKmers &info, int alignment){
    int h = 0;
    vector<int> & seeds = (alignment == 1)? info.seeds1: info.seeds2;
    map <Node, bitset<64> > & crumbs = (alignment == 1)? info.crumbs1: info.crumbs2;
    for (int i = (cur.qpos % k) ? cur.qpos / k + 1: cur.qpos / k; i < seeds.size(); ++i){
        bool check = crumbs[cur.p][i];
        if (check == false) h++;
    }
    //cout <<"Heuristic for the state: "<< cur.qpos << " " << cur.p.u << " "<<cur.p.rpos << " " << h << endl; 
    return h;
}

cost_t heuristic(Statesr cur, int k, MatchingKmers &info, char *heuristic_method, int alignment){
    if (strcmp(heuristic_method, "dijkstra_heuristic") == 0)return 0;
    if (strcmp(heuristic_method, "seed_heuristic") == 0) return seed_heuristic(cur, k, info, alignment);
}

Statesr CreateStatesr(Statesr cur, int k, MatchingKmers &info, char *heuristic_method, cost_t stepcost, int alignment){
    return Statesr(cur.qpos, cur.p, 0, heuristic(cur, k, info, heuristic_method, alignment), stepcost);
}

vector <Statesr> NextStatesr(Statesr cur, char curqbp, const string &ref, int k, MatchingKmers & info, char *heuristic_method, int alignment){
    vector <Statesr> next;
    if (cur.p.is_in_trie()){
        if (cur.p.u->is_leaf()){
            vector <int> last = info.last;
            vector <int> prevpos = info.prevpos;
            for (int i = last[cur.p.u->num]; i != -1; i = prevpos[i]){
                next.push_back(CreateStatesr(Statesr(cur.qpos, Node(i+1)), k, info, heuristic_method, 0, alignment));
            }
        }
        else{
            next.push_back(CreateStatesr(Statesr(cur.qpos+1, cur.p), k, info, heuristic_method, 1, alignment));
            for (int i = 0; i < 4; ++i)
                if (cur.p.u->child[i] != nullptr){
                    if (base[i] == curqbp)
                        next.push_back(CreateStatesr(Statesr(cur.qpos+1, Node(cur.p.u->child[i])), k, info, heuristic_method, 0, alignment));
                    else next.push_back(CreateStatesr(Statesr(cur.qpos+1, Node(cur.p.u->child[i])), k, info, heuristic_method, 1, alignment));
                    next.push_back(CreateStatesr(Statesr(cur.qpos, Node(cur.p.u->child[i])), k, info, heuristic_method, 1, alignment));
                }
        }
    }
    else{
        if (cur.p.rpos < ref.size()){
            if (ref[cur.p.rpos] == curqbp)
                next.push_back(CreateStatesr(Statesr(cur.qpos+1, Node(cur.p.rpos+1)), k, info, heuristic_method, 0, alignment));
            else next.push_back(CreateStatesr(Statesr(cur.qpos+1, Node(cur.p.rpos+1)), k, info, heuristic_method, 1, alignment));
            next.push_back(CreateStatesr(Statesr(cur.qpos, Node(cur.p.rpos+1)), k, info, heuristic_method, 1, alignment));
        }
        next.push_back(CreateStatesr(Statesr(cur.qpos+1, cur.p), k, info, heuristic_method, 1, alignment));
    }
    return next;
}

bool toexplore(map<pair<int, Node>, int> &expandedstates, Statesr &cur){
    auto it = expandedstates.find({cur.qpos, cur.p});
    if (it == expandedstates.end()){
        expandedstates[{cur.qpos, cur.p}] = cur.g;
        return true;
    }
    else if (cur.g < it->second){
        it -> second = cur.g;
        return true;
    }
    return false;
}

bool is_greedy_available(Statesr cur, string &query, string &ref){
    if (cur.p.is_in_trie())return false;
    if (cur.p.rpos < ref.size() && query[cur.qpos] == ref[cur.p.rpos])
        return true;
    return false;
}

cost_t astar_single_read_alignment(string &query, string &ref, int d, int k, Trie *root, MatchingKmers &info, char *heuristic_method, char *showcntexplstates, char *triestart, int alignment){
    ///if there is alignment parameter, it is used to help debuging pair-end alignment
    int n = query.size();
    int m = ref.size();
    priority_queue<Statesr> q;
    map<pair<int, Node>, cost_t> expandedstates;
    int cntexpansions = 0;
    Statesr cur;
    if (strcmp(triestart, "Yes") == 0){
        cur = CreateStatesr(Statesr(0, Node(root)), k, info, heuristic_method, 0, alignment);
        cout << "Missing crumbs in the root of the trie tree: "<< cur.h << endl;
        q.push(cur);
        for (int i = m - d + 1; i <= m; ++i){
            cur = CreateStatesr(Statesr(0, Node(i)), k, info, heuristic_method, 0, alignment);
            q.push(cur);
        }
    }
    else{
        for (int i = 0; i <= m; ++i){
            cur = CreateStatesr(Statesr(0, Node(i)), k, info, heuristic_method, 0, alignment);
            q.push(cur);
        }
    }
    while(!q.empty()){
        cur = q.top();
        q.pop();
        cntexpansions++;
        cout << "cntexpansions == " << cntexpansions << " qpos == " << cur.qpos << " g == " << cur.g << " h == " << cur.h << " f == " << cur.g + cur.h << "\n";
        if (cur.qpos == n)
            break;
        if (toexplore(expandedstates, cur)){
            if (is_greedy_available(cur, query, ref)){
                Statesr topush = CreateStatesr(Statesr(cur.qpos+1, Node(cur.p.rpos+1)), k, info, heuristic_method, 0, alignment);
                topush.g += cur.g;
                q.push(topush);
            }
            else{
                vector <Statesr> next = NextStatesr(cur, query[cur.qpos], ref, k, info, heuristic_method, alignment);
                for (auto i:next){
                    i.g = cur.g + i.stepcost;
                    q.push(i);
                }
            }
        }
    }
    if (strcmp(showcntexplstates, "Yes") == 0)
        cout << "Expanded states: " << cntexpansions << "\n";
    return cur.g;
}
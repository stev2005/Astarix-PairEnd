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
    1 for the first alignment, 2 for the second alignment*/
    vector <int> last;///last: the end position of a last occurance of a dmer in the reference
    vector <int> prevpos;///prevpos: end positions of previous occurances of a dmer in the reference
    vector <Trie*> backtotrieconnection;///backtotrieconnection: pointer to trie leaf from which a given bp of the ref is accessed (for dmers)
    vector <int> lastkmer;///same definition as last but for kmers instead of dmers
    vector <int> prevposkmer; ///same definition as prevpos but for kmers instead of dmers
    vector <Trie*> backtotrieconnectionkmer;///same definition as backtotrieconnection but for kmers instead of dmers
    //map<Node, bitset<64> > crumbs, crumbs1, crumbs2;
    crumbs_t crumbs, crumbs1, crumbs2;
    vector<unordered_set<int> > crumbseeds1;
    vector<unordered_set<int> > crumbseeds2;
    void clearquerydata(){
        seeds.clear();
        seeds1.clear();
        seeds2.clear();
        crumbs.clear();
        crumbs1.clear();
        crumbs2.clear();
        crumbseeds1.clear();
        crumbseeds2.clear();
        lastkmer.clear();//not done in the old
        prevposkmer.clear();//not done in the old
        backtotrieconnectionkmer.clear();//not done in the old
    }
};

struct  Statesr{
    ///State single read
    int qpos;
    Node p;
    cost_t g;///edit distance of alignment of using qpos and p
    cost_t h;///value of heuristic function of using qpos and p
    //cost_t stepcost;///what to add to the already achived cost of the previous of state <qpos, p>
    Statesr(){}
    Statesr(int _qpos, Node _p){
        qpos = _qpos;
        p = _p;
        g = 0;
        h = 0;
        //stepcost = 0;
    }
    Statesr(int _qpos, Node _p, cost_t _g, cost_t _h){
        qpos = _qpos;
        p = _p;
        g = _g;
        h = _h;
    }
    /*Statesr(int _qpos, Node _p, cost_t _g, cost_t _h, cost_t _stepcost){
        qpos = _qpos;
        p = _p;
        g = _g;
        h = _h;
        stepcost = _stepcost;
    }*/
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

bool is_available_to_crumb(vector<unordered_set<int> > & crumbseeds, int num, int pos){///needed for pair-end
    if (crumbseeds[num].find(pos) != crumbseeds[num].end())
        return true;
    else return false;
}

void getcrumbs(const string &ref, const int d, const int k, crumbs_t &crumbs,
                const vector<int> &seeds, const vector<Trie*> &backtotrieconnection, const vector<int> &lastkmer,
                const vector<int> &prevposkmer, int alignment, vector<unordered_set<int> > & crumbseeds){
    /*alignment: what type of alignment is used
    valuews to receive:
        - 0 when single read has its crumbs set on the Gr+
        - 1 when first read of pair-end has its crumbs set on the Gr+
        - 2 when second read of pair-end has its crumbs set on the Gr+
        crumbseeds are needed for pairend getting of crumbs
        */
    set<Node> st;
    set<Node> trienodes;
    int ndel = seeds.size();
    int nins = seeds.size();
    for (int i = 0; i < seeds.size(); ++i){
        if (seeds[i] >= 0){
            int seedpos = i * k;
            for (int j = lastkmer[seeds[i]]; j != -1; j = prevposkmer[j]){
                int seedstart = j - k + 1;///start of a seed in the reference;
                for (int back = 0; back < seedpos + ndel; ++back){
                    int rpos = seedstart - back;
                    if (rpos >= 0){
                        crumbs[Node(rpos)][i] = true;
                        st.insert(Node(rpos));
                    }
                    if (seedstart - rpos > seedpos - nins - d){
                        Trie* cur = backtotrieconnection[rpos];
                        while (cur != nullptr){
                            crumbs[Node(cur)][i] = true;
                            trienodes.insert(Node(cur));
                            st.insert(Node(cur));
                            cur = cur->parent;
                        }
                    }
                }
            }
        }
    }
    cout << "Number of Nodes with at least one crumb: " << st.size() << endl;
    cout << "Number of Trie Nodes with at least one crumb: " << trienodes.size() << endl;
    cout << "Number of Ref Nodes with at least one crumb: " << st.size() - trienodes.size() << endl;
}

cost_t seed_heuristic(int qpos, Node p, int k, vector<int> &seeds, crumbs_t & crumbs){
    cost_t hvalue = 0;
    int i = (qpos % k)? qpos / k + 1: qpos / k;
    for (; i < seeds.size(); ++i)
        if (crumbs[p][i] == false)
            ++hvalue;
    return hvalue;
}

cost_t choose_heuristic(int qpos, Node p, int k, vector<int> &seeds, crumbs_t &crumbs){
    if (heuristic == "seed_heuristic")
        return seed_heuristic(qpos, p, k, seeds, crumbs);
    else if (heuristic == "Dijkstra_heuristic")
        return 0;
    else{
        cerr << "The choosen heuristic is unavailable.\n";
        cerr << "Choosen heuristic: " << heuristic << "\n";
        cerr << "Program is going to abort.\n";
        assert(false);
    }
}

Statesr createStatesr(int qpos, Node p, cost_t g, int k, vector<int> &seeds, crumbs_t &crumbs){
    return Statesr(qpos, p, g, choose_heuristic(qpos, p, k, seeds, crumbs));
}

Statesr createStatesr(int qpos, int rpos, cost_t g, int k, vector<int> &seeds, crumbs_t &crumbs){
    return createStatesr(qpos, Node(rpos), g, k, seeds, crumbs);
}

Statesr createStatesr(int qpos, Trie *u, cost_t g, int k, vector<int> &seeds, crumbs_t &crumbs){
    return createStatesr(qpos, Node(u), g, k, seeds, crumbs);
}

inline void push_first_states_in_q(priority_queue<Statesr> &q, int m, int d, int k, Trie *rootdmer, vector<int> &seeds, crumbs_t &crumbs){
    q.push(createStatesr(0, rootdmer, 0, k, seeds, crumbs));
    cout << "Missing crumbs in the Trie root: " << q.top().h << endl;
    if (rootdmer->is_leaf())
        cout << "The root is a leaf\n";
    else cout << "The root is not a leaf\n";
    for (int i = m - d + 1; i <= m; ++i)
        q.push(createStatesr(0, i, 0, k, seeds, crumbs));
}

map<pair<int, Node>, cost_t>& get_expanded_states(bool del = false){
    static map<pair<int, Node>, cost_t> expanded_states;
    if (del == true)
        expanded_states.clear();
    return expanded_states;
}

bool to_explore(int qpos, Node p, cost_t g){
    //static map<pair<int, Node>, cost_t> expanded_states;
    map<pair<int, Node>, cost_t> & expanded_states = get_expanded_states();
    if (expanded_states.find({qpos, p}) == expanded_states.end()){
        expanded_states[{qpos, p}] = g;
        return true;
    }
    else{
        if (expanded_states[{qpos, p}] > g){
            expanded_states[{qpos, p}] = g;
            return true;
        }
        return false;
    }
    assert(false);
}

bool gready_available(string &query, string &ref, int qpos, Node p){
    if (!p.is_in_trie())
        return false;
    if (ref.size() > p.rpos && query[qpos] == ref[p.rpos])
        return true;
    return false;
}

vector<Statesr> & get_next_states_sr(int qpos, Node p, char cqpos, string &ref, int k, vector<int> &last, vector<int> &prevpos, vector<int> &seeds, crumbs_t &crumbs){
    static vector<Statesr> next;
    next.clear();
    if (p.is_in_trie()){
        if (p.u->is_leaf()){
            for (int i = last[p.u->num]; i != -1;  i = prevpos[i])
                next.push_back(createStatesr(qpos, i + 1, 0, k, seeds, crumbs));
        }
        else{
            next.push_back(createStatesr(qpos + 1, p.u, 1, k, seeds, crumbs));
            for (int i = 0; i < 4; ++i){
                if (p.u->child[i] != nullptr){
                    if (base[i] == cqpos)
                        next.push_back(createStatesr(qpos + 1, p.u->child[i], 0, k, seeds, crumbs));
                    else next.push_back(createStatesr(qpos + 1, p.u->child[i], 1, k, seeds, crumbs));
                    next.push_back(createStatesr(qpos, p.u->child[i], 1, k, seeds, crumbs));
                }
            }
        }
    }
    else{
        next.push_back(createStatesr(qpos + 1, p.rpos, 1, k, seeds, crumbs));
        if (p.rpos < ref.size()){
            next.push_back(createStatesr(qpos, p.rpos + 1, 1, k, seeds, crumbs));
            if (cqpos == ref[p.rpos])
                next.push_back(createStatesr(qpos + 1, p.rpos + 1, 0, k, seeds, crumbs));
            else next.push_back(createStatesr(qpos + 1, p.rpos + 1, 1, k, seeds, crumbs));
        }
    }
    return next;
}

void showinfoaboutstatesrancestors(vector<Statesr> &nextst, MatchingKmers &info){
    //cout << "Inheritors of the root:\n";
    cout << "Inheritor of the root with minimum f:\n";
    /*for (auto i: nextst)
        i.print();*/
        Statesr minf(-1, Node(0), 1000, 1000);
        for (auto i: nextst)
            if (i.g + i.h < minf.g + minf.h)
                minf = i;
    cout << "Number of missing crumbs of Node: " << info.crumbs[minf.p].count() << " ";
    minf.print();
    cout << endl;
}

cost_t astar_single_read_alignment(string &query, string &ref, int d, int k, Trie *rootdmer, MatchingKmers &info){
    int n = query.size();
    int m = ref.size();
    int cntexpansions = 0;
    int cntexpansionsTrie = 0;
    int cntexpansionsref = 0;
    int cntTrienodeswithoutcrumbs = 0;
    int cntreexpandedTrienodes = 0;
    int cntreexpandedrefnodes = 0;
    map<pair<int, Node>, cost_t> & expandedstates = get_expanded_states();
    priority_queue<Statesr> q;
    push_first_states_in_q(q, m, d, k, rootdmer, info.seeds, info.crumbs);
    Statesr cur;
    bool showninheritorsfirsttime = false;
    int cntshow = 0;
    int cntswitchfromtrietoline = 0;
    bool istrie = true;
    while (!q.empty()){
        cur = q.top();
        cntexpansions++;
        q.pop();
        /*if (cntshow < 10){
            cout << "Number of missing crumbs of Node: " << info.crumbs[cur.p].count() << " ";
            cur.print();
        }*/
            
        if (cur.p.is_in_trie()){
            if (istrie == false)
                istrie = true;
            cntexpansionsTrie++;
            if (info.crumbs[cur.p].count() == 0)
                cntTrienodeswithoutcrumbs++;
            auto it = expandedstates.find({cur.qpos, cur.p});
            if (it != expandedstates.end())
                cntreexpandedTrienodes++;
        }
        else{
            istrie = true;
            cntswitchfromtrietoline++;
            cntexpansionsref++;
            auto it = expandedstates.find({cur.qpos, cur.p});
            if (it != expandedstates.end())
                cntreexpandedrefnodes++;
        }
        if (cur.qpos == n)
            break;
        if (to_explore(cur.qpos, cur.p, cur.g)){
            /*while (to_explore(cur.qpos, cur.p, cur.g) && gready_available(query, ref, cur.qpos, cur.p) && cur.qpos < n ){
                cur.qpos++;
                cur.p.rpos++;
                cntexpansions++;
            }
            if (cur.qpos == n)
                break;*/
            if (gready_available(query, ref, cur.qpos, cur.p)){
                cur.qpos++;
                cur.p.rpos++;
                q.push(cur);
            }
            else{
                vector<Statesr> & nextst = get_next_states_sr(cur.qpos, cur.p, query[cur.qpos], ref, k, info.last, info.prevpos, info.seeds, info.crumbs);
                /*if (cntshow < 10){
                    cntshow++;
                    showinfoaboutstatesrancestors(nextst, info);
                }*/
                for (auto i: nextst){
                    i.g += cur.g;
                    q.push(i);
                }
            }
            
        }
    }
    
    cout << "Expanded states: " << cntexpansions << "\n";
    cout << "Expanded trie states: " << cntexpansionsTrie << "\n";
    cout << "Expanded trie states (%): " << (double) cntexpansionsTrie / (double) cntexpansions * (double) 100 << "%\n";
    cout << "Expanded trie states without any crumb: " << cntTrienodeswithoutcrumbs << endl;
    cout << "Expanded trie states without any crumb (% Trie expansions): " << (double) cntTrienodeswithoutcrumbs / (double) cntexpansionsTrie * (double) 100 << "%\n";
    cout << "Expanded trie states reexpanded: " << cntreexpandedTrienodes << endl;
    cout << "Expanded trie states reexpanded (% Trie expansions): " << (double) cntreexpandedTrienodes / (double) cntexpansionsTrie * (double) 100 << "%\n";
    cout << "Expanded ref states: " << cntexpansionsref << "\n";
    cout << "Expanded ref states (%): " << (double) cntexpansionsref / (double) cntexpansions * (double) 100 << "%\n";
    cout << "Expanded ref states reexpanded (% ref expansions): " << (double) cntreexpandedrefnodes / (double) cntexpansionsref * (double) 100 << "%\n";
    cout << "Times switching from trie to linear search: " << cntswitchfromtrietoline << "\n";
    cout << "Band: " << (double) cntexpansions / (double) n << "\n";
    get_expanded_states(true);
    return cur.g;
}
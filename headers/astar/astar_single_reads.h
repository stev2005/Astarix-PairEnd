#pragma once

#include "../header.h"
#include <bits/stdc++.h>
#include "statesstruct.h"
#include "single_read_heursitic.h"

using namespace std;


bool not_available_to_crumb(vector<unordered_set<int> > & crumbseeds, int num, int pos){///needed for pair-end
    /*if (crumbseeds[num].find(pos) != crumbseeds[num].end())
        return true;
    else return false;*/
    return (crumbseeds[num].find(pos) == crumbseeds[num].end());
}

void getcrumbs(const string &ref, const int d, const int k, crumbs_t &crumbs,
                const vector<int> &seeds, const vector<Trie*> &backtotrieconnection, const vector<int> &lastkmer,
                const vector<int> &prevposkmer, int read, vector<unordered_set<int> > & crumbseeds){
    /*read: which read is being set on crumbs
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
                if (read != 0)
                    if (not_available_to_crumb(crumbseeds, i, j))
                        continue;
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
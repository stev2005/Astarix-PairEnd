#pragma once

#include "../header.h"
#include <bits/stdc++.h>
#include "statesstruct.h"
#include "single_read_heursitic.h"

using namespace std;


bool not_available_to_crumb(vector<unordered_set<int> > & crumbseeds, int num, int pos){///needed for pair-end
    return (crumbseeds[num].find(pos) == crumbseeds[num].end());
}

//gets the crumbs on Gr+ and returns their count
int getcrumbs(const string &ref, const int d, const int k, crumbs_t &crumbs,
                const vector<int> &seeds, const vector<Trie*> &backtotrieconnection, const vector<int> &lastkmer,
                const vector<int> &prevposkmer, int read, vector<unordered_set<int> > & crumbseeds){
    /*read: which read is being set on crumbs
    valuews to receive:
        - 0 when single read has its crumbs set on the Gr+
        - 1 when first read of pair-end has its crumbs set on the Gr+
        - 2 when second read of pair-end has its crumbs set on the Gr+
        crumbseeds are needed for pairend getting of crumbs
    */
    /*cout << "elements in crumbseeds: \n";
    for (int i = 0; i < (int) crumbseeds.size(); ++i){
        cout << "   in set i: " << i << "\n";
        for (auto it = crumbseeds[i].begin(); it != crumbseeds[i].end(); ++it)
            cout << "       " << *it << "\n";
        cout << "\n";
    }
    cout << "\n";*/
    //cerr << "starts sr get crumbs\n";
    set<Node> st;
    //set<Node> trienodes;
    int ndel = seeds.size();
    int nins = seeds.size();
    int cntsetcrumbs = 0;
    int cntTriesetcrumbs = 0;
    for (int i = 0; i < seeds.size(); ++i){
        //cerr << "i: " << i << " seeds[i]: " << seeds[i] << "\n";
        if (seeds[i] >= 0){
            //cntseedcrumbs = 0;
            int seedpos = i * k;
            for (int j = lastkmer[seeds[i]]; j != -1; j = prevposkmer[j]){
                //cerr << "j: " << j << "\n";
                int seedstart = j - k + 1;///start of a seed in the reference;
                if (read != 0)
                    if (not_available_to_crumb(crumbseeds, i, j))
                        continue;
                //cerr << "filtered accepted crumb here\n";
                for (int back = 0; back < seedpos + ndel; ++back){
                    int rpos = seedstart - back;
                    //cerr << "rpos: " << rpos << "\n";
                    if (rpos >= 0){
                        crumbs[Node(rpos)][i] = true;
                        cntsetcrumbs++;
                        //st.insert(Node(rpos));
                        //cerr << "if for rpos >= 0 passed\n";
                        if (seedstart - rpos > seedpos - nins - d){
                            //cerr << "print again rpos: " << rpos << "\n";
                            Trie* cur = backtotrieconnection[rpos];
                            while (cur != nullptr){
                                cntsetcrumbs++;
                                cntTriesetcrumbs++;
                                crumbs[Node(cur)][i] = true;
                                //trienodes.insert(Node(cur));
                                //st.insert(Node(cur));
                                cur = cur->parent;
                            }
                        }
                    }
                    
                }
            }
            //cout << "seed[" << i << "] has "<< cntseedcrumbs << " crumbs in the Gr+\n";
        }
    }
    cout << "Trie crumbs set without the optimization: " << cntTriesetcrumbs << "\n";
    //cerr << "End get sr crumbs\n";
    /*for (int i = 0; i < st.size(); ++i)
        cout << "seed[" << i << "] has "<< crumbs[i].size() << "appereances in the reference\n";*/
    /*cout << "Number of Nodes with at least one crumb: " << st.size() << endl;
    cout << "Number of Trie Nodes with at least one crumb: " << trienodes.size() << endl;
    cout << "Number of Ref Nodes with at least one crumb: " << st.size() - trienodes.size() << endl;*/
    return st.size();
    //return cntsetcrumbs;
}

inline void get_Node_crumbs(Node curnode, int nseeds, int &maxcntcrumbs, crumbs_t &crumbs, set<int> &crumbsidx){
    int cntcrumbs = crumbs[curnode].count();
    maxcntcrumbs = (maxcntcrumbs < cntcrumbs)? cntcrumbs: maxcntcrumbs;
    for (int j = 0; j < nseeds; ++j)
        if (crumbs[curnode][j]) crumbsidx.insert(j);
}

/*int get_min_lack_of_crumbs(Trie *cur, int nseeds, crumbs_t &crumbs, const vector<int> &last, const vector<int> &prevpos, set<int> &crumbsidx){
    int maxcntcrumbs = -1;
    if (cur->is_leaf())
        for (int i = last[cur->num]; i != -1; i = prevpos[i])
            get_Node_crumbs(Node(i), nseeds, maxcntcrumbs, crumbs, crumbsidx);
    else{
        for (int i = 0; i < 4; i++)
            if (cur->child[i])
                get_Node_crumbs(Node(cur->child[i]), nseeds, maxcntcrumbs, crumbs, crumbsidx);
    }
    return maxcntcrumbs;
}*/

int get_min_lack_of_crumbs(int nseeds, const vector<Node> &heirs, crumbs_t &crumbs, set<int> &crumbsidx){
    int maxcntcrumbs = 0;
    for (auto heir: heirs)
        get_Node_crumbs(heir, nseeds, maxcntcrumbs, crumbs, crumbsidx);
    return maxcntcrumbs;
}

void getcrumbs_trieopt(const string &ref, const int d, const int k, crumbs_t &crumbs,
                const vector<int> &seeds, const vector<Trie*> &backtotrieconnection, const vector<int> &lastkmer,
                const vector<int> &prevposkmer, const vector<int> &last, const vector<int> &prevpos, int read, vector<unordered_set<int> > & crumbseeds){
    /*read: which read is being set on crumbs
    valuews to receive:
        - 0 when single read has its crumbs set on the Gr+
        - 1 when first read of pair-end has its crumbs set on the Gr+
        - 2 when second read of pair-end has its crumbs set on the Gr+
        crumbseeds are needed for pairend getting of crumbs
    */
    //cerr << "Optimized crumbs function\n";
    int ndel = seeds.size();
    int nins = seeds.size();
    //queue<Trie*> q;///to set crumbs in the trie
    //unordered_map<Trie*, vector<Node> > tcs;///Trie Crumbs Setting
    unordered_map<Trie*, int> maxcntcrumbs;
    queue<Trie*> tcs;///Trie Crumbs Setting
    int cntsetcrumbs = 0;
    for (int i = 0; i < (int)seeds.size(); ++i){
        if (seeds[i] >= 0){
            int seedpos = i * k;
            for (int j = lastkmer[seeds[i]]; j != -1; j = prevposkmer[j]){
                int seedstart = j - k + 1;
                for (int back = 0; back < seedpos + ndel; ++back){
                    int rpos = seedstart - back;
                    if (rpos >= 0){
                        crumbs[Node(rpos)][i] = true;
                        if (seedstart - rpos > seedpos - nins - d){
                            Trie* cur = backtotrieconnection[rpos];
                            //q.push(cur);
                            //tcs[cur].push_back(Node(rpos));
                            crumbs[Node(cur)][i] = true;
                            int rposcrumbscnt = crumbs[Node(rpos)].count();
                            //maxcntcrumbs[cur] = (rposcrumbscnt > maxcntcrumbs[cur])? rposcrumbscnt: maxcntcrumbs[cur];
                            if (rposcrumbscnt > maxcntcrumbs[cur])
                                maxcntcrumbs[cur] = rposcrumbscnt;
                            tcs.push(cur);
                        }
                    }
                }
            }
        }
    }
    int cntTriesetcrumbs = 0;
    unordered_set<Trie*> settedTrie;
    while (!tcs.empty()){
        Trie* cur = tcs.front();
        tcs.pop();
        if (settedTrie.find(cur) != settedTrie.end())
            continue;
        int numseed = 0;
        while (crumbs[cur].count() > maxcntcrumbs[cur]){
            crumbs[cur][numseed] = false;
            ++numseed;
        }
        cntTriesetcrumbs += maxcntcrumbs[cur];
        if (cur->parent == nullptr)
            continue;
        Trie* par = cur->parent;
        for (numseed = 0; numseed < k; ++numseed)
            if (crumbs[cur][numseed])
                crumbs[par][numseed] = true;
        if (maxcntcrumbs[par] < maxcntcrumbs[cur])
            maxcntcrumbs[par] = maxcntcrumbs[cur];
        tcs.push(par);
        settedTrie.insert(cur);
    }
    cout << "Trie crumbs set with the optimization: " << cntTriesetcrumbs << "\n";
    /*while(tcs.size()){
        unordered_map<Trie*, vector<Node> > newtcs;
        for (auto node: tcs){
            Trie* cur = node.first;
            vector<Node> &heirs = node.second;
            set<int> crumbsidx;
            int crumbscnt = get_min_lack_of_crumbs(seeds.size(), heirs, crumbs, crumbsidx);
            Node curnode = Node(cur);
            auto it = crumbsidx.end();
            --it;
            while (crumbscnt){
                crumbs[curnode][*it] = true;
                --it;
                ++cntTriesetcrumbs;
                //auto it2 = it++;
                --crumbscnt;
            }
            if (cur->parent)
                newtcs[cur->parent].push_back(Node(cur));
        }
        tcs.clear();
        tcs = newtcs;
    }*/
    //cout << "Trie crumbs set with the optimization: " << cntTriesetcrumbs << "\n";
    /*while (!q.empty()){
        Trie *cur = q.front();
        q.pop();
        set<int> crumbsidx;
        int crumbscnt = get_min_lack_of_crumbs(cur, seeds.size(), crumbs, last, prevpos, crumbsidx);
        auto it = crumbsidx.end();
        --it;
        Node curnode = Node(cur);
        while (crumbscnt){
            crumbs[curnode][*it] = true;
            --it;
            --crumbscnt;
        }
        if (cur->parent) q.push(cur->parent);
    }*/
}

inline void push_first_states_in_q(priority_queue<Statesr> &q, int m, int d, int k, Trie *rootdmer, vector<int> &seeds, crumbs_t &crumbs){
    q.push(createStatesr(0, rootdmer, 0, k, seeds, crumbs));
    //cout << "Missing crumbs in the Trie root: " << q.top().h << endl;
    /*if (rootdmer->is_leaf())
        cout << "The root is a leaf\n";
    else cout << "The root is not a leaf\n";*/
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
    //cout << "get single read inheritors:\n";
    static vector<Statesr> next;
    next.clear();
    //cout << "qpos: " << qpos << " p.u: "<< p.u << " p.rpos: " << p.rpos << "\n";
    if (cqpos == special_sign){
        /*if (p.is_in_trie()){
            if (p.u->is_leaf()){
                for (int i = last[p.u->num]; i != -1;  i = prevpos[i])
                    next.push_back(Statesr(qpos, Node(i + 1), special_cost, 0));
            }
            else{
                for (int i = 0; i < 4; ++i)
                    if (p.u->child[i] != nullptr)
                        next.push_back(Statesr(qpos + 1, p.u->child[i], special_cost, 0));
            }
        }
        else next.push_back(Statesr(qpos + 1, Node(p.rpos + 1), special_cost, 0));*/
        next.push_back(Statesr(qpos + 1, p, special_cost, special_heuristic_cost));
        return next;
    }
    if (p.is_in_trie()){
        //cout << "in the trie\n";
        if (p.u->is_leaf()){
            //cout << "It is leaf\n";
            for (int i = last[p.u->num]; i != -1;  i = prevpos[i]){
                //cout << "inheritor\n";
                next.push_back(createStatesr(qpos, i + 1, 0, k, seeds, crumbs));
            }
        }
        else{
            //cout << "It's not leaf\n";
            next.push_back(createStatesr(qpos + 1, p.u, 1, k, seeds, crumbs));
            //cout << "inheritor\n";
            for (int i = 0; i < 4; ++i){
                if (p.u->child[i] != nullptr){
                    if (base[i] == cqpos)
                        next.push_back(createStatesr(qpos + 1, p.u->child[i], 0, k, seeds, crumbs));
                    else next.push_back(createStatesr(qpos + 1, p.u->child[i], 1, k, seeds, crumbs));
                    next.push_back(createStatesr(qpos, p.u->child[i], 1, k, seeds, crumbs));
                    //cout << "inheritor\n";
                    //cout << "inheritor\n";
                }
            }
        }
    }
    else{
        next.push_back(createStatesr(qpos + 1, p.rpos, 1, k, seeds, crumbs));
        //cout << "inheritor\n";
        if (p.rpos < ref.size()){
            next.push_back(createStatesr(qpos, p.rpos + 1, 1, k, seeds, crumbs));
            //cout << "inheritor\n";
            if (cqpos == ref[p.rpos])
                next.push_back(createStatesr(qpos + 1, p.rpos + 1, 0, k, seeds, crumbs));
            else next.push_back(createStatesr(qpos + 1, p.rpos + 1, 1, k, seeds, crumbs));
            //cout << "inheritor\n";
        }
    }
    //cout << "The end of getting heirs\n";
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

void showcounters_for_the_best_aligner(int cntexpansions, int cntexpansionsTrie, int cntexpansionsref, int cntTrienodeswithoutcrumbs,
int cntreexpandedTrienodes, int cntreexpandedrefnodes, int cntswitchfromtrietoline, int n){
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
}

vector<pair<cost_t, int> > astar_single_read_alignment(string &query, string &ref, int d, int k, Trie *rootdmer, MatchingKmers &info, int numaligns){
    vector< pair<cost_t, int> > alignments;
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
    //clock_t t = clock();
    push_first_states_in_q(q, m, d, k, rootdmer, info.seeds, info.crumbs);
    Statesr cur;
    bool showninheritorsfirsttime = false;
    int cntshow = 0;
    int cntswitchfromtrietoline = 0;
    bool istrie = true;
    int cntaligns = 0;
    auto startt = chrono::high_resolution_clock::now();
    int minmaxcost = -1;

    BucketQueue<cost_t, Statesr> Q;
    int init_h_value = info.seeds.size() - info.crumbs[Node(rootdmer)].count();
    Q.Push(0 + init_h_value, createStatesr(0, rootdmer, 0, k, info.seeds, info.crumbs));
    for (int i = m - d + 1; i <= m; ++i)
        Q.Push(0 + init_h_value, createStatesr(0, i, 0, k, info.seeds, info.crumbs));



    while (!Q.Empty()){
        auto nowt =chrono::high_resolution_clock::now();
        chrono::duration<double> takentime = nowt - startt;
        if (takentime.count() > 7){
            cerr << "Alignment time more than 7 sec. Not alignable sample\n";
            return alignments;
        }


        //cur = q.top();
        cntexpansions++;
        //q.pop();
        cur = Q.Top().second;
        Q.Pop();
        /*if (cntshow < 10){
            cout << "Number of missing crumbs of Node: " << info.crumbs[cur.p].count() << " ";
            cur.print();
        }*/
        if (minmaxcost < cur.g){
            minmaxcost = cur.g;
            cerr << "New mimimal maximum cost achieved: " << minmaxcost << "\n";
        }
        if (cur.p.is_in_trie()){
            /*if (istrie == false)
                istrie = true;*/
            cntexpansionsTrie++;
            /*if (info.crumbs[cur.p].count() == 0)
                cntTrienodeswithoutcrumbs++;*/
            /*auto it = expandedstates.find({cur.qpos, cur.p});
            if (it != expandedstates.end())
                cntreexpandedTrienodes++;*/
        }
        else{
            //istrie = true;
            //cntswitchfromtrietoline++;
            cntexpansionsref++;
            /*auto it = expandedstates.find({cur.qpos, cur.p});
            if (it != expandedstates.end())
                cntreexpandedrefnodes++;*/
        }
        if (cur.qpos == n){
            /*if (cntaligns == 0)
                showcounters_for_the_best_aligner(cntexpansions, cntexpansionsTrie, cntexpansionsref,cntTrienodeswithoutcrumbs,
                    cntreexpandedTrienodes, cntreexpandedrefnodes, cntswitchfromtrietoline, n);*/
            alignments.push_back({cur.g, cur.p.rpos});
            cntaligns++;
            if (cntaligns == numaligns)
                break;
        }
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
                //q.push(cur);
                Q.Push(cur.g + cur.h, cur);
            }
            else{
                vector<Statesr> & nextst = get_next_states_sr(cur.qpos, cur.p, query[cur.qpos], ref, k, info.last, info.prevpos, info.seeds, info.crumbs);
                /*if (cntshow < 10){
                    cntshow++;
                    showinfoaboutstatesrancestors(nextst, info);
                }*/
                for (auto i: nextst){
                    i.g += cur.g;
                    //q.push(i);
                    Q.Push(i.g + i.h, i);
                }
            }
            
        }
    }
    //t = clock() - t;
    auto endt = chrono::high_resolution_clock::now();
    chrono::duration<double> takentime = endt - startt;
    double aligntime = takentime.count();
    //cerr << "Band: " << (double)cntexpansions / (double)n << "\n";
    //cerr << "Alignment time: " << aligntime << "\n";
    //cout << "Band: " << (double)cntexpansions / (double)n << "\n";
    //cout << "Alignment time: " << aligntime << "\n";
    /*evalsts.cntexpansions += cntexpansions;
    evalsts.band += ((double)cntexpansions / (double)n);*/
    get_expanded_states(true);
    //showcounters_for_the_best_aligner(cntexpansions, cntexpansionsTrie, cntexpansionsref,cntTrienodeswithoutcrumbs,
    //    cntreexpandedTrienodes, cntreexpandedrefnodes, cntswitchfromtrietoline, n);
    if (cur.g < 128)
        seevals[cur.g].add_entry((double)cntexpansions / (double)n, cntexpansions, cntexpansionsTrie, cntexpansionsref, aligntime);
    return alignments;
}
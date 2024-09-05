#pragma once
#include "../header.h"
#include <bits/stdc++.h>
#include "statesstruct.h"
#include "single_read_heursitic.h"
using namespace std;

inline void getcrumbs(const string &ref, const int d, const int k, const vector<int> &seeds, const vector<int> &lastkmer,
                      const vector<int> &prevposkmer, const vector<Trie*> &backtotrieconnection, crumbs_t &crumbs){
    int szs = seeds.size();
    for (int i = 0; i < szs; ++i){
        if (seeds[i] == -1) continue;
        int seedpos = i * k;
        for (int j = lastkmer[seeds[i]]; j != -1; j = prevposkmer[j]){
            int seedloc = j - k + 1;
            for (int back = 0; back < seedpos + ndel; ++ back){
                int rpos = seedloc - back;
                if (rpos < 0) break;
                crumbs[Node(rpos)][i] = true;
                if (back > seedpos - d - nins){
                    Trie* cur = backtotrieconnection[rpos];
                    while (cur != nullptr){
                        crumbs[Node(cur)][i] = true;
                        cur = cur->parent;
                    }
                }
            }
        }
    }
}

inline void  getcrumbs_trieopt(const string &ref, const int d, const int k, const vector<int> &seeds, const vector<int> &lastkmer,
                      const vector<int> &prevposkmer, const vector<Trie*> &backtotrieconnection, crumbs_t &crumbs){
    deque<Trie*> tcs;
    unordered_map<Trie*, unordered_map<int, bitset<64> > > refchildcrumbs;
    int szs = seeds.size();
    for (int i = 0; i < szs; ++i){
        if (seeds[i] == -1) continue;
        int seedpos = i * k;
        for (int j = lastkmer[seeds[i]]; j != -1; j = prevposkmer[j]){
            int seedloc = j - k + 1;
            for (int back = 0; back < seedpos + ndel; ++ back){
                int rpos = seedloc - back;
                if (rpos < 0) break;
                crumbs[Node(rpos)][i] = true;
                if (back > seedpos - d - nins){
                    Trie* cur = backtotrieconnection[rpos];
                    tcs.push_back(cur);
                    refchildcrumbs[cur][rpos][i] = true;
                }
            }
        }
    }
    unordered_map<Trie*, int> maxcntcrumbs;
    for (Trie *cur: tcs){
        for (pair<int, bitset<64> > heir: refchildcrumbs[cur]){
            if (maxcntcrumbs[cur] < heir.second.count())
                maxcntcrumbs[cur] = heir.second.count();
            crumbs[cur] |= heir.second;
        }
    }
    unordered_set<Trie*> settedTrie;
    while (!tcs.empty()){
        Trie *cur = tcs.front();
        tcs.pop_front();
        if (settedTrie.find(cur) != settedTrie.end())
            continue;
        settedTrie.insert(cur);
        Node curN = Node(cur);
        int numseed = 0;
        while (crumbs[curN].count() > maxcntcrumbs[cur]){
            crumbs[curN][numseed] = false;
            ++numseed;
        }
        if (cur->parent == nullptr)
            continue;
        Trie* par = cur->parent;
        Node parN = Node(par);
        crumbs[parN] |= crumbs[curN];
        if (maxcntcrumbs[par] < maxcntcrumbs[cur])
            maxcntcrumbs[par] = maxcntcrumbs[cur];
        tcs.push_back(par);
    }
}

inline void push_first_states_in_queue(int m, int d, int k, Trie *rootdmer, BucketQueueSE &Q, MatchingKmers &info){
    Statesr cur(createStatesr(0, rootdmer, 0, k, info.seeds, info.crumbs));
    //cerr << "<0, rootdmer, + >'s h: " << cur.h << "\n";
    //count_neg_one(info.seeds);
    cur.negative = false;
    Q.Push(cur);
    for (int i = m - d + 1; i <= m; ++i){
        cur =  createStatesr(0, i, 0, k, info.seeds, info.crumbs);
        cur.negative = false;
        Q.Push(cur);
    }
    cur = createStatesr(0, rootdmer, 0, k, info.nseeds, info.ncrumbs);
    //cerr << "<0, rootdmer, - >'s h: " << cur.h << "\n";
    //count_neg_one(info.nseeds);
    cur.negative = true;
    Q.Push(cur);
    for (int i = m - d + 1; i <= m; ++i){
        cur = createStatesr(0, i, 0, k, info.nseeds, info.ncrumbs);
        cur.negative = true;
        Q.Push(cur);
    }
}

map<tuple<int, Node, bool>, cost_t>& get_expanded_states(bool del = false){
    static map<tuple<int, Node, bool>, cost_t> expanded_states;
    if (del == true)
        expanded_states.clear();
    return expanded_states;
}

bool to_explore(int qpos, Node p, bool negative, cost_t g){
    map<tuple<int, Node, bool>, cost_t> & ex_st = get_expanded_states();
    tuple<int, Node, bool> cur({qpos, p, negative});
    if (ex_st.find(cur) == ex_st.end()) return true;
    if (ex_st[cur] > g) return true;
    return false;
}

inline void expand_state(int qpos, Node p, bool negative, cost_t g){
    map<tuple<int, Node, bool>, cost_t > & ex_st = get_expanded_states();
    tuple<int, Node, bool> cur({qpos, p, negative});
    ex_st[cur] = g;
}

bool is_gready_available_se(Node p, char cqpos, string &ref){
    if (p.is_in_trie()) return false;
    return (p.rpos < ref.size() && cqpos == ref[p.rpos]);
}

vector<Statesr> get_next_states_sr(int qpos, Node p, char cqpos, string &ref, int k, vector<int> &last, vector<int> &prevpos, vector<int> &seeds, crumbs_t &crumbs){
    vector<Statesr> next;
    if (cqpos == special_sign){
        next.push_back(createStatesr(qpos, p, special_cost, k, seeds, crumbs));
        next.push_back(createStatesr(qpos + 1, p, special_cost, k, seeds, crumbs));
    }
    else if (p.is_in_trie()){
        if (p.u->isleaf()){
            for (int i = last[p.u->num]; i != -1; i = prevpos[i]){
                next.push_back(createStatesr(qpos, i, 0, k, seeds, crumbs));
            }
        }
        else{
            next.push_back(createStatesr(qpos + 1, p, cins, k, seeds, crumbs));
            for (int i = 0; i < alphabetsz; ++i){
                if (p.u->child[i]){
                    if (cqpos == base[i]) next.push_back(createStatesr(qpos + 1, p.u->child[i], cequal, k, seeds, crumbs));
                    else next.push_back(createStatesr(qpos + 1, p.u->child[i], csub, k, seeds, crumbs));
                    next.push_back(createStatesr(qpos, p.u->child[i], cdel, k, seeds, crumbs));
                }
            }
        }
    }
    else{
        /*if (p.rpos < ref.size() && cqpos == ref[p.rpos])*/
        if (is_gready_available_se(p, cqpos, ref)) next.push_back(createStatesr(qpos + 1, p.rpos + 1, cequal, k, seeds, crumbs));
        else{
            if (p.rpos < ref.size()){
                next.push_back(createStatesr(qpos + 1, p.rpos + 1, csub, k, seeds, crumbs));
                next.push_back(createStatesr(qpos, p.rpos + 1, cdel, k, seeds, crumbs));
            }
            next.push_back(createStatesr(qpos + 1, p, cins, k, seeds, crumbs));
        }
    }
    return next;
}

inline void astar_single_read_alignment(string &query, string &nquery, string &ref, int d, int k, Trie *rootdmer, MatchingKmers &info){
    cerr << "Aligner run\n";
    int n = query.size();
    int m = ref.size();
    BucketQueueSE Q;
    push_first_states_in_queue(m, d, k, rootdmer, Q, info);
    cerr << "first states pushed in Q\n";
    int minmaxcost = -1;
    Statesr cur;
    auto startt = gettimenow_chrono();
    while (!Q.Empty()){
        cur = Q.Top();
        Q.Pop();
        if (runtimechrono (startt, gettimenow_chrono()) > 7){
            cerr << "Alignment time more than 7 sec. Not alignable sample\n";
            get_expanded_states(true);
            return;
        }
        //show_minmaxcost(minmaxcost, cur, query, nquery);
        if (cur.qpos == n) break;
        if (!to_explore(cur.qpos, cur.p, cur.negative, cur.g)) continue;
        else expand_state(cur.qpos, cur.p, cur.negative, cur.g);
        string & curquery = (cur.negative)? nquery: query;
        vector<int> & curseeds = (cur.negative)? info.nseeds: info.seeds;
        crumbs_t & curcrumbs = (cur.negative)? info.ncrumbs: info.crumbs;
        vector<Statesr> next = move(get_next_states_sr(cur.qpos, cur.p, curquery[cur.qpos], ref, k, info.last, info.prevpos, curseeds, curcrumbs));
        for (auto i: next){
            i.g += cur.g;
            i.negative = cur.negative;
            Q.Push(i);
        }
    }
    get_expanded_states(true);
    auto endt = move(gettimenow_chrono());
    double takentime = runtimechrono(startt, endt);
    cerr << "Taken time: " << takentime << "s.\n";
    cerr << "Cost: " << cur.g << '\n';
    cout << "Taken time: " << takentime << "s.\n";
    cout << "Cost: " << cur.g << '\n';
}

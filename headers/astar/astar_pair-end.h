#pragma once

#include "../header.h"
#include <bits/stdc++.h>
#include "statesstruct.h"
#include "pairend_read_heuristic.h"
#include "astar_single_reads.h"

using namespace std;

int insdist, drange, innerdist, readdist;
int punishl, punishr, cntpunishes;

inline void getcrumbs_pairend(string &ref, int qp1sz, int qp2sz, int d, int k, MatchingKmers &info){
    nins = get_nins_ndel_value(qp1sz, info.seeds1.size(), cins);
    ndel = get_nins_ndel_value(qp1sz, info.seeds1.size(), cdel);
    getcrumbs(ref, d, k, info.seeds1, info.lastkmer, info.prevposkmer, info.backtotrieconnection, info.crumbs1);
    getcrumbs(ref, d, k, info.nseeds1, info.lastkmer, info.prevposkmer,  info.backtotrieconnection, info.ncrumbs1);
    nins = get_nins_ndel_value(qp2sz, info.seeds2.size(), cins);
    ndel = get_nins_ndel_value(qp2sz, info.seeds2.size(), cdel);
    getcrumbs(ref, d, k, info.seeds2, info.lastkmer, info.prevposkmer, info.backtotrieconnection, info.crumbs2);
    getcrumbs(ref, d, k, info.nseeds2, info.lastkmer, info.prevposkmer, info.backtotrieconnection, info.ncrumbs2);
}

inline void getcrumbs_trieopt_pairend(string &ref, int qp1sz, int qp2sz, int d, int k, MatchingKmers &info){
    nins = get_nins_ndel_value(qp1sz, info.seeds1.size(), cins);
    ndel = get_nins_ndel_value(qp1sz, info.seeds1.size(), cdel);
    getcrumbs_trieopt(ref, d, k, info.seeds1, info.lastkmer, info.prevposkmer, info.backtotrieconnection, info.crumbs1);
    getcrumbs_trieopt(ref, d, k, info.nseeds1, info.lastkmer, info.prevposkmer,  info.backtotrieconnection, info.ncrumbs1);
    nins = get_nins_ndel_value(qp2sz, info.seeds2.size(), cins);
    ndel = get_nins_ndel_value(qp2sz, info.seeds2.size(), cdel);
    getcrumbs_trieopt(ref, d, k, info.seeds2, info.lastkmer, info.prevposkmer, info.backtotrieconnection, info.crumbs2);
    getcrumbs_trieopt(ref, d, k, info.nseeds2, info.lastkmer, info.prevposkmer, info.backtotrieconnection, info.ncrumbs2);
}

inline void push_init_prstate_in_q(priority_queue<Statepr> &q, Node &&p1, Node &&p2, int k, vector<int> &seeds1, vector<int> &seeds2,
                                   crumbs_t &crumbs1, crumbs_t &crumbs2, bool negative){
    cost_t h = pairend_heuristic(0, p1, p2, k, seeds1, seeds2, crumbs1, crumbs2);
    auto cur = createStatepr(0, p1, p2, 0, h);
    cur.negative = negative;
    q.push(cur);
}

inline void push_first_prstates_in_q_starnd(priority_queue<Statepr> &q, int m, Trie *root, int d, int k,vector<int> &seeds1,
                                            vector<int> &seeds2, crumbs_t &crumbs1, crumbs_t &crumbs2, bool negative){
    Node Nroot(root);
    push_init_prstate_in_q(q, move(Nroot), move(Nroot), k, seeds1, seeds2, crumbs1, crumbs2, negative);
    for (int i = m - d + 1; i <= m; ++i){
        push_init_prstate_in_q(q, move(Nroot), Node(i), k, seeds1, seeds2, crumbs1, crumbs2, negative);
        push_init_prstate_in_q(q, Node(i), move(Nroot), k, seeds1, seeds2, crumbs1, crumbs2, negative);
    }
    for (int i = m - d + 1; i <= m; ++i)
        for (int j = i; j <= m; ++j)
            push_init_prstate_in_q(q, Node(i), Node(i), k, seeds1, seeds2, crumbs1, crumbs2, negative);

}

inline void push_first_prstates_in_q(priority_queue<Statepr> &q, int m, Trie *root, int d, int k, MatchingKmers &info){
    vector<int> *curseeds1 = &info.seeds1, *curseeds2 = &info.nseeds2;
    crumbs_t *curcrumbs1 = &info.crumbs1, *curcrumbs2 = &info.ncrumbs2;
    push_first_prstates_in_q_starnd(q, m, root, d, k, *curseeds1, *curseeds2, *curcrumbs1, *curcrumbs2, false);
    curseeds1 = &info.seeds2;
    curseeds2 = &info.nseeds1;
    curcrumbs1 = &info.crumbs2;
    curcrumbs2 = &info.ncrumbs1;
    push_first_prstates_in_q_starnd(q, m, root, d, k, *curseeds1, *curseeds2, *curcrumbs1, *curcrumbs2, true);
}

inline void increasecnt(Node &p1, Node &p2, long long &cntexpansions, long long &cntTrieTrieexpansions, long long &cntrefTrieexpansions, long long &cntTrierefexpansions, long long &cntrefrefexpansions){
    ++cntexpansions;
    if (p1.is_in_trie() && p2.is_in_trie())
        ++cntTrieTrieexpansions;
    else if(p1.is_in_trie() && !p2.is_in_trie())
        ++cntTrierefexpansions;
    else if(!p1.is_in_trie() && p2.is_in_trie())
        ++cntrefTrieexpansions;
    else if(!p1.is_in_trie() && !p2.is_in_trie())
        ++cntrefrefexpansions;
    else{
        cerr << "There is no counter for this combination of Nodes\n";
        cerr << "Program is going to abort";
        assert(false);
    }
}

map<tuple<int, Node, Node, bool>, cost_t>& get_expanded_prstates(bool del = false){
    static map<tuple<int, Node, Node, bool>, cost_t> expanded_prstates;
    if (del == true)
        expanded_prstates.clear();
    return expanded_prstates;
}

bool to_explore_pr(int qpos, Node &&p1, Node &&p2, cost_t g, bool negative){
    map<tuple<int, Node, Node, bool>, cost_t> & expanded_prstates = get_expanded_prstates();
    tuple<int, Node, Node, bool> state(qpos, p1, p2, negative);
    if (expanded_prstates.find(state) == expanded_prstates.end()) return true;
    if (expanded_prstates[state] > g) return true;
    return false;
}

inline void expand_state_pr(int qpos, Node &p1, Node &p2, cost_t &g, bool &negative){
    map<tuple<int, Node, Node, bool>, cost_t> & expanded_prstates = get_expanded_prstates();
    tuple<int, Node, Node, bool> state(qpos, p1, p2, negative);
    expanded_prstates[state] = g;
}

bool punish(Node &p1, Node &p2){
    if (p1.is_in_trie() || p2.is_in_trie()) return false;
    int dist = p2.rpos - p1.rpos;
    if(!(punishl <= dist && dist <= punishr)){
        ++cntpunishes;
        return true;
    }
    return false;
}

vector<Statepr> get_next_states_pr(int qpos, Node &p1, Node &p2, int k, char cqpos1, char cqpos2, string &ref,
                                   vector<int> &last, vector<int> &prevpos, vector<int> &seeds1, vector<int> &seeds2,
                                   crumbs_t &crumbs1, crumbs_t &crumbs2){
    vector<Statesr> next1 = move(get_next_states_sr(qpos, p1, cqpos1, ref, k, last, prevpos, seeds1, crumbs1));
    vector<Statesr> next2 = move(get_next_states_sr(qpos, p2, cqpos2, ref, k, last, prevpos, seeds2, crumbs2));
    bool gravail1 = is_gready_available_se(p1, cqpos1, ref);
    bool gravail2 = is_gready_available_se(p2, cqpos2, ref);
    if (gravail1 && !gravail2){
        next1.push_back(createStatesr(qpos + 1, p1, cins, k, seeds1, crumbs1));
        if (p1.rpos < ref.size()) next1.push_back(createStatesr(qpos, p1.rpos + 1, cdel, k, seeds1, crumbs1));
    }
    if (!gravail1 && gravail2){
        next2.push_back(createStatesr(qpos + 1, p2, cins, k, seeds2, crumbs2));
        if (p2.rpos < ref.size()) next2.push_back(createStatesr(qpos, p2.rpos + 1, cdel, k, seeds2, crumbs2));
    }
    vector<Statepr> nextpr;
    Statesr present = createStatesr(qpos, p1, 0, k, seeds1, crumbs1);
    //combines <qpos, p1> with <qpos, p2>'s inheritors
    for (auto i: next2)
        if (present.qpos == i.qpos && !punish(present.p, i.p))
            nextpr.push_back(createStatepr(present.qpos, present.p, i.p, present.g + i.g, present.h + i.h));
    present = createStatesr(qpos, p2, 0, k, seeds2, crumbs2);
    //combines <qpos, p1> with <qpos, p2>'s inheritors
    for (auto i: next1)
        if (present.qpos == i.qpos && !punish(i.p, present.p))
            nextpr.push_back(createStatepr(present.qpos, i.p, present.p, i.g + present.g, i.h + present.h));
    for (auto i: next1)
        for (auto j: next2)
            if (i.qpos == j.qpos && !punish(i.p, j.p))
                nextpr.push_back(createStatepr(i.qpos, i.p, j.p, i.g + j.g, i.h + j.h));
    return nextpr;
}

inline void show_minmaxcost(int &minmaxcost, Statepr &cur, string &query1, string &query2){
    if (minmaxcost < cur.g){
        minmaxcost = cur.g;
        cerr << "New mimimal maximum cost achieved: " << minmaxcost << " h: " << cur.h << " cur.negative: " << cur.negative
        << " cur.qpos: " << cur.qpos << " is in trie: " << cur.p1.is_in_trie() << " is in trie: " << cur.p2.is_in_trie()
        /*<< " query[cur.qpos - 1]: " << query1[cur.qpos - 1] << " " << query2[cur.qpos - 1]*/ << "\n";
    }
}

bool gready_available_pr(string & read1, string &read2, string &ref, int &qpos, Node &p1, Node &p2){
    if (p1.is_in_trie()) return false;
    if (p2.is_in_trie()) return false;
    if (ref.size() <= p1.rpos) return false;
    if (ref.size() <= p2.rpos) return false;
    if (!(read1[qpos] == ref[p1.rpos] || read1[qpos] == special_sign)) return false;
    if (!(read2[qpos] == ref[p2.rpos] || read2[qpos] == special_sign)) return false;
    return true;
}

bool gready_available_pr(Statepr &cur, vector<Statepr> &nextpr, Statepr &heir){
    if (cur.p1.is_in_trie()) return false;
    if (cur.p2.is_in_trie()) return false;
    for (auto i: nextpr){
        if (i.g == 0 && i.qpos == cur.qpos + 1 && i.p1.rpos == cur.p1.rpos + 1 && i.p2.rpos == cur.p2.rpos + 1){
            /*if (i.p1.rpos != cur.p1.rpos + 1){
                cerr << "cur's info: " << cur.qpos << " " << cur.p1.rpos << " " << cur.p2.rpos << " " << cur.g << " " << cur.h << " " << cur.negative << "\n";
                cerr << "i's info: " << i.qpos << " " << i.p1.rpos << " " << i.p2.rpos << " " << i.g << " " << i.h << "\n";
                assert(i.p1.rpos == cur.p1.rpos + 1);
            }
            assert(i.p2.rpos == cur.p2.rpos + 1);*/
            heir = i;
            heir.negative = cur.negative;
            heir.g = cur.g;
            return true;
        }
    }
    return false;
}

Statepr astar_pairend_read_alignment(pair<string, string> &queryp, pair<string, string> &nqueryp, string &ref, int d, int k, Trie *root, MatchingKmers &info){
    int n = queryp.first.size();
    int m = ref.size();
    priority_queue<Statepr> q;
    push_first_prstates_in_q(q, m, root, d, k, info);
    Statepr cur;
    punishl = readdist - drange;
    punishr = readdist + drange;
    long long cntexpansions = 0;
    long long cntTrieTrieexpansions = 0;
    long long cntrefTrieexpansions = 0;
    long long cntTrierefexpansions = 0;
    long long cntrefrefexpansions = 0;
    //auto startt = gettimenow_chrono();
    int minmaxcost = -1;
    while (!q.empty()){
        cur = q.top();
        q.pop();
        /*if (runtimechrono (startt, gettimenow_chrono()) > 7){
            cerr << "Alignment time more than 7 sec. Not alignable sample\n";
            get_expanded_states(true);
            return;
        }*/
        show_minmaxcost(minmaxcost, cur, queryp.first, queryp.second);
        increasecnt(cur.p1, cur.p2, cntexpansions, cntTrieTrieexpansions, cntrefTrieexpansions, cntTrierefexpansions, cntrefrefexpansions);
        if (cur.qpos == n) break;
        if (!to_explore_pr(cur.qpos, move(cur.p1), move(cur.p2), cur.g, cur.negative)) continue;
        expand_state_pr(cur.qpos, cur.p1, cur.p2, cur.g, cur.negative);
        vector<int> *curseeds1, *curseeds2;
        crumbs_t *curcrumbs1, *curcrumbs2;
        string *read1, *read2;
        if (cur.negative){
            read1 = &queryp.second;
            read2 = &nqueryp.first;
            curseeds1 = &info.seeds2;
            curseeds2 = &info.nseeds1;
            curcrumbs1 = &info.crumbs2;
            curcrumbs2 = &info.ncrumbs1;
        }
        else{
            read1 = &queryp.first;
            read2 = &nqueryp.second;
            curseeds1 = &info.seeds1;
            curseeds2 = &info.nseeds2;
            curcrumbs1 = &info.crumbs1;
            curcrumbs2 = &info.ncrumbs2;
        }
        vector<Statepr> nextpr;
        Statepr heir = cur;
        do{
            cur = heir;
            if (cur.qpos == n) break;
            nextpr = move(get_next_states_pr(cur.qpos, cur.p1, cur.p2, k, (*read1)[cur.qpos], (*read2)[cur.qpos],
                                        ref, info.last, info.prevpos, *curseeds1, *curseeds2, *curcrumbs1, *curcrumbs2));
        }while (gready_available_pr(cur, nextpr, heir));
        if (cur.qpos == n) break;
        for (auto i: nextpr){
            i.g += cur.g;
            i.negative = cur.negative;
            q.push(i);
        }
    }
    get_expanded_prstates(true);
    //auto endt = gettimenow_chrono();
    //double aligntime = runtimechrono(startt, endt);
    peevals[cur.g].update_astar_cnts(cntexpansions, cntTrieTrieexpansions, cntTrierefexpansions, cntrefTrieexpansions, cntrefrefexpansions,
                                     (double)cntexpansions / (double) ((int)queryp.first.size() << 1), cntpunishes/*, aligntime*/);
    cerr << "cur.qpos: " << cur.qpos << " cur.negative: " << cur.negative << " cntexpansions: " <<  cntexpansions <<"\n";
    //cerr << "Alignment time: " << aligntime << "s.\n";
    cerr << "Band: " << (double)cntexpansions / (double) ((int)queryp.first.size() << 1) << "\n";
    
    //cout << "Alignmnet time: " << aligntime << "s.\n";
    cout << "Cost: " << cur.g << "\n";
    cout << "Band: " << (double)cntexpansions / (double) ((int)queryp.first.size() << 1) << "\n";
    cout << "End position read1: " << cur.p1.rpos << "\n";
    cout << "End position read2: " << cur.p2.rpos << "\n";
    cout << "Negativity of strand (first read): " << cur.negative << "\n";
    return cur;
}
    /*Statepr cheapest = cur;
    auto startt2 = gettimenow_chrono(); 
    while(!q.empty()){
        cur = q.top();
        q.pop();
        show_minmaxcost(minmaxcost, cur, queryp.first, queryp.second);
        increasecnt(cur.p1, cur.p2, cntexpansions, cntTrieTrieexpansions, cntrefTrieexpansions, cntTrierefexpansions, cntrefrefexpansions);
        if (cur.g + cur.h > cheapest.g) break;
        if (cur.qpos == n) {
            cerr << "New instance: " << cur.qpos << " " << cur.p1.rpos << " " << cur.p2.rpos << "\n";
            continue;
        }
        if (!to_explore_pr(cur.qpos, cur.p1, cur.p2, cur.g, cur.negative)) continue;
        expand_state_pr(cur.qpos, cur.p1, cur.p2, cur.g, cur.negative);
        vector<int> *curseeds1, *curseeds2;
        crumbs_t *curcrumbs1, *curcrumbs2;
        string *read1, *read2;
        if (cur.negative){
            read1 = &queryp.second;
            read2 = &nqueryp.first;
            curseeds1 = &info.seeds2;
            curseeds2 = &info.nseeds1;
            curcrumbs1 = &info.crumbs2;
            curcrumbs2 = &info.ncrumbs1;
        }
        else{
            read1 = &queryp.first;
            read2 = &nqueryp.second;
            curseeds1 = &info.seeds1;
            curseeds2 = &info.nseeds2;
            curcrumbs1 = &info.crumbs1;
            curcrumbs2 = &info.ncrumbs2;
        }
        vector<Statepr> nextpr = move(get_next_states_pr(cur.qpos, cur.p1, cur.p2, k, (*read1)[cur.qpos], (*read2)[cur.qpos],
                                      ref, info.last, info.prevpos, *curseeds1, *curseeds2, *curcrumbs1, *curcrumbs2));
        for (auto i: nextpr){
            i.g += cur.g;
            i.negative = cur.negative;
            q.push(i);
        }
    }
    auto endt2 = gettimenow_chrono();
    auto takentime2 = runtimechrono(startt2, endt2);
    cerr << "Time for finding new instances with cost " << cheapest.g << ": " << takentime2 << "\n";*/
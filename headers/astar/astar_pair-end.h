#pragma once

#include "../header.h"
#include <bits/stdc++.h>
#include "statesstruct.h"
#include "pairend_read_heuristic.h"
//#include "astar_single_reads.h"

using namespace std;

int insdist, drange, innerdist, readdist;
int punishl, punishr;
cost_t infheuristic;
int cntinfhvalues;
/*void calc_innerdist_readdist(int _insdist, int _drange){
    glinsdist = _insdist;
    gldrange = _drange;
    glinnersdist = glinsdist - readsz * 2; 
    int inndist = insdist - readsz*2;///inner distance
    int readdist = inndist + readsz;///distance between reads position with same indexes
}*/

inline void get_seeds_matches_sorted(vector<int> &seeds, vector <int> &lastkmer, vector<int> &prevposkmer, vector<pair<int, int> > &matches){
    /*
    matches.first: position of seed
    matches.second: num of seed
    */
    //static int cnt = 1;
    //cout << "get matches" << cnt << "\n";
    for (int i = 0; i < (int)seeds.size(); ++i){
        int num = seeds[i];
        if (num == -1) continue;
        //cout << "seed[i] == " << num << endl;
        for (int j = lastkmer[num]; j != -1; j = prevposkmer[j]){
            //cout << "   position: " << j << "\n";
            matches.push_back({j, i});
        }
    }
    sort(matches.begin(), matches.end());
    /*cout << "after sorting\n";
    for (auto cur: matches)
        cout << cur.first << " " << cur.second << "\n";
    cout << "\n";*/
    //cnt++;
}

bool is_in_range(int num, int l, int r){
    return (l <= num && num <= r);
}

int upperbs(vector<pair<int, int> > &matches, int pos){
    int l = -1, r = matches.size(), mid;
    ///l - the rightest which is <= than pos
    ///r - the leftestest which is > than pos
    while (l + 1 < r){
        mid = (r - l) / 2 + l;
        if (matches[mid].first > pos)
            r = mid;
        else l = mid;
    }
    return l;
}

int lowerbs(vector<pair<int, int> > &matches, int pos){
    int l = -1, r = matches.size(), mid;
    ///l - the rightest which is < than pos
    ///r - the leftest which is >=  pos
    while (l + 1 < r){
        mid = (r - l) / 2 + l;
        if (matches[mid].first >= pos)
            r = mid;
        else l = mid;
    }
    return r;
}

inline void calc_search_pos(int posseed, int drange, int readdist, int &sposseed, int &curlb, int &currb, bool dir){
    if (dir == true){
        ///posseed is in matches1, and searching in matches2
        sposseed = posseed + readdist;
    }
    else{
        ///posseed is in matches2, and searching in matches1
        sposseed = posseed - readdist;
    }
    curlb = sposseed - drange;
    currb = sposseed + drange;
}

inline void crumb_seed_matches(vector<pair<int, int> > &matches1, vector<pair<int, int> > &matches2, /*int drange, int readdist,*/
                                            vector<unordered_set<int> > &crumbseeds, bool dir){
    /*static int cnt = 1;
    cout << "crumb_seed_matches cnt: " << cnt << endl;*/
    int sz1 = matches1.size();
    int sz2 = matches2.size();
    for (int i = 0; i < sz1; ++i){
        int numseed = matches1[i].second;
        int posseed = matches1[i].first;
        int sposseed, curlb, currb;
        calc_search_pos(posseed, drange, readdist, sposseed, curlb, currb, dir);
        //cout << "numseed: " << numseed << " posseed: " << posseed << " sposseed: " << sposseed << " curlb: " << curlb << " currb: " << currb << "\n";
        int idx = lowerbs(matches2, sposseed);
        if (is_in_range(idx, 0, sz2 - 1) && is_in_range(matches2[idx].first, curlb, currb))
            crumbseeds[numseed].insert(posseed);
        else{
            idx++;
            /*
            It is possiblie that posseed2[idx] places to be out of range compared to posseed1.
            But meanwhile that doesn't make impossible that possed2[idx + 1] is in range compared to posseed1.
            */
            if (is_in_range(idx, 0, sz2 - 1) && is_in_range(matches2[idx].first, curlb, currb))
                crumbseeds[numseed].insert(posseed);
        }
    }
}

inline void show_filter_matches(vector<unordered_set<int> > & crumbseeds){
    static int cnt = 1;
    cout << "show_filter_matches"<<cnt<<":\n";
    int sz = crumbseeds.size();
    for (int i = 0; i < sz; ++i){
        cout << "i == " << i << ":\n";
        for (auto it = crumbseeds[i].begin(); it != crumbseeds[i].end(); ++it)
            cout << "   match: " << *it << "\n";
        cout << "\n";
    }
    cnt++;
}

inline void filter_matches(MatchingKmers &info, /*int  insdist, int drange,*/ int readsz){
    innerdist = insdist - readsz*2;///inner distance
    readdist = innerdist + readsz;///distance between reads position with same indexes
    vector<pair<int, int> > matches1, matches2;
    get_seeds_matches_sorted(info.seeds1, info.lastkmer, info.prevposkmer, matches1);
    get_seeds_matches_sorted(info.seeds2, info.lastkmer, info.prevposkmer, matches2);
    info.crumbseeds1.resize(info.seeds1.size());
    info.crumbseeds2.resize(info.seeds2.size());
    //make_crumbs_appropriate_matches(matches1, matches2, lb, rb, info.crumbseeds1, info.crumbseeds2);
    crumb_seed_matches(matches1, matches2, /*drange, readdist,*/ info.crumbseeds1, true);
    crumb_seed_matches(matches2, matches1, /*drange, readdist,*/ info.crumbseeds2, false);
    /*show_filter_matches(info.crumbseeds1);
    show_filter_matches(info.crumbseeds2);
    cout << "end of the filtering function\n";*/
}

inline void get_crumbs_pairend(string &ref, int d, int k, MatchingKmers &info){
    getcrumbs(ref, d, k, info.crumbs1, info.seeds1, info.backtotrieconnection, info.lastkmer, info.prevposkmer, 1, info.crumbseeds1);
    getcrumbs(ref, d, k, info.crumbs2, info.seeds2, info.backtotrieconnection, info.lastkmer, info.prevposkmer, 2, info.crumbseeds2);
    cerr << "crumbing pairend ended\n";
}

inline void push_first_prstates_in_q(priority_queue<Statepr> &q, int m, Trie *root, int d, int k, MatchingKmers &info){
    cost_t h = pairend_heuristic(0, Node(root), Node(root), k, info);
    cerr << "<0, root, root> heuristic is: " << h << endl;
    Statepr cur = createStatepr(0, root, root, 0, h);
    q.push(cur);
    /*for (int i = m - d + 1; i < m; ++i){
        h = pairend_heuristic(0, Node(root), Node(i), k, info);
        cur = createStatepr(0, root, i, 0, h);
        q.push(cur);
        h = pairend_heuristic(0, Node(i), Node(root), k, info);
        cur = createStatepr(0, i, root, 0, h);
        q.push(cur);
    }
    for (int i = m - d + 1; i < m; ++i){
        for (int j = i; j < m; ++j){
            h = pairend_heuristic(0, Node(i), Node(j), k, info);
            cur = createStatepr(0, i, j, 0, h);
            q.push(cur);
        }
    }*/
}

map<tuple<int, Node, Node>, cost_t>& get_expanded_prstates(bool del = false){
    static map<tuple<int, Node, Node>, cost_t> expanded_prstates;
    if (del == true)
        expanded_prstates.clear();
    return expanded_prstates;
}

bool to_explore_pr(int qpos, Node p1, Node p2, cost_t g){
    //cout << "to explore:\n";
    //cout << "qpos: " << qpos  << " p1.u: " << p1.u << " p1.rpos: " << p1.rpos << " p2.u: " << p2.u << " p2.rpos: " << p2.rpos << " g: " << g << "\n";
    map<tuple<int, Node, Node>, cost_t> &expanded_prstates = get_expanded_prstates();
    tuple<int, Node, Node> state(qpos, p1, p2);
    if (expanded_prstates.find(state) == expanded_prstates.end()){
        //cout << "First expansion of the state\n";
        expanded_prstates[state] = g;
        return true;
    }
    else{
        //cout << "already expanded with value: " << expanded_prstates[state] << "\n";
        if (expanded_prstates[state] > g){
            expanded_prstates[state] = g;
            return true;
        }
        return false;
    }
    assert(false);
}

bool gready_available_pr(pair<string, string> &queryp, string &ref, int qpos, Node p1, Node p2){
    if (p1.is_in_trie() == false && p2.is_in_trie() == false){
        if (ref.size() > p1.rpos && ref.size() > p2.rpos &&
            queryp.first[qpos] == ref[p1.rpos] && queryp.second[qpos] == ref[p2.rpos])
            return true;
    }
    return false;
}

bool punish(Node p1, Node p2, cost_t h1, cost_t h2){
    //it matter which Node is given first as parameter because dist is not taken by absolute value!
    if (h1 + h2 >= infheuristic){
        cntinfhvalues++;
        return true;
    }
    if (p1.is_in_trie() == false && p2.is_in_trie() == false){
        int dist = p2.rpos - p1.rpos;
        //return true;
        if (!is_in_range(dist, punishl, punishr)){
            if (h1 + h2 >= infheuristic)
                cntinfhvalues++;
            return true;
        }
        //else return true;
    }
    return false;
}

vector<Statesr> receivenextsr(int qpos, Node p, char cqpos, string & ref, int k, vector<int> &last, vector<int> &prevpos, vector<int> &seeds, crumbs_t & crumbs){
    vector<Statesr> & buffer = get_next_states_sr(qpos, p, cqpos, ref, k, last, prevpos, seeds, crumbs);
    vector<Statesr> ret = buffer;
    return ret;
}

vector<Statepr>& get_next_pr(int qpos, Node p1, Node p2, int k, pair<string, string> &queryp, string &ref, MatchingKmers &info){
    static vector<Statepr> nextpr;
    nextpr.clear();
    vector<Statesr> v1 = receivenextsr(qpos, p1, queryp.first[qpos], ref, k, info.last, info.prevpos, info.seeds1, info.crumbs1);
    vector<Statesr> v2 = receivenextsr(qpos, p2, queryp.second[qpos], ref, k, info.last, info.prevpos, info.seeds2, info.crumbs2);
    Statesr present = createStatesr(qpos, p1, 0, k, info.seeds1, info.crumbs1);
    for (auto i: v2)
        if (present.qpos == i.qpos && !punish(present.p, i.p, present.h, i.h))
            nextpr.push_back(createStatepr(present.qpos, present.p, i.p, present.g + i.g, present.h + i.h));
    present = createStatesr(qpos, p2, 0, k, info.seeds2, info.crumbs2);
    for (auto i: v1)
        if (present.qpos == i.qpos && !punish(i.p, present.p, i.h, present.h))
            nextpr.push_back(createStatepr(present.qpos, i.p, present.p, i.g + present.g, i.h + present.h));
    for (auto i: v1)
        for (auto j: v2)
            if (i.qpos == j.qpos && !punish(i.p, j.p, i.h, j.h))
                nextpr.push_back(createStatepr(i.qpos, i.p, j.p, i.g + j.g, i.h + j.h));
    return nextpr;
}

cost_t astar_pairend_read_alignment(pair<string, string> &queryp, string &ref, int d, int k, Trie *root, MatchingKmers &info){
    cerr << "in the paired-end alignment function\n";
    cout << "infheuristic value: " << infheuristic << endl;
    int n = queryp.first.size();
    int m = ref.size();
    priority_queue<Statepr> q;
    punishl = readdist - drange;
    punishr = readdist + drange;
    push_first_prstates_in_q(q, m, root, d, k, info);
    Statepr cur;
    int cntexpansions = 0;
    while (!q.empty()){
        cur = q.top();
        q.pop();
        cntexpansions++;
        if (cur.qpos == n)
            break;
        if (to_explore_pr(cur.qpos, cur.p1, cur.p2, cur.g)){
            if (gready_available_pr(queryp, ref, cur.qpos, cur.p1, cur.p2)){
                /*cost_t h = seed_heuristic(cur.qpos + 1, Node(cur.p1.rpos + 1), k, info.seeds1, info.crumbs1) +
                           seed_heuristic(cur.qpos + 1, Node(cur.p2.rpos + 1), k, info.seeds2, info.crumbs2); */
                cost_t h = pairend_heuristic(cur.qpos + 1, Node(cur.p1.rpos + 1), Node(cur.p2.rpos + 1), k, info);
                Statepr topush = createStatepr(cur.qpos + 1, cur.p1.rpos + 1, cur.p2.rpos + 1, cur.g, h);
                q.push(topush);
            }
            else{
                vector<Statepr> &nextpr = get_next_pr(cur.qpos, cur.p1, cur.p2, k, queryp, ref, info);
                for (auto i: nextpr){
                    i.g += cur.g;
                    q.push(i);
                }
            }
        }
    }
    get_expanded_prstates(true);
    cout << "Band: " << cntexpansions / (n * 2) << "\n";
    return cur.g;
}

/*vector<Statepr>& get_next_states_pr(int qpos, Node p1, Node p2, int k, pair<string, string> & queryp, string &ref, MatchingKmers &info){
    static vector<Statepr> nextpr;
    nextpr.clear();
    vector<Statesr>& next1 = get_next_states_sr(qpos, p1, queryp.first[qpos], ref, k, info.last, info.prevpos, info.seeds1, info.crumbs1);
    vector<Statesr>& next2 = get_next_states_sr(qpos, p2, queryp.second[qpos], ref, k, info.last, info.prevpos, info.seeds2, info.crumbs2);
    next1.push_back(createStatesr(qpos, p1, 0, k, info.seeds1, info.crumbs1));
    next2.push_back(createStatesr(qpos, p2, 0, k, info.seeds2, info.crumbs2));
    cout <<"next1.size(): " << next1.size() << " next2.size(): " << next2.size() << "\n";
    for (auto i: next1)
        for (auto j: next2)
            if (i.qpos == j.qpos && !punish(i.p, j.p, i.h, j.h)){
                if (!(i.qpos == qpos && i.p == p1 && j.p == p2))
                    nextpr.push_back(createStatepr(i.qpos, i.p, j.p, i.g + j.g, i.h + j.h));
            }
    return nextpr;
}*/

/*cost_t astar_pairend_read_alignment(pair<string, string> &queryp, string &ref, int d, int k, Trie *root, MatchingKmers &info){
    cerr << "in the paired-end alignment function\n";
    cout << "infheuristic value: " << infheuristic << endl;
    int n = queryp.first.size();
    int m = ref.size();
    punishl = readdist - drange;
    punishr = readdist + drange;
    priority_queue<Statepr> q;
    cerr << "before call of func. for pushing first states\n";
    push_first_prstates_in_q(q, m, root, d, k, info);
    cerr << "afte call of func. for pushing first states\n";
    Statepr cur;
    int maxqpos = -1;
    int cntexpansions = 0;
    int cntshowinheritors = 0;
    cerr << "Exactly before while\n";
    while(!q.empty()){
        cur = q.top();
        q.pop();
        cntexpansions++;
        /*if (cntexpansions % 20 == 0){
            cerr << "A pair-end state qpos: " << cur.qpos << "\n";
        }
        if (cur.qpos > maxqpos){
            maxqpos = cur.qpos;
            cerr << "A new maxqpos value: " << maxqpos << "\n";
        }
        if (cur.qpos % 25 == 0)
            cerr << "cur.qpos: " << cur.qpos << endl;
        if (cur.qpos == n)
            break;
        if (to_explore_pr(cur.qpos, cur.p1, cur.p2, cur.g)){
            if (gready_available_pr(queryp, ref, cur.qpos, cur.p1, cur.p2)){
                //Statepr topush = createStatepr(cur.qpos, cur.p1.rpos + 1, cur.p2.rpos + 1, cur.g, k, info);
                cost_t h = pairend_heuristic(cur.qpos + 1, Node(cur.p1.rpos + 1), Node(cur.p2.rpos + 1), k, info);
                Statepr topush = createStatepr(cur.qpos + 1, cur.p1.rpos + 1, cur.p2.rpos, cur.g, h);
                q.push(topush);
            }
            else{
                vector<Statepr> &nextpr = get_next_states_pr(cur.qpos, cur.p1, cur.p2, k, queryp, ref, info);
                for (auto i: nextpr){
                    
                    i.g += cur.g;
                    if(cntshowinheritors < 100){
                        cout<< "inheritor: qpos: " << i.qpos  << " p1.u: " << i.p1.u << " p1.rpos: " << i.p1.rpos << " p2.u: " << i.p2.u << " p2.rpos: "
                         << i.p2.rpos << " g: " << i.g << " h: " << i.h << "\n";
                    }
                    q.push(i);
                }
                if (cntshowinheritors < 100){
                    cntshowinheritors++;
                    cout << endl;
                }
            }
        }
    }
    get_expanded_prstates(true);
    cout << "Band: " << cntexpansions / (n * 2) << "\n";
    return cur.g;
}*/


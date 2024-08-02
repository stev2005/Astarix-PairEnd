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
int cntfilteredmatche1spr, cntfilteredmatche2spr;
int indaligns;
int occurposlimit;///maximum positions of the min(u->positions.size(), v->positions.size()) in case of we check whether or not we check there is a pair of suitable positions among u.positions and v.positions


inline void get_seeds_matches_sorted(const vector<int> &seeds, const vector <int> &lastkmer, const vector<int> &prevposkmer, vector<pair<int, int> > &matches){
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

int lowerbs(const vector<pair<int, int> > &matches, int pos){
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

int lowerbs(const vector<int> &positions, int pos){
    int l = -1, r = positions.size(), mid;
    ///l - the rightest which is < than pos
    ///r - the leftest which is >=  pos
    while (l + 1 < r){
        mid = (r - l) / 2 + l;
        //mid = (l + r) / 2;
        if (positions[mid] >= pos)
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

/// @brief Computes which of the matches of a seed should be used as crumbs for calculation of the heuristic and /*returns count of legit matches*/()
/// @param matches1 first read's seeds' matches
/// @param matches2 second read's seeds' matches
/// @param crumbseeds hash_sets for saving which seed should be used as crumbs for first seed
/// @param dir says if matches1 are matches of the first seed(true) or the second one (false)(using makes the function universal for both reads)
void crumb_seed_matches(const vector<pair<int, int> > &matches1, const vector<pair<int, int> > &matches2, /*int drange, int readdist,*/
                                            vector<unordered_set<int> > &crumbseeds, bool dir){
    /*static int cnt = 1;
    cout << "crumb_seed_matches cnt: " << cnt << endl;*/
    int sz1 = matches1.size();
    int sz2 = matches2.size();
    //int cntlegitmatcheslocal = 0;
    for (int i = 0; i < sz1; ++i){
        int numseed = matches1[i].second;
        int posseed = matches1[i].first;
        int sposseed, curlb, currb;
        calc_search_pos(posseed, drange, readdist, sposseed, curlb, currb, dir);
        //cout << "numseed: " << numseed << " posseed: " << posseed << " sposseed: " << sposseed << " curlb: " << curlb << " currb: " << currb << "\n";
        int idx = lowerbs(matches2, sposseed);
        if (is_in_range(idx, 0, sz2 - 1) && is_in_range(matches2[idx].first, curlb, currb)){
            crumbseeds[numseed].insert(posseed);
            //cntlegitmatcheslocal++;
        }
        else{
            idx++;
            /*
            It is possiblie that posseed2[idx] places to be out of range compared to posseed1.
            But meanwhile that doesn't make impossible that possed2[idx + 1] is in range compared to posseed1.
            */
            if (is_in_range(idx, 0, sz2 - 1) && is_in_range(matches2[idx].first, curlb, currb)){
                crumbseeds[numseed].insert(posseed);
                //cntlegitmatcheslocal++;
            }
            idx-=2;
            /*
            It is possiblie that posseed2[idx] places to be out of range compared to posseed1.
            But meanwhile that doesn't make impossible that possed2[idx + 1] is in range compared to posseed1.
            */
            if (is_in_range(idx, 0, sz2 - 1) && is_in_range(matches2[idx].first, curlb, currb)){
                crumbseeds[numseed].insert(posseed);
                //cntlegitmatcheslocal++;
            }
        }
    }
    //return cntlegitmatcheslocal;
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

inline void filter_matches(MatchingKmers &info, /*int  insdist, int drange,*/ const int readsz){
    innerdist = insdist - readsz*2;///inner distance
    readdist = innerdist + readsz;///distance between reads position with same indexes
    vector<pair<int, int> > matches1, matches2;
    get_seeds_matches_sorted(info.seeds1, info.lastkmer, info.prevposkmer, matches1);
    get_seeds_matches_sorted(info.seeds2, info.lastkmer, info.prevposkmer, matches2);
    /*cout << "Number of seeds' mathces per number of seeds for read1: " << (double) matches1.size() / (double) info.seeds1.size() << "\n";
    cout << "Number of seeds' mathces per number of seeds for read2: " << (double) matches2.size() / (double) info.seeds2.size() << "\n";
    cout << "Number of seeds' matches per nuumber of seeds for both reads" <<
        (double) (matches1.size() + matches2.size()) / (double) (info.seeds1.size() + info.seeds2.size()) << "\n";*/
    evalsts.matchesperseed +=  (double) (matches1.size() + matches2.size()) / (double) (info.seeds1.size() + info.seeds2.size());
    info.crumbseeds1.resize(info.seeds1.size());
    info.crumbseeds2.resize(info.seeds2.size());
    //make_crumbs_appropriate_matches(matches1, matches2, lb, rb, info.crumbseeds1, info.crumbseeds2);
    crumb_seed_matches(matches1, matches2, /*drange, readdist,*/ info.crumbseeds1, true);
    crumb_seed_matches(matches2, matches1, /*drange, readdist,*/ info.crumbseeds2, false);
    cntfilteredmatche1spr = 0;
    cntfilteredmatche2spr = 0;
    for (auto i: info.crumbseeds1)
        cntfilteredmatche1spr += i.size();
    for (auto i: info.crumbseeds2)
        cntfilteredmatche2spr += i.size();
    /*cout << "Number of seeds' filtered mathces per number of seeds for read1: " << (double) cntfilteredmatche1spr / (double) info.seeds1.size() << "\n";
    cout << "Number of seeds' filtered mathces per number of seeds for read2: " << (double) cntfilteredmatche2spr / (double) info.seeds2.size() << "\n";
    cout << "Number of seeds' filtered matches per nuumber of seeds for both reads" <<
        (double) (cntfilteredmatche1spr + cntfilteredmatche2spr) / (double) (info.seeds1.size() + info.seeds2.size()) << "\n";*/
    evalsts.legitmatchesperseed += (double) (cntfilteredmatche1spr + cntfilteredmatche2spr) / (double) (info.seeds1.size() + info.seeds2.size());
    /*show_filter_matches(info.crumbseeds1);
    show_filter_matches(info.crumbseeds2);
    cout << "end of the filtering function\n";*/
}

inline void get_crumbs_pairend(string &ref, int d, int k, MatchingKmers &info){
    cerr << "Begin of get_crumbs_paired end\n";
    /*int cntsetcrumbs1 =*/ getcrumbs(ref, d, k, info.crumbs1, info.seeds1, info.backtotrieconnection, info.lastkmer, info.prevposkmer, 1/*, info.crumbseeds1*/);
    cerr << "Here1\n";
    /*int cntsetcrumbs2 = */getcrumbs(ref, d, k, info.crumbs2, info.seeds2, info.backtotrieconnection, info.lastkmer, info.prevposkmer, 2/*, info.crumbseeds2*/);
    cerr << "Here2\n";
    /*cout << "Number of crumbs per number of filtered matches for read1: " << (double) cntsetcrumbs1 / (double) cntfilteredmatche1spr << "\n";
    cout << "Number of crumbs per number of filtered matches for read2: " << (double) cntsetcrumbs2 / (double) cntfilteredmatche2spr << "\n";
    cout << "Number of crumbs per number of filtered matches for bith reads: " <<
        (double) (cntsetcrumbs1 + cntsetcrumbs2) / (double) (cntfilteredmatche1spr + cntfilteredmatche2spr) << "\n";*/
    //evalsts.crumbsperlegitmatch += (double) (cntsetcrumbs1 + cntsetcrumbs2) / (double) (cntfilteredmatche1spr + cntfilteredmatche2spr);
    cerr << "crumbing pairend ended\n";
}

inline void get_crumbs_pairedend_trie_opt(string &ref, int d, int k, MatchingKmers &info){
    getcrumbs_trieopt(ref, d, k, info.crumbs1, info.seeds1, info.backtotrieconnection, info.lastkmer, info.prevposkmer/*, info.last, info.prevpos*/, 1/*, info.crumbseeds1*/);
    getcrumbs_trieopt(ref, d, k, info.crumbs2, info.seeds2, info.backtotrieconnection, info.lastkmer, info.prevposkmer/*, info.last, info.prevpos*/, 2/*, info.crumbseeds2*/);
}

inline void push_first_prstates_in_q(priority_queue<Statepr> &q, int m, Trie *root, int d, int k, MatchingKmers &info){
    cost_t h = pairend_heuristic(0, Node(root), Node(root), k, info);
    //cerr << "<0, root, root> heuristic is: " << h << endl;
    Statepr cur = createStatepr(0, root, root, 0, h);
    q.push(cur);
    for (int i = m - d + 1; i < m; ++i){
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
    }
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
    //if (p1.is_in_trie() == false && p2.is_in_trie() == false){
    if (!p1.is_in_trie() && !p2.is_in_trie()){
        if (ref.size() > p1.rpos && ref.size() > p2.rpos &&
            queryp.first[qpos] == ref[p1.rpos] && queryp.second[qpos] == ref[p2.rpos])
            return true;
    }
    return false;
}

bool old_punish(Node p1, Node p2, cost_t h1, cost_t h2){
    //it matters which Node is given first as parameter because dist is not taken by absolute value!
    /*if (h1 + h2 >= infheuristic){
        cntinfhvalues++;
        return true;
    }*/
    //if (p1.is_in_trie() == false && p2.is_in_trie() == false){
    if (!p1.is_in_trie() && !p2.is_in_trie()){
        int dist = p2.rpos - p1.rpos;
        if (!is_in_range(dist, punishl, punishr)){
            //if (h1 + h2 >= infheuristic)
                cntinfhvalues++;
            return true;
        }
    }
    return false;
}

///inline void calc_search_pos(int posseed, int drange, int readdist, int &sposseed, int &curlb, int &currb, bool dir)

bool punishtwotries(Node p1, Node p2){
    //return false;
    const vector <int> & positions1 = p1.u->positions;
    const vector <int> & positions2 = p2.u->positions;
    int sz1 = positions1.size();
    int sz2 = positions2.size();
    if (sz1 > occurposlimit)
        return false;
    if (sz2 > occurposlimit)
        return false;
    int pointer1 = 0, pointer2 = 0;
    while (pointer1 < sz1 && pointer2 < sz2){
        int dist = positions2[pointer2] - positions1[pointer1];
        if (dist < punishl)
            pointer2++;
        else if (dist > punishr)
            pointer1++;
        else return false;
    }
    return true;
    /*const vector <int> & positions1 = p1.u->positions;
    const vector <int> & positions2 = p2.u->positions;
    int sz1 = positions1.size();
    int sz2 = positions2.size();
    if (sz1 > occurposlimit)
        return false;
    if (sz2 > occurposlimit)
        return false;
    for (auto pos: positions1)
        for (auto pos2: positions2){
            int dist = pos2 - pos;
            if (is_in_range(dist, punishl, punishr))
                return false;
        }*/
    /*bool dir = true;//True if p1 corresponds to the left alignment and p2 - to the right one. False - vice versa
    if (p1.u->positions.size() > p2.u->positions.size()){
        swap(p1, p2);
        dir = false;
    }
    const vector <int> & positions1 = p1.u->positions;
    const vector <int> & positions2 = p2.u->positions;
    int sz1 = positions1.size();
    int sz2 = positions2.size();
    if (sz1 > occurposlimit)
        return false;
    for (auto pos: positions1){
        int searchpos;
        if (dir) searchpos = pos + readdist;
        else searchpos = pos - readdist;
        int idx = lowerbs(positions2, searchpos);
        if (is_in_range(idx, 0, sz2 - 1)){
            int dist = abs(pos - positions2[idx]);
            if (is_in_range(dist, punishl, punishr))
                return false;
        }
        idx--;
        if (is_in_range(idx, 0, sz2 - 1)){
            int dist = abs(pos - positions2[idx]);
            if (is_in_range(dist, punishl, punishr))
                return false;
        }
    }*/
    return true;
}

bool punishonetrieoneref(Node p1, Node p2){
    return false;
    bool dir = true;
}

bool punishtworef(Node p1, Node p2){
    int dist = p2.rpos - p1.rpos;
    if (!is_in_range(dist, punishl, punishr)){
        cntinfhvalues++;
        return true;
    }
    return false;
}

bool punish(Node p1, Node p2){
    if (p1.is_in_trie() && p2.is_in_trie())
        return punishtwotries(p1, p2);
    if (!p1.is_in_trie() && !p2.is_in_trie())
        return punishtworef(p1, p2);
    return punishonetrieoneref(p1, p2);

    /*if (p1.is_in_trie()^p2.is_in_trie())
        
    
    cerr << "In function punish(Node p1, Node p2) none of the checks has been passed. Program is going to abort\n";
    assert(false);*/
}

vector<Statesr> receivenextsr(int qpos, Node p, char cqpos, string & ref, int k, vector<int> &last, vector<int> &prevpos, vector<int> &seeds, crumbs_t & crumbs){
    vector<Statesr> & buffer = get_next_states_sr(qpos, p, cqpos, ref, k, last, prevpos, seeds, crumbs);
    vector<Statesr> ret = buffer;
    return ret;
}

//gets inheritors of single-read states <qpos, p1> and <qpos, p2>.
//Then combines them resulting in a list of inheritors of paired-end state <qpos, p1, p2>
vector<Statepr>& get_next_pr(int qpos, Node p1, Node p2, int k, pair<string, string> &queryp, string &ref, MatchingKmers &info){
    static vector<Statepr> nextpr;
    nextpr.clear();
    //v1, v2, list of inheritors of the single-read states
    vector<Statesr> v1 = receivenextsr(qpos, p1, queryp.first[qpos], ref, k, info.last, info.prevpos, info.seeds1, info.crumbs1);
    vector<Statesr> v2 = receivenextsr(qpos, p2, queryp.second[qpos], ref, k, info.last, info.prevpos, info.seeds2, info.crumbs2);
    //the two not nested fors are combing 
    Statesr present = createStatesr(qpos, p1, 0, k, info.seeds1, info.crumbs1);
    //combines <qpos, p1> with <qpos, p2>'s inheritors
    for (auto i: v2)
        if (present.qpos == i.qpos && !punish(present.p, i.p))
            nextpr.push_back(createStatepr(present.qpos, present.p, i.p, present.g + i.g, present.h + i.h));
    present = createStatesr(qpos, p2, 0, k, info.seeds2, info.crumbs2);
    //combines <qpos, p2> with <qpos, p1>'s inheritors
    for (auto i: v1)
        if (present.qpos == i.qpos && !punish(i.p, present.p))
            nextpr.push_back(createStatepr(present.qpos, i.p, present.p, i.g + present.g, i.h + present.h));
    //combines <qpos, p1>'s inheritors with <qpos, p2>'s inheritors
    for (auto i: v1)
        for (auto j: v2)
            if (i.qpos == j.qpos && !punish(i.p, j.p))
                nextpr.push_back(createStatepr(i.qpos, i.p, j.p, i.g + j.g, i.h + j.h));
    return nextpr;
}

inline void increasecnt(int qpos, Node p1, Node p2, long long &cntexpansions, long long &cntTrieTrieexpansions, long long &cntrefTrieexpansions, long long &cntTrierefexpansions, long long &cntrefrefexpansions){
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

cost_t astar_pairend_read_alignment(pair<string, string> &queryp, string &ref, int d, int k, Trie *root, MatchingKmers &info){
    //cerr << "in the paired-end alignment function\n";
    //cout << "infheuristic value: " << infheuristic << endl;
    int n = queryp.first.size();
    int m = ref.size();
    cntinfhvalues = 0;
    priority_queue<Statepr> q;
    punishl = readdist - drange;
    punishr = readdist + drange;
    push_first_prstates_in_q(q, m, root, d, k, info);
    Statepr cur;
    long long cntexpansions = 0;
    long long cntTrieTrieexpansions = 0;
    long long cntrefTrieexpansions = 0;
    long long cntTrierefexpansions = 0;
    long long cntrefrefexpansions = 0;

    int minmaxcost = -1;

    while (!q.empty()){
        cur = q.top();
        q.pop();
        //cntexpansions++;
        increasecnt(cur.qpos, cur.p1, cur.p2, cntexpansions, cntTrieTrieexpansions, cntrefTrieexpansions, cntTrierefexpansions, cntrefrefexpansions);
        if (cur.qpos == n)
            break;

        /*if (minmaxcost < cur.g){
            minmaxcost = cur.g;
            cerr << "New mimimal maximum cost achieved: " << minmaxcost << " cur.qpos: " << cur.qpos
            << " in the trie <u, v>: " << (cur.p1.is_in_trie() & cur.p2.is_in_trie())
            << " cur.negative: " << cur.negative << "\n";
        }*/

        if (to_explore_pr(cur.qpos, cur.p1, cur.p2, cur.g)){
            if (gready_available_pr(queryp, ref, cur.qpos, cur.p1, cur.p2)){
                /*cost_t h = seed_heuristic(cur.qpos + 1, Node(cur.p1.rpos + 1), k, info.seeds1, info.crumbs1) +
                           seed_heuristic(cur.qpos + 1, Node(cur.p2.rpos + 1), k, info.seeds2, info.crumbs2); */
                cost_t h = pairend_heuristic(cur.qpos + 1, Node(cur.p1.rpos + 1), Node(cur.p2.rpos + 1), k, info);
                Statepr nextstate = createStatepr(cur.qpos + 1, cur.p1.rpos + 1, cur.p2.rpos + 1, cur.g, h);
                q.push(nextstate);
            }
            else{
                vector<Statepr> &nextpr = get_next_pr(cur.qpos, cur.p1, cur.p2, k, queryp, ref, info);
                for (auto i: nextpr){
                    i.g += cur.g;
                    //if (i.g + i.h < 15) cerr << "inheritor: qpos: " << i.qpos << " g: " << i.g << " h:" << i.h << "\n";
                    q.push(i);
                }
            }
        }
    }
    get_expanded_prstates(true);
    /*cout << "Expanded states:" << cntexpansions << "\n";
    cout << "Expanded states <qpos, Trie*, Trie*>: " << cntTrieTrieexpansions << "\n";
    cout << "Expanded states <qpos, Trie*, Trie*> (% all states ): " << (double)cntTrieTrieexpansions / (double) cntexpansions * (double) 100<< "%\n";    
    cout << "Expanded states <qpos, Trie*, ref>: " << cntTrierefexpansions << "\n"; 
    cout << "Expanded states <qpos, Trie*, ref> (% all states): " << (double) cntTrierefexpansions / (double) cntexpansions * (double) 100<< "%\n"; 
    cout << "Expanded states <qpos, ref, Trie*>: " << cntrefTrieexpansions << "\n";
    cout << "Expanded states <qpos, ref, Trie*> (% all states): " << (double) cntrefTrieexpansions / (double) cntexpansions * (double) 100<< "%\n";
    cout << "Expanded states <qpos, ref, ref>: " << cntrefrefexpansions << "\n";
    cout << "Expanded states <qpos, ref, ref> (% all states): " << (double) cntrefrefexpansions / (double) cntexpansions * (double) 100 << "%\n";*/  
    cout << "Band: " << (double) cntexpansions / (double)(n * 2) << "\n";
    cout << "Times heuristic is infinity: " << cntinfhvalues << "\n";
    evalsts.update_astar_cnts(cntexpansions, cntTrieTrieexpansions, cntTrierefexpansions, cntrefTrieexpansions, cntrefrefexpansions, (double) cntexpansions / (double)(n * 2), cntinfhvalues);
    double perTrieTrie = (double)cntTrieTrieexpansions / (double) cntexpansions * (double) 100;
    double perTrieref = (double) cntTrierefexpansions / (double) cntexpansions * (double) 100;
    double perrefTrie = (double) cntrefTrieexpansions / (double) cntexpansions * (double) 100;
    double perrefref = (double) cntrefrefexpansions / (double) cntexpansions * (double) 100;
    evalsts.update_astar_percentages(perTrieTrie, perTrieref, perrefTrie, perrefref);
    return cur.g;
}

/*cost_t astar_pairend_read_alignment_independent(pair<string, string> &queryp, string &ref, int d, int k, Trie *root, MatchingKmers &info, int numaligns){
    vector<pair<cost_t, int> > v1, v2;
    info.seeds = info.seeds1;
    info.crumbs = info.crumbs1;
    v1 = astar_single_read_alignment(queryp.first, ref, d, k, root, info, numaligns);
    info.seeds = info.seeds2;
    info.crumbs = info.crumbs2;
    v2 = astar_single_read_alignment(queryp.second, ref, d, k, root, info, numaligns);
    cost_t g = (1<<30);
    punishl = readdist - drange;
    punishr = readdist + drange;
    for (auto cur1: v1)
        for (auto cur2: v2){
            int dist = cur2.second - cur1.second;
            if (is_in_range(dist, punishl, punishr)){
                if (cur1.first + cur2.first < g)
                    g = cur1.first + cur2.first;
            }
        }
    return g;
}*/
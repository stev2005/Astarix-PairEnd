#pragma once

#include "../header.h"
#include <bits/stdc++.h>
#include "../trie.h"
#include "astar_single_reads.h"

using namespace std;

struct Statepr{
    int qpos;
    Node p1;
    Node p2;
    cost_t g;
    cost_t h;
    Statepr(){}
    Statepr(int _qpos, Node _p1, Node _p2){
        qpos = _qpos;
        p1 = _p1;
        p2 = _p2;
        g = 0;
        h = 0;
    }
    Statepr(int _qpos, Node _p1, Node _p2, cost_t _g, cost_t _h){
        qpos = _qpos;
        p1 = _p1;
        p2 = _p2;
        g = _g;
        h = _h;
    }
    bool operator<(const Statepr &other) const{//only for priority_queue
        return g + h >  other.g + other.h;
    }
};

void get_matches(MatchingKmers &info, vector<pair<int, int> > & matches, int alignment){
    vector<int> & lastkmer = info.lastkmer;
    vector<int> & prevposkmer = info.prevposkmer;
    vector<int> seeds = (alignment == 1)? info.seeds1: info.seeds2;
    for (int i = 0; i < seeds.size(); ++i){
        if (seeds[i] == -1) continue;
        for (int j = lastkmer[seeds[i]]; j != -1; j = prevposkmer[j])
            matches.push_back({j, i});
    }
}

void filter_matches_a_seed(vector<pair<int, int> > &matches1, vector<pair<int, int> > &matches2,
vector<unordered_set<int> > &crumbseeds, int dmatch, bool leftdir, bool rightdir){
    int sz1 = matches1.size();
    int sz2 = matches2.size();
    int pos1, pos2;
    if (rightdir == true){
        pos1 = 0;
        pos2 = 0;
        while(pos1 < sz1 && pos2 < sz2){
            pair<int, int> cur1 = matches1[pos1];
            pair<int, int> cur2 = matches2[pos2];
            if (cur1.first > cur2.first)
                pos2++;
            else{
                if (cur2.first - cur1.first <= dmatch){
                    crumbseeds[cur1.second].insert(cur1.first);
                    //cerr << "crumbseed match found\n";
                }
                pos1++;
            }
        }
    }
    if (leftdir == true){
        pos1 = sz1 - 1;
        pos2 = sz2 - 1;
        while (pos1 >= 0 && pos2 >= 0){
            pair<int, int> cur1 = matches1[pos1];
            pair<int, int> cur2 = matches2[pos2];
            if (cur1 < cur2)
                pos2--;
            else{
                if (cur1.first - cur2.first <= dmatch){
                    crumbseeds[cur1.second].insert(cur1.first);
                    //cerr << "crumbseed match found\n";
                }
                pos1--;
            }
        }
    }
}


void filter_matches(MatchingKmers &info, int k, int dmatch){
    //cerr << "in filter matches\n";
    info.crumbseeds1.resize(info.seeds1.size());
    info.crumbseeds2.resize(info.seeds2.size());
    vector<pair<int, int> > matches1;///first-position where a match is; second- num of order of a seed
    get_matches(info, matches1, 1);
    //cerr << "seed1 matches got\n";
    vector<pair<int, int> > matches2;
    get_matches(info, matches2, 2);
    sort(matches1.begin(), matches1.end());
    sort(matches2.begin(), matches2.end());
    /*cerr << "matches both seeds sorted\n";
    cout << "matches1.size() == "<<matches1.size() << " matches2.size() == "<< matches2.size() << "\n";
    cout << "mathces1:\n";
    for (auto i: matches1)
        cout << i.first << " " << i.second << "\n";
    cout << "\n";
    cout << "matches2:\n";
    for (auto i: matches2)
        cout << i.first << " "<< i.second <<"\n";
    cout << "\n";*/
    filter_matches_a_seed(matches1, matches2, info.crumbseeds1, dmatch, true, true);
    filter_matches_a_seed(matches2, matches1, info.crumbseeds2, dmatch, true, true);
}

void howmanycrumbs_seeds_have(MatchingKmers & info, int k){
    cout << "In howmanycrumbs_seeds_have:\n";
    vector<int> & last = info.lastkmer;
    vector<int> & prevpos = info.prevposkmer;
    vector<int> & seeds1 = info.seeds1;
    cout <<"first alignmetn: \n";
    for (int i = 0; i < seeds1.size(); ++i){
        int cnt = 0;
        for (int j = last[seeds1[i]]; j != -1; j = prevpos[j])
            cnt++;
        cout << "For seed[" << i<<"] there are "<< cnt <<" matches and " <<info.crumbseeds1[i].size() << "available.\n";
    }
    cout << endl;
    vector <int> & seeds2 = info.seeds2;
    cout <<"second alignmetn: \n";
    for (int i = 0; i < seeds2.size(); ++i){
        int cnt = 0;
        for (int j = last[seeds2[i]]; j != -1; j = prevpos[j])
            cnt++;
        cout << "For seed[" << i<<"] there are "<< cnt <<" matches and " <<info.crumbseeds2[i].size() << "available.\n";
    }
    cout << endl;
}

inline void getcrumbs_pairend(string &ref, int d, int k, MatchingKmers &info){
    clock_t t;
    t = clock();
    getcrumbs(ref, d, k, info, 1);
    t = clock() - t;
    cout << "Precompute of crumbs1: " << (double) t / CLOCKS_PER_SEC << "s.\n";
    t = clock();
    getcrumbs(ref, d, k, info, 2);
    t = clock() - t;
    cout << "Precompute of crumbs2: " << (double) t / CLOCKS_PER_SEC << "s.\n";
}

cost_t pairend_heuristic(Statesr one, Statesr two, char *heuristic_method, int dmatch){
    if (strcmp(heuristic_method, "dijkstra_heuristic") == 0) return 0;
    if (strcmp(heuristic_method, "seed_heuristic") == 0){
        if (!one.p.is_in_trie() && !two.p.is_in_trie()){
            if (abs(one.p.rpos - two.p.rpos) > dmatch*2)
                return inf;
        }
        return one.h + two.h;
    }
    assert(false);
}

Statepr CreateStatepr(Statesr one, Statesr two, char *heuristic_method, int dmatch){
    return Statepr(one.qpos, one.p, two.p, 0, pairend_heuristic(one, two, heuristic_method, dmatch));
}

auto mapcmpStatepr = [] (Statepr cur, Statepr other){
    if (cur.qpos != other.qpos) return cur.qpos < other.qpos;
    if (!(cur.p1 == other.p1)) return cur.p1 < other.p1;
    return cur.p2 < other.p2;
};

bool toexplorepairend(map<Statepr, cost_t, decltype(mapcmpStatepr)> & expandedstatespr, Statepr cur){
    Statepr mapchech = Statepr(cur.qpos, cur.p1, cur.p2);
    auto it = expandedstatespr.find(mapchech);
    if (it == expandedstatespr.end()){
        expandedstatespr[mapchech] = cur.g;
        return true;
    }
    else if (it->second > cur.g){
        it->second = cur.g;
        return true;
    }
    return false;
}

/*void print_memory(){
    static ofstream 
    static clock_t prev = clock();
    clock_t cur = clock();
    clock_t cldiff = cur - prev;
    double diff = diff / CLOCKS_PER_SEC;
    if (diff > 10 )
}*/

cost_t astar_pairend_read_alignment(pair<string, string> &query, string &ref, int d, int k, Trie *root, MatchingKmers &info, char *heuristic_method, char *showcntexplstates, char *triestart, int dmatch){
    string q1 = query.first;///left end of query;
    string q2 = query.second;///right end of query;
    int n = q1.size();
    int m = ref.size();
    Statepr cur;
    priority_queue <Statepr> q;
    //set < pair<int, pair<Node, Node> > > visited;
    if (strcmp(triestart, "Yes") == 0){
        Statesr one = CreateStatesr(Statesr(0, Node(root)), k, info, heuristic_method, 0, 1);
        Statesr two = CreateStatesr(Statesr(0, Node(root)), k, info, heuristic_method, 0, 2);
        //cout << "heuristic of single read: "<< one.h << endl;
        cur = CreateStatepr(one, two, heuristic_method, dmatch);
        cout << "heuristic of paired end state<0, rootdmer, rootdmer>: "<<cur.h<<endl;
        q.push(cur);
        for (int i = m - d + 1; i <= m; ++i){
            one = CreateStatesr(Statesr(0, Node(i)), k, info, heuristic_method, 0, 1);
            two = CreateStatesr(Statesr(0, Node(root)), k, info, heuristic_method, 0, 2);
            cur = CreateStatepr(one, two, heuristic_method, dmatch);
            q.push(cur);
            one = CreateStatesr(Statesr(0, Node(root)), k, info, heuristic_method, 0, 1);
            two = CreateStatesr(Statesr(0, Node(i)), k, info, heuristic_method, 0, 2);
            cur = CreateStatepr(one, two, heuristic_method, dmatch);
            q.push(cur);
        }
        for (int i = m - d + 1; i <= m; ++i)
            for (int j = m - d + 1; j <= m; ++j){
                one = CreateStatesr(Statesr(0, Node(i)), k, info, heuristic_method, 0, 1);
                two = CreateStatesr(Statesr(0, Node(j)), k, info, heuristic_method, 0, 2);
                cur = CreateStatepr(one, two, heuristic_method, dmatch);
                q.push(cur);
            }
    }
    else{
        for (int i = 0; i <= m; ++i)
            for (int j = 0; j <= m; ++j){
                Statesr one = CreateStatesr(Statesr(0, Node(i)), k, info, heuristic_method, 0, 1);
                Statesr two = CreateStatesr(Statesr(0, Node(j)), k, info, heuristic_method, 0, 2);
                cur = CreateStatepr(one, two, heuristic_method, dmatch);
                q.push(cur);
            }
    }
    map<Statepr, cost_t, decltype(mapcmpStatepr)> expandedstatespr (mapcmpStatepr);
    int cntexpansions = 0;
    int cntreturninf = 0;
    while (!q.empty()){
        //cout << "q has elements\n";
        cur = q.top();
        q.pop();
        cntexpansions++;
        //cout << "Poping from the queue: " << "cur.qpos == "<< cur.qpos<< " p1.is_in_trie:" << cur.p1.is_in_trie()<< " p2.is_in_trie: "<< cur.p2.is_in_trie() << " " << cur.g << " " << cur.h << " " << cur.h + cur.g << "\n";
        if (cur.qpos == n) break;
        if (toexplorepairend(expandedstatespr, cur)){
            //cout << "Exploring this state\n";
            //visited.insert({cur.qpos, {cur.p1, cur.p2}});
            if (is_greedy_available(Statesr(cur.qpos, cur.p1), q1, ref) && is_greedy_available(Statesr(cur.qpos, cur.p2), q2, ref)){    
                    Statesr one = CreateStatesr(Statesr(cur.qpos+1, Node(cur.p1.rpos+1)), k, info, heuristic_method, 0, 1);
                    Statesr two = CreateStatesr(Statesr(cur.qpos+1, Node(cur.p2.rpos+1)), k, info, heuristic_method, 0, 2);
                    Statepr topush = CreateStatepr(one, two, heuristic_method, dmatch);
                    /*topush.g1 = cur.g1;
                    topush.g2 = cur.g2;*/
                    topush.g += cur.g;
                    q.push(topush); 
                }
            else{
                vector<Statesr> next1 = NextStatesr(Statesr(cur.qpos, cur.p1), q1[cur.qpos], ref, k, info, heuristic_method, 1);
                vector<Statesr> next2 = NextStatesr(Statesr(cur.qpos, cur.p2), q2[cur.qpos], ref, k, info, heuristic_method, 2);
                next1.push_back(CreateStatesr(Statesr(cur.qpos, cur.p1), k, info, heuristic_method, 0, 1));
                next2.push_back(CreateStatesr(Statesr(cur.qpos, cur.p2), k, info, heuristic_method, 0, 2));
                //cout << "next1.size() = " << next1.size() << " next2.size() = " << next2.size() << endl; 
                for (auto i1: next1)
                    for (auto i2: next2)
                        if (i1.qpos == i2.qpos){
                            Statepr next = CreateStatepr(i1, i2, heuristic_method, dmatch);
                            /*next.g1 = cur.g1 + i1.stepcost;
                            next.g2 = cur.g2 + i2.stepcost;*/
                            if (next.h == inf)
                                cntreturninf++;
                            else{
                                next.g += cur.g + i1.stepcost + i2.stepcost;
                                q.push(next);
                            }
                        }
            }
        }
    }
    cout << cur.qpos << " n = " << n << " q.size() = " << q.size() << endl;
    assert (cur.qpos == n);
    cout << "Number of heuristic funct. returning inf: " << cntreturninf << "\n";
    if (strcmp(showcntexplstates, "Yes") == 0)
        cout << "Expanded states == " << cntexpansions << "\n";
    cout << "Band: " << cntexpansions / n << "(fraction floored)\n";

    return cur.g;
}
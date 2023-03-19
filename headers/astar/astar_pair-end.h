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
    bool operator<(const Statepr &other) const{
        return g + h >  other.g + other.h;
    }
};

void get_matches(MatchingKmers &info, vector<pair<int, int> > & matches, int alignment){
    vector<int> & last = info.last;
    vector<int> & prevpos = info.prevpos;
    vector<int> seeds = (alignment == 1)? info.seeds1: info.seeds2;
    for (int i = 0; i < seeds.size(); ++i)
        for (int j = last[seeds[i]]; j != -1; j = prevpos[j])
            matches.push_back({j, i});
}

void filter_matches(MatchingKmers &info, int k){
    vector<pair<int, int> > matches1;
    get_matches(info, matches1, 1);
    vector<pair<int, int> > matches2;
    get_matches(info, matches2, 2);
    sort(matches1.begin(), matches1.end());
    sort(matches2.begin(), matches2.end());
    /*cout << "matches1.size() == "<<matches1.size() << " matches2.size() == "<< matches2.size() << "\n";
    cout << "mathces1:\n";
    for (auto i: matches1)
        cout << i.first << " " << i.second << "\n";
    cout << "\n";
    cout << "matches2:\n";
    for (auto i: matches2)
        cout << i.first << " "<< i.second <<"\n";
    cout << "\n";*/
    int l = 0, sz1 = matches1.size();
    int r = 0, sz2 = matches2.size();
    vector <unordered_set<int> > & seedsph1 = info.seedsph1;
    seedsph1.resize(info.seeds1.size());
    vector <unordered_set<int> > & seedsph2 = info.seedsph2;
    seedsph2.resize(info.seeds2.size());
    int last1 = -10001, last2 = -10001;///the lastesr occurance of a seed from each alignment
    while (l < sz1 && r < sz2){
        pair<int, int> cur;
        if (matches1[l].first < matches2[r].first){
            cur = matches1[l];
            //last1 = cur.first;
            l++;
            //if (abs(cur.first - last2) <= 10000)
            if (r > 0 && abs(cur.first - matches2[r-1].first) <= 10000)
                seedsph1[cur.second].insert(cur.first);
        }
        else{
            cur = matches2[r];
            r++;
            //last2 = cur.first;
            //if (abs(cur.second - last1) <= 10000)
            if (l > 0 && abs(cur.first - matches1[l-1].first) <= 10000)
                seedsph2[cur.second].insert(cur.first);
        }
    }
    while (l < sz1){
        pair<int, int> cur;
        cur = matches1[l];
        l++;
        if (abs(cur.first - matches2[r-1].first) <= 10000)
            seedsph1[cur.second].insert(cur.first);
        else break;
    }
    while (r < sz2){
        pair<int, int> cur;
        cur = matches2[r];
        r++;
        if (abs(cur.first - matches1[l-1].first) <= 10000)
            seedsph2[cur.second].insert(cur.first);
        else break;
    }
    last1 = inf;
    last2 = inf;
    l = sz1 - 1;
    r = sz2 - 1;
    while(l >= 0 && r >= 0){
        pair<int, int> cur;
        if (matches1[l].first > matches2[r].first){
            cur = matches1[l];
            l--;
            last1 = cur.first;
            if (r + 1 < sz2 && abs(cur.first - matches2[r + 1].first) <= 10000)
                seedsph1[cur.second].insert(cur.first);
        }
        else{
            cur = matches2[r];
            r--;
            last2 = cur.second;
            if (l + 1 < sz1 && abs(cur.first - matches1[l + 1].first) <= 10000)
                seedsph2[cur.second].insert(cur.first);
        }
    }
    while (l >= 0){
        pair<int, int> cur;
        cur = matches1[l];
        l--;
        if (abs(cur.first - matches2[r + 1].first) <= 10000)
            seedsph1[cur.second].insert(cur.first);
        else break;
    }
    while (r >= 0){
        pair<int, int> cur;
        cur = matches2[r];
        r--;
        if (abs(cur.first - matches1[l + 1].first) <= 10000)
            seedsph2[cur.second].insert(cur.first);
        else break;
    }
}

cost_t pairend_heuristic(Statesr one, Statesr two, char *heuristic_method){
    if (strcmp(heuristic_method, "dijkstra_heuristic") == 0) return 0;
    if (strcmp(heuristic_method, "seed_heuristic") == 0){
        if (!one.p.is_in_trie() && !two.p.is_in_trie()){
            if (abs(one.p.rpos - two.p.rpos) > 10000)
                return inf;
        }
        return (one.h + two.h <= 20)? one.h + two.h: inf;
    }
    assert(false);
}

Statepr CreateStatepr(Statesr l, Statesr r, char *heuristic_method){
    return Statepr(l.qpos, l.p, r.p, 0, pairend_heuristic(l, r, heuristic_method));
}

cost_t astar_pairend_read_alignment(pair<string, string> &query, string &ref, int k, Trie *root, MatchingKmers &info, char *heuristic_method, char *showcntexplstates, char *triestart){
    string q1 = query.first;///left end of query;
    string q2 = query.second;///right end of query;
    int n = q1.size();
    int m = ref.size();
    Statepr cur;
    priority_queue <Statepr> q;
    set < pair<int, pair<Node, Node> > > visited;
    if (strcmp(triestart, "Yes") == 0){
        Statesr one = CreateStatesr(Statesr(0, Node(root)), k, info, heuristic_method, 0, 1);
        Statesr two = CreateStatesr(Statesr(0, Node(root)), k, info, heuristic_method, 0, 2);
        cout << "heuristic of single read: "<< one.h << endl;
        cur = CreateStatepr(one, two, heuristic_method);
        cout << "heuristic of paired end read: "<<cur.h<<endl;
        q.push(cur);
        /*for (int i = m - k + 1; i <= m; ++i){
            one = CreateStatesr(Statesr(0, Node(i)), k, info, heuristic_method, 0, 1);
            two = CreateStatesr(Statesr(0, Node(root)), k, info, heuristic_method, 0, 2);
            cur = CreateStatepr(one, two, heuristic_method);
            q.push(cur);
            cur = CreateStatepr(two, one, heuristic_method);
            q.push(cur);
        }
        for (int i = m - k + 1; i <= m; ++i)
            for (int j = m - k + 1; j <= m; ++j){
                one = CreateStatesr(Statesr(0, Node(i)), k, info, heuristic_method, 0, 1);
                two = CreateStatesr(Statesr(0, Node(j)), k, info, heuristic_method, 0, 2);
                cur = CreateStatepr(one, two, heuristic_method);
                q.push(cur);
            }*/
    }
    else{
        for (int i = 0; i <= m; ++i)
            for (int j = 0; j <= m; ++j){
                Statesr one = CreateStatesr(Statesr(0, Node(i)), k, info, heuristic_method, 0, 1);
                Statesr two = CreateStatesr(Statesr(0, Node(j)), k, info, heuristic_method, 0, 2);
                cur = CreateStatepr(one, two, heuristic_method);
                q.push(cur);
            }
    }
    while (!q.empty()){
        //cout << "q has elements\n";
        cur = q.top();
        q.pop();
        if (cur.qpos == n) break;
        if (visited.find({cur.qpos, {cur.p1, cur.p2}}) == visited.end()){
            visited.insert({cur.qpos, {cur.p1, cur.p2}});
            /*if (!cur.p1.is_in_trie() && !cur.p2.is_in_trie() && cur.p1.rpos < m && cur.p2.rpos < m &&
                ref[cur.p1.rpos] == q1[cur.qpos] && ref[cur.p2.rpos] == q2[cur.qpos]){*/
            if (is_greedy_available(Statesr(cur.qpos, cur.p1), q1, ref) && is_greedy_available(Statesr(cur.qpos, cur.p2), q2, ref)){    
                    Statesr one = CreateStatesr(Statesr(cur.qpos+1, Node(cur.p1.rpos+1)), k, info, heuristic_method, 0, 1);
                    Statesr two = CreateStatesr(Statesr(cur.qpos+1, Node(cur.p2.rpos+1)), k, info, heuristic_method, 0, 2);
                    Statepr topush = CreateStatepr(one, two, heuristic_method);
                    topush.g += cur.g;
                    q.push(topush); 
                }
            else{
                vector next1 = NextStatesr(Statesr(cur.qpos, cur.p1), q1[cur.qpos], ref, k, info, heuristic_method, 1);
                vector next2 = NextStatesr(Statesr(cur.qpos, cur.p2), q2[cur.qpos], ref, k, info, heuristic_method, 2);
                next1.push_back(CreateStatesr(Statesr(cur.qpos, cur.p1), k, info, heuristic_method, 0, 1));
                next2.push_back(CreateStatesr(Statesr(cur.qpos, cur.p2), k, info, heuristic_method, 0, 2));
                /*cout << "next1.size(): "<< next1.size() << "\n";
                cout << "next2.size(): "<< next2.size() << "\n";*/
                for (auto i1: next1)
                    for (auto i2: next2)
                        if (i1.qpos == i2.qpos){
                            Statepr next = CreateStatepr(i1, i2, heuristic_method);
                            next.g = cur.g + i1.stepcost + i2.stepcost;
                            if (next.h != inf)
                                q.push(next);
                        }
            }
        }
    }
    assert (cur.qpos == n);
    if (strcmp(showcntexplstates, "Yes") == 0)
        cout << "Explored states == " << visited.size() << " ";
    return cur.g;
}
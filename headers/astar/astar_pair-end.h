#pragma once

#include "../header.h"
#include <bits/stdc++.h>
#include "statesstruct.h"
#include "astar_single_reads.h"

using namespace std;

 inline void get_seeds_matches_sorted(vector<int> &seeds, vector <int> &lastkmer, vector<int> &prevposkmer, vector<pair<int, int> > &matches){
    /*
    matches.first: position of seed
    matches.second: num of seed
    */
   static int cnt = 1;
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
    cnt++;
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

inline void crumb_seed_matches(vector<pair<int, int> > &matches1, vector<pair<int, int> > &matches2, int drange, int readdist,
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


inline void filter_matches(MatchingKmers &info, int  insdist, int drange, int readsz){
    int inndist = insdist - readsz*2;///inner distance
    int readdist = inndist + readsz;///distance between reads position with same indexes
    vector<pair<int, int> > matches1, matches2;
    get_seeds_matches_sorted(info.seeds1, info.lastkmer, info.prevposkmer, matches1);
    get_seeds_matches_sorted(info.seeds2, info.lastkmer, info.prevposkmer, matches2);
    info.crumbseeds1.resize(info.seeds1.size());
    info.crumbseeds2.resize(info.seeds2.size());
    //make_crumbs_appropriate_matches(matches1, matches2, lb, rb, info.crumbseeds1, info.crumbseeds2);
    crumb_seed_matches(matches1, matches2, drange, readdist, info.crumbseeds1, true);
    crumb_seed_matches(matches2, matches1, drange, readdist, info.crumbseeds2, false);
    //show_filter_matches(info.crumbseeds1);
    //show_filter_matches(info.crumbseeds2);
}

inline void get_crumbs_pairend(string &ref, int d, int k, MatchingKmers &info){
    getcrumbs(ref, d, k, info.crumbs1, info.seeds1, info.backtotrieconnection, info.lastkmer, info.prevposkmer, 1, info.crumbseeds1);
    getcrumbs(ref, d, k, info.crumbs2, info.seeds2, info.backtotrieconnection, info.lastkmer, info.prevposkmer, 2, info.crumbseeds2);
}


inline void push_first_prstates_in_q(priority_queue<Statepr> &q, int m, Trie *root){

}

cost_t astar_pairend_read_alignment(pair<string, string> &query, string &ref, int d, int k, Trie *root, MatchingKmers &info){
    int n = query.first.size();
    int m = ref.size();
    priority_queue<Statepr> q;
    return 0;
}


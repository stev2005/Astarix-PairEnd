#pragma once

//#define NDEBUG
#include <bits/stdc++.h>
using namespace std;
char base[4] = {'A', 'C', 'G', 'T'};

typedef int cost_t;
typedef long long eval_t;//integers
typedef double eval_tr;//real number
cost_t inf = (1<<30);
cost_t maximum_edit_cost;

int nindel = 0;///can be taken as ndel and nins if deletion cost ==  insertion cost
string heuristic;

struct Evaluations{
    eval_t cntexpansions, cntTrieTrieexpansions, cntTrierefexpansions, cntrefTrieexpansions, cntrefrefexpansions, cntpunishedstates;
    /// cnts percentages to cntexpansions;
    eval_tr percntTrieTrieexpansions, percntTrierefexpansions, percntrefTrieexpansions, percntrefrefexpansions;
    eval_tr band;
    int ntests;
    eval_tr matchesperseed, legitmatchesperseed, crumbsperlegitmatch;
    eval_tr aligntime, filtermatchestime, getcrumbstime;
    int d, k, drange;
    ///stats for single read
    eval_tr sraligntime, srcrumbingtime;
    
    Evaluations(){
        cntexpansions = 0;
        cntTrieTrieexpansions = 0;
        cntTrierefexpansions = 0;
        cntrefTrieexpansions = 0;
        cntrefrefexpansions = 0;
        band = 0;
        ntests = 0;
    }
    
    void update_astar_cnts(eval_t _cntexpansions, eval_t _cntTrieTrieexpansions,eval_t _cntTrierefexpansions, eval_t _cntrefTrieexpansions,
    eval_t _cntrefrefexpansions, eval_tr _band, eval_t _cntpunishedstates){
        cntexpansions += _cntexpansions;
        cntTrieTrieexpansions += _cntTrieTrieexpansions;
        cntTrierefexpansions += _cntTrierefexpansions;
        cntrefTrieexpansions += _cntrefTrieexpansions;
        cntrefrefexpansions += _cntrefrefexpansions;
        band += _band;
        cntpunishedstates += _cntpunishedstates;
    }

    void update_astar_percentages(eval_tr _percntTrieTrieexpansions, eval_tr _percntTrierefexpansions, eval_tr _percntrefTrieexpansions, eval_tr _percntrefrefexpansions){
        percntTrieTrieexpansions += _percntTrieTrieexpansions;
        percntTrierefexpansions += _percntTrierefexpansions;
        percntrefTrieexpansions += _percntTrierefexpansions;
        percntrefrefexpansions += _percntrefrefexpansions;
    }

    void print_single_read_stats(){
        cout << "Stats for single read group:\n";
        cout << "   Number of tests: " << ntests << "\n";
        cout << "   run-time setting crumbs: " << getcrumbstime << " (sec.)\n";
        cout << "   run-time joint alignment: " << aligntime << " (sec.)\n";
        cout << "   Trie depth: " << d << "\n";
        cout << "   Seeds lenght: " << k << " (bp.)\n";
        cout << "   Drange: " << drange << "\n";

    }

    void print_avg_stats(){
        eval_tr ntestsr = ntests;
        cout << "Average Stats for this test group:\n";
        cout << "   Number of tests: " << ntests << "\n";
        cout << "   Error-rate: " << "(you have to know it in advance)\n";
        cout << "   Avg. runtime filtering legit from faux matches: " << filtermatchestime / ntestsr << " (sec.)\n";
        cout << "   Avg. run-time setting crumbs: " << getcrumbstime / ntestsr<< " (sec.)\n";
        cout << "   Avg. run-time joint alignment: " << aligntime / ntestsr << " (sec.)\n";
        cout << "   Trie depth: " << d << "\n";
        cout << "   Seeds lenght: " << k << " (bp.)\n";
        cout << "   Drange: " << drange << "\n";
        cout << "   Avg. matches per seed: " << matchesperseed / ntestsr << "\n";
        cout << "   Avg. legit matches per seed: " << legitmatchesperseed / ntestsr << "\n";
        cout << "   Avg. crumbs per legit match: " << crumbsperlegitmatch / ntestsr << "\n";
        cout << "   Avg. band: " << band / ntestsr << "\n";
        cout << "   Avg. expanded states: " << (double) cntexpansions / ntestsr << "\n";
        cout << "   Avg. expanded states u, v ∈ Trie: " << (double) cntTrieTrieexpansions / ntestsr << "\n";
        cout << "   Avg. expanded states u ∈ Trie, v ∈ Gr: " << (double) cntTrierefexpansions / ntestsr << "\n";
        cout << "   Avg. expanded states u ∈ Gr, v ∈ Trie: " << (double) cntrefTrieexpansions / ntestsr << "\n";
        cout << "   Avg. expanded states u, v ∈ Gr: " << (double) cntrefrefexpansions / ntestsr << "\n";
        cout << "   Avg. punished states: " << (double) cntpunishedstates / ntestsr << "\n";
        cout << "   Avg. expanded states u, v∈ Trie (% of all): " << percntTrieTrieexpansions / ntestsr << "\n";
        cout << "   Avg. expanded states u ∈ Trie, v ∈ Gr (% of all): " << percntTrierefexpansions / ntestsr << "\n";
        cout << "   Avg. expanded states u∈Gr, v∈Trie (% of all): " << percntrefTrieexpansions / ntestsr << "\n";
        cout << "   Avg. expanded states u, v ∈ Gr (% of all): " << percntrefrefexpansions / ntests << endl;
    }

    void print_stats(){
        eval_tr ntestsr = ntests;
        cout << "Stats for this test group:\n";
        cout << "   Number of tests: " << ntests << "\n";
        cout << "   Error-rate: " << "(you have to know it in advance)\n";
        cout << "   runtime filtering legit from faux matches: " << filtermatchestime  << " (sec.)\n";
        cout << "   run-time setting crumbs: " << getcrumbstime << " (sec.)\n";
        cout << "   run-time joint alignment: " << aligntime << " (sec.)\n";
        cout << "   Trie depth: " << d << "\n";
        cout << "   Seeds lenght: " << k << " (bp.)\n";
        cout << "   Drange: " << drange << "\n";
        cout << "   matches per seed: " << matchesperseed  << "\n";
        cout << "   legit matches per seed: " << legitmatchesperseed << "\n";
        cout << "   crumbs per legit match: " << crumbsperlegitmatch << "\n";
        cout << "   band: " << band << "\n";
        cout << "   expanded states: " << cntexpansions << "\n";
        cout << "   expanded states u, v ∈ Trie: " << cntTrieTrieexpansions << "\n";
        cout << "   expanded states u ∈ Trie, v ∈ Gr: " << cntTrierefexpansions << "\n";
        cout << "   expanded states u ∈ Gr, v ∈ Trie: " << cntrefTrieexpansions << "\n";
        cout << "   expanded states u, v ∈ Gr: " << cntrefrefexpansions << "\n";
        cout << "   punished states: " << cntpunishedstates << "\n";
        cout << "   expanded states u, v∈ Trie (% of all): " << percntTrieTrieexpansions << "\n";
        cout << "   expanded states u ∈ Trie, v ∈ Gr (% of all): " << percntTrierefexpansions << "\n";
        cout << "   expanded states u∈Gr, v∈Trie (% of all): " << percntrefTrieexpansions << "\n";
        cout << "   expanded states u, v ∈ Gr (% of all): " << percntrefrefexpansions << endl;
    }
};
Evaluations evalsts;

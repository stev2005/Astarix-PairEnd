#pragma once

//#define NDEBUG
#include <bits/stdc++.h>
#include <chrono>
#include "bucketqueue.h"
#include <signal.h>
using namespace std;
const int alphabetsz = 4;
const char base[alphabetsz] = {'A', 'T', 'G', 'C'};

typedef int cost_t;
typedef long long eval_t;//integers
typedef double eval_tr;//real number
cost_t inf = (1<<30);
cost_t maximum_edit_cost;
char special_sign = '$';
cost_t special_cost = 0;
cost_t special_heuristic_cost = 0;
cost_t cequal = 0;
cost_t csub = 1;
cost_t cdel = 1;
cost_t cins = 1;
cost_t cdmin = 1;
int ndel = 0;
int nins = 0;
const string single_end_alignment = "single-read";
const string paired_end_alignment = "paired-end";
const string paired_end_alignment_4D = "paired-end-4D";

int get_nins_ndel_value(int qsz, int ssz, cost_t cop){
    return ((qsz * cequal + ssz * cdmin) % cop)? (qsz * cequal + ssz * cdmin) / cop + 1: (qsz * cequal + ssz * cdmin);
}

int nindel = 0;///can be taken as ndel and nins if deletion cost ==  insertion cost

chrono::time_point<chrono::high_resolution_clock> gettimenow_chrono(){
    return chrono::high_resolution_clock::now();
}

double runtimechrono(chrono::time_point<chrono::high_resolution_clock> start, chrono::time_point<chrono::high_resolution_clock> finish){
    chrono::duration<double> takentime = finish - start;
    return takentime.count();
}

struct EvaluationsPE{
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
    
    EvaluationsPE(){
        cntexpansions = 0;
        cntTrieTrieexpansions = 0;
        cntTrierefexpansions = 0;
        cntrefTrieexpansions = 0;
        cntrefrefexpansions = 0;
        band = 0;
        ntests = 0;
    }
    
    void update_astar_cnts(eval_t _cntexpansions, eval_t _cntTrieTrieexpansions,eval_t _cntTrierefexpansions, eval_t _cntrefTrieexpansions,
    eval_t _cntrefrefexpansions, eval_tr _band, eval_t _cntpunishedstates/*, eval_tr _aligntime*/){
        //++ntests;
        cntexpansions += _cntexpansions;
        cntTrieTrieexpansions += _cntTrieTrieexpansions;
        cntTrierefexpansions += _cntTrierefexpansions;
        cntrefTrieexpansions += _cntrefTrieexpansions;
        cntrefrefexpansions += _cntrefrefexpansions;
        band += _band;
        cntpunishedstates += _cntpunishedstates;
        //aligntime += _aligntime;
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
        //cout << "   Avg. runtime filtering legit from faux matches: " << filtermatchestime / ntestsr << " (sec.)\n";
        cout << "   Avg. run-time setting crumbs: " << getcrumbstime / ntestsr<< " (sec.)\n";
        cout << "   Avg. run-time joint alignment: " << aligntime / ntestsr << " (sec.)\n";
        cout << "   Trie depth: " << d << "\n";
        cout << "   Seeds lenght: " << k << " (bp.)\n";
        cout << "   Drange: " << drange << "\n";
        //cout << "   Avg. matches per seed: " << matchesperseed / ntestsr << "\n";
        //cout << "   Avg. legit matches per seed: " << legitmatchesperseed / ntestsr << "\n";
        //cout << "   Avg. crumbs per legit match: " << crumbsperlegitmatch / ntestsr << "\n";
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
        //cout << "Stats for this test group:\n";
        cout << "   Number of tests: " << ntests << "\n";
        //cout << "   Error-rate: " << "(you have to know it in advance)\n";
        //cout << "   runtime filtering legit from faux matches: " << filtermatchestime  << " (sec.)\n";
        //cout << "   run-time setting crumbs: " << getcrumbstime << " (sec.)\n";
        cout << "   run-time joint alignment: " << aligntime << " (sec.)\n";
        cout << "   Trie depth: " << d << "\n";
        cout << "   Seeds lenght: " << k << " (bp.)\n";
        cout << "   Drange: " << drange << "\n";
        //cout << "   matches per seed: " << matchesperseed  << "\n";
        //cout << "   legit matches per seed: " << legitmatchesperseed << "\n";
        //cout << "   crumbs per legit match: " << crumbsperlegitmatch << "\n";
        cout << "   band: " << band << "\n";
        cout << "   expanded states: " << cntexpansions << "\n";
        cout << "   expanded states u, v ∈ Trie: " << cntTrieTrieexpansions << "\n";
        cout << "   expanded states u ∈ Trie, v ∈ Gr: " << cntTrierefexpansions << "\n";
        cout << "   expanded states u ∈ Gr, v ∈ Trie: " << cntrefTrieexpansions << "\n";
        cout << "   expanded states u, v ∈ Gr: " << cntrefrefexpansions << "\n";
        cout << "   punished states: " << cntpunishedstates << "\n";
        cout << "   expanded states u, v∈ Trie (% of all): " << (eval_tr) cntTrieTrieexpansions / (eval_tr)cntexpansions * (eval_tr) 100 << "\n";
        cout << "   expanded states u ∈ Trie, v ∈ Gr (% of all): " << (eval_tr) cntTrierefexpansions / (eval_tr)cntexpansions * (eval_tr) 100<< "\n";
        cout << "   expanded states u∈Gr, v∈Trie (% of all): " << cntrefTrieexpansions / (eval_tr)cntexpansions * (eval_tr )100<< "\n";
        cout << "   expanded states u, v ∈ Gr (% of all): " << cntrefrefexpansions / (eval_tr)cntexpansions * (eval_tr) 100<< endl;
    }
};
EvaluationsPE peevals[128];

struct EvaluationsSE{
    eval_t cntexpansions, cntTrieexpansions, cntrefexpansions;
    eval_tr band, aligntime, precomptime;
    eval_t ntests;
    EvaluationsSE(){
        cntexpansions = 0;
        cntTrieexpansions = 0;
        cntrefexpansions = 0;
        band = 0;
        ntests = 0;
    }
    void add_entry(eval_tr _band, eval_t _cntexpansions, eval_t _cntTrieexpansions, eval_t _cntrefexpansions, eval_tr _aligntime){
        ntests++;
        band += _band;
        cntexpansions += _cntexpansions;
        cntTrieexpansions += _cntTrieexpansions;
        cntrefexpansions += _cntrefexpansions;
        aligntime += _aligntime;
    }
    void print_info(){
        eval_tr ntestsr = ntests;
        cout << "   Tests: " << ntests << "\n";
        cout << "   Avg. Band: " << band / ntestsr << "\n";
        cout << "   Avg. Alignment time (sec.): " << aligntime / ntestsr << "\n"; 
        cout << "   Avg. Precomputation time (sec.): " << precomptime / ntestsr << "\n";
        cout << "   Avg. Number of expansions: " << cntexpansions / ntestsr<< "\n";
        cout << "   Avg. Number of Trie expansions: " << cntTrieexpansions / ntestsr<< "\n";
        cout << "   Avg. Number of Ref. expandions: " << cntrefexpansions / ntestsr << "\n";
    }
};

EvaluationsSE seevals[128];

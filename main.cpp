#include "headers/header.h"
#include <bits/stdc++.h>
#include "headers/trie.h"
#include "headers/readinput.h"
#include "headers/readparameters.h"
#include "headers/astar/statesstruct.h"
#include "headers/astar/astar_single_reads.h"
#include "headers/astar/astar_pair-end.h"
using namespace std;

inline void init_precompute(int argc, char *argv[], int &d, int &k, string &ref, string &fileref, string &filequery,
                            string &filequery1, string &filequery2,
                            string &typealignment, string &heuristic, string &triecrumbsopt, int &insdist, int &drange,
                            Trie *&rootdmer, Trie *&rootkmer, MatchingKmers &info){
    cerr << "Precomppute started\n";
    parameters_default_values(d, k, fileref, filequery, filequery1, filequery2, typealignment, heuristic, triecrumbsopt, insdist, drange);
    read_parameters(argc, argv, d, k, fileref, filequery, filequery1, filequery2, typealignment, heuristic, triecrumbsopt, insdist, drange);
    read_reference(fileref, ref);
    if (typealignment == single_end_alignment) set_query_input_file(filequery);
    else set_pe_query_input_file(filequery1,  filequery2);
    auto startt = move(gettimenow_chrono());
    cerr << "To build tries\n";
    construct_trie_dmer(ref, d, rootdmer, info.last, info.prevpos, info.backtotrieconnection);
    construct_trie_kmer(ref, k, rootkmer, info.lastkmer, info.prevposkmer, info.backtotrieconnectionkmer);
    cerr << "Precompute done\n";
}

string get_reverse_complement(string s){
    reverse(s.begin(), s.end());
    for (int i = 0; i < (int)s.size(); ++i)
        for (int j = 0; j < alphabetsz; ++j)
            if (s[i] == base[j]){
                s[i] = base[j ^ 1];
                break;
            }
    return s;
}

inline void print_query_seeds(vector<int> &seeds, vector<int> &nseeds){
    cerr << "start of print_query_seeds\n";
    cerr << "Positive seeds:\n";
    for (auto i: seeds)
        cerr << i << " ";
    cerr << "\nNegative seeds:\n";
    for (auto i: nseeds)
        cerr << i << " ";
    cerr << "\nend of print_query_seeds()\n";
}

inline void make_ends_equally_long(string &s1, string &s2){
    string add(abs((int)s1.size() - (int)s2.size()), special_sign);
    if (s1.size() < s2.size()) s1 += add;
    else s2 +=  add;
}

int main(int argc, char *argv[]){
    cerr << "Start of the program\n";
    int d, k;
    string ref;
    string fileref, filequery;
    string filequery1, filequery2;
    string typealignment;
    string heuristic;
    string triecrumbsopt;
    Trie *rootdmer = new Trie(), *rootkmer = new Trie();
    MatchingKmers info;
    init_precompute(argc, argv, d, k, ref, fileref, filequery, filequery1, filequery2, typealignment, heuristic,
    triecrumbsopt, insdist, drange, rootdmer, rootkmer, info);
    int testcase = 0;
    while (!queyreof(typealignment)){
        ++testcase;
        cerr << "Query: " << testcase << "\n";
        cout << "Query: " << testcase << "\n";
        if (typealignment == "single-read"){
            string query = get_single_read_query();
            string nquery = move(get_reverse_complement(query));
            if (heuristic == single_end_alignment){
                cerr << "seed heuristic choosen\n";
                query_into_seeds(query, k, rootkmer, info.seeds);
                query_into_seeds(nquery, k, rootkmer, info.nseeds);
                cerr << "query's seeds obtained\n";
                print_query_seeds(info.seeds, info.nseeds);
                nins = get_nins_ndel_value(query.size(), info.seeds.size(), cins);
                ndel = get_nins_ndel_value(query.size(), info.seeds.size(), cdel);
                assert(nins);
                assert(ndel);
                if (triecrumbsopt == "yes"){
                    getcrumbs_trieopt(ref, d, k, info.seeds, info.lastkmer, info.prevposkmer, info.backtotrieconnection, info.crumbs);
                    getcrumbs_trieopt(ref, d, k, info.nseeds, info.lastkmer, info.prevposkmer, info.backtotrieconnection, info.ncrumbs);
                }
                else{
                    getcrumbs(ref, d, k, info.seeds, info.lastkmer, info.prevposkmer, info.backtotrieconnection, info.crumbs);
                    getcrumbs(ref, d, k, info.nseeds, info.lastkmer, info.prevposkmer, info.backtotrieconnection, info.ncrumbs);
                }
                cerr << "prep comp for h. done\n";
            }
            astar_single_read_alignment(query, nquery, ref, d, k, rootdmer, info);
        }
        else if (typealignment == paired_end_alignment){
            pair<string, string> queryp;
            queryp = move(get_pair_end_query());
            cout << "the query\n";
            cout << "   " << queryp.first << "\n";
            cout << "   " << queryp.second << "\n";
            pair<string, string> nqueryp;
            nqueryp.first = get_reverse_complement(queryp.first);
            nqueryp.second = get_reverse_complement(queryp.second);
            if (heuristic == "seed_heuristic"){
                cerr << "seed heuristic choosen\n";
                query_into_seeds(queryp.first, k, rootkmer, info.seeds1);
                query_into_seeds(nqueryp.second, k, rootkmer, info.nseeds2);
                query_into_seeds(nqueryp.first, k, rootkmer, info.nseeds1);
                query_into_seeds(queryp.second, k, rootkmer, info.seeds2);
                if (triecrumbsopt == "yes") getcrumbs_trieopt_pairend(ref, queryp.first.size(), queryp.second.size(), d, k, info);
                else getcrumbs_pairend(ref, queryp.first.size(), queryp.second.size(), d, k, info);
                cerr << "prep comp for h. done\n";
            }
            readdist = insdist - queryp.second.size();
            innerdist = readdist - queryp.first.size();
            make_ends_equally_long(queryp.first, queryp.second);
            make_ends_equally_long(nqueryp.first, nqueryp.second);
            cost_t rezult = astar_pairend_read_alignment(queryp, nqueryp, ref, d, k, rootdmer, info);
        }
        else{
            cerr << "No such alignment type is supported\n";
            cerr << "Program is going to abort\n";
            abort();
        }
        info.clearquerydata();
        cout << "\n";
    }
    delete rootdmer;
    delete rootkmer;
    close_query_in_files();
    return 0;
}

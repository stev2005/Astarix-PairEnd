#include "headers/header.h"
#include<bits/stdc++.h>
//#include "headers/dp.h"
#include "headers/trie.h"
#include "headers/astar/astar_single_reads.h"
#include "headers/astar/astar_pair-end.h"
#include "headers/readparameters.h"
#include "headers/readinput.h"
using namespace std;

void printtriecrumbs(map <Node, bitset<64> > &crumbs, Trie *T, string s){
    cout << "string s: "<< s << " crumbs: ";
    for (int i = 0; i < 4; ++i)
        cout << crumbs[Node(T)][i] << " ";
    cout << endl;
    for (int i = 0; i < 4; ++i)
        if (T->child[i] != nullptr)
            printtriecrumbs(crumbs, T->child[i], s + base[i]);
}

void printoutcrumbs(map<Node, bitset<64> > &crumbs, Trie *root){
    for (auto it = crumbs.begin(); it != crumbs.end(); ++it){
        Node cur = it->first;
        if (cur.is_in_trie() == false){
            cout << "Node in the reference "<<cur.rpos<<"\n";
            for (int i = 0; i < 4; ++i)
                cout << it->second[i] << " ";
            cout << endl;
        }
    }
    printtriecrumbs(crumbs, root, "");
}

inline void building_tries(string &ref, int d, int k, Trie *&rootdmer, Trie *&rootkmer, MatchingKmers &info){
    clock_t t = clock();
    construct_trie(ref, d, rootdmer, info.last, info.prevpos, info.backtotrieconnection);
    construct_trie(ref, k, rootkmer, info.lastkmer, info.prevposkmer, info.backtotrieconnectionkmer);
    t = clock() - t;
    cout << "constructing trie: "<< (double) t / CLOCKS_PER_SEC << "s.\n"; 
}

inline void printmatches(MatchingKmers &info){
    vector<pair<int, int> > matches;
    vector<int> & lastkmer = info.lastkmer;
    vector<int> & prevposkmer = info.prevposkmer;
    vector<int> & seeds = info.seeds;
    for (int i = 0; i < seeds.size(); ++i){
        if (seeds[i] == -1) continue;
        for (int j = lastkmer[seeds[i]]; j != -1; j = prevposkmer[j])
            matches.push_back({j, i});
    }
    sort(matches.begin(), matches.end());
    cout << "mathces1:\n";
    for (auto i: matches)
        cout << i.first << " " << i.second << "\n";
    cout << "\n";
}

double runtime(clock_t t){
    return (double) t / CLOCKS_PER_SEC;
}

int main(int argc, char *argv[]){
    /*ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);*/
    cerr <<"Start of the program\n";
    int d, k, locinsdist, locdrange;
    int locindaligns;
    string typealignment;
    string& heuristiclocal = heuristic;
    string fileref, filequery;
    parameters_default_values(d, k, typealignment, heuristiclocal, locinsdist, locdrange, fileref, filequery, infheuristic, locindaligns);
    read_parameters(argc, argv, d, k, typealignment, heuristiclocal, locinsdist, locdrange, fileref, filequery, infheuristic, locindaligns);
    insdist = locinsdist;
    drange = locdrange;
    indaligns = locindaligns;
    evalsts.d = d;
    evalsts.k = k;
    evalsts.drange = drange;
    cout << "D: " << d << " k: " << k << endl;
    string ref;
    int testcases;
    cerr << "int k has a value equal to " << k << "\n";
    clock_t t = clock();
    //cin >> ref ;
    read_reference(fileref, ref);
    t = clock() - t;
    cerr << "entered reference\n";
    cout << "reading reference: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
    set_query_in_file(filequery);
    //cin >> testcases ;
    testcases = get_num_testcases();
    evalsts.ntests = testcases;
    //cerr << "entered num of tescases\n" ;
    MatchingKmers info;
    Trie *rootdmer = new Trie();
    Trie *rootkmer = new Trie();
    building_tries(ref, d, k, rootdmer, rootkmer, info);
    cout << "\n";
    cerr << "inited kmers\n" ;
    for (int testcase=1; testcase<=testcases; ++testcase/*, cout<<endl*/){
        cerr << "Query "<< testcase << ":\n";
        int rezult;
        //t = clock();
        if (typealignment == "single-read"){
            //cerr << "If for single read alingment\n";
            string query;
            //cin>>query;
            query = get_single_read_query();
            t = clock() - t;
            //cout << "Reading query: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
            maximum_edit_cost = query.size() + 1;
            if (heuristic == "seed_heuristic"){
                t = clock();
                info.seeds = query_into_seeds(query, k, rootkmer);
                nindel = info.seeds.size();
                t = clock() - t;
                cout << "breaking query into seeds: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
                t = clock();
                getcrumbs(ref, d, k, info.crumbs, info.seeds, info.backtotrieconnection,
                info.lastkmer, info.prevposkmer, 0, vector<unordered_set<int> > () = {});
                t = clock() - t;
                cout << "Precompute of crumbs: " << (double) t / CLOCKS_PER_SEC << "s.\n";
                //printoutcrumbs(info.crumbs, root);
                //printmatches(info);
            }
            t = clock();
            vector<pair<cost_t, int> > alignments;
            alignments = astar_single_read_alignment(query, ref, d, k, rootdmer, info, 1);
            cout << "Cost: " << alignments.front().first << "\n";
            t = clock() - t;
            cout << "Alignment: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
        }
        else if (typealignment == "paired-end_independent"){
            pair <string, string> queryp;
            queryp = get_pair_end_query();
            nindel = queryp.first.size() / k;
            if (queryp.first.size() % k != 0)
                nindel++;
            info.seeds1 = query_into_seeds(queryp.first, k, rootkmer);
            info.seeds2 = query_into_seeds(queryp.second, k, rootkmer);
            t = clock();
            getcrumbs(ref, d, k, info.crumbs1, info.seeds1, info.backtotrieconnection, info.lastkmer, info.prevposkmer, 0, info.crumbseeds1);
            getcrumbs(ref, d, k, info.crumbs2, info.seeds2, info.backtotrieconnection, info.lastkmer, info.prevposkmer, 0, info.crumbseeds2);
            t = clock() - t;
            evalsts.getcrumbstime += runtime(t);
            t = clock();
            cost_t rezult = astar_pairend_read_alignment_independent(queryp, ref, d, k, rootdmer, info, indaligns);
            cout << "Independent alignement passed\n";
            cout << "Cost: " << rezult << endl;
        }
        else if (typealignment == "paired-end"){
            ///paired-end alignment joint
            //cerr << "main: paired-end\n";
            pair <string, string> queryp;
            queryp = get_pair_end_query();
            nindel = queryp.first.size() / k;
            if (queryp.first.size() % k != 0)
                nindel++;
            info.seeds1 = query_into_seeds(queryp.first, k, rootkmer);
            info.seeds2 = query_into_seeds(queryp.second, k, rootkmer);
            //cerr << "Here 1\n";
                t = clock();
                filter_matches(info, queryp.first.size());
                t = clock() - t;
                evalsts.filtermatchestime += runtime(t);
                //cerr << "filtering not the problme\n";
                //howmanycrumbs_seeds_have(info, k);
                    //cerr << "Here2\n";
                    t = clock();
                    //cerr << "Here3\n";
                        get_crumbs_pairend(ref, d, k, info);
                    //cerr << "Here4\n";
                    t = clock() - t;
                    //cerr << "Here5\n";
                    evalsts.getcrumbstime += runtime(t);
                    //cerr << "Here6\n";
                    t = clock();
                    //cerr << "crumbing not the problem\n";
            //cerr << "Here 10\n";
            rezult = astar_pairend_read_alignment(queryp, ref, d, k, rootdmer, info);
            t = clock() - t;
            evalsts.aligntime += runtime(t);
            //cerr << "alignment passed\n";
            //cout << "Cost: " << rezult << "\n"; 
            //cout << "Alignment: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
            //cerr << "Cost: " << rezult << "\n"; 
            //cerr << "Alignment: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
        }
        else{
            cerr << "No such alignment type is supported\n";
            cerr << "Program is going to abort\n";
            abort();
        }
        t = clock();
        info.clearquerydata();
        t = clock() - t;
        //cout << "Cleaning help vectors: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
        //cout << "Here 20\n";
    }
    //cerr << "Here 30\n";
    evalsts.print_stats();
    cerr << "End of the main.\n";
    return 0;
}
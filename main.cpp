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
    construct_trie(ref, d, rootdmer, info.last, info.prevpos, info.backtotrieconnection, true);
    construct_trie(ref, k, rootkmer, info.lastkmer, info.prevposkmer, info.backtotrieconnectionkmer, false);
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

void check_are_vectors_sorted(Trie* T){
    if (T == nullptr)
        return;
    int sz = T->positions.size();
    cout << "sz: " << sz << endl;
    for (int i = 1; i < sz; ++i)
        if (T->positions[i - 1] > T->positions[i]){
            cerr << "Not sorted\n";
        }
    for (int i = 0; i < 4; ++i){
        cout << "From: " << T << " to child: " << i << endl; 
        check_are_vectors_sorted(T->child[i]);
    }
}

void make_paired_ends_same_size(pair<string, string> &q){
    while (q.first.size() !=  q.second.size()){
        if (q.first.size() < q.second.size())
            q.first.push_back(special_sign);
        else q.second.push_back(special_sign);
    }
}

string get_reverse_complement(string s){
    reverse(s.begin(), s.end());
    for (int i = 0; i < (int)s.size(); ++i){
        for (int j = 0; j < alphabetsz; ++j)
            if (s[i] == base[j]){
                s[i] = base[j ^ 1];
                break;
            }
        /*switch (s[i])
        {
        case base[0]:
            s[i] = base[1];
            break;
        case base[1]:
            s[i] = base[0];
            break;
        case base[2]:
            s[i] = base[3];
            break;
        case base[3]:
            s[i] = base[2];
            break;
        default:
            assert(false);
            break;
        }*/
    }
    return s;
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
    string triecrumbsopt;
    parameters_default_values(d, k, typealignment, heuristiclocal, locinsdist, locdrange, fileref, filequery, infheuristic, locindaligns, occurposlimit, triecrumbsopt);
    read_parameters(argc, argv, d, k, typealignment, heuristiclocal, locinsdist, locdrange, fileref, filequery, infheuristic, locindaligns, occurposlimit, triecrumbsopt);
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
    cerr << "occurposlimit: " << occurposlimit << "\n";
    /*check_are_vectors_sorted(rootdmer);
    cerr << "Ended check sorted\n";
    return 0;*/
    for (int testcase=1; testcase<=testcases; ++testcase/*, cout<<endl*/){
        cerr << "Query "<< testcase << ":\n";
        //cout << "Query "<< testcase << ":\n";
        int rezult;
        //t = clock();
        if (typealignment == "single-read"){
            //cerr << "If for single read alingment\n";
            string query;
            //cin>>query;
            query = get_single_read_query();
            string nquery = get_reverse_complement(query);
            //cerr << "nquery: " << nquery << "\n";
            t = clock() - t;
            //cout << "Reading query: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
            maximum_edit_cost = query.size() + 1;
            auto startprecomp = gettimenow_chrono();
            if (heuristic == "seed_heuristic"){
                //t = clock();
                info.seeds = query_into_seeds(query, k, rootkmer);
                info.nseeds = query_into_seeds(nquery, k, rootkmer);
                nindel = info.seeds.size();
                //t = clock() - t;
                //cout << "breaking query into seeds: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
                //t = clock();
                if (triecrumbsopt == "yes"){
                    getcrumbs_trieopt(ref, d, k, info.crumbs, info.seeds, info.backtotrieconnection,
                    info.lastkmer, info.prevposkmer, info.last, info.prevpos, 0, vector<unordered_set<int> > () = {});
                    getcrumbs_trieopt(ref, d, k, info.ncrumbs, info.nseeds, info.backtotrieconnection,
                    info.lastkmer, info.prevposkmer, info.last, info.prevpos, 0, vector<unordered_set<int> > () = {});
                }
                else{
                    getcrumbs(ref, d, k, info.crumbs, info.seeds, info.backtotrieconnection,
                    info.lastkmer, info.prevposkmer, 0, vector<unordered_set<int> > () = {});
                    getcrumbs(ref, d, k, info.ncrumbs, info.nseeds, info.backtotrieconnection,
                    info.lastkmer, info.prevposkmer, 0, vector<unordered_set<int> > () = {});
                }
                //t = clock() - t;
                /*getcrumbs_trieopt(ref, d, k, info.crumbs, info.seeds, info.backtotrieconnection,
                    info.lastkmer, info.prevposkmer, info.last, info.prevpos, 0, vector<unordered_set<int> > () = {});
                getcrumbs(ref, d, k, info.crumbs, info.seeds, info.backtotrieconnection,
                    info.lastkmer, info.prevposkmer, 0, vector<unordered_set<int> > () = {});*/
                //cout << "Precompute of crumbs: " << (double) t / CLOCKS_PER_SEC << "s.\n";
                //evalsts.getcrumbstime += runtime(t);
                //printoutcrumbs(info.crumbs, root);
                //printmatches(info);
            }

            //t = clock();
            vector<pair<cost_t, int> > alignments;
            alignments = astar_single_read_alignment(query, nquery, ref, d, k, rootdmer, info, 1);
            //cerr << "alignmnets.size(): " << alignments.size() << "\n"; 
            //cerr << "Here after single-end alignment\n";
            //cerr << "Cost: " << alignments.front().first << "\n";
            //t = clock() - t;
            //cout << "Alignment: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
            //evalsts.aligntime += runtime(t);
            double precomptime = runtimechrono(startprecomp, gettimenow_chrono());
            if (alignments.size()) seevals[alignments.front().first].precomptime += precomptime;
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
            make_paired_ends_same_size(queryp);
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
            cerr << "Cost: " << rezult << "\n"; 
            cerr << "Alignment: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
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
    //evalsts.print_stats();
    for (int i = 0; i < 128; ++i, cout << "\n"){
        cout << "Cost: " << i << "\n";
            seevals[i].print_info();
        cout << "\n";
    }
    cerr << "End of the main.\n";
    return 0;
}

/*else if (typealignment == "paired-end_independent"){
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
        }*/
#include "headers/header.h"
#include<bits/stdc++.h>
//#include "headers/dp.h"
#include "headers/trie.h"
#include "headers/astar/astar_single_reads.h"
#include "headers/astar/astar_pair-end.h"
using namespace std;

int charstring_to_int(char *num){
    int len = strlen(num);
    int n = 0, digit;
    for (int i = 0; i < len; ++i){
        digit = (int)num [i] - (int)'0';
        n *= 10;
        n += digit;
    }
    return n;
}

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

inline void program_arguments_to_variables(char *argv[], char *&typealignment, int &d, int &k, char *&heuristic, char *&shownexplstates, char *&triestart){
    typealignment = argv[1];///first argument: aligning single reads or paired-end
    d = charstring_to_int(argv[2]);///second argument: value of D
    k = charstring_to_int(argv[3]);///third argument: value of k
    heuristic = argv[4];///fourth argument: used heuristic
    shownexplstates = argv[5];///fifth argument: show or not show explored states
    triestart = argv[6];///sixt argument: triestart: Yes or No
}

inline void building_tries(string &ref, int d, int k, Trie *&rootdmer, Trie *&rootkmer, MatchingKmers &info){
    clock_t t = clock();
    construct_trie(ref, d, rootdmer, info.last, info.prevpos, info.backtotrieconnection);
    construct_trie(ref, k, rootkmer, info.lastkmer, info.prevposkmer, info.backtotrieconnectionkmer);
    t = clock() - t;
    cout << "constructing trie: "<< (double) t / CLOCKS_PER_SEC << "s.\n"; 
}

int main(int argc, char *argv[]){
    /*ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);*/
    cerr <<"Start of the program\n";
    int d, k;
    char *typealignment, *heuristic_method, *shownexplstates, *triestart;
    program_arguments_to_variables(argv, typealignment, d, k, heuristic_method, shownexplstates, triestart);
    
    string ref, query;
    pair <string, string> queryp;
    int testcases;
    cerr << "int k has a value equal to " << k << "\n";
    clock_t t = clock();
    cin >> ref ;
    t = clock() - t;
    cerr << "entered reference\n";
    cout << "reading reference: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
    cin >> testcases ;
    cerr << "entered num of tescases\n" ;
    MatchingKmers info;
    Trie *rootdmer = new Trie();
    Trie *rootkmer = new Trie();
    building_tries(ref, d, k, rootdmer, rootkmer, info);
    cout << "\n";
    cerr << "inited kmers\n" ;
    for (int testcase=1; testcase<=testcases; ++testcase, cout<<endl){
        cout << "Query "<< testcase << ":\n";
        int rezult;
        t = clock();
        if (strcmp(typealignment, "single-read") == 0){
            cin>>query;
            t = clock() - t;
            cout << "Reading query: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
            if (strcmp(heuristic_method, "seed_heuristic") == 0){
                t = clock();
                info.seeds1 = query_into_seeds(query, k, rootdmer);
                t = clock() - t;
                cout << "breaking query into seeds: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
                t = clock();
                getcrumbs(ref, k, info, 0);
                t = clock() - t;
                cout << "Precompute of crumbs: " << (double) t / CLOCKS_PER_SEC << "s.\n";
                //printoutcrumbs(info.crumbs, root);
            }
            t = clock();
            rezult = astar_single_read_alignment(query, ref, d, k, rootdmer, info, heuristic_method, shownexplstates, triestart, 1);
            cout << "Cost: " << rezult << "\n";
            t = clock() - t;
            cout << "Alignment: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
        }
        else{
            t = clock();
            cin>>queryp.first>>queryp.second;
            t = t - clock();
            cout << "Reading query: "<<(double) t / CLOCKS_PER_SEC << "s.\n";
            /*if (strcmp(heuristic_method, "seed_heuristic") == 0){
                t = clock();
                info.seeds1 = query_into_seeds(queryp.first, k, rootdmer);
                t = clock() - t;
                cout << "breaking query1 into seeds: "<< (double) t / CLOCKS_PER_SEC << "s.\n"; 
                t = clock();
                info.seeds2 = query_into_seeds(queryp.second, k, rootdmer);
                t = clock() - t;
                cout << "breaking query2 into seeds: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
                t = clock();
                filter_matches(info, k);
                t = clock() - t;
                cout << "Filtering matches: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
                //howmanycrumbs_seeds_have(info, k);
                t = clock();
                getcrumbs(ref, k, info, 1);
                t = clock() - t;
                cout << "Precompute of crumbs1: " << (double) t / CLOCKS_PER_SEC << "s.\n";
                t = clock();
                getcrumbs(ref, k, info, 2);
                t = clock() - t;
                cout << "Precompute of crumbs2: " << (double) t / CLOCKS_PER_SEC << "s.\n";
                cout << "Size of crumbs1 and crumbs2: "<< info.crumbs1.size() << " " << info.crumbs2.size() << "\n";
                cout << info.crumbs1[rootd] << " " <<info.crumbs2[root] << "\n"; 
                //printcountofcrumbs(root, info, k);
            }*/
            /*t = clock();
            rezult = astar_single_read_alignment(queryp.first, ref, k, root, info, argv[3], argv[4], argv[5], 1);
            cout << rezult << "\n";
            t = clock() - t;
            cout << "Alignment first read: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
            t = clock();
            rezult = astar_single_read_alignment(queryp.second, ref, k, root, info, argv[3], argv[4], argv[5], 2);
            cout << rezult << "\n";
            t = clock() - t;
            cout << "Alignment second read: " << (double) t / CLOCKS_PER_SEC << "s.\n";  */
            /*t = clock();
            rezult = astar_pairend_read_alignment(queryp, ref, k, rootdmer, info, heuristic_method, shownexplstates, triestart);
            cout<<rezult<<"\n";
            t = clock() - t;
            cout << "Alignment: "<< (double) t / CLOCKS_PER_SEC << "s.\n";*/
        }
        t = clock();
        info.clearquerydata();
        t = clock() - t;
        cout << "Cleaning help vectors: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
    }
    //cout<<"End of the main program.\n";
    return 0;
}
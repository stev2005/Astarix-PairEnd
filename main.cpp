#include "headers/header.h"
#include<bits/stdc++.h>
//#include "headers/dp.h"
//#include "headers/dijkstra/dijkstra.h"
//#include "headers/dijkstra/dijkstranew.h"
//#include "headers/dijkstratrie/dijkstratrie.h"
//#include "headers/dijkstratrie/dijkstratrienew.h"
#include "headers/trie.h"
//#include "headers/dijkstrapairedend/dijkstrapairedend.h"
//#include "headers/dijkstrapairedend/dijkstrapairedendtrie.h"
//#include "headers/astar/astar.h"
#include "headers/astar/astar_TrieStart.h"
using namespace std;

/*void seqalignment(){
    for ( int testcase=1 ; testcase <= testcases ; ++ testcase ) {
        //query . input();
        cin >> query ;
        int rezult;
        //rezult = minimum_edit_distance_dp ( dp ,  query ,  ref ) ;
        //rezult = minimum_edit_distance_dijkstranew( query ,  ref ) ;
        //rezult=minimum_edit_distance_dijkstra_trie(query, ref, T, last, prevpos);
        rezult=minimum_edit_distance_dijkstratrienew(query, ref, T, last, prevpos);
        cout << rezult << "\n";
    }
}*/

int main(){
    /*ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);*/
    //DNA ref ,  query;
    string ref;
    string query;
    //pair <string, string> query;
    int testcases;
    //ref.input();
    //cout<<"Waiting input data. Message from main.exe\n";
    cin >> ref ; 
    cin >> testcases ;
    assert(cout<<"entered reference\n");
    ///Vital for dp seq approach
    //vector < vector < int > > dp ;
    //init_dp_table ( dp ,  ref ) ;
    //Vital for Trie 
    MatchingKmers info;
    Trie *root=new Trie();
    construct_trie(ref, root, info.last, info.prevpos);
    assert(cout<<"inited kmers\n");
    //construct_trie_simple(ref, root, last, prevpos);
    //printout_kmers(T, "");
    for (int testcase=1; testcase<=testcases; ++testcase){
        assert(cout<<"Enter the queries\n");
        //cin>>query.first>>query.second;
        cin>>query;
        assert(cout<<"testcase=="<<testcase<<" entered\n");
        info.seeds = query_into_seeds(query, log2(ref.size())/2, root);
        assert(cout<<"inited seeds\n");
        ///seeds: does (seed[i]>=0) or doesn't(seed[i]==-1) the ith seed match a kmer
        int rezult;
        //rezult = edit_distance_dijkstratrienew(query, ref, root, last, prevpos);
        //rezult = edit_distance_dijkstrapairedend_trie(query, ref, root, last, prevpos);
        //rezult = edit_distance_pairedend(query, ref);
        //rezult = align_astar(query, ref, seeds, last);
        rezult = align_astar_TrieStart(query, ref, root, info.seeds, info.last, info.prevpos);
        cout<<rezult<<"\n";
    }
    //cout<<"End of the main program.\n";
    return 0;
}
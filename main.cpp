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

int main(int argc, char *argv[]){
    ///first argument: aligning single reads or paired-end
    ///second argument: value of k
    ///third argument: used heuristic
    ///fourth argument: show or not chow explored states
    ///fifth argument: triestart: Yes or No
    /*ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);*/
    //DNA ref ,  query;
    /*cout<<"arguments:\n";
    for (int i = 0; i< argc; ++i)
        cout<<argv[i]<<endl;*/
    assert(cout<<"Start of the program\n");
    string ref;
    string query;
    pair <string, string> queryp;
    int testcases, k = charstring_to_int(argv[2]);
    assert(cout<<"int k has a value equal to "<<k<<"\n");
    cin >> ref ;
    assert(cout<<"entered reference\n");
    cin >> testcases ;
    assert(cout<<"entered num of tescases\n");
    ///Vital for dp seq approach
    //vector < vector < int > > dp ;
    //init_dp_table ( dp ,  ref ) ;
    //Vital for Trie 
    MatchingKmers info;
    Trie *root=new Trie();
    //construct_trie(ref, root, info.last, info.prevpos);
    construct_trie_simple(ref, k, root, info.last, info.prevpos);
    assert(cout<<"inited kmers\n");
    //printout_kmers(T, "");
    for (int testcase=1; testcase<=testcases; ++testcase){
        assert(cout<<"Enter the queries\n");
        //cin>>query.first>>query.second;
        int rezult;
        if (strcmp(argv[1], "single-read") == 0){
            cin>>query;
            assert("Entered a query\n");
            info.seeds = query_into_seeds(query, k, root);
            assert(cout<<"inited seeds\n");
            rezult = astar_single_read_alignment(query, ref, k, root, info, argv[3], argv[4], argv[5]);
            cout<<rezult<<"\n";
        }
        else cin>>queryp.first>>queryp.second;/*To do astar for paired end*/
        ///seeds: does (seed[i]>=0) or doesn't(seed[i]==-1) the ith seed match a kmer
        //rezult = edit_distance_dijkstratrienew(query, ref, root, last, prevpos);
        //rezult = edit_distance_dijkstrapairedend_trie(query, ref, root, last, prevpos);
        //rezult = edit_distance_pairedend(query, ref);
        //rezult = align_astar(query, ref, info.seeds, info.last);
        
    }
    //cout<<"End of the main program.\n";
    return 0;
}
#include "headers/header.h"
#include<bits/stdc++.h>
//#include "headers/dp.h"
//#include "headers/dijkstra/dijkstra.h"
//#include "headers/dijkstra/dijkstranew.h"
//#include "headers/dijkstratrie/dijkstratrie.h"
//#include "headers/dijkstratrie/dijkstratrienew.h"
#include "headers/trie.h"
#include "headers/dijkstrapairedend/dijkstrapairedend.h"
#include "headers/dijkstrapairedend/dijkstrapairedendtrie.h"
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
    //DNA ref ,  query;
    string ref;
    //string query;
    pair <string, string> query;
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
    vector<int>last, prevpos;
    Trie *T=new Trie();
    //construct_trie(ref, T, last, prevpos);
    construct_trie_simple(ref, T, last, prevpos);
    assert(cout<<"inited kmers\n");
    for (int testcase=1; testcase<=testcases; ++testcase){
        assert(cout<<"Enter the queries\n");
        cin>>query.first>>query.second;
        //cin>>query;
        assert(cout<<"testcase=="<<testcase<<" entered\n");
        int rezult;
        /*rezult = edit_distance_dijkstratrienew(query, ref, T, last, prevpos);
        cout<<rezult<<"\n";*/
        rezult = edit_distance_dijkstra_paired_end_trie(query, ref, T, last, prevpos);
        cout<<rezult<<" ";
        rezult = edit_distance_pairedend(query, ref);
        cout<<rezult<<"\n";
    }
    //cout<<"End of the main program.\n";
    return 0;
}
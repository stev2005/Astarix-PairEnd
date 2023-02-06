#include "headers/header.h"
#include<bits/stdc++.h>
#include "headers/dp.h"
//#include "headers/dijkstra/dijkstra.h"
#include "headers/dijkstra/dijkstranew.h"
//#include "headers/dijkstratrie/dijkstratrie.h"
#include "headers/dijkstratrie/dijkstratrienew.h"
#include "headers/trie.h"
using namespace std;

int main(){
    //DNA ref ,  query;
    string ref ,  query ;
    int testcases;
    //ref.input();
    //cout<<"Waiting input data. Message from main.exe\n";
    cin >> ref ; 
    cin >> testcases ;
    ///Vital fo dp approach
    //vector < vector < int > > dp ;
    //init_dp_table ( dp ,  ref ) ;

    /// Vital for Trie 
    vector<int>last, prevpos;
    Trie *T=new Trie();
    //cout<<"here1\n";
    init_kmers(ref, T, last, prevpos);

    /*cout<<"here2\n";
    print_out_last_prevpos(last, prevpos);
    printout_kmers(T, "");
    cout<<"\n";*/

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
    //cout<<"End of the main program.\n";
    return 0;
}
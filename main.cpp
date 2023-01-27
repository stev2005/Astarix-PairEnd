#include<bits/stdc++.h>
//#include "DNA.h"
//#include "dpalignment.h"
#include "dijkstra.h"
#include "dijkstratrie.h"
#include "trie.h"
using namespace std;

int main(){
    //DNA ref , query;
    string ref , query ;
    int testcases;
    //ref.input();
    cin >> ref ; 
    cin >> testcases ;
    //vector < vector < int > > dp ;
    //init_dp_table ( dp , ref ) ;
    vector<int>last,prevpos;
    Trie *T=new Trie();
    //cout<<"here1\n";
    init_seeds(ref,T,last,prevpos);
    /*cout<<"here2\n";
    print_out_last_prevpos(last,prevpos);
    printout_seeds(T,"");
    cout<<"\n";*/
    for ( int testcase=0 ; testcase < testcases ; ++ testcase , cout << "\n" ) {
        //query . input();
        cin >> query ;
        int rezult;
        /*rezult = minimum_edit_distance_dp ( dp , query , ref ) ;
        cout << "dp edit distance == " << rezult << "\n" ;*/
        rezult = minimum_edit_distance_dijkstra ( query , ref ) ;
        cout << "dijkstra edit distance == " << rezult << "\n" ;
        rezult=minimum_edit_distance_dijkstra_tri(query,ref,T,last,prevpos);
        cout << "dijkstra with trie edit distance == " <<rezult << "\n";
    }
    return 0;
}
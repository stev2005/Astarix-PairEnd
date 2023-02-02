#include"header.h"
#include<bits/stdc++.h>
//#include "DNA.h"
#include "dpalignment.h"
#include "dijkstra.h"
#include "dijkstratrie.h"
#include "trie.h"
using namespace std;
string int_to_string(int num){
    string s="";
    if(num==0)
        return "0";
    while (num)
    {
        s=(char)(num%10+(int)'0')+s;
        num/=10;
    }
    return s;
}

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
    /*string cdpath="/root/serverexample/Astarix-PairEnd/tests/out";
    string pathfileм
    fstream fout;
    fout.open("");*/
    for ( int testcase=1 ; testcase <= testcases ; ++ testcase ) {
        //query . input();
        cin >> query ;
        int rezult;
        /*rezult = minimum_edit_distance_dp ( dp ,  query ,  ref ) ;
        cout<<rezult<<"\n";*/
        /*rezult = minimum_edit_distance_dijkstra ( query ,  ref ) ;
        cout<<rezult<<"\n";*/
        //cout << "dijkstra edit distance == " << rezult << "\n" ;
        rezult=minimum_edit_distance_dijkstra_trie(query, ref, T, last, prevpos);
        cout << rezult << "\n";
        //cout << "dijkstra with trie edit distance == " <<rezult << "\n";
    }
    //cout<<"End of the main program.\n";
    return 0;
}
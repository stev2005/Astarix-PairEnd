#include<bits/stdc++.h>
//#include "DNA.h"
#include "dpalignment.h"
#include "dijkstra.h"
using namespace std;

void inline init_dp ( vector < vector <int> > &dp , string &ref) {
	dp . resize ( ref.size() + 1 ) ;
	for ( int i = 0 ; i < (int) dp.size() ; ++ i)
		dp[i].resize( ref.size() + 1 ) ;
}

int main(){
	//DNA ref , query;
	string ref , query ;
	int testcases;
	//ref.input();
	cin >> ref ; 
	cin >> testcases ;
	vector < vector < int > > dp ;
	init_dp ( dp , ref ) ;
	for ( int testcase=0 ; testcase < testcases ; ++ testcase , cout << "\n" ) {
		//query . input();
		cin >> query ;
		int rezult;
		rezult = minimum_edit_distance_dp ( dp , query , ref ) ;
		cout << "dp edit distance == " << rezult << "\n" ;
		rezult = minimum_edit_distance_dijkstra ( query , ref ) ;
		cout << "dijkstra edit distance == " << rezult << "\n" ;
	}
	return 0;
}
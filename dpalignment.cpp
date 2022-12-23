#include<bits/stdc++.h>
using namespace std;
const int maxn=1024;
struct DNA{
    string seqId;
    string Ref;
    string add_info;
    string quality_val;
    int size(){
        return Ref.size();
    }
    char get_base_at_pos(int pos){
        return Ref[pos-1];
    }
    void input(){
        cin >> seqId;
        cin >> Ref;
        cin >> add_info;
        cin >> quality_val;
    }
    void output(){
        cout << seqId << "\n";
        cout << Ref << "\n";
        cout << add_info << "\n";
        cout << quality_val << "\n";
    }
};

int dp[maxn][maxn];
int min_edit_dist(DNA &query,DNA &Ref){
	memset ( dp , 0 , sizeof ( dp ) ) ;
	int n , m ;
	char c1 , c2 ;
	n = query . size() ;
	m = Ref . size() ;
	for ( int i = 0 ; i <= m ; ++i )
		dp [ 0 ] [ i ] = 0 ;
	for ( int i=1 ; i <= n ; ++i ){
		dp [ i ] [ 0 ] = dp [ i-1 ] [ 0 ] + 1 ;
		for ( int j = 1 ; j <= m ; ++j ){
			dp [ i ] [ j ]= dp [ i - 1 ] [ j ] + 1 ;
			dp [ i ] [ j ]= min ( dp [ i ] [ j ] , dp [ i ] [ j - 1 ] + 1 ) ;
			c1 = query . get_base_at_pos ( i ) ;
			c2 = Ref . get_base_at_pos  ( j ) ;
			if ( c1 == c2 )
				dp [ i ] [ j ] = min ( dp [ i ] [ j ] , dp [ i - 1 ] [ j - 1 ] ) ;
			else dp [ i ] [ j ] = min ( dp [ i ] [ j ] , dp [ i - 1 ] [ j - 1 ] + 1 ) ;
		}
	}
	int ans = dp [ n ] [ 0 ] ;
	for ( int i = 1 ; i <= m ; ++ i )
		ans = min ( ans , dp [ n ] [ i ] ) ;
	return ans ;
}
int main(){
	DNA Ref , query ;
	int Testcases ;
	Ref . input() ;
	cin >> Testcases;
	for ( int i = 0 ; i < Testcases; ++ i ){
		query . input () ;
		cout << min_edit_dist ( query , Ref ) << "\n" ;
	}
}
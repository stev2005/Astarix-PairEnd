#ifndef DPALIGNMENT_H
#define DPALIGNMENT_H
#include<bits/stdc++.h>
//#include "DNA.h"
using namespace std;

void inline init_dp_table ( vector < vector <int> > &dp , string &ref) {
    dp . resize ( ref.size() + 1 ) ;
    for ( int i = 0 ; i < (int) dp.size() ; ++ i)
        dp[i].resize( ref.size() + 1 ) ;
}

int minimum_edit_distance_dp ( vector < vector <int> > &dp , string &query,string &Ref ) {
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
            //c1 = query . get_base_at_pos ( i ) ;
            c1 = query [ i - 1 ] ;
            //c2 = Ref . get_base_at_pos  ( j ) ;
            c2 = Ref [ j - 1 ] ;
            if ( c1 == c2 )
                dp [ i ] [ j ] = min ( dp [ i ] [ j ] , dp [ i - 1 ] [ j - 1 ] ) ;
            else dp [ i ] [ j ] = min ( dp [ i ] [ j ] , dp [ i - 1 ] [ j - 1 ] + 1 ) ;
        }
    }
    int ans = dp [ n ] [ 0 ] ;
    for ( int i = 1 ; i <= m ; ++ i )
        ans = min ( ans , dp [ n ] [ i ] ) ;
    //cout << "Explored states dp : " << n*m << "\n" ;
    return ans ;
}

#endif // DPALIGNMENT
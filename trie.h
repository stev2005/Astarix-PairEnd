#pragma once

#include<bits/stdc++.h>
using namespace std;

struct Trie{
    Trie *a , *g , *t , *c ;
    map < int , char > seeds ;
    int iEnd , depth ;
    Trie (){
        a = nullptr ;
        g = nullptr ;
        t = nullptr ;
        c = nullptr ;
        iEnd = 0 ;
        depth = 0 ;
    }
}

inline void init_trie(Trie *&T){
    T = new Trie() ;
}

void insert_seed ( Trie *&T, string &s , int pos , int num){
    if ( pos == s.size() ){
        T -> iEnd = num;
        T -> depth = s.size() ;
        return ;
    }
    seeds .  insert ( { num , s[pos] } ) ;
    if ( s[pos] == 'A' ){
        if ( T -> A == nullptr )
            init_trie ( T -> a ) ;
        insert_word ( T -> a , s , pos + 1 , num ) ;
    }
    else if ( s[pos] == 'G' ){
        if ( T -> G == nullptr )
            init_trie ( T -> g ) ;
        insert_word ( T -> g , s , pos + 1 , num ) ;
    }
    else if ( s[pos] == 'T' ){
        if ( T -> t ==  nullptr )
            init_trie ( T -> t ) ;
        insert_word ( T -> t , s , pos + 1 , num ) ;
    }
    else{
        if ( T -> c == nullptr )
            init_trie ( T -> c ) ;
        insert_word ( T -> c , s , pos + 1 , num ) ;
    }
}

void get_nth_seed ( Trie *&T , string *&s , int n ){
    if ( T == NULL )
        return ;
    char lett = T -> seeds [n] ;
    s += lett ;
    switch(lett){
        case 'A': get_nth_seed ( T-> a , s , n ) ;
                  break ;
        case 'G': get_nth_seed ( T-> g , s , n ) ;
                  break ;
        case 'T': get_nth_seed ( T-> t , s , n ) ;
                  break ;
        case 'C': get_nth_seed ( T-> c , s , n ) ;
                  break ;
    }
}
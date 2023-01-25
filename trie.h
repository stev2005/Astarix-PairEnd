#pragma once

#include  <bits/stdc++.h>
using namespace std;

char base[4]={'A','C','G','T'};

struct Trie{
    Trie *child[4];
    int num;
    Trie(){
        for(int i=0;i<4;++i)
            child[i]=nullptr;
        num=-1;
    }
};

/*inline void init_trie(Trie *&T){
    T = new Trie() ;
}*/

void insert_seed ( Trie *&T, string &s , int pos , int &num){
    if ( pos == s.size() ){
        if(T->num==-1)
            T->num = num;
        else num=T->num;
        return ;
    }
    for(int i=0;i<4;++i)
        if(s[pos]==base[i]){
            if(T->child[i]==nullptr)
                T->child[i]=new Trie();
            insert_seed(T->child[i],s,pos+1,num);
        }
}

void printout_seeds(Trie *T,string s){
    bool leaf=true;
    for(int i=0;i<4;++i){
        if(T->child[i]==nullptr)
            continue;
        leaf=false;
        printout_seeds(T->child[i],s+base[i]);
    }
    if(leaf) cout<<s<<" "<<T->num<<"\n";
}

/*void insert_seed ( Trie *&T, string &s , int pos , int &num){
    if ( pos == s.size() ){
        if(T->num==-1)
            T->num = num;
        else num=T->num;
        return ;
    }
    //seeds .  insert ( { num , s[pos] } ) ;
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
}*/

/*void get_nth_seed ( Trie *&T , string *&s , int n ){
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
}*/


/*struct Trie{
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
};*/


/*struct Trie{
    Trie *a, *g, *t, *c;
    int num;
    Trie(){
        a=nullptr;
        g=nullptr;
        t=nullptr;
        c=nullptr;
        num=-1;
    }
};*/
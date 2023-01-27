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

#pragma once

#include "header.h"
#include  <bits/stdc++.h>
using namespace std;

struct Trie{
    Trie *child[4];
    int num;
    Trie(){
        for (int i = 0; i < 4; ++i)
            child[i]=nullptr;
        num=-1;
    }
    
    bool is_leaf(){
        for (int i = 0; i < 4; ++i)
            if (child[i] != nullptr)
                return false;
        return true;
    }

    int num_of_leaf(char c){
        for (int i = 0; i < 4; ++i)
            if(child[i] != nullptr && base[i] == c)
                return i;
        return -1;
    }

};



/*void insert_kmer( Trie *&T,  string &s ,  int pos ,  int &num){
    if (pos == s.size()){
        if(T->num == -1)
            T->num = num;
        else num = T->num;
        return ;
    }
    for(int i = 0; i<4; ++i)
        if (s[pos] == base[i]){
            if(T->child[i]==nullptr)
                T->child[i]=new Trie();
            insert_kmer(T->child[i], s, pos+1, num);
        }
}*/

void insert_kmer(Trie *&T, string::iterator it, string::iterator bound, int &num){
    if (it == bound){
        if(T->num == -1)
            T->num = num;
        else num = T->num;
        return ;
    }
    for (int i = 0; i < 4; ++i)
        if (*it == base[i]){
            if (T->child[i] == nullptr)
                T->child[i] = new Trie();
            insert_kmer(T->child[i], it+1, bound, num);
        }
}

/*inline void construct_trie(string &ref, Trie *&T, vector<int>&last, vector<int>&prevpos){
    int m=ref.size();
    int k=log2(m);
    int sz;
    prevpos.resize(m, -1);
    last.resize(m, -1);
    int cntkmer=0, prevcnt;
    string kmer;
    for(int i=0;i<m;i+=k){
        kmer=ref.substr(i, k);
        sz=kmer.size();
        prevcnt=cntkmer;
        insert_kmer(T, kmer, 0, cntkmer);
        prevpos[i+sz-1]=last[cntkmer];
        last[cntkmer]=i+sz-1;
        if(prevcnt==cntkmer)
            ++cntkmer;
        else cntkmer=prevcnt;
    }
}*/

inline void construct_trie_simple(string &ref, Trie *&T, vector<int>&last, vector<int>&prevpos){
    int m = ref.size();
    int k=log2(m);
    int sz;
    prevpos.resize(m, -1);
    last.resize(m, -1);
    int cntkmer=0, prevcnt;
    string::iterator st,fi;
    for(int i=0;i<m-k+1;++i){
        assert(cout<<"kmer with beign i=="<<i<<"\n");
        st = ref.begin()+i;
        fi = st+k;
        prevcnt=cntkmer;
        sz = k;
        insert_kmer(T, st, fi, cntkmer);
        assert(cout<<"inserted kmer\n");
        assert(cout<<"cntkmer=="<<cntkmer<<"\n");
        prevpos[i+sz-1]=last[cntkmer];
        last[cntkmer]=i+sz-1;
        if(prevcnt==cntkmer)
            ++cntkmer;
        else cntkmer=prevcnt;
    }
}

inline void print_out_last_prevpos(vector<int>&last, vector<int>&prevpos){
    for(auto cur:last)
        cout<<cur<<" ";
    cout<<"\n";
    for(auto cur:prevpos)
        cout<<cur<<" ";
    cout<<"\n";
}

void printout_kmers(Trie *T, string s){
    bool leaf=true;
    for(int i=0;i<4;++i){
        if(T->child[i]==nullptr)
            continue;
        leaf=false;
        printout_kmers(T->child[i], s+base[i]);
    }
    if(leaf) cout<<s<<" "<<T->num<<"\n";
}

#pragma once

#include "header.h"
#include  <bits/stdc++.h>
using namespace std;

struct Trie{
    Trie *child[4];
    int num;///the number of this kmer
    char bp;
    int firstapp;///first position fo ending of kmers got from childs of this Trie vertex
    Trie(){
        for (int i = 0; i < 4; ++i)
            child[i] = nullptr;
        num = -1;
        firstapp = -1;
        bp='\0';
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

void insert_kmer( Trie *&T,  string &s ,  int pos ,  int &num, int posapp){
    if (pos == s.size()){
        if(T->num == -1){
            T->num = num;
            T->firstapp = posapp;
        }
        else num = T->num;
        return ;
    }
    for(int i = 0; i<4; ++i)
        if (s[pos] == base[i]){
            if (T->child[i]==nullptr){
                T->child[i]=new Trie();
                //T->child[i]->bp=base[i];
            }    
            insert_kmer(T->child[i], s, pos+1, num, posapp);
            if (T->firstapp == -1)
                T->firstapp = T->child[i]->firstapp;
            else T->firstapp = min(T->firstapp, T->child[i]->firstapp);
        }
}

inline void construct_trie(string &ref, int k, Trie *&T, vector<int>&last, vector<int>&prevpos){
    int m=ref.size();
    int sz;
    prevpos.resize(m, -1);
    last.resize(m, -1);
    int cntkmer=0, prevcnt;
    string kmer;
    for(int i=0;i<m;i+=k){
        kmer=ref.substr(i, k);
        sz=kmer.size();
        prevcnt=cntkmer;
        insert_kmer(T, kmer, 0, cntkmer, i+sz-1);
        prevpos[i+sz-1]=last[cntkmer];
        last[cntkmer]=i+sz-1;
        if(prevcnt==cntkmer)
            ++cntkmer;
        else cntkmer=prevcnt;
    }
}

inline void construct_trie_simple(string &ref, int k, Trie *&T, vector<int>&last, vector<int>&prevpos){
    int m = ref.size();
    int sz;
    prevpos.resize(m, -1);
    last.resize(m, -1);
    int cntkmer=0, prevcnt;
    //string::iterator st,fi;
    string kmer;
    for(int i=0;i<m-k+1;++i){
        prevcnt=cntkmer;
        kmer = ref.substr(i, k);
        sz = kmer.size();
        insert_kmer(T, kmer, 0, cntkmer, i);
        //assert(cout<<"inserted kmer\n");
        //assert(cout<<"cntkmer=="<<cntkmer<<"\n");
        prevpos[i+sz-1]=last[cntkmer];
        last[cntkmer]=i+sz-1;
        if(prevcnt==cntkmer)
            ++cntkmer;
        else cntkmer=prevcnt;
    }
}

int kmer_exists(string &seed, int pos, Trie *T){
    if (pos == seed.size())
        return T->num;
    for (int i = 0; i < 4; ++i)
        if (seed[pos] == base [i]){
            if (T->child[i] == nullptr)
                return -1;
            else return kmer_exists(seed, pos+1, T->child[i]);
        }
}

vector<int> query_into_seeds(string &query, int k, Trie *root){
    vector<int>seeds;
    int n = query.size(), num;
    string seed;
    for (int i = 0; i < n; i+=k){
        seed = query.substr(i, k);
        num = kmer_exists(seed, 0, root);
        seed.push_back(num);
    }
    return seeds;
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

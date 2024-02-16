#pragma once

#include "header.h"
#include  <bits/stdc++.h>
using namespace std;

struct Trie{
    Trie *child[4];
    Trie *parent;
    int num;///the number of this kmer
    //char bp;
    //int firstapp;///first position fo ending of kmers got from childs of this Trie vertex
    vector<int> positions;
    Trie(){
        for (int i = 0; i < 4; ++i)
            child[i] = nullptr;
        parent = nullptr;
        num = -1;
        //firstapp = -1;
        //bp='\0';
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

void insert_kmer(Trie *&T,  string &s ,  int pos ,  int &num, int posapp, Trie *&tocon, bool savepos){
    if (savepos)
        T->positions.push_back(savepos);
    if (pos == s.size()){
        if(T->num == -1){
            T->num = num;
            //T->firstapp = posapp;
        }
        else num = T->num;
        //return T;
        tocon = T;
        return;
    }
    for(int i = 0; i<4; ++i)
        if (s[pos] == base[i]){
            if (T->child[i] == nullptr){
                T->child[i] = new Trie();
                T->child[i]->parent = T;
                //T->child[i]->bp=base[i];
            }    
            insert_kmer(T->child[i], s, pos+1, num, posapp, tocon, savepos);
            /*if (T->firstapp == -1)
                T->firstapp = T->child[i]->firstapp;
            else T->firstapp = min(T->firstapp, T->child[i]->firstapp);*/
        }
}

inline void construct_trie(string &ref, int k, Trie *&T, vector<int> &last, vector<int> &prevpos, vector<Trie*> &backtotrieconnection, bool savepos){
    int m = ref.size();
    int sz;
    prevpos.resize(m, -1);
    last.resize(m, -1);
    backtotrieconnection.resize(m, nullptr);
    int cntkmer = 0, prevcnt;
    string kmer;
    for(int i = 0; i < m - k + 1; ++i){
        prevcnt=cntkmer;
        kmer = ref.substr(i, k);
        sz = kmer.size();
        Trie *con;
        insert_kmer(T, kmer, 0, cntkmer, i, con, savepos);
        backtotrieconnection[i+sz-1] = con;
        prevpos[i+sz-1]=last[cntkmer];
        last[cntkmer]=i+sz-1;
        if(prevcnt==cntkmer)
            ++cntkmer;
        else cntkmer=prevcnt;
    }
    for (int i = m - k + 1; i < m; ++i)///the last suffixes of the reference have length < k 
        backtotrieconnection.push_back(nullptr);
}

inline void construct_trie_info_about_transitions_into_ref(string &ref, int k, Trie *&T, vector<int> &last, vector<int> &prevpos, vector<Trie*> &backtotrieconnection){
    int m = ref.size();
    int sz;
    prevpos.resize(m, -1);
    last.resize(m, -1);
    backtotrieconnection.resize(m, nullptr);
    int cntkmer = 0, prevcnt;
    string kmer;
    unordered_map<Trie*, int> timesexitleaf;
    for(int i = 0; i < m - k + 1; ++i){
        prevcnt=cntkmer;
        kmer = ref.substr(i, k);
        sz = kmer.size();
        Trie *con;
        insert_kmer(T, kmer, 0, cntkmer, i, con, false);
        backtotrieconnection[i+sz-1] = con;
        prevpos[i+sz-1]=last[cntkmer];
        last[cntkmer]=i+sz-1;
        if(prevcnt==cntkmer)
            ++cntkmer;
        else cntkmer=prevcnt;
        timesexitleaf[con]++;
    }
    for (int i = m - k + 1; i < m; ++i)///the last suffixes of the reference have length < k 
        backtotrieconnection.push_back(nullptr);
    map<int, int> numofleafsperexitcnt;
    for (auto it = timesexitleaf.begin(); it !=  timesexitleaf.end(); ++it)
        numofleafsperexitcnt[it->second]++;
    for (auto it = numofleafsperexitcnt.begin(); it != numofleafsperexitcnt.end(); ++it)
        cout << "exits in the reference: " << it-> first << ", number of leaf: " << it -> second << endl;
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
    return -1;
}

vector<int> query_into_seeds(string &query, int k, Trie *root){
    //cout <<"Trie.h query in seeds: query == " << query << endl;
    vector<int>seeds;
    int n = query.size(), num;
    string seed;
    for (int i = 0; i < n - k + 1; i+=k){
        seed = query.substr(i, k);
        //cout << "seed == "<<seed;
        num = kmer_exists(seed, 0, root);
        seeds.push_back(num);
    }
    //cout <<"Trie.h query in seeds: seeds.size == "<<seeds.size()<<endl; 
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

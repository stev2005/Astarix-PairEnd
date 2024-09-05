#pragma once

#include "header.h"
#include  <bits/stdc++.h>
using namespace std;

struct Trie{
    Trie *child[alphabetsz];
    Trie *parent;
    int num;
    Trie(){
        for (int i = 0; i < alphabetsz; ++i)
            child[i] = nullptr;
        parent = nullptr;
        num = -1;
    }
    ~Trie() {
        for (int i = 0; i < alphabetsz; ++i) {
            if (child[i] != nullptr) {
                delete child[i];
                child[i] = nullptr;
            }
        }
    }
    bool isleaf(){
        for (int i = 0; i < alphabetsz; ++i)
            if (child[i] != nullptr) return false;
        return true;
    }
};

void insert_kmer(Trie* T, Trie* &tocon, deque<char>::iterator it, deque<char>::iterator endit, int &numkmer){
    if (it == endit){
        if (T->num == -1)
            T->num = numkmer;
        else numkmer = T->num;
        tocon = T;//will be used for connnection between trie index and l.ref.
        return;
    }
    char c = *it;
    for (int i = 0; i < alphabetsz; ++i){
        if (c == base[i]){
            if (T->child[i] == nullptr){
                T->child[i] = new Trie();
                T->child[i]->parent = T;
            }
            insert_kmer(T->child[i], tocon, it + 1, endit, numkmer);
        }
    }
}

void insert_kmer(Trie *&T,  string &s ,  int pos ,  int &num, Trie *&tocon){
    if (pos == s.size()){
        if(T->num == -1){
            T->num = num;
        }
        else num = T->num;
        tocon = T;
        return;
    }
    for(int i = 0; i < alphabetsz; ++i)
        if (s[pos] == base[i]){
            if (T->child[i] == nullptr){
                T->child[i] = new Trie();
                T->child[i]->parent = T;
            }    
            insert_kmer(T->child[i], s, pos + 1, num, tocon);
        }
}

inline void construct_trie(const string &ref, const int k, Trie *&T, vector<int> &last, vector<int> &prevpos, vector<Trie*> &backtotrieconnection){
    int m = ref.size();
    int recordedkmer = 0;//the smallest non-negative not used number for a kmer/dmer (depending on which trie being built)
    last.resize(m, -1);
    prevpos.resize(m, -1);
    backtotrieconnection.resize(m, nullptr);
    deque<char> kmer;

    /*for (int i = 0; i < k - 1 ; ++i)
        kmer.push_back(ref[i]);*/
    //cerr << "Inserting kmers\n";
    static int treecnt = 0;
    for (int i = 0; i < m - k + 1; ++i/*, cerr << "\n"*/){
        int rpos = i + k - 1;
        string s(ref.substr(i, k - 1));
        Trie *tocon;
        int numkmer = recordedkmer;
        //assert(kmer.size() == k - 1);
        //insert_kmer(T, tocon, kmer.begin(), kmer.end(), numkmer);
        insert_kmer(T, s, 0, numkmer, tocon);
        //cerr << "kmer inserted" << treecnt << "\n";
        prevpos[rpos] = last[numkmer];
        last[numkmer] = rpos;
        if (numkmer == recordedkmer)
            ++recordedkmer;
        //kmer.pop_front();
        //kmer.push_back(ref[rpos]);
    }
    ++treecnt;
    cerr << "Trie constructed. recordedkmer: " << recordedkmer << "\n";
}

inline void construct_trie_dmer(const string &ref, const int d, Trie *&T, vector<int> &last, vector<int> &prevpos, vector<Trie*> &backtotrieconnection){
    int m = ref.size();
    int recorddmer = 0;//the smallest non-negative not used number for a dmer (depending on which trie being built)
    last.resize(m, -1);
    prevpos.resize(m, -1);
    backtotrieconnection.resize(m, nullptr);
    deque<char> dmer;
    int strsz = d - 1;
    for (int i = 0; i < strsz; ++i)
        dmer.push_back(ref[i]);
    assert(dmer.size() == strsz);
    for (int i = 0; i < m - d + 1; ++i){
        Trie *tocon;
        int numdmer = recorddmer;
        insert_kmer(T, tocon, dmer.begin(), dmer.end(), numdmer);
        backtotrieconnection[i + d - 1] = tocon;
        prevpos[i + strsz /*- 1*/] = last[numdmer];
        last[numdmer] = i + strsz /*- 1*/;
        if (numdmer == recorddmer)
            ++recorddmer;
        dmer.pop_front();
        dmer.push_back(ref[i + strsz]);
        assert(i + strsz < m);
    }
    cerr << "recorddmer: " << recorddmer << "\n";
}

inline void construct_trie_kmer(const string &ref, const int k, Trie *&T, vector<int> &last, vector<int> &prevpos, vector<Trie*> &backtotrieconnection){
    int m = ref.size();
    int recordkmer = 0;//the smallest non-negative not used number for a dmer (depending on which trie being built)
    last.resize(m, -1);
    prevpos.resize(m, -1);
    backtotrieconnection.resize(m, nullptr);
    deque<char> kmer;
    for (int i = 0; i < k - 1; ++i)
        kmer.push_back(ref[i]);
    for (int i = k - 1; i < m - k + 1; ++i){
        kmer.push_back(ref[i]);
        assert(kmer.size() == k);
        Trie *tocon;
        int numkmer = recordkmer;
        insert_kmer(T, tocon, kmer.begin(), kmer.end(), numkmer);
        backtotrieconnection[i] = tocon;
        prevpos[i] = last[numkmer];
        last[numkmer] = i;
        if (recordkmer == numkmer)
            ++recordkmer;
        kmer.pop_front();
    }
    cerr << "recorkdmer: " << recordkmer << "\n";
}

int kmer_exists(Trie *T, string::iterator it, string::iterator endit){
    if (it == endit) return T->num;
    for (int i = 0; i < alphabetsz; ++i){
        if (base[i] == *it){
            if (T->child[i] == nullptr)
                return -1;
            else return kmer_exists(T->child[i], it + 1, endit);
        }
    }
    return -1;
}

inline void query_into_seeds(string &query, int k, Trie* root, vector<int> &seeds){
    int n = query.size();
    seeds.clear();
    for (int i = 0; i < n - k + 1; i += k){
        string s = move(query.substr(i, k));
        assert(s.size() == k);
        seeds.push_back(kmer_exists(root, s.begin(), s.end()));
    }
}
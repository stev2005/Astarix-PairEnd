#pragma once

#include"header.h"
#include<bits/stdc++.h>
using namespace std;

inline void read_reference(string &filename, string &ref){
    //cerr << "Start of inline void read_reference(string &filename, string &ref)\n";
    ifstream fin;
    fin.open(filename);
    if (!fin.is_open()){
        cerr << "File for reading referene does not exist\n";
        assert(fin.is_open());
        exit(-1);
    }
    string buffer;//">all genome" line
    getline(fin, buffer);
    fin >> ref;
    assert(ref.size() > 0);
    fin.close();
}

ifstream fin2; ///SE queries input file; .in file format supported
ifstream finread1, finread2; ///PE queries input file prefixes; .in file supported; TODO .fq format

inline void set_query_input_file(string &filename){
    fin2.open(filename);
    if (!fin2.is_open()){
        cerr << "File for reading query samples does not exist\n";
        assert(fin2.is_open());
        exit(-1);
    }
}

inline void set_pe_query_input_file(string &file1, string &file2){
    //string s(prefix + "1");
    finread1.open(file1);
    //s.pop_back();
    //s += "2";
    finread2.open(file2);
    if (!finread1.is_open()){
        cerr << "File for reading first end query samples does not exist\n";
        assert(finread1.is_open());
        exit(-1);
    }
    if (!finread2.is_open()){
        cerr << "File for reading second end query samples does not exist\n";
        assert(finread2.is_open());
        exit(-1);
    }
}

int get_num_testcases(){
    int num;
    fin2 >> num;
    return num;
}

string get_single_read_query(){
    string s;
    fin2 >> s;
    return s;
}

pair<string, string> get_pair_end_query(){
    string seqid, descr, quality;
    pair<string, string> query;
    finread1 >> seqid;
    finread1 >> query.first;
    finread1 >> descr;
    finread1 >> quality;
    finread2 >> seqid;
    finread2 >> query.second;
    finread2 >> descr;
    finread2 >> quality;
    return query;
}

inline void close_query_in_files(){
    fin2.close();
    finread1.close();
    finread2.close();
}

bool queyreof(string &typealignment){
    if (typealignment == "single-read") return fin2.eof();
    else if (typealignment == "paired-end") return finread1.eof();
    else{
        cerr << "No such alignment supported\n";
        cerr << "Terminating the program\n";
        exit(-1);
    }
}
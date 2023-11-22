#pragma once

#include"header.h"
#include<bits/stdc++.h>
using namespace std;

inline void read_reference(string &filename, string &ref){
    ifstream fin;
    fin.open(filename);
    fin >> ref;
    assert(ref.size() > 0);
    fin.close();
}

ifstream fin2;

void set_query_in_file(string &filename){
    fin2.open(filename);
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
    pair<string, string> query;
    fin2 >> query.first;
    fin2 >> query.second;
    return query;
}

void close_query_in_file(){
    fin2.close();
}
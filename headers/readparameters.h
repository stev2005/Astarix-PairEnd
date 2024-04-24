#pragma once

#include "header.h"
#include <bits/stdc++.h>
using namespace std;

int string_to_int(string s){
    //int sz = strlen(c);
    int sz = s.size();
    int num = 0;
    for (int i = 0; i < sz; ++i)
        num = num * 10 + ((int)s[i] - (int)'0');
    return num;
}

inline void read_parameters(int argc, char *argv[], int &d, int &k, string &typealignment, string &heuristic, int &insdist, int &drange, string &fileref, string &filequery, cost_t &infheuristic, int &indaligns, int &occurposlimit, string &triecrumbsopt){
    for (int i = 1; i < argc; i += 2){
        string argument(argv[i]);
        string value(argv[i + 1]);
        //cerr << "i: " << i << " argument: " << argument << " value: " << value << "\n";
        if (argument == "--trie-depth")
            d = string_to_int(value);
        else if (argument == "--seed-len")
            k = string_to_int(value);
        else if (argument == "--alignment")
            typealignment = string(argv[i + 1]);
        else if (argument == "--heuristic")
            heuristic = string (value);
        else if (argument == "--insert-distance")
            insdist = string_to_int(value);
        else if (argument == "--filter-distance-difference")
            drange = string_to_int(value);
        else if (argument == "--reference")
            fileref = value;
        else if (argument == "--query")
            filequery = value;
        else if (argument == "--punish-heuristic-cost")
            infheuristic = string_to_int(value);
        else if (argument == "--independent-aligns")
            indaligns = string_to_int(value);
        else if (argument == "--trie-positions-limit-checker")
            occurposlimit = string_to_int(value);
        else if (argument == "--trie-crumbs-opt")
            triecrumbsopt = value;
        else{
            cerr << argument << " is not a valid program argument\n";
            cerr << "Program is going to abort\n";
            assert(false);
        }
    }
}

inline void parameters_default_values(int &d, int &k, string &typealignment, string &heuristic, int &insdist, int &drange, string &fileref, string &filequery, cost_t &infheuristic, int &indaligns, int &occurposlimit, string &triecrumbsopt){
    d = 10;
    k = 0;
    typealignment = "single-read";
    heuristic = "seed_heuristic";
    insdist = 5000;
    drange = 25;
    fileref = "data/single-reads/reference.in";
    filequery = "data/single-reads/tests/1.in";
    infheuristic = 50;
    indaligns = 1;
    occurposlimit = 0;
    triecrumbsopt = "yes";
}

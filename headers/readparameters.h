#pragma once

#include "header.h"
#include "astar/single_read_heursitic.h"
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

inline void read_parameters(int argc, char *argv[], int &d, int &k, string &fileref, string &filequery, 
                            string &filequery1, string &filequery2,
                            string &typealignment, string &heuristic,
                            string &triecrumbsopt, int &insdist, int &drange){
    for (int i = 1; i < argc; i += 2){
        string argument(argv[i]);
        string value(argv[i + 1]);
        //cerr << "argument: " << argument << "\n";
        //cerr << "value: " << value << "\n\n";
        if (argument == "--trie-depth")
            d = string_to_int(value);
        else if (argument == "--seed-len")
            k = string_to_int(value);
        else if (argument == "--reference")
            fileref = value;
        else if (argument == "--query")
            filequery = value;
        else if (argument == "--query1")
            filequery1 = value;
        else if (argument == "--query2")
            filequery2 = value;
        else if (argument == "--alignment")
            typealignment = value;
        else if (argument == "--insert-distance")
            insdist = string_to_int(value);
        else if (argument == "--filter-distance-difference")
            drange = string_to_int(value);
        else if (argument == "--heuristic"){
            heuristic = string (value);
            select_heuristic(heuristic);
        }
        else if (argument == "--trie-crumbs-opt"){
            triecrumbsopt = value;
        }
        else if (argument == "--trie-positions-limit-checker"){
            
        }
        else{
            cerr << argument << " is not a valid program argument\n";
            cerr << "Program is going to abort\n";
            assert(false);
        }
    }
}

inline void parameters_default_values(int &d, int &k, string &fileref, string &filequery, string &filequery1, string &filequery2,
                                      string &typealignment, string &heuristic,
                                      string &triecrumbsopt, int &insdist, int &drange){
    d = 10;
    k = 11;
    fileref = "";
    filequery = "";
    filequery1 = "";
    filequery2 = "";
    typealignment = "";
    heuristic = "seed_heuristic";
    triecrumbsopt = "yes";
    insdist = 1000;
    drange = 50;
}

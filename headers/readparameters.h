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

inline void read_parameters(int argc, char *argv[], int &d, int &k, string &typealignment, string &heuristic, int &insdist, int &drange){
    for (int i = 1; i < argc; i += 2){
        string argument(argv[i]);
        string value(argv[i + 1]);
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
        else if (argument == "--filter-difference")
            drange = string_to_int(value);
        else{
            cerr << argument << " is not a valid program argument\n";
            cerr << "Program is going to abort\n";
            assert(false);
        }
    }
}

inline void parameters_default_values(int &d, int &k, string &typealignment, string &heuristic, int &insdist, int &drange){
    d = 10;
    k = 13;
    typealignment = "single-read";
    heuristic = "seed_heuristic";
    insdist = 5000;
    drange = 25;
}

/*inline void program_arguments_to_variables(char *argv[], char *&typealignment, int &d, int &k, char *&heuristic, char *&shownexplstates, char *&triestart, int &dmatch){
    typealignment = argv[1];///first argument: aligning single reads or paired-end
    d = charstring_to_int(argv[2]);///second argument: value of D
    k = charstring_to_int(argv[3]);///third argument: value of k
    heuristic = argv[4];///fourth argument: used heuristic
    shownexplstates = argv[5];///fifth argument: show or not show explored states
    triestart = argv[6];///sixth argument: triestart: Yes or No
    if (strcmp(typealignment, "pairend-read") == 0)
        dmatch = charstring_to_int(argv[7]);///seventh argument: distance of filtering matches
}*/

/*int charstring_to_int(char* num){
    int len = strlen(num);
    int n = 0, digit;
    for (int i = 0; i < len; ++i){
        digit = (int)num [i] - (int)'0';
        n *= 10;
        n += digit;
    }
    return n;
}*/
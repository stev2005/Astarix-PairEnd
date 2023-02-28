#pragma once

#define NDEBUG
#include <vector>
using namespace std;
char base[4] = {'A', 'C', 'G', 'T'};

typedef int cost_t;

struct MatchingKmers{
    vector <int> seeds, last, prevpos;
    ///last: the end position of a last occurance of a kmer in the reference
    ///prevpos: end positions of previous occurances of a kmer in the reference
    ///seeds: does (seed[i]>=0) or doesn't(seed[i]==-1) the ith seed match a kmer
};


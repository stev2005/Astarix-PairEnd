#include "headers/header.h"
#include<bits/stdc++.h>
//#include "headers/dp.h"
#include "headers/trie.h"
#include "headers/astar/astar_single-reads.h"
#include "headers/astar/astar"
using namespace std;

/*void seqalignment(){
    for ( int testcase=1 ; testcase <= testcases ; ++ testcase ) {
        //query . input();
        cin >> query ;
        int rezult;
        //rezult = minimum_edit_distance_dp ( dp ,  query ,  ref ) ;
        //rezult = minimum_edit_distance_dijkstranew( query ,  ref ) ;
        //rezult=minimum_edit_distance_dijkstra_trie(query, ref, T, last, prevpos);
        rezult=minimum_edit_distance_dijkstratrienew(query, ref, T, last, prevpos);
        cout << rezult << "\n";
    }
}*/

int charstring_to_int(char *num){
    int len = strlen(num);
    int n = 0, digit;
    for (int i = 0; i < len; ++i){
        digit = (int)num [i] - (int)'0';
        n *= 10;
        n += digit;
    }
    return n;
}

void printtriecrumbs(map <Node, bitset<64> > &crumbs, Trie *T, string s){
    cout << "string s: "<< s << " crumbs: ";
    for (int i = 0; i < 4; ++i)
        cout << crumbs[Node(T)][i] << " ";
    cout << endl;
    for (int i = 0; i < 4; ++i)
        if (T->child[i] != nullptr)
            printtriecrumbs(crumbs, T->child[i], s + base[i]);
}

void printoutcrumbs(map<Node, bitset<64> > &crumbs, Trie *root){
    for (auto it = crumbs.begin(); it != crumbs.end(); ++it){
        Node cur = it->first;
        if (cur.is_in_trie() == false){
            cout << "Node in the reference "<<cur.rpos<<"\n";
            for (int i = 0; i < 4; ++i)
                cout << it->second[i] << " ";
            cout << endl;
        }
    }
    printtriecrumbs(crumbs, root, "");
}

int main(int argc, char *argv[]){
    ///first argument: aligning single reads or paired-end
    ///second argument: value of k
    ///third argument: used heuristic
    ///fourth argument: show or not chow explored states
    ///fifth argument: triestart: Yes or No
    /*ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);*/
    //DNA ref ,  query;
    assert(cout<<"Start of the program\n");
    string ref;
    string query;
    pair <string, string> queryp;
    int testcases, k = charstring_to_int(argv[2]);
    assert(cout<<"int k has a value equal to "<<k<<"\n");
    clock_t t = clock();
    cin >> ref ;
    t = clock() - t;
    assert(cout<<"entered reference\n");
    cin >> testcases ;
    cout << "reading reference: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
    assert(cout<<"entered num of tescases\n");
    ///Vital for dp seq approach
    //vector < vector < int > > dp ;
    //init_dp_table ( dp ,  ref ) ;
    //Vital for Trie 
    MatchingKmers info;
    t = clock();
    Trie *root=new Trie();
    //construct_trie(ref, root, info.last, info.prevpos);
    construct_trie_simple(ref, k, root, info.last, info.prevpos, info.connection);
    t = clock() - t;
    cout << "constructing trie: "<< (double) t / CLOCKS_PER_SEC << "s.\n"; 
    cout << "\n";
    assert(cout<<"inited kmers\n");
    for (int testcase=1; testcase<=testcases; ++testcase, cout<<endl){
        cout << "Query "<< testcase << ":\n";
        int rezult;
        t = clock();
        if (strcmp(argv[1], "single-read") == 0){
            cin>>query;
            //info.qsize = query.size();
            t = clock() - t;
            cout << "Reading query: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
            //map <Node, bitset<64> > crumbs;
            if (strcmp(argv[3], "seed_heuristic") == 0){
                t = clock();
                info.seeds1 = query_into_seeds(query, k, root);
                t = clock() - t;
                cout << "breaking query into seeds: "<< (double) t / CLOCKS_PER_SEC << "s.\n"; 
                t = clock() - t;
                ///info.crumbs = getcrumbs(ref, k, info);
                getcrumbs(ref, k, info, 1);
                t = clock() - t;
                cout << "Precompute of crumbs: " << (double) t / CLOCKS_PER_SEC << "s.\n";
                //printoutcrumbs(info.crumbs, root);
            }
            t = clock();
            rezult = astar_single_read_alignment(query, ref, k, root, info, argv[3], argv[4], argv[5]);
            cout<<rezult<<"\n";
            t = clock() - t;
            cout << "Alignment: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
            t = clock();
            info.seeds1.clear();
            info.crumbs1.clear();
            t = clock() - t;
            cout << "Cleaning help vectors: "<< (double) t / CLOCKS_PER_SEC << "s.\n";
        }
        else{
            cin>>queryp.first>>queryp.second;
            /*To do astar for paired end*/
        }
        
        
        ///seeds: does (seed[i]>=0) or doesn't(seed[i]==-1) the ith seed match a kmer
        //rezult = edit_distance_dijkstratrienew(query, ref, root, last, prevpos);
        //rezult = edit_distance_dijkstrapairedend_trie(query, ref, root, last, prevpos);
        //rezult = edit_distance_pairedend(query, ref);
        //rezult = align_astar(query, ref, info.seeds, info.last);
    }
    //cout<<"End of the main program.\n";
    return 0;
}
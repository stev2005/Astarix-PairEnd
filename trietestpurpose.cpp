#include <bits/stdc++.h>
#include "headers/trie.h"
using namespace std;
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    string ref;
    cin >> ref;
    Trie *root = new Trie();
    vector<int> last;
    vector<int> prevpos;
    vector<Trie*> backtotrieconnection;
    construct_trie_info_about_transitions_into_ref(ref, 10, root, last, prevpos, backtotrieconnection);
    return 0;
}
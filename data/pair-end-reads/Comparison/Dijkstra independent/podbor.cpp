#include<bits/stdc++.h>
using namespace std;
pair<string, string> qs[10010];
mt19937_64 rg;
int main(){
    rg.seed(time(0));
    ifstream fin;
    ofstream fout;
    fin.open("Group4.in");
    int n;
    fin >> n;
    for (int i = 1; i <= n; ++i)
        fin >> qs[i].first >> qs[i].second;
    fin.close();
    fout.open("DijTests.in");
    unordered_set<int> used;
    fout << "100\n";
    for (int i = 1; i <= 100; ++i){
        int pos;
        do{
            pos = rg() % (n + 1);
        }while (used.find(pos) != used.end());
        used.insert(pos);
        fout << qs[pos].first << " " << qs[pos].second << endl;
    }
    fout.close();
    return 0;
}
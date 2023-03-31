#include<bits/stdc++.h>
using namespace std;
mt19937 rg;
char base[4] = {'A', 'C', 'G', 'T'};
pair<int, string> getrandomgene(string &ref){
    int sz = ref.size();
    int pos = rg()%(sz-3001);
    string ret = ref.substr(pos, 3000);
    return {pos, ret};
}
pair<string, string> getquery(string &ref){
    int sz = ref.size();
    int pos = rg()%(sz-5001);
    string ret = ref.substr(pos, 5000);
    pair<string, string> query;
    query.first = ret.substr(0, 250);
    query.second = ret.substr(ret.size()-1-250, 250);
    return query;
}
string changing(string s){
    int sz = s.size();
    int times = rg()%11;
    for(int i = 0; i < times; ++i){
        int bp = rg()%4;
        int pos = rg()%sz;
        s[pos] = base[bp];
    }
    return s;
}
int find_dif(string s1, string s2){
    int cnt=0;
    for (int i =0 ;i <s1.size(); ++i)
        if (s1[i] != s2[i])
            cnt++;
    return cnt;
}
int main(){
    rg.seed(time(0));
    ifstream fin;
    ofstream fout;
    fin.open("original.in");
    fout.open("newref.in");
    string ref, refnew;
    fin >> ref;
    fin.close();
    refnew = ref;
    for (int i = 0; i < (1<<7); ++i){
        pair<int, string> snew = getrandomgene(refnew);
        const string s= snew.second;
        refnew.insert(snew.first, s);
    }
    fout<<refnew<<endl;
    int tests = 10;
    fout << tests << endl;
    while (tests){
        pair<string, string> query = getquery(refnew);
        string q1 = changing(query.first);
        string q2 = changing(query.second);
        fout<<q1<<"\n"<<q2<<"\n\n";
        cerr << "differences: "<< find_dif(query.first, q1) << " " << find_dif(query.second, q2) << "\n";
        --tests;
    }
    fout.close();
    return 0;
}
/*
differences: 4 3
differences: 1 2
differences: 4 5
differences: 4 6
differences: 3 5
differences: 0 1
differences: 6 1
differences: 2 9
differences: 6 5
differences: 7 1
*/
#include<bits/stdc++.h>
using namespace std;
char base[4] = {'A', 'C', 'G', 'T'};
mt19937_64 rg;
string int_to_string(int num){
    string s="";
    if(num==0)
        return "0";
    while (num)
    {
        s=(char)(num%10+(int)'0')+s;
        num/=10;
    }
    return s;
}
string genstring(int n){
    string s="";
    int num;
    for(int i=0;i<n;++i){
        num=rg()%4;
        s+=base[num];
    }
    return s;
}
void gentests(){
    int tests;
    cout<<"Enter cnt of tests: ";
    cin>>tests;
    int n, len, maxqcnt, qcnt;
    cout<<"Enter len of referance: ";
    cin>>n;
    cout<<"Ënter max query cnt: ";
    cin>>maxqcnt;
    string path, target, filename;
    path="/root/serverexample/Astarix-PairEnd/tests-paired-end/";
    string ref, query;
    fstream fout;
    for(int test=1; test<=tests; ++test){
        filename=int_to_string(test)+".in";
        target=path+filename;
        fout.open(target, ios_base::app);
        ref= genstring(n);
        fout<<ref<<endl;
        qcnt=rg()%maxqcnt+1;
        if (qcnt<5)
            qcnt=5;
        fout<<qcnt<<endl;
        for(int i=0; i<qcnt; ++i){
            len= rg()%(n/3)+1;
            query= genstring(len);
            fout<<query<<" ";
            query= genstring(len);
            fout<<query<<endl;
        }
        fout.close();
        filename=int_to_string(test)+"sol";
        target=path+filename;
        fout.open(target, ios_base::app);
        fout.close();
    }
}
int main(){
    rg.seed(time(0));
    gentests();
    return 0;
}
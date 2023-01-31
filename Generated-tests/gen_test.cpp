#include<bits/stdc++.h>
using namespace std;
char base[4] = {'A', 'C', 'G', 'T'};
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
        num=rand()%4;
        s+=base[num];
    }
    return s;
}
void gentest(int test,int limit){
    string testname="input";
    testname+=int_to_string(test);
    testname+=".txt";
    cout<<"testname: "<<testname<<endl;
    fstream fout;
    fout.open(testname, ios_base::app);
    //fout.open("input.txt");
    int n=rand()%limit+30;
    string ref=genstring(n);
    fout<<ref<<"\n";
    int testcase=15,len;
    fout<<testcase<<"\n";
    string query;
    for(int i=0;i<testcase;++i){
        query="";
        len=rand()%(n-10);
        if(len==0)len=1;
        query=genstring(len);
        fout<<query<<"\n";
    }
    fout.close();
}
void purpose1(){
    srand(time(0));
    int numtests,limit;
    //cin>>numtests>>limit;
    cout<<"Enter numtests: ";
    cin>>numtests;
    cout<<"Enter limit: ";
    cin>>limit;
    for(int test=1;test<=numtests;++test)
        gentest(test,limit);
    cout<<"End of the program\n";
}
void genoutputs(){
    int numtests;
    cin>>numtests;
    string testname="";
    string path="/root/serverexample/Astarix-PairEnd/tests/out/";
    string target;
    for(int test=1;test<=numtests;++test){
        testname="output";
        testname+=int_to_string(test);
        testname+=".out";
        target=path+testname;
        fstream fout;
        fout.open(target, ios_base::app);
        fout.close();
    }
}
int main(){
    //purpose1();
    genoutputs();
    return 0;
}
#ifndef DNA_H
#define DNA_H
#include<bits/stdc++.h>
using namespace std;

class DNA{
	public:
	string seqId;
    string Ref;
    string add_info;
    string quality_val;
    int size(){
        return Ref.size();
    }
    char get_base_at_pos(int pos){
        return Ref[pos-1];
    }
    void input(){
        cin >> seqId;
        cin >> Ref;
        cin >> add_info;
        cin >> quality_val;
    }
    void output(){
        cout << seqId << "\n";
        cout << Ref << "\n";
        cout << add_info << "\n";
        cout << quality_val << "\n";
    }
};

#endif // DNA_H
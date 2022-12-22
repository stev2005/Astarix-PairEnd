#include<iostream>
#include<string>
#include<queue>
using namespace std;
struct DNA{
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
struct Node{
    int querypos, refpos, cost;
    Node(){}
    Node(int _querypos, int _refpos, int _cost){
        querypos=_querypos;
        refpos=_refpos;
        cost=_cost;
    }
    bool operator< (const Node& cell) const{
        return cost>cell.cost;
    }
};
int Dijkstra(DNA &query,DNA &Ref){
    int n, m;
    n=query.size();
    m=Ref.size();
    priority_queue<Node>q;
    for (int i=0; i<=m; ++i)
        q.push(Node(0,i,0));
    Node w,nb;
    char c1,c2;
    while (!q.empty()){
        w=q.top();
        q.pop();
        if (w.querypos==n)break;
        if (w.refpos<m){
            nb=Node(w.querypos,w.refpos+1,w.cost+1);
            q.push(nb);
        }
        if (w.querypos<n&&w.refpos<m){
            c1=Ref.get_base_at_pos(w.refpos+1);
            c2=query.get_base_at_pos(w.querypos+1);
            //nb=Node(w.querypos+1,w.refpos+1,w.cost+(c1==c2)?1:0);
            nb=Node(w.querypos+1,w.refpos+1,w.cost);
            if(c1!=c2)++nb.cost;
            q.push(nb);
        }
        if (w.querypos<n){
            nb=Node(w.querypos+1,w.refpos,w.cost+1);
            q.push(nb);
        }
    }
    return w.cost;
}
int main(){
    DNA Ref, query;
    int N;
    cout <<"Enter reference:\n";
    Ref.input();
    cout << "Enter number of queries\n";
    cin >> N;
    for (int i=0;i<N;++i){
        cout << "Enter query:\n";
        query.input();
        cout << "Minimum edit distance:"<<Dijkstra(query,Ref) << endl;
    }
    return 0;
}

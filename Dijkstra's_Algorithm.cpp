#include<iostream>
#include<string>
#include<queue>
using namespace std;
struct DNA
{
    string seqId;
    string Ref;
    string add_info;
    string quality_val;
    int get_size_of_sequence()
    {
        return Ref.size();
    }
    char get_base_at_pos(int pos)
    {
        return Ref[pos-1];
    }
    void input()
    {
        cin>>seqId;
        cin>>Ref;
        cin>>add_info;
        cin>>quality_val;
    }
    void output()
    {
        cout<<seqId<<"\n";
        cout<<Ref<<"\n";
        cout<<add_info<<"\n";
        cout<<quality_val<<"\n";
    }
};
struct Node
{
    int row,col,price;
    Node()
    {

    }
    Node(int _row,int _col,int _price)
    {
        row=_row;
        col=_col;
        price=_price;
    }
    bool operator<(const Node& cell)const
    {
        return price>cell.price;
    }
};
int Dijkstra(DNA &query,DNA &Ref)
{
    int n,m;
    n=query.get_size_of_sequence();
    m=Ref.get_size_of_sequence();
    priority_queue<Node>q;
    for(int i=0;i<=m;++i)
        q.push(Node(0,i,0));
    Node w,nb;
    char c1,c2;
    while(!q.empty())
    {
        w=q.top();
        q.pop();
        if(w.row==n)break;
        if(w.col<m)
        {
            nb=Node(w.row,w.col+1,w.price+1);
            q.push(nb);
        }
        if(w.row<n&&w.col<m)
        {
            c1=Ref.get_base_at_pos(w.col+1);
            c2=query.get_base_at_pos(w.row+1);
            //nb=Node(w.row+1,w.col+1,w.price+(c1==c2)?1:0);
            nb=Node(w.row+1,w.col+1,w.price);
            if(c1!=c2)++nb.price;
            q.push(nb);
        }
        if(w.row<n)
        {
            nb=Node(w.row+1,w.col,w.price+1);
            q.push(nb);
        }
    }
    return w.price;
}
int main()
{
    DNA Ref,query;
    int N;
    cout<<"Enter reference:\n";
    Ref.input();
    cout<<"Enter number of queries\n";
    cin>>N;
    for(int i=0;i<N;++i)
    {
        cout<<"Enter query:\n";
        query.input();
        cout<<"Minimum edit distance:"<<Dijkstra(query,Ref)<<endl;
    }
    return 0;
}

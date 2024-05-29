#pragma once
#include<bits/stdc++.h>
using namespace std;
template <typename Prior, typename Vertex>
class BucketQueue{
    private:
        unordered_map<Prior, queue<Vertex> > prq;///the priority-bucket queue;
        using Comparator = std::function<bool(Prior, Prior)>;
        Comparator cmpprior = [] (Prior a, Prior b){return a > b;};
        //static bool cmpprior(Prior a, Prior b){return a > b;};
        //priority_queue<Prior, vector<Prior>, decltype(cmpprior)> priorities(cmpprior);
        priority_queue<Prior, vector<Prior>, Comparator> priorities{cmpprior};
        int size;

    public:
        BucketQueue(){
            size = 0;
        }

        ~BucketQueue(){
            prq.clear();
            //priorities.clear();
            while (!priorities.empty())
                priorities.pop();
        }

        void Push(Prior pri, Vertex ver){
            if (prq.find(pri) == prq.end())
                priorities.push(pri);
            prq[pri].push(ver);
            size++;
        }

        bool Empty(){
            return priorities.empty();
        }

        pair<Prior, Vertex> Top(){
            if (priorities.empty())
                assert(false);
            Prior pri = priorities.top();
            Vertex ver = prq[pri].front();
            return {pri, ver};
        }

        void Pop(){
            if (priorities.empty())
                assert(false);
            Prior pri = priorities.top();
            prq[pri].pop();
            if (prq[pri].empty()){
                auto it = prq.find(pri);
                prq.erase(it);
                priorities.pop();
            }
            size--;
        }

        int Size(){
            return size;
        }
};
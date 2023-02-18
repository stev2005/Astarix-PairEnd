#pragma once

#include "../header.h"
#include "../trie.h"
#include <bits/stdc++.h>

using namespace std;

struct Nodeptr{
    int qpos, lpos, rpos, cost;
    Trie *u, *v;

    Nodeptr(){}

    Nodeptr(int _qpos, int _lpos, int _rpos, Trie *_u, Trie *_v, int _cost){
        qpos = _qpos;
        lpos = _lpos;
        rpos = _rpos;
        u = _u;
        v = _v;
        cost = _cost;
    }

    bool operator<(const Nodeptr &state)const{
        return cost>state.cost;
    }

    void print(){
        cout<<"Parameters and cost of the state: ";
        cout<<"qpos=="<<qpos<<" lpos=="<<lpos<<" rpos=="<<rpos;
        cout<<" u=="<<u<<" v=="<<v<<" cost=="<<cost<<endl;
    }
};

auto setNodeptrcmp = [](Nodeptr c, Nodeptr d){
    if (c.qpos == d.qpos){
        if (c.lpos == d.lpos){
            if (c.rpos == d.rpos){
                if (c.u == d.u )
                    return c.v < d.v;
                return c.u < d.u;
            }
            return c.rpos < d.rpos;
        }
        return c.lpos < d.lpos;
    }
    return c.qpos < d.qpos;
};

int edit_distance_pairedend_trie(pair <string, string> &query, string &ref, Trie *T, vector<int>&last, vector<int>&prevpos){
    assert(cout<<"Enter in the dijkstra function\n");
    priority_queue <Nodeptr> q;
    set <Nodeptr, decltype(setNodeptrcmp)> explored (setNodeptrcmp);
    Nodeptr w, nb;
    w = Nodeptr (0, -1, -1, T, T, 0);
    q.push(w);
    string left = query.first;
    string right = query.second;
    int n = left.size();
    int m = ref.size();
    while (!q.empty()){
        w = q.top();
        q.pop();
        //w.print();
        if (explored.find(w) == explored.end())
            explored.insert(w);
        else continue;
        if (w.qpos == n)
            return w.cost;
        if (w.u == nullptr && w.v == nullptr){
            if (w.lpos < m && w.rpos < m){
                nb = Nodeptr(w.qpos+1, w.lpos+1, w.rpos+1, nullptr, nullptr, w.cost);
                if (left[w.qpos] != ref[w.lpos])
                    ++nb.cost;
                if (right[w.qpos] != ref[w.rpos])
                    ++nb.cost;
                q.push(nb);
                if (nb.cost == w.cost)
                    continue;

                nb = Nodeptr(w.qpos, w.lpos+1, w.rpos+1, nullptr, nullptr, w.cost+2);
                q.push(nb);   
            }
            nb = Nodeptr(w.qpos+1, w.lpos, w.rpos, nullptr, nullptr, w.cost+2);
            q.push(nb);

            if (w.lpos < m){
                nb = Nodeptr(w.qpos, w.lpos+1, w.rpos, nullptr, nullptr, w.cost+1);
                q.push(nb);

                nb = Nodeptr(w.qpos+1, w.lpos+1, w.rpos, nullptr, nullptr, w.cost+1);
                if (left[w.qpos] != ref[w.lpos])
                    ++nb.cost;
                q.push(nb);
            }

            if (w.rpos < m){
                nb = Nodeptr(w.qpos, w.lpos, w.rpos+1, nullptr, nullptr, w.cost+1);
                q.push(nb);

                nb = Nodeptr(w.qpos+1, w.lpos, w.rpos+1, nullptr, nullptr, w.cost+1);
                if (right[w.qpos] != ref[w.rpos])
                    ++nb.cost;
            }
        }

        if (w.u == nullptr && w.rpos == -1){
            bool leaf = w.v->is_leaf();
            if (leaf){
                for (int i = last[w.v->num]; i != -1; i = prevpos[i]){
                    nb = Nodeptr(w.qpos, w.lpos, i+1, nullptr, nullptr, w.cost);
                    q.push(nb);
                }
                continue;
            }
            if (w.lpos < m){
                int num = w.v->num_of_leaf(right[w.rpos]);
                if (num != -1 && left[w.qpos] == ref [w.lpos]){
                    nb = Nodeptr(w.qpos+1, w.lpos+1, -1, nullptr, w.v->child[num], w.cost);
                    q.push(nb);
                    continue;
                }

                nb = Nodeptr(w.qpos, w.lpos+1, -1, nullptr, w.v, w.cost+1);
                q.push(nb);

                nb = Nodeptr(w.qpos+1, w.lpos+1, -1, nullptr, w.v, w.cost+1);
                if (left[w.qpos] != ref[w.lpos])
                    ++nb.cost;
                q.push(nb);
            }

            nb = Nodeptr(w.qpos+1, w.lpos, -1, nullptr, w.v , w.cost+2);
            q.push(nb);
            
            for(int i = 0; i < 4; ++i)
                if (w.v->child[i] != nullptr){
                    if(w.lpos < m){
                        nb = Nodeptr(w.qpos+1, w.lpos+1, -1, nullptr, w.v->child[i], w.cost);
                        if (left[w.qpos] != ref[w.lpos])
                            ++nb.cost;
                        if (right[w.qpos] != base[i])
                            ++nb.cost;
                        q.push(nb);

                        nb = Nodeptr(w.qpos, w.lpos+1, -1, nullptr, w.v->child[i], w.cost+2);
                        q.push(nb);
                    }

                    nb = Nodeptr(w.qpos, w.lpos, -1, nullptr, w.v->child[i], w.cost+1);
                    q.push(nb);

                    nb = Nodeptr(w.qpos+1, w.lpos, -1, nullptr, w.v->child[i], w.cost+1);
                    if (right[w.qpos] != base[i])
                        ++nb.cost;
                    q.push(nb);                    
                }
        }

        if (w.lpos == -1 && w.v == nullptr){
            bool leaf = w.u->is_leaf();
            if (leaf){
                for (int i = last[w.u->num]; i != -1; i = prevpos[i]){
                    nb = Nodeptr(w.qpos, i+1, w.rpos, nullptr, nullptr, w.cost);
                    q.push(nb);
                }
                continue;
            }
            if (w.rpos < m){
                int num = w.u->num_of_leaf(left[w.qpos]);
                if (num != -1 && right[w.qpos] == ref[w.rpos]){
                    nb = Nodeptr(w.qpos+1, -1, w.rpos+1, w.u->child[num], nullptr, w.cost);
                    q.push(nb);
                    continue;
                }

                nb = Nodeptr(w.qpos, -1, w.rpos+1, w.u, nullptr, w.cost+1);
                q.push(nb);

                nb = Nodeptr(w.qpos+1, -1, w.rpos+1, w.u, nullptr, w.cost+1);
                if (right[w.qpos] != ref[w.rpos])
                    ++nb.cost;
                q.push(nb);
            }

            nb = Nodeptr(w.qpos+1, -1, w.rpos, w.u, nullptr, w.cost+2);
            q.push(nb);

            for (int i = 0; i < 4; ++i)
                if (w.u->child[i] != nullptr){
                    if(w.rpos < m){
                        nb = Nodeptr(w.qpos+1, -1, w.rpos+1, w.u->child[i], nullptr, w.cost);
                        if (left[w.qpos] != base[i])
                            ++nb.cost;
                        if (right[w.qpos] == ref[w.rpos])
                            ++nb.cost;
                        q.push(nb);

                        nb = Nodeptr(w.qpos, -1, w.rpos+1, w.u->child[i], nullptr, w.cost+2);
                        q.push(nb);
                    }

                    nb = Nodeptr(w.qpos, -1, w.rpos, w.u->child[i], nullptr, w.cost+1);
                    q.push(nb);

                    nb = Nodeptr(w.qpos+1, -1, w.rpos, w.u->child[i], nullptr, w.cost+1);
                    if (left[w.qpos] != base[i])
                        ++nb.cost;
                    q.push(nb);
                }
        }

        if (w.lpos == -1 && w.rpos == -1){
            bool leafl = w.u->is_leaf();
            if (leafl){
                for (int i = last[w.u->num]; i != -1; i = prevpos[i]){
                    nb = Nodeptr(w.qpos, i+1, -1, nullptr,  w.v, w.cost);
                    q.push(nb);
                }
                continue;
            }
            bool leafr = w.v->is_leaf();
            if (leafr){
                for (int i = last[w.v->num]; i != -1; i = prevpos[i]){
                    nb = Nodeptr(w.qpos, -1, i+1, w.u, nullptr, w.cost);
                    q.push(nb);
                }
                continue;
            }
            /*if (w.qpos == 0 && w.cost == 0){
                ///moving the start position
                for (int i = 0 ; i < 4; ++i){
                    if (w.u->child[i] != nullptr){
                        for (int j = 0; j < 4; ++j)
                            if (w.v->child[j] != nullptr){
                                nb = Nodeptr(w.qpos, -1, -1, w.u->child[i], w.v->child[j], w.cost);
                                q.push(nb);
                            }
                        nb = Nodeptr(w.qpos, -1, -1, w.u->child[i], w.v, w.cost);
                        q.push(nb);
                    }
                    if (w.v->child[i] != nullptr){
                        nb = Nodeptr(w.qpos, -1, -1, w.u, w.v->child[i], w.cost);
                        q.push(nb);
                    }
                }
            }*/
            int numl = w.u->num_of_leaf(left[w.qpos]);
            int numr = w.v->num_of_leaf(right[w.qpos]);
            if (numl != -1 && numr != -1){
                nb = Nodeptr(w.qpos+1, -1, -1, w.u->child[numl], w.v->child[numr], w.cost);
                q.push(nb);
                continue;
            }
            
            nb = Nodeptr(w.qpos+1, -1, -1, w.u, w.v, w.cost+2);
            q.push(nb);

            for (int i=0 ;i < 4; ++i){
                if (w.u->child[i] != nullptr){
                    for (int j=0; j < 4; ++j)
                        if (w.v->child[j] != nullptr){
                            nb = Nodeptr(w.qpos+1, -1, -1, w.u->child[i], w.v->child[j], w.cost);
                            if (left[w.qpos] != base[i])
                                ++nb.cost;
                            if (right[w.qpos] != base[j])
                                ++nb.cost;
                            q.push(nb);

                            nb = Nodeptr(w.qpos, -1, -1, w.u->child[i], w.v->child[j], w.cost+2);
                            q.push(nb);
                        }
                    
                    nb = Nodeptr(w.qpos, -1, -1, w.u->child[i], w.v, w.cost+1);
                    q.push(nb);

                    nb = Nodeptr(w.qpos+1, -1, -1, w.u->child[i], w.v, w.cost+1);
                    if (left[w.qpos] != base[i])
                        ++nb.cost;
                    q.push(nb);
                }
                if(w.v->child[i] != nullptr){
                    nb = Nodeptr(w.qpos, -1, -1, w.u, w.v->child[i], w.cost+1);
                    q.push(nb);

                    nb = Nodeptr(w.qpos+1, -1, -1, w.u, w.v->child[i], w.cost+1);
                    if (right[w.qpos] != base[i])
                        ++nb.cost;
                    q.push(nb);
                }
            }
        }
    }
}
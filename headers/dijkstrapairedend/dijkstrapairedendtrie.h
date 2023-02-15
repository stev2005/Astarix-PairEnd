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

    Nodeptr(int _qpos, int _lpos, int _rpos, int _cost){
        qpos = _qpos;
        lpos = _lpos;
        rpos = _rpos;
        cost = _cost;
        u = nullptr;
        v = nullptr;
    }

    Nodeptr(int _qpos, int _lpos, Trie *_v, int _cost){
        qpos = _qpos;
        lpos = _lpos;
        rpos = -1;
        cost = _cost;
        u = nullptr;
        v = _v;
    }

    Nodeptr(int _qpos, Trie *_u, int _rpos, int _cost){
        qpos = _qpos;
        lpos = -1;
        rpos = _rpos;
        cost = _cost;
        u = _u;
        v = nullptr;
    }

    Nodeptr(int _qpos, Trie *_u, Trie *_v, int _cost){
        qpos = _qpos;
        lpos = -1;
        rpos = -1;
        cost = _cost;
        u = _u;
        v = _v;
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

struct Expanded{
    int qpos, lpos, rpos;
    Trie *u, *v;

    Expanded(){}

    Expanded(int _qpos, int _lpos, int _rpos, Trie *_u, Trie *_v){
        qpos = _qpos;
        lpos = _lpos;
        rpos = _rpos;
        u = _u;
        v = _v;
    }

    Expanded(int _qpos, int _lpos, int _rpos){
        qpos = _qpos;
        lpos = _lpos;
        rpos = _rpos;
        u = nullptr;
        v = nullptr;
    }

    Expanded(int _qpos, int _lpos, Trie *_v){
        qpos = _qpos;
        lpos = _lpos;
        rpos = -1;
        u = nullptr;
        v = _v;
    }

    Expanded(int _qpos, Trie *_u, int _rpos){
        qpos = _qpos;
        lpos = -1;
        rpos = _rpos;
        u = _u;
        v = nullptr;
    }

    Expanded(int _qpos, Trie *_u, Trie *_v){
        qpos = _qpos;
        lpos = -1;
        rpos = -1;
        u = _u;
        v = _v;
    }

    bool operator<(const Expanded &state)const{
        if (qpos == state.qpos){
            if (u == state.u){
                if (v == state.v){
                    if (lpos == state.lpos)
                        return rpos < state.rpos;
                    return lpos < state.lpos;
                }
                return v < state.v;
            }
            return u < state.u;
        }
        return qpos < state.qpos;
    }

};

int edit_distance_pairedend_trie(pair <string, string> &query, string &ref, Trie *T, vector<int>&last, vector<int>&prevpos){
    string left = query.first;
    string right = query.second;
    int n = left.size();
    int m = ref.size();
    priority_queue <Nodeptr> q;
    Nodeptr w, nb;
    w = Nodeptr(0, -1, -1, T, T, 0);
    q.push(w);
    set <Expanded> visited;
    Expanded cur_state;
    while (!q.empty()){
        assert(cout<<"\n In the while of the problematic function\n");
        w = q.top();
        q.pop();
        w.print();
        if (w.qpos == n)
            break;
        cur_state = Expanded (w.qpos, w.lpos, w.rpos, w.u, w.v);
        if (visited.find(cur_state) == visited.end()){
            visited.insert(cur_state);
            assert(cout<<"First occurance of this state\n");
        }
        else{
            assert(cout<<"Previously found this state\n");
            continue;
        } 
        if (w.u==nullptr && w.v==nullptr){
            assert(cout<<"Case where using w.u==NULL and w.v==NULL\n");
            if (w.lpos<m && w.rpos<m){
                nb = Nodeptr(w.qpos+1, w.lpos+1, w.rpos+1, w.cost);
                if (left[w.qpos] != ref[w.lpos])
                    ++nb.cost;
                if (right[w.qpos] != ref[w.rpos])
                    ++nb.cost;
                q.push(nb);
                if (w.cost == nb.cost)
                    continue;
                
                nb = Nodeptr(w.qpos, w.lpos+1, w.rpos+1, w.cost+2);
                q.push(nb);
            }

            nb = Nodeptr(w.qpos+1, w.lpos, w.rpos, w.cost+2);
            q.push(nb);

            if (w.rpos<m){
                nb = Nodeptr(w.qpos, w.lpos, w.rpos+1, w.cost+1);
                q.push(nb);
                nb = Nodeptr(w.qpos+1, w.lpos, w.rpos+1, w.cost+1);
                if (right[w.qpos] != ref[w.rpos])
                    ++nb.cost;
                q.push(nb);
            }
            
            if (w.lpos<m){
                nb = Nodeptr(w.qpos, w.lpos+1, w.rpos, w.cost+1);
                q.push(nb);
                nb = Nodeptr(w.qpos+1, w.lpos+1, w.rpos, w.cost+1);
                if (left[w.qpos] != ref[w.rpos])
                    ++nb.cost;
                q.push(nb);
            }
        }

        if (w.u==nullptr && w.rpos==-1){
            assert(cout<<"Case where w.u==NULL and w.v points sth\n");
            bool leaf = w.v->is_leaf();
            if (leaf){
                for (int i = last[w.v->num]; i != -1; i = prevpos[i]){
                    nb = Nodeptr(w.qpos, w.lpos, i+1, w.cost);
                    q.push(nb);
                }
                continue;
            }
            nb = Nodeptr(w.qpos+1, w.lpos, w.v, w.cost+2);
            q.push(nb);
            for (int i = 0; i < 4; ++i){
                if (w.v->child[i] ==  nullptr)
                    continue;
                //leaf = false;
                if (w.lpos<m){
                    nb = Nodeptr(w.qpos+1, w.lpos+1, w.v->child[i], w.cost);
                    if (left[w.qpos] != ref[w.lpos])
                        ++nb.cost;
                    if (right[w.qpos] != base[i])
                        ++nb.cost;
                    q.push(nb);
                    if (nb.cost ==  w.cost)
                        continue;

                    nb = Nodeptr(w.qpos, w.lpos+1, w.v->child[i], w.cost+2);
                    q.push(nb);
                    
                    nb = Nodeptr(w.qpos, w.lpos+1, w.v, w.cost+1);
                    q.push(nb);
                    
                    nb = Nodeptr(w.qpos+1, w.lpos+1, w.v, w.cost+1);
                    if (left[w.qpos] != ref[w.lpos])
                        ++nb.cost;
                    q.push(nb);
                }
                nb = Nodeptr(w.qpos, w.lpos, w.v->child[i], w.cost+1);
                q.push(nb);

                nb = Nodeptr(w.qpos+1, w.lpos, w.v->child[i], w.cost+1);
                if (right[w.qpos] != base[i])
                    ++nb.cost;
                q.push(nb);
            }
        }

        if (w.lpos==-1 && w.v==nullptr){
            assert(cout<<"Case where w.u points sth and w.v==NULL\n");
            bool leaf = w.u->is_leaf();
            if(leaf){
                for(int i = last[w.u->num]; i != -1; i = prevpos[i]){
                    nb = Nodeptr(w.qpos, i+1, w.rpos, w.cost);
                    q.push(nb);
                }
            }
            nb = Nodeptr(w.qpos+1, w.u, w.rpos, w.cost+2);
            q.push(nb);
            for (int i = 0; i < 4; ++i){
                if (w.u->child[i] == nullptr)
                    continue;
                //leaf = false;
                if(w.rpos<m){
                    nb = Nodeptr(w.qpos+1, w.u->child[i], w.rpos+1, w.cost);
                    if (left[w.qpos] != base[i])
                        ++nb.cost;
                    if (right[w.qpos] != ref[w.rpos])
                        ++nb.cost;
                    q.push(nb);
                    if (nb.cost == w.cost)
                        continue;
                    
                    nb = Nodeptr(w.qpos, w.u->child[i], w.rpos+1, w.cost+2);
                    q.push(nb);

                    nb = Nodeptr(w.qpos, w.u, w.rpos+1, w.cost+1);
                    q.push(nb);

                    nb = Nodeptr(w.qpos+1, w.u, w.rpos+1, w.cost+1);
                    if (right[w.qpos] != ref[w.rpos])
                        ++nb.cost;
                    q.push(nb);
                }
                nb = Nodeptr(w.qpos, w.u->child[i], w.rpos, w.cost+1);
                q.push(nb);

                nb = Nodeptr(w.qpos+1, w.u->child[i], w.rpos, w.cost+1);
                if (left[w.qpos] != base[i])
                    ++nb.cost;
                q.push(nb);
            }
        }

        if (w.lpos==-1 && w.rpos==-1){
            assert(cout<<"Case where w.u!=NULL and w.v!=NULL\n");
            if (w.qpos == 0 && w.cost == 0){
                /*moving the start the position
                Proof of correctness: only to ways to have cost==0 are
                            1. w.qpos is increasing with 1 at each step
                               with the other parameters
                            2. An alingment from these positions (pointed
                               by the parameters) has not started yet
                */
                for (int i = 0; i < 4; ++i)
                    for (int j = 0 ; j < 4; ++j){
                        if (w.u->child[i] != nullptr){
                            nb = Nodeptr(w.qpos, w.u->child[i], w.v, w.cost);
                            q.push(nb);
                        }
                        if (w.v->child[j] != nullptr){
                            nb = Nodeptr(w.qpos, w.u, w.v->child[j], w.cost);
                            q.push(nb);
                        }
                        if (w.u->child[i] != nullptr && w.v->child[j] == nullptr){
                            nb = Nodeptr(w.qpos, w.u->child[i], w.v->child[j], w.cost);
                            q.push(nb);
                        }
                    }
            }

            bool leafl = w.u->is_leaf();
            assert(cout<<"leafl=="<<leafl<<"\n");
            if (leafl){
                for (int i = last[w.u->num]; i != -1; i = prevpos[i]){
                    nb = Nodeptr(w.qpos, i+1, w.v, w.cost);
                    q.push(nb);
                }
                continue;
            }
            bool leafr = w.v->is_leaf();
            assert(cout<<"leafr=="<<leafr<<"\n");
            if (leafr){
                for (int j = last[w.v->num]; j != -1; j = prevpos[j]){
                    nb = Nodeptr(w.qpos, w.u, j+1, w.cost);
                    q.push(nb);
                }
                continue;
            }
            nb = Nodeptr(w.qpos+1, w.u, w.v, w.cost);
            q.push(nb);
            assert(cout<<"Zeroth pushed: ");
            nb.print();
            for (int i = 0; i < 4; ++i){
                if (w.u->child[i] == nullptr)
                    continue;
                //leafl = false;
                for (int j = 0; j < 4; ++j){
                    if (w.v->child[j] == nullptr)
                        continue;
                    //leafr = false;
                    nb = Nodeptr(w.qpos+1, w.u->child[i], w.v->child[j], w.cost);
                    if (left[w.qpos] != base[i])
                        ++nb.cost;
                    if (right[w.qpos] != base[j])
                        ++nb.cost;
                    assert(cout<<"First pushed: ");
                    nb.print();
                    q.push(nb);
                    if (nb.cost == w.cost)
                        continue;
                    
                    nb = Nodeptr(w.qpos, w.u->child[i], w.v->child[j], w.cost+2);
                    assert(cout<<"Second pushed: ");
                    nb.print();
                    q.push(nb);

                    nb = Nodeptr(w.qpos, w.u, w.v->child[j], w.cost+1);
                    q.push(nb);
                    assert(cout<<"Third pushed: ");
                    nb.print();

                    nb = Nodeptr(w.qpos+1, w.u, w.v->child[j], w.cost+1);
                    if (right[w.qpos] != base[j])
                        ++nb.cost;
                    q.push(nb);
                    assert(cout<<"Fourth pushed: ");
                    nb.print();
                }
                nb = Nodeptr(w.qpos, w.u->child[i], w.v, w.cost+1);
                q.push(nb);
                assert(cout<<"Fifth pushed: ");
                nb.print();
                nb = Nodeptr(w.qpos+1, w.u->child[i], w.v, w.cost+1);
                if (left[w.qpos] != base[i])
                    ++nb.cost;
                q.push(nb);
                assert(cout<<"Sixth pushed: ");
                nb.print();
            }
        }
    }
    return w.cost;
}
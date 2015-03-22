//
//  Learning.h
//  Crawler
//
//  Created by 長谷川 知里 on 2012/08/28.
//
//

#ifndef Crawler_Learning_h
#define Crawler_Learning_h

#include "resource.h"
#include "Node.h"

class _int_ptr {
public:
    _int_ptr(int* adrs){ address = adrs; };
    int *address;
    bool operator>(_int_ptr n) const { return (*address) > *(n.address); };
    bool operator<(_int_ptr n) const { return (*address) < *(n.address); };
};

class Learning {
private:
    vector<Node> nodes;
    vector<int> vec;
    priority_queue<_int_ptr> pque;

public:
    Learning(int n);
    void showVector();
    vector<int>* getVector(){return &vec;};
};

#endif

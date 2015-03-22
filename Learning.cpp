//
//  Learning.cpp
//  Crawler
//
//  Created by 長谷川 知里 on 2012/08/28.
//
//

#include "Learning.h"

Learning::Learning(int n){
    vector<_int_ptr> array;
    int v[10];
    for( int i=0; i<n; i++ ){
        v[i] = i;
        _int_ptr value(&v[i]);
        pque.push(value);
    }

    v[5] = -10;
    
    for( int i=0; i<n; i++ ){
        cout << *(pque.top().address) << endl;
        pque.pop();
    }
}

void Learning::showVector() {
    Node* np = &nodes[nodes.size()/2];
    for( vector<Node>::iterator itr = nodes.begin(); itr!=nodes.end(); itr++ ){
        cout << itr-nodes.begin() << " ";
    }
    cout << "The value pointed by np is " << (*np).getNodeID() << endl;
    vector<Node>::iterator itr = nodes.begin();
    nodes.erase(itr);
    np--;
    
    for( vector<Node>::iterator itr = nodes.begin(); itr!=nodes.end(); itr++ ){
        cout << itr-nodes.begin() << " ";
    }
    cout << "The value pointed by np is " << (*np).getNodeID() << endl;
}
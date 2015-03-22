//
//  RandomizedGreedy.h
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/15.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef Crawler_RandomizedGreedy_h
#define Crawler_RandomizedGreedy_h

#include "Crawler.h"

class HighDegreeAdaptive : public Crawler {
    
private:
    queue<int> que;
    priority_queue<_ptr_node> pque;
    vector<Node> array;
    int nidx;
    
    int getNodeWithMaxDegree(vector<Node> *nvct);
    int findUncrawledNode(vector<Node> nds);
    void decreaseDegree(vector<Node> *nvct, int nodeID);
    void decreaseDegree(int nodeID);
    
public:
    HighDegreeAdaptive(const Network net, const int seed);
    void Crawl(vector<Node> nds, const int nv, const int ne, const int s);

};

#endif

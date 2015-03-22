//
//  Greedy.h
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/12.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef Crawler_Greedy_h
#define Crawler_Greedy_h

#include "Crawler.h"

class HypotheticalGreedy : public Crawler {

private:
    priority_queue<Node> pque;
    int FLCC; // fraction of largest connected cluster
    int findUncrawledNode(vector<Node> nds);
    int nidx;
    int nitr;

public:
    HypotheticalGreedy(){;};
    HypotheticalGreedy(const Network net, const int seed);
    void Crawl(vector<Node> nds, const int nv, const int ne, const int s);
    int getFLCC(){ return FLCC; };
};

#endif

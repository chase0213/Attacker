//
//  DFS.h
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/07.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef Crawler_DFS_h
#define Crawler_DFS_h

#include "Crawler.h"

class DFS : public Crawler {
    
private:
    stack<int> stk;
    int findUncrawledNode(vector<Node> nds);
    int nidx;
    
public:
    DFS(const Network net, const int seed);
    void Crawl(vector<Node> nds, const int nv, const int ne, const int s);
};

#endif

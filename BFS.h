//
//  BFS.h
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/07.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef Crawler_BFS_h
#define Crawler_BFS_h

#include "Crawler.h"

class BFS : public Crawler {

private:
    queue<int> que;
    int FLCC; // fraction of largest connected cluster
    int findUncrawledNode(vector<Node> nds);
    int nidx;
    
public:
    BFS(){;};
    BFS(const Network net, const int seed);
    void Crawl(vector<Node> nds, const int nv, const int ne, const int s);
    int getFLCC(){ return FLCC; };
    
};

#endif

//
//  BiassedRandomWalk.h
//  Attacker
//
//  Created by 長谷川 知里 on 2012/10/23.
//  Copyright (c) 2012年 C. HASEGAWA. All rights reserved.
//

#ifndef __Attacker__BiassedRandomWalk__
#define __Attacker__BiassedRandomWalk__

#include <iostream>
#include "Crawler.h"

class BiassedRandomWalk : public Crawler {

private:
    int FLCC; // fraction of largest connected cluster
    int findUncrawledNode(vector<Node> nds);
    int ThrowDiceForNextNode(int current, double d, double b);
    int nidx;
    int nitr;
    int alreadyNotCrawled();
//    int findUncrawledNode(vector<Node>);
    queue<Node> adjacent;
    priority_queue<_ptr_node> pque;
    vector<Node> array;
    vector<Node> nodes;
    int getNodeWithMaxDegree(vector<Node> *nvct);
    void decreaseDegree(vector<Node> *nds, int nodeID);
    void decreaseDegree(int nodeID);
public:
    BiassedRandomWalk(){;};
    BiassedRandomWalk(const Network net, const int seed, double a, double b, double epsilon);
    void Crawl(const Network net, const int s, const double a, const double b, const double epsilon);
};

#endif /* defined(__Attacker__BiassedRandomWalk__) */

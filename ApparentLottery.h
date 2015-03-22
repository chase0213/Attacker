//
//  ApparentLottery.h
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/30.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef Crawler_ApparentLottery_h
#define Crawler_ApparentLottery_h

#include "Crawler.h"

class ApparentLottery : public Crawler {
    
private:
    int CSDQ; // currently sum of degrees in queue
    queue<int> que;
    vector<NodeWithDegree> nvct;
    
    int getNodeWithMaxDegree(vector<NodeWithDegree> *nvct);
    void decreaseDegree(vector<NodeWithDegree> *nvct, int nodeID);
    
public:
    ApparentLottery(const Network net, const int seed);
    
};

#endif

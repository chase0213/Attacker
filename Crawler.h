//
//  Crawler.h
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/07.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef Crawler_Crawler_h
#define Crawler_Crawler_h

#include "stdafx.h"
#include "Network.h"
#include "resource.h"

class Crawler {
    
    friend class BFS;
    friend class DFS;
    friend class HypotheticalGreedy;
    friend class HighDegreeAdaptive;
    friend class ApparentLottery;
    friend class BiassedRandomWalk;

private:
    
    int seed;
    
    int V;
    int E;
    
    vector<int> Vseen; // Vseenカウント用
    vector<int> Vcrawled;
    vector<int> Eseen; // Eseenカウント用
    vector<int> memoryInUse;
    
    const int countVSeen(const bool* const isSeen);
    const int countVSeen(vector<Node>);
    const int countCurrentESeen(vector<Node> nds, const vector<Link> links);
    const int countCurrentESeen(const bool* const isCrawled, const vector<Link> links);

public:
    Crawler(){};
    void initCrawling(vector<Node>);
    const int IDtoIndex(vector<Node> nds,const int nid);
    void showCrawling(string strAlg);
    void showCrawling(string strAlg, bool isVisualized);
    void showMemoryInUse(string strAlg);
    int getE(){ return E; };
};

#endif

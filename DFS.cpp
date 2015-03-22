//
//  DFS.cpp
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/07.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//


#include "DFS.h"
#include "resource.h"

DFS::DFS( const Network net, const int seed ){
    Crawl(net.getNodes(), net.getV(), net.getE(), seed);
}

void DFS::Crawl(vector<Node> nds, const int nv, const int ne, const int s) {
    
    // initialization
    seed = s;
    V = nv;
    E = ne;
    nidx = 0;
    
    vector<Node> nodes = nds;
    vector<Link> links = nodes[seed].getLinks();
    
    // crawledフラグとseenフラグの初期化
    initCrawling(nodes);
    
    // 最初のcrawling
    Vcrawled.push_back(seed);
    nodes.at(seed).isCrawled = true;
    nodes.at(seed).isSeen = true;
    
    for( int i=0; i<links.size(); i++ ){
        int v = links[i].getDestination();
        stk.push(v);
        nodes.at(v).isSeen = true;
    }
    
    memoryInUse.push_back((int)(stk.size()));
    const int nvs = countVSeen(nodes);
    Vseen.push_back(nvs);
//    const int nes = countCurrentESeen(nodes, links);
    Eseen.push_back((int)links.size());
    
    // iterative crawling (searching)
    while( Vcrawled.size()<V ){
        
        // pop front node
        // queue が空だったら、未訪問のノードを追加
        if( stk.empty() ){
            int newSeedIndex = findUncrawledNode(nodes);
            
            // 見つからなかったらどこかがおかしい。
            if( newSeedIndex==-1 ){
                cerr << "new seed is not found" << endl;
                break;
            }
            
            nodes.at(newSeedIndex).isCrawled = true;
            nodes.at(newSeedIndex).isSeen = true;
            int dstIndex = newSeedIndex;
            stk.push(nodes.at(dstIndex).getNodeID());
            
        }
        
        int u = stk.top();
        stk.pop();
        
        // uにcrawlする
        Vcrawled.push_back(u);
        nodes.at(u).isCrawled = true;
        
        int nvs = 0;
        int nes = 0;
        
        links = nodes[u].getLinks();
        for( int i=0; i<links.size(); i++ ){
            int v = links[i].getDestination();
            
            if( !nodes.at(v).isSeen ){
                nodes.at(v).isSeen = true;
                stk.push(v);
                nvs++;
            }
            
            if( !nodes.at(v).isCrawled ){
                nes++;
            }
        }
        
        memoryInUse.push_back((int)(stk.size()));
        Vseen.push_back(Vseen.at(Vseen.size()-1)+nvs);
        Eseen.push_back(Eseen.at(Eseen.size()-1)+nes);
        
//        const int nvs = countVSeen(nodes);
//        Vseen.push_back(nvs);
//        const int nes = countCurrentESeen(nodes, links);
//        Eseen.push_back(nes+Eseen[Eseen.size()-1]);

    }

}

// 未訪問のノードが見つかればそのidを、見つからなければ-1を返す（最大V-1回）
int DFS::findUncrawledNode(vector<Node> nds){
    for( ; nidx<V; nidx++ ){
        if( !nds[nidx].isCrawled )
            return nidx;
    }
    
    return -1;
}
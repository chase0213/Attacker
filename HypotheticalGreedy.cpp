//
//  HypotheticalGreedy.cpp
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/11.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "HypotheticalGreedy.h"
#include "resource.h"

HypotheticalGreedy::HypotheticalGreedy(const Network net, const int s) {
    Crawl(net.getNodes(), net.getV(), net.getE(), s);
}

// 未訪問のノードが見つかればそのidを、見つからなければ-1を返す（最大V-1回）
int HypotheticalGreedy::findUncrawledNode(vector<Node> nds){
    for( ; nidx<V; nidx++ ){
        if( !nds[nidx].isCrawled )
            return nidx;
    }
    
    return -1;
}

void HypotheticalGreedy::Crawl(vector<Node> nds, const int nv, const int ne, const int s) {

    // initialization
    seed = s;
    V = nv;
    E = ne;
    
    nidx = 0;
    
    // グラフが存在しなければ終了
    if ( V<=0 ) return;
    

    vector<Node> nodes = nds;
    vector<Link> links = nodes.at(seed).getLinks();

    initCrawling(nodes);
    
    Vcrawled.push_back(seed);
    nodes.at(seed).isCrawled = true;
    nodes.at(seed).isSeen = true;
    
    for( int i=0; i<links.size(); i++ ){
        int dst = links[i].getDestination();
        pque.push(nodes.at(dst));
        nodes.at(dst).isSeen = true;
    }
    
    memoryInUse.push_back((int)(pque.size()));
    const int ns = countVSeen(nodes);
    Vseen.push_back(ns);
//    const int nes = countCurrentESeen(nodes, links);
    Eseen.push_back((int)links.size());
    
    
    FLCC = 0;
    int tempFLCC = 0;
    
    // iterative crawling (searching)
    while( Vcrawled.size()<V ){
        
        // pop front node
        // priority queue が空だったら、未訪問のノードを追加
        if( pque.empty() ){
            int newSeedIndex = findUncrawledNode(nodes);
            
            // 見つからなかったらどこかがおかしい。
            if( newSeedIndex==-1 ){
                cerr << "new seed is not found" << endl;
                break;
            }
            
            nodes.at(newSeedIndex).isCrawled = true;
            nodes.at(newSeedIndex).isSeen = true;
            int dstIndex = newSeedIndex;
            pque.push(nodes.at(dstIndex));
            
            // 最大のs(Q)を保持
            if( FLCC < tempFLCC ) FLCC = tempFLCC;
            tempFLCC = 0;
            
            if( FLCC >= V/2 ) break;
            
        }
        // priority queue から次数最大のノードを取り出す
        int u = (pque.top()).getNodeID();
        
        pque.pop();
        tempFLCC++;
        
        Vcrawled.push_back(u);
        nodes.at(u).isCrawled = true;
        
        int nvs = 0;
        int nes = 0;
        
        links = nodes.at(u).getLinks();
        for( int i=0; i<links.size(); i++ ){
            int v = links.at(i).getDestination();
            
            if( !nodes.at(v).isSeen ){
                nodes.at(v).isSeen = true;
                pque.push(nodes.at(v));
                nvs++;
            }
            
            if( !nodes.at(v).isCrawled ){
                nes++;
            }
            
            nodes.at(v).decreaseDegree();
        }
        
        memoryInUse.push_back((int)(pque.size()));
        Vseen.push_back(Vseen.at(Vseen.size()-1)+nvs);
        Eseen.push_back(Eseen.at(Eseen.size()-1)+nes);
        
        
    }
    
    // 最終的に最大のs(Q)を保持
    if( FLCC < tempFLCC ) FLCC = tempFLCC;
        
    while( !pque.empty() ) pque.pop();
    
}

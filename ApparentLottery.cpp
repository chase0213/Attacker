//
//  ApparentLottery.cpp
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/29.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "ApparentLottery.h"
#include "resource.h"

ApparentLottery::ApparentLottery(const Network net, const int s) {    
    
    // initialization
    seed = s;
    V = net.getV();
    E = net.getE();
    CSDQ = 0;
    
    bool* isCrawled = new bool[V];
    bool* isSeen = new bool[V];
    for( int i=0; i<V; i++ ){
        isCrawled[i] = false;
        isSeen[i] = false;
    }
    
    Vcrawled.push_back(seed);
    isCrawled[seed] = true;
    isSeen[seed] = true;
    
    vector<Node> nodes = net.getNodes();
    vector<Link> links = nodes[seed].getLinks();
    for( int i=0; i<links.size(); i++ ){
        int dst = links[i].getDestination()+offset;
        NodeWithDegree nwd(dst,nodes[dst].getDegree());
        nwd.decreaseDegree();
        nvct.push_back(nwd);
        CSDQ += nwd.getDegree();
        //        sort(nvct.begin(),nvct.end(),greater<NodeWithDegree>());
        isSeen[dst] = true;
    }
    const int ns = countVSeen(isSeen);
    Vseen.push_back(ns);
    const int nes = countCurrentESeen(isCrawled, links);
    Eseen.push_back(nes);
    
    // listを一回参照しました
    //    Rgrd += Rl;
    
    // iterative crawling (searching)
    while( Vcrawled.size()<V ){
        
        if( nvct.empty() ) break;
        int u = getNodeWithMaxDegree(&nvct);
        
        Vcrawled.push_back(u);
        isCrawled[u] = true;
        
        links = nodes[u].getLinks();
        for( int i=0; i<links.size(); i++ ){
            int v = links[i].getDestination()+offset;
            if( !isSeen[v] ){
                isSeen[v] = true;
                NodeWithDegree nwd(v,nodes[v].getDegree());
                nvct.push_back(nwd);
                CSDQ += nwd.getDegree();
            }
            decreaseDegree(&nvct,nodes[v].getNodeID()+offset);
            //            sort(nvct.begin(),nvct.end(),greater<NodeWithDegree>());
        }
        const int nvs = countVSeen(isSeen);
        Vseen.push_back(nvs);
        const int nes = countCurrentESeen(isCrawled, links);
        Eseen.push_back(nes+Eseen[Eseen.size()-1]);
        
        
        // listを一回ずつ参照してます
        //        Rgrd += Rl;
    }
    
    delete [] isCrawled;
    delete [] isSeen;
}

int ApparentLottery::getNodeWithMaxDegree(vector<NodeWithDegree> *nvct){
    
    // 次数の高い順にソート
    sort((*nvct).begin(),(*nvct).end(),greater<NodeWithDegree>());
    
    int dice = CSDQ > 0 ? rand()%CSDQ : 0; // サイコロを振る
    int dsum = 0;
    vector<NodeWithDegree>::iterator itr;
    for( itr = (*nvct).begin(); itr!=(*nvct).end(); itr++ ){
        dsum += (*itr).getDegree();
        if( dice < dsum ){
            break;
        }
    }
    
    // Queue内の次数和を再計算
    CSDQ -= (*itr).getDegree();
    
    // 最大次数を持つ要素のidを保存
    int id = (*itr).getNodeID();
    
    // 取り出した最大次数を持つ要素を削除
    (*nvct).erase(itr);
    
    return id;
}

void ApparentLottery::decreaseDegree(vector<NodeWithDegree> *nvct, int nodeID){
    vector<NodeWithDegree>::iterator itr = (*nvct).begin();
    while( itr!=(*nvct).end() ){
        if( (*itr).getNodeID()==nodeID ){
            (*itr).decreaseDegree();
            CSDQ--;
            return;
        }
        ++itr;
    }
}

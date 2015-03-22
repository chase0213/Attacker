//
//  RandomizedGreedy.cpp
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/11.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "HighDegreeAdaptive.h"
#include "resource.h"


HighDegreeAdaptive::HighDegreeAdaptive(const Network net, const int s) {
    Crawl(net.getNodes(), net.getV(), net.getE(), s);
}

// 未訪問のノードが見つかればそのidを、見つからなければ-1を返す（最大V-1回）
int HighDegreeAdaptive::findUncrawledNode(vector<Node> nds){
    for( ; nidx<V; nidx++ ){
        if( !nds[nidx].isCrawled )
            return nidx;
    }
    
    return -1;
}

void HighDegreeAdaptive::Crawl(vector<Node> nds, const int nv, const int ne, const int s) {

    // initialization
    seed = s;
    V = nv;
    E = ne;
    
    nidx = 0;

    vector<Node> nodes = nds;
    vector<Link> links = nodes.at(seed).getLinks();
    
    initCrawling(nodes);

    Vcrawled.push_back(seed);
    nodes.at(seed).isCrawled = true;
    nodes.at(seed).isSeen = true;
    
    for( int i=0; i<links.size(); i++ ){
        int dst = links[i].getDestination();
        nodes.at(dst).decreaseDegree();
//        array.push_back(nodes.at(dst)); // priority_queueが機能してくれないので仕方なく追加
        _ptr_node pn(&nodes.at(dst));
        pque.push(pn);
        nodes.at(dst).isSeen = true;
    }
    
    memoryInUse.push_back((int)(array.size()));
    const int nvs = countVSeen(nodes);
    Vseen.push_back(nvs);
//    const int nes = countCurrentESeen(nodes, links);
    Eseen.push_back((int)links.size());
    
    // iterative crawling (searching)
    while( Vcrawled.size()<V ){
        
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
//            array.push_back(nodes.at(dstIndex));
            pque.push(&nodes.at(dstIndex));
        }
        
//        int u = getNodeWithMaxDegree(&array);
        
        int u = (*(pque.top().pnode)).getNodeID();
        pque.pop();
        
        Vcrawled.push_back(u);
        nodes.at(u).isCrawled = true;

        int nvs = 0;
        int nes = 0;
        
        links = nodes[u].getLinks();
        for( int i=0; i<links.size(); i++ ){
            int v = links[i].getDestination();
            
            if( !nodes.at(v).isSeen ){
                nodes.at(v).isSeen = true;
                _ptr_node pn(&nodes.at(v));
                pque.push(pn);
//                array.push_back(nodes.at(v)); // priority_queueが機能してくれないので仕方なく追加
                nvs++;
            }
            
            if( !nodes.at(v).isCrawled ){
                nes++;
            }
            
//            decreaseDegree(&array, v);
            
            nodes.at(v).decreaseDegree();
        }
        
        memoryInUse.push_back((int)(pque.size()));
        Vseen.push_back(Vseen.at(Vseen.size()-1)+nvs);
        Eseen.push_back(Eseen.at(Eseen.size()-1)+nes);
        
    }
}

int HighDegreeAdaptive::getNodeWithMaxDegree(vector<Node> *nvct){

    vector<Node>::iterator maxItr = max_element((*nvct).begin(), (*nvct).end());

    // 最大次数を持つ要素のindexを保存
    int maxIndex = (*maxItr).getNodeID();
    // 取り出した最大次数を持つ要素を削除
    (*nvct).erase(maxItr);
    
    return maxIndex;
}

void HighDegreeAdaptive::decreaseDegree(vector<Node> *nds, int nodeID){
    vector<Node>::iterator itr = (*nds).begin();
    while( itr!=(*nds).end() ){
        if( (*itr).getNodeID()==nodeID ){
            (*itr).decreaseDegree();
            return;
        }
        ++itr;
    }
}

void HighDegreeAdaptive::decreaseDegree(int nodeID) {
    priority_queue<_ptr_node> alias = pque;
    while( !alias.empty() ){
        if( (*(alias.top().pnode)).getNodeID() == nodeID ){
            (*(alias.top().pnode)).decreaseDegree();
            return;
        }
        alias.pop();
    }
}

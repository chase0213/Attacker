//
//  BiassedRandomWalk.cpp
//  Attacker
//
//  Created by 長谷川 知里 on 2012/10/23.
//  Copyright (c) 2012年 C. HASEGAWA. All rights reserved.
//

#include "BiassedRandomWalk.h"

BiassedRandomWalk::BiassedRandomWalk(Network net, const int seed, double a, double b, double epsilon) {
    Crawl(net, seed, a, b, epsilon);
}

void BiassedRandomWalk::Crawl(Network net, const int s, const double a, const double b, const double epsilon){
    
    double dSum = 0.0;
    
    // initialization
    seed = s;
    V = net.getV();
    E = net.getE();
    nodes = net.getNodes();
    nidx = 0;
    
//    const int dT = pow(nodes.size(),a);

    // crawledフラグとseenフラグの初期化
    initCrawling(nodes);
    
    vector<Link> links;
    vector<int> temp;
    
    int current = seed;

    
    // Biassed Random Wald to find high degree nodes
    int trial = 0;
    
//    const double x = pow(nodes.size(),1-2*a*b*(1-epsilon));
//    const double k = 3.0;
    int tLimit = pow(nodes.size(),1-(4.0/3.0)*a*(1-a));
//    const double kmin = net.getMinDegree();
    const double ta = pow((double)nodes.size(),a);
//    const double dLimit = kmin*kmin*nodes.size()/(ta*ta);
//    const int kt = net.getAverageDegree();
    
    srand((int)time(NULL));
    
    // Biased Random Walkの部分
    
    int nta = 0;
    for( int i=0; i<nodes.size(); i++ ){
        if( nodes.at(i).getDegree() >= ta ){
            nta++;
        }
    }
    
//    ofstream ofs;
//    ostringstream oss;
    int cTA = 0;
    
//    oss << "SE_BRW_BA_a" << a << ".txt";
//    ofs.open(oss.str().c_str());
    
    int notCahnged = 0;
//    while( notCahnged < 0.005*nodes.size() && trial < tLimit*0.05 ){ 暫定的にこれを使用
    
//    while( trial < nodes.size() ){ // 予備実験用
    if( epsilon >= 1.0 ){
        tLimit = nta = (int)(nodes.size());
    }
    
    
    // Random Walk Begins from Here
    
    while( trial < tLimit ){
        
        // Queueが空でなければ空になるまで要素を取り出す
        if( !adjacent.empty() ){
            for( int i=0; i<adjacent.size(); i++ )
                adjacent.pop();
        }

        // currentのノードを探索する
        int nvs = 0;
        int nes = 0;
        links = nodes[current].getLinks();
        for( vector<Link>::iterator itr=links.begin(); itr!=links.end(); itr++ ){
            int v = (*itr).getDestination();
            adjacent.push(nodes.at(v));
            
            if( !nodes.at(v).isCrawled && !nodes.at(current).isCrawled ){
                nes++;
                if( !nodes.at(v).isSeen ) {
                    nodes.at(v).isSeen = true;
//                    nodes.at(v).decreaseDegree();
                    temp.push_back(v);
                    nvs++;
                    
                    // 予備実験用：t^aを超える次数のノードをカウントする
                    if( nodes.at(v).getDegree() >= ta )
                        cTA++;
                }
            }
        }
        
        if( epsilon>=1.0 ){
            memoryInUse.push_back((int)(adjacent.size()));
        } else {
            memoryInUse.push_back((int)(temp.size()));
        }
        
        if( !nodes.at(current).isCrawled ){
            Vcrawled.push_back(current);
            nodes.at(current).isCrawled = true;
            nodes.at(current).isSeen = true;
        }

        if( trial!=0 ){
            Vseen.push_back(Vseen.at(Vseen.size()-1)+nvs);
            Eseen.push_back(Eseen.at(Eseen.size()-1)+nes);
        } else {
            Vseen.push_back(nvs);
            Eseen.push_back(nes);
        }
        
        
        // 予備実験用
//        ofs << (double)cTA/(double)nta << endl;
        
        // currentの更新
        current = ThrowDiceForNextNode(current,dSum,b);
        if( current==-1 )
            current = rand()%nodes.size();
        
        trial++;
        
        
    }
    
    // 予備実験用
//    ofs.close();
//    return;
    
    
//    cout << "#nodes in Vcrawled after RW : " << Vcrawled.size() << endl;
    
    // HDA探索の部分
    
    for( int i=0; i<temp.size(); i++ ){
        if( !nodes.at(temp.at(i)).isCrawled )
            pque.push(&nodes.at(temp.at(i)));
//            array.push_back(nodes.at(temp.at(i))); // priority_queueが機能してくれないので仕方なく追加
    }
    
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
//            array.push_back(nodes.at(newSeedIndex));
            pque.push(&nodes.at(newSeedIndex));
            Vcrawled.push_back(newSeedIndex);
        }
        
//        int u = getNodeWithMaxDegree(&array);
        
        // priority queue から次数最大のノードを取り出す
        int u = (*(pque.top().pnode)).getNodeID();
        pque.pop();
        if( nodes.at(u).isCrawled )
            continue;

        Vcrawled.push_back(u);
        nodes.at(u).isCrawled = true;
        
        int nvs = 0;
        int nes = 0;
        
        links = nodes.at(u).getLinks();
        for( int i=0; i<links.size(); i++ ){
            int v = links.at(i).getDestination();
            
            if( !nodes.at(v).isCrawled) {
                nes++;
                if( !nodes.at(v).isSeen ){
                    nodes.at(v).decreaseDegree();
                    nodes.at(v).isSeen = true;
//                    array.push_back(nodes.at(v));
                    pque.push(&nodes.at(v));
                    nvs++;
                }
            }
            
            nodes.at(v).decreaseDegree();
//            decreaseDegree(&array, v);
        }
        
        memoryInUse.push_back((int)(pque.size()));
        Vseen.push_back(Vseen.at(Vseen.size()-1)+nvs);
        Eseen.push_back(Eseen.at(Eseen.size()-1)+nes);
        
    }
    
//    cout << "# of nodes not crawled is " << alreadyNotCrawled() << endl;
    
}


// 次に進むべきノードのIDを返す。エラー発生時には-1を返す。
int BiassedRandomWalk::ThrowDiceForNextNode(int current, double d, double b) {
    
    queue<Node> que = adjacent;
    double dSum = 0.0;
    for( int i=0; i<que.size(); i++ ){
        int v = que.front().getNodeID();
        que.pop();
        dSum += pow(nodes.at(current).getDegree()*nodes.at(v).getDegree(),b);
    }
    // ジャンプを許可する場合
//    dSum += pow(nodes.at(current).getDegree()*nodes.at(current).getDegree(),b);
    
    double dice = rand()/(double)INT_MAX*dSum;
    double th = 0.0;
    for( int i=0; i<adjacent.size(); i++ ){
        int v = adjacent.front().getNodeID();
        adjacent.pop();
        th += pow(nodes.at(current).getDegree()*nodes.at(v).getDegree(),b);
        if( dice <= th )
            return v;
    }
    return -1;
}

int BiassedRandomWalk::alreadyNotCrawled() {
    int notCrawled = 0;
    for( int i=0; i<nodes.size(); i++ ){
        if( !nodes.at(i).isCrawled ){
            notCrawled++;
        }
    }
    return notCrawled;
}


// 未訪問のノードが見つかればそのidを、見つからなければ-1を返す（最大V-1回）
int BiassedRandomWalk::findUncrawledNode(vector<Node> nds){
    for( ; nidx<V; nidx++ ){
        if( !nds[nidx].isCrawled )
            return nidx;
    }
    
    return -1;
}

int BiassedRandomWalk::getNodeWithMaxDegree(vector<Node> *nvct){
    
    vector<Node>::iterator maxItr = max_element((*nvct).begin(), (*nvct).end());
    
    // 最大次数を持つ要素のindexを保存
    int maxIndex = (*maxItr).getNodeID();
    // 取り出した最大次数を持つ要素を削除
    (*nvct).erase(maxItr);
    
    return maxIndex;
}

void BiassedRandomWalk::decreaseDegree(vector<Node> *nds, int nodeID){
    vector<Node>::iterator itr = (*nds).begin();
    while( itr!=(*nds).end() ){
        if( (*itr).getNodeID()==nodeID ){
            (*itr).decreaseDegree();
            return;
        }
        ++itr;
    }
}

void BiassedRandomWalk::decreaseDegree(int nodeID) {
    priority_queue<_ptr_node> alias = pque;
    while( !alias.empty() ){
        if( (*(alias.top().pnode)).getNodeID() == nodeID ){
            (*(alias.top().pnode)).decreaseDegree();
            return;
        }
        alias.pop();
    }
}
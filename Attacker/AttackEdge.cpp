//
//  AttackEdge.cpp
//  Attacker
//
//  Created by 長谷川 知里 on 2012/09/10.
//  Copyright (c) 2012年 C. HASEGAWA. All rights reserved.
//

#include "AttackEdge.h"

void AttackEdge::RandomAttack() {
    
    // 乱数のシードを設定
    srand((unsigned int)time(NULL));
    
    vector<Node> nodes = (*np).getNodes();
    
    int *rnd = new int[(int)nodes.size()];
    for( int i=0; i<nodes.size(); i++ )
        rnd[i] = i;
    
    for( int i=(int)nodes.size()-1; i>0; i-- ){
        int w = rnd[i];
        int r = rand()%i;
        rnd[i] = rnd[r];
        rnd[r] = w;
    }
    
    for( int i=0; i<nodes.size(); i++ ){
        (*np).calculateNodeRobustness();
        // ランダムに削除
        (*np).removeNode(rnd[i]);
    }
    
    (*np).calculateNodeRobustness();
    
}

void AttackEdge::GreedyAttack() {
}

void AttackEdge::HighDegreeAdaptive() {
}

void AttackEdge::HighCentralityAdaptive() {
    
}
//
//  AttackNode.cpp
//  Crawler
//
//  Created by 長谷川 知里 on 2012/08/26.
//
//

#include "AttackNode.h"

void AttackNode::RandomAttack() {
    
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

void AttackNode::GreedyAttack() {
    vector<Node> nodes = (*np).getNodes();
    sort(nodes.begin(),nodes.end(),greater<Node>());
    for( vector<Node>::iterator itr=nodes.begin(); itr!=nodes.end(); itr++ ){
        (*np).calculateNodeRobustness();
        // 次数の高い順に削除
        (*np).removeNode((*itr).getNodeID());
    }
    
    (*np).calculateNodeRobustness();
}

void AttackNode::HighDegreeAdaptive() {
    
    vector<Node> nodes = (*np).getNodes();
    sort(nodes.begin(),nodes.end(),greater<Node>());
    
    for( int i=0; i<nodes.size(); i++ )
    {
        (*np).calculateNodeRobustness();
        nodes = (*np).getNodes();
       
        int max = (int)(max_element(nodes.begin(),nodes.end()) - nodes.begin());
        // デバッグ用
//        cout << "Attacked Node is " << max << endl;
        
        // 全てのノードが孤立していたら空ループを回す（Robustness計算のため）
        if ( nodes.at(max).getDegree()==0 ) continue;
        
        // 暫定的に次数の高い順に削除
        (*np).removeNode(max);
        
    }
    
    (*np).calculateNodeRobustness();
    
}

void AttackNode::HighCentralityAdaptive() {
    
}
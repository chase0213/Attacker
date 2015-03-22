//
//  AntiGreedyOptimization.cpp
//  Attacker
//
//  Created by 長谷川 知里 on 2012/11/05.
//  Copyright (c) 2012年 C. HASEGAWA. All rights reserved.
//

#include "Reconstruction.h"

void Reconstruction::antiHGAOptimization(const int sel) {
    
    // 次数の昇順にソート
    vector<Node> nodes = (*np).getNodes();
    priority_queue<Node> sorted;
    
    // 空のネットワークを作成して*npにコピー
    Network net;
    (*np) = net;
    for( int i=0; i<nodes.size(); i++ ){
        Node node(i,"");
        (*np).addNode(node);
    }
    
    for( vector<Node>::iterator itr=nodes.begin(); itr!=nodes.end(); itr++ ){
        sorted.push((*itr));
    }
    
    for( int i=0; i<nodes.size(); i++ ){
        Node top = sorted.top();
        int deg = top.getDegree();
        sorted.pop();
        vector<Node> temp;
        for( int j=0; j<deg; j++ ){
            temp.push_back(sorted.top());
            sorted.pop();
        }
        for( int j=0; j<deg; j++ ){
            temp.at(j).decreaseDegree();
            sorted.push(temp.at(j));
        }
        
        int in1 = top.getNodeID();
        for( int j=0; j<deg; j++ ){
            int in2 = temp.at(j).getNodeID();
            Link l1(in1,in2);
            Link l2(in2,in1);
            (*np).addLinkToNode(l1, in1);
            (*np).addLinkToNode(l2, in2);
        }
    }

}

/*
void Reconstruction::antiHGAOptimization(const int sel) {
    
    // 次数の昇順にソート
    vector<Node> nodes = (*np).getNodes();
    vector<Node> sorted = (*np).getNodes();
    sort(sorted.begin(), sorted.end());
    
    // 次数分布を作成
    int dMax = (*np).getMaxDegree();
    int *dHistgram = new int[dMax+1];
    vector<DDISTRIBUTION> dd;
    for( int i=0; i<dMax+1; i++ ) dHistgram[i] = 0;
    for( int i=0; i<nodes.size(); i++ )
        dHistgram[nodes.at(i).getDegree()]++;
    for( int i=0; i<dMax+1; i++ ){
        if( dHistgram[i]>0 ){
            DDISTRIBUTION dnew(i,dHistgram[i]);
            dd.push_back(dnew);
        }
    }
    
    Network net;
    for( int i=0; i<nodes.size(); i++ ){
        Node node(i,"");
        net.addNode(node);
    }
    
    
    typedef vector<int> DS;
    DS ds(dMax+1);
    for( int i=0; i<ds.size(); i++ ) ds.at(i) = 0;
    vector<DS> dsarray;
    
    int r = 0;
    int appeared = -1; // #nodes appeared
    
    // 初期状態の設定
    while( r<dMax ){
        ds.at(r) = 0;
        r++;
    }
    
    while( appeared!=0 ) {
        appeared = addOne(&ds);
        if( appeared!=nodes.size() || !isValidCombination(sorted, ds) )
            continue;
        dsarray.push_back(ds);
    }
    
    ofstream ofs("candidate.txt");
    for( vector<DS>::iterator itr=dsarray.begin(); itr!=dsarray.end(); itr++ ){
        for( vector<int>::iterator iti=(*itr).begin(); iti!=(*itr).end(); iti++ ){
            ofs << (*iti) << " ";
        }
        ofs << endl;
    }
    ofs.close();
    
}

int Reconstruction::addOne(vector<int> *ds){
    // 1を足す計算
    int i = (int)((*ds).size()-1);
    do {
        (*ds).at(i) = ((*ds).at(i) + 1) % (*np).getV();
    } while( i>0 && (*ds).at(i--)==0 );
    
    // 次数和のカウント
    int sum = 0;
    for( int i=0; i<(*ds).size(); i++ ){
        sum += (*ds).at(i);
    }
    return sum;
}

const bool Reconstruction::isValidCombination(vector<Node> sorted, vector<int> cand) {
    int current = 0;
//    int pdeg = 0; // 前段の頂点次数
//    int npdeg = 0; // 前段の頂点数
    vector<int> pcand;
    vector<Node> candidate;
    
    // 実効次数に関するループ：全体で|V|回の繰り返し
    for( vector<int>::iterator itr=cand.begin(); itr!=cand.end(); itr++ ) {
        
        if( *itr==0 )
            continue;

        
        // 共通の実効次数を持つ頂点を候補とする
        for( int i=current; i<(*itr)+current; i++ ) {
            candidate.push_back(sorted.at(i));
        }
        
        current += (*itr);
        // 実効次数の計算
        int k = (int)(itr-cand.begin());
        
        if( current>=sorted.size() ) k = 0;
        
        // もしネットワークが構成できないのであれば、falseを返す
        if( !isValidCombination(candidate, k, pcand) )
            return false;

//        pdeg = k;
//        npdeg = (*itr); // 現在のレイヤーの頂点数を記憶
        
        pcand.erase(pcand.begin(), pcand.end());
        
        for( int i=0; i<(*itr); i++ ){
            pcand.push_back(k);
        }
        
        // 候補の初期化
        candidate.erase(candidate.begin(), candidate.end());
        
    }
    
    return true;
}

// check if the degree sequence is valid using Erdos-Gallai Theorem
// cand is a candidate of
const bool Reconstruction::isValidCombination(vector<Node> cand, int k, vector<int> pcand ) {
    
    vector<int> ds; // degree sequence
    
    for( vector<int>::iterator itr=pcand.begin(); itr!=pcand.end(); itr++ ){
        ds.push_back((*itr));
    }
    
    for( vector<Node>::iterator itr=cand.begin(); itr!=cand.end(); itr++ ){
        int deg = (*itr).getDegree()-k;
        if( deg<0 )
            return false;
        
        ds.push_back(deg); // 後段へ出る辺数を除いた次数でネットワークを形成する
    }
    sort(ds.begin(),ds.end(),greater<int>());
    
    // Erdos-Gallaiの定理を使ってネットワークの構成判定
    int n = (int)ds.size();
    int dsum;
    int minsum;
    for( int i=0; i<n; i++ ){
        dsum = minsum = 0;
        for( int j=0; j<i+1; j++ ) dsum += ds.at(j);
        for( int j=i+1; j<n; j++ ) minsum += (ds.at(j) >= i+1) ? i+1 : ds.at(j);
        if( dsum > i*(i+1) + minsum ) return false;
    }
    
    // 次数和が偶数でなければ作れない
    if( dsum%2!=0 )
        return false;
    
    return true;
}
 
 */

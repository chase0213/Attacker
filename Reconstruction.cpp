//
//  Reconstruction.cpp
//  Attacker
//
//  Created by 長谷川 知里 on 2012/09/06.
//  Copyright (c) 2012年 C. HASEGAWA. All rights reserved.
//

#include "Reconstruction.h"
#include "AttackNode.h"
#include "AttackEdge.h"

void Reconstruction::GreedyFlip(const int sel) {
    
    int r=0;
    // 更新回数カウント用のQueus
    queue<bool> rQueue;
    for( int i=0; i<ITR_LIMIT; i++ ){
        rQueue.push(false);
    }
    
    // 乱数のシードを設定
    srand((unsigned int)time(NULL));
    
    Network net = (*np); // 実際に攻撃するネットワーク
    
    vector<Node> nodes = net.getNodes();
    
    // とりあえず初期段階のRobustnessを計算
    AttackNode an(&net);
    an.HighDegreeAdaptive();
    double rn = net.getNodeRobustness();
    
    net = *np; // Attack前の状態に戻す
    
    long dc = 0; // debug用カウンタ
    
    // 終了条件 if there were improvements less than 1% in last 10000 iterations
    while ( r >= 0.01*ITR_LIMIT || dc<=ITR_LIMIT ){
        
        // ランダムに2本のエッジを選択して
        int in1, in2, ie1, ie2;
        in1 = rand()%(nodes.size());
        in2 = rand()%(nodes.size());
        ie1 = rand()%nodes.at(in1).getLinks().size();
        ie2 = rand()%nodes.at(in2).getLinks().size();
        // flipしてみる（失敗したらもう一度乱数生成）
        if ( !net.FlipLinks(in1, ie1, in2, ie2) ){
            continue;
        }
        
        // Attack Algorithmの選択
        AttackNode an(&net);
        an.HighDegreeAdaptive();
        double rnn = net.getNodeRobustness();
        
        // Flip後のRobustnessの方が大きければ
        if( rnn > rn && sel==1 ){
            if( !rQueue.front() ) r++;
            rQueue.pop();
            rQueue.push(true);
            rn = rnn; // Robustnessを更新
            (*np).FlipLinks(in1, ie1, in2, ie2); // Flip
            net = (*np); // コピー
        } else {
            if( rQueue.front() ) r--;
            rQueue.pop();
            rQueue.push(false);
            net = (*np);
        }
        
        dc++;
        if ( dc%1000 == 0 && dc != 0 ){
            cout << dc << " times iterated, the update ratio is " << r/(double)ITR_LIMIT << endl;
        }
        
    };
    cout << "total " << dc << " times iterated, the update ratio is " << r/(double)ITR_LIMIT << endl;
    
}

void Reconstruction::LocalGreedy(const int sel) {
    
    // 乱数のシードを設定
    srand((unsigned int)time(NULL));
    
    Network net = (*np); // 実際に攻撃するネットワーク
    
    vector<Node> nodes = net.getNodes();
    
    // とりあえず初期段階のRobustnessを計算
    AttackNode an(&net);
    an.HighDegreeAdaptive();
    double rn = net.getNodeRobustness();
    
    net = *np; // Attack前の状態に戻す
    
    int c = 0; // counter
    
    long dc = 0; // debug用カウンタ
    
    // 終了条件 if there is no improvement in last 10000 iterations
    while ( c<ITR_LIMIT ){
        
        // ランダムに2本のエッジを選択して
        int in1, in2, ie1, ie2;
        in1 = rand()%(nodes.size());
        in2 = rand()%(nodes.size());
        ie1 = rand()%nodes.at(in1).getLinks().size();
        ie2 = rand()%nodes.at(in2).getLinks().size();
        
        
        // flipしてみる（失敗したらもう一度乱数生成）
        if ( !net.FlipLinks(in1, ie1, in2, ie2) || !net.flipedLinksAreInsideCircle(in1,ie1,in2,ie2) ){
            continue;
        }
        
        // Attack Algorithmの選択
        AttackNode an(&net);
        an.HighDegreeAdaptive();
        double rnn = net.getNodeRobustness();
        
        // Flip後のRobustnessの方が大きければ
        //        if( (rnn > rn && sel==1) || (ren > re && sel==2) || (rnn > rn && ren > re && sel==3) ){
        if( rnn > rn && sel==1 ){
            c = 0; // カウンタはリセット
            rn = rnn; // Robustnessを更新
            (*np).FlipLinks(in1, ie1, in2, ie2); // Flip
            net = (*np); // コピー
        } else {
            c++;
            net = (*np);
        }
        
        dc++;
        if ( dc%1000 == 0 && dc != 0 ){
            cout << dc << " times iterated" << endl;
        }
        
        if ( c==5000 ) cout << "There is no improvement in last 5000 iterations" << endl;
        if ( c==7500 ) cout << "There is no improvement in last 7500 iterations" << endl;
        if ( c==9000 ) cout << "There is no improvement in last 9000 iterations" << endl;
        
    };
    
}

void Reconstruction::LocalSelectiveGreedy(const int sel){
    // 乱数のシードを設定
    srand((unsigned int)time(NULL));
    
    Network net = (*np); // 実際に攻撃するネットワーク
    
    vector<Node> nodes = net.getNodes();
    
    // とりあえず初期段階のRobustnessを計算
    AttackNode an(&net);
    an.HighDegreeAdaptive();
    double rn = net.getNodeRobustness();
    
    net = *np; // Attack前の状態に戻す
    
    int c = 0; // counter
    
    long dc = 0; // debug用カウンタ
    
    // 終了条件 if there is no improvement in last 10000 iterations
    while ( c<ITR_LIMIT ){
        
        // ランダムに2頂点を選択して
        int in1, in2, ie1, ie2;
        in1 = rand()%(nodes.size());
        in2 = rand()%(nodes.size());
        
        // 元のリンク
        vector<Link> links1 = nodes.at(in1).getLinks();
        vector<Link> links2 = nodes.at(in2).getLinks();
        sort(links1.begin(),links1.end());
        sort(links2.begin(),links2.end());
        
        // Unit Disc Graphのリンク
        vector<Link> UDLinks1 = nodes.at(in1).getUDLinks();
        vector<Link> UDLinks2 = nodes.at(in2).getUDLinks();
        sort(UDLinks1.begin(),UDLinks1.end());
        sort(UDLinks2.begin(),UDLinks2.end());
        
        // 候補を作成
        vector<int> candidates1;
        vector<int> candidates2;
        
        int idx = 0;
        
        // 候補群1を設定
        
        vector<Link> longer = links1.size()>=UDLinks2.size() ? links1 : UDLinks2;
        vector<Link> shorter = links1.size()<UDLinks2.size() ? links1 : UDLinks2;
        
        // 候補が存在しなければもう一度選択しなおす
        if( shorter.size()==0 )
            continue;
        
        for( int i=0; i<longer.size(); i++ ){
            
            if( idx>=shorter.size() )
                break;
            
            if( longer.at(i).getDestination() == shorter.at(idx).getDestination() ){
                candidates1.push_back(longer.at(i).getDestination());
                idx++;
            }
        }
        
        // 候補が存在しなければもう一度選択しなおす
        if( candidates1.empty() )
            continue;
        
        
        // 候補群2を設定
        
        idx = 0;
        
        longer = links2.size()>=UDLinks1.size() ? links2 : UDLinks1;
        shorter = links2.size()<UDLinks1.size() ? links2 : UDLinks1;
        
        // 候補が存在しなければもう一度選択しなおす
        if( shorter.size()==0 )
            continue;
        
        for( int i=0; i<longer.size(); i++ ){
            
            if( idx>=shorter.size() )
                break;
            
            if( longer.at(i).getDestination() == shorter.at(idx).getDestination() ){
                candidates2.push_back(longer.at(i).getDestination());
                idx++;
            }
        }
        
        // 候補が存在しなければもう一度選択しなおす
        if( candidates2.empty() )
            continue;
        
        
        // 候補が決定したら、候補群の中から1つずつ選ぶ
        ie1 = rand()%candidates1.size();
        ie2 = rand()%candidates2.size();
        
        
        // flipしてみる（失敗したらもう一度乱数生成）
        if ( !net.FlipLinks(in1, ie1, in2, ie2) || !net.flipedLinksAreInsideCircle(in1,ie1,in2,ie2) ){
            continue;
        }
        
        // Attack Algorithmの選択
        AttackNode an(&net);
        an.HighDegreeAdaptive();
        double rnn = net.getNodeRobustness();
        
        // Flip後のRobustnessの方が大きければ
        //        if( (rnn > rn && sel==1) || (ren > re && sel==2) || (rnn > rn && ren > re && sel==3) ){
        if( rnn > rn && sel==1 ){
            c = 0; // カウンタはリセット
            rn = rnn; // Robustnessを更新
            (*np).FlipLinks(in1, ie1, in2, ie2); // Flip
            net = (*np); // コピー
        } else {
            c++;
            net = (*np);
        }
        
        dc++;
        if ( dc%1000 == 0 && dc != 0 ){
            cout << dc << " times iterated" << endl;
        }
        
        if ( c==5000 ) cout << "There is no improvement in last 5000 iterations" << endl;
        if ( c==7500 ) cout << "There is no improvement in last 7500 iterations" << endl;
        if ( c==9000 ) cout << "There is no improvement in last 9000 iterations" << endl;
        
    };

}



/*
 
void Reconstruction::GreedyFlip() {
    vector< vector<FELEMENT> > FMatrix((*np).getV(),(*np).getV());

    // 先にF-Matrixを計算しておく O(n^2)（隣接行列を使用）
    vector<Node> nodes = (*np).getNodes();

    int i = 0;
    for( vector<Node>::iterator itr1 = nodes.begin(); itr1!=nodes.end(); itr1++, i++ ){
        int j = 0;
        for( vector<Node>::iterator itr2 = nodes.begin(); itr2!=nodes.end(); itr2++, j++ ){
            if( (*itr1).isConnected((*itr2).getNodeID()) ){
                FMatrix[i][j].d = 1.0;
            } else {
                FMatrix[i][j].d =
                    (*itr1).getDistance((*itr2))
                    + fabs((*itr1).getDegree()-(*itr2).getDegree()) / (double)(*np).getMaxDegree();
                if( FMatrix[i][j].d>1.0 )
                    FMatrix[i][j].d = 1.0;
            }
            FMatrix[i][j].eid = j;
        }
    }
    
    int nexti = 0;
    for( vector<Node>::iterator itn=nodes.begin(); itn!=nodes.end(); itn++ ){
        vector<Link> links = (*itn).getLinks();
        nexti = 0;
        vector<FELEMENT> fv = FMatrix.at(itn-nodes.begin());
        sort(fv.begin(),fv.end());
        for( vector<Link>::iterator itl=links.begin(); itl!=links.end(); itl++, nexti++ ){
            vector<Link> lks = nodes.at(fv.at(nexti).eid).getLinks();
            for( vector<Link>::iterator itll=lks.begin(); itll!=lks.end(); itll++ ){
                int in1 = (*itn).getNodeID();
                int ie1 = (int)(itl - links.begin());
                int in2 = (*itll).getDestination();
                int ie2 = nodes.at(in2).getLinkIndex((*itll).getSource());
                
                // 弱めの条件（既存のリンクがどうであれフリップ可能）
                double prob = (1.0 - FMatrix.at(in1).at(ie2).d) * (1.0 - FMatrix.at(in2).at(ie1).d);
                double dice = rand()/(double)RAND_MAX;
                if( prob > dice ){
                    (*np).FlipLinks(in1, ie1, in2, ie2);
                    break;
                }
            } // 相手のリンクに関するループ
        } // 自分のリンクに関するループ
    } // 自分のノードに関するループ
    
}
 
*/



void Reconstruction::antiGreedyOptimization(const int sel) {
    
    // 次数の昇順にソート
    vector<Node> nodes = (*np).getNodes();
    vector<Node> sorted = (*np).getNodes();
    sort(sorted.begin(),sorted.end());
    
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
    
    // 空のネットワークを作成して*npにコピー
    Network net;
    (*np) = net;
    for( int i=0; i<nodes.size(); i++ ){
        Node node(i,"");
        (*np).addNode(node);
    }
    
    int bk = 0;
    while( sorted.at(bk).getDegree()==dd.at(0).degree ){
        vector<Link> links = sorted.at(bk).getLinks();
        for( vector<Link>::iterator itr=links.begin(); itr!=links.end(); itr++ ){
            outerEdges.push((*itr));
        }
        bk++;
    }
    
    int r = 1;
    while( r < dd.size() ){
        MakeConnection(sorted,bk,dd.at(r).degree);
        while( bk<sorted.size() && sorted.at(bk).getDegree()==dd.at(r).degree )
            bk++;
        r++;
    }
    FinalizeNetworkConnectingRandomly();
    
}

void Reconstruction::MakeConnection(vector<Node> sorted, int bk, int d){
    // もしd次のノードが全て前段のノードと接続されていなければ、内部で連結する
    int next = ConnectWithLowers(sorted,bk,d);
    if( next!=-1 ){
        ConnectInside(sorted,bk,d,next);
    }
}

int Reconstruction::ConnectWithLowers(vector<Node> sorted, int bk, int d){
    int i = 0;
    while( !outerEdges.empty() && bk+i<sorted.size() && sorted.at(bk+i).getDegree()==d ){
        
        Link l1(outerEdges.top().getSource(),sorted.at(bk+i).getNodeID());
        Link l2(sorted.at(bk+i).getNodeID(),outerEdges.top().getSource());
        (*np).addLinkToNode(l1, outerEdges.top().getSource());
        (*np).addLinkToNode(l2, sorted.at(bk+i).getNodeID());
        outerEdges.pop();
        
        vector<Link> links = sorted.at(bk+i).getLinks();
        for( int j=1; j<links.size(); j++ )
            outerEdges.push(links.at(j));
        
        i++;
    }
    
    // 前段からの辺がなく、まだ繋ぎ切れていないノードが存在するならそのノードの先頭idを返す
    if( outerEdges.empty() && bk+i<sorted.size() && sorted.at(bk+i).getDegree()==d )
        return bk+i;
    
    // すべてつなぎ終えたら-1を返す
    return -1;
}

// 
void Reconstruction::ConnectInside(vector<Node> sorted, int bk, int d, int next){
}

void Reconstruction::FinalizeNetworkConnectingRandomly(){
    vector<Link> links;
    while( !outerEdges.empty() ){
        links.push_back(outerEdges.top());
        outerEdges.pop();
    }
    
    int *perm = MakeRandomPermutation(0, (int)links.size());
    
    vector<Node> nodes = (*np).getNodes();
    for( int i=0; i<links.size(); i+=2 ){
        Link l1(links.at(perm[i]).getSource(), links.at(perm[i+1]).getSource());
        Link l2(links.at(perm[i+1]).getSource(), links.at(perm[i]).getSource());
        (*np).addLinkToNode(l1, links.at(perm[i]).getSource());
        (*np).addLinkToNode(l2, links.at(perm[i+1]).getSource());
    }
    
    delete [] perm;
        
    
}

/*
void Reconstruction::antiGreedyOptimization(const int sel){
    
    // 次数の降順にソート
    vector<Node> nodes = (*np).getNodes();
    vector<Node> sorted = (*np).getNodes();
    sort(sorted.begin(),sorted.end(),greater<Node>());
    
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
    
    // メイン部分
    int r = dd.at(dd.size()-1).degree;
    while( r>1 ){
        int di, dj, dk;
        sorted = (*np).getNodes();
        sort(sorted.begin(),sorted.end(),greater<Node>());
        FindNonEmptyDegrees(dd,&di,&dj,&dk,r);
        MakeConnection(dHistgram, sorted, di, dj, dk);
        r = dj; 
    }
    
    delete [] dHistgram;
}
*/

// r以下で最大の0でない次数を順にdi,dj,dkにセットする
void Reconstruction::FindNonEmptyDegrees(vector<DDISTRIBUTION> dd, int *di, int *dj, int *dk, int r) {
    (*di) = (*dj) = (*dk) = 0;
    int b = (int)dd.size()-1;
    while( dd.at(b).degree!=r ) b--;
    (*di) = b>=0 ? dd.at(b).degree : 0;
    (*dj) = b>=1 ? dd.at(b-1).degree : 0;
    (*dk) = b>=2 ? dd.at(b-2).degree : 0;
}

// r以下で最大の0でない次数を順にdi,dj,dkにセットする
void Reconstruction::FindNonEmptyDegrees(vector<DDISTRIBUTION> dd, int *di, int *dj, int *dk, int *dm, int *dl, int r) {
    (*di) = (*dj) = (*dk) = (*dm) = (*dl) = 0;
    int b = (int)dd.size()-1;
    while( dd.at(b).degree!=r ) b--;
    (*di) = b>=0 ? dd.at(b).degree : 0;
    (*dj) = b>=1 ? dd.at(b-1).degree : 0;
    (*dk) = b>=2 ? dd.at(b-2).degree : 0;
    (*dm) = b>=3 ? dd.at(b-3).degree : 0;
    (*dl) = b>=4 ? dd.at(b-4).degree : 0;
}

// 繋ぎ変える部分
void Reconstruction::MakeConnection(int dHistgram[], vector<Node> sorted, int di, int dj, int dk){
    
    int nk   = dHistgram[di];
    int nkm1 = dHistgram[dj];
    int nkm2 = dHistgram[dk];
    
    if( dj==0 && dk==0 ) return;
    
    // swapして良い辺の個数
    int swappable = (int)(nkm1*dj*nk/(double)(nk+nkm2));
    
    // beginning of nodes with degrees dj
    int bgnNkm1 = 0;
    while( sorted.at(bgnNkm1).getDegree()!=dj ) bgnNkm1++;
    
    vector<Node> sNodes;
    if( swappable>=nk ){
    
        for( int i=bgnNkm1; i<bgnNkm1+nk; i++ )
            sNodes.push_back(sorted.at(i));

    } else {
        
//        int *perm = MakeRandomPermutation(bgnNkm1,nkm1);
        for( int k=0; k<swappable; k++ ){
            int rnd = rand()%nkm1 + bgnNkm1;
            sNodes.push_back(sorted.at(rnd));
        }
    }

    // swapして良い辺の個数回だけ繰り返し
    int nitr = swappable>=nk ? nk : swappable;
    
    int bgnNk = bgnNkm1-nk;
    int in1, in2, ie1, ie2;
    for( int i=bgnNk; i<bgnNk+nitr; i++ ){
        
        vector<Node> nodes = (*np).getNodes();
        
        vector<Link> links1 = sorted.at(i).getLinks();
        vector<Link> links2 = sNodes.at(i-bgnNk).getLinks();
        in1 = sorted.at(i).getNodeID();
        in2 = sNodes.at(i-bgnNk).getNodeID();
        
        // すでに隣接していたらスキップ
        if( nodes.at(in1).isConnectedAndSetNecessary(in2) ) continue;
        
        int rnd1 = links1.size()>=1 ? rand()%links1.size() : 0;
        int rnd2 = links2.size()>=1 ? rand()%links2.size() : 0;
        
        bool isNecessary = false;
        if( bgnNk>=1 && di!=(*np).getMaxDegree()
                     && nodes.at(links1.at(rnd1).getDestination()).getDegree()==sorted.at(bgnNk-1).getDegree() )
            isNecessary = true;
        
        if( !isNecessary ){
            ie1 = sorted.at(i).getLinkIndex(links1.at(rnd1).getDestination());
        } else {
            int r1 = links1.size()>=1 ? (rnd1+1)%(links1.size()) : 0;
            ie1 = sorted.at(i).getLinkIndex(links1.at(r1).getDestination());
        }
        ie2 = sNodes.at(i-bgnNk).getLinkIndex(links2.at(rnd2).getDestination());

        (*np).SwapLinksIN1toIN2(in1, in2, ie1, ie2);
//        (*np).SwapLinksIN1toIN2AllowedDoubleEdge(in1, in2, ie1, ie2);
    }
}

// 繋ぎ変える部分
void Reconstruction::MakeConnection(int dHistgram[], vector<Node> sorted, int di, int dj, int dk, int dm, int dl){
    
    int nk   = dHistgram[di];
    int nkm1 = dHistgram[dj];
    int nkm2 = dHistgram[dk];
//    int nkm3 = dHistgram[dm];
    int nkm4 = dHistgram[dl];
    
    if( dj==0 && dk==0 ) return;
    
    // swapして良い辺の個数
    int swappable1 = (int)(nkm1*dj*nk/(double)((nk+nkm2)*2.0));
    
    // beginning of nodes with degrees dj
    int bgnNkm1 = 0;
    while( sorted.at(bgnNkm1).getDegree()!=dj ) bgnNkm1++;
    
    
    vector<Node> sNodes1;
    if( swappable1>=nk ){
        
        for( int i=bgnNkm1; i<bgnNkm1+nk; i++ )
            sNodes1.push_back(sorted.at(i));
        
    } else {
        
        for( int k=0; k<swappable1; k++ ){
            int rnd = rand()%nkm1 + bgnNkm1;
            sNodes1.push_back(sorted.at(rnd));
        }
    }
    
    // swapして良い辺の個数回だけ繰り返し
    int nitr1 = swappable1>=nk ? nk : swappable1;
    
    int bgnNk = bgnNkm1-nk;
    int in1, in2, ie1, ie2;
    for( int i=bgnNk; i<bgnNk+nitr1; i++ ){
        
        vector<Node> nodes = (*np).getNodes();
        
        vector<Link> links1 = sorted.at(i).getLinks();
        vector<Link> links2 = sNodes1.at(i-bgnNk).getLinks();
        in1 = sorted.at(i).getNodeID();
        in2 = sNodes1.at(i-bgnNk).getNodeID();
        
        // すでに隣接していたらスキップ
        if( nodes.at(in1).isConnectedAndSetNecessary(in2) ) continue;
        
        int rnd1 = links1.size()>=1 ? rand()%links1.size() : 0;
        int rnd2 = links2.size()>=1 ? rand()%links2.size() : 0;
        
        bool isNecessary = false;
        if( bgnNk>=1 && di!=(*np).getMaxDegree()
           && nodes.at(links1.at(rnd1).getDestination()).getDegree()==sorted.at(bgnNk-1).getDegree() )
            isNecessary = true;
        
        if( !isNecessary ){
            ie1 = sorted.at(i).getLinkIndex(links1.at(rnd1).getDestination());
        } else {
            int r1 = links1.size()>=1 ? (rnd1+1)%(links1.size()) : 0;
            ie1 = sorted.at(i).getLinkIndex(links1.at(r1).getDestination());
        }
        ie2 = sNodes1.at(i-bgnNk).getLinkIndex(links2.at(rnd2).getDestination());
        
        (*np).SwapLinksIN1toIN2(in1, in2, ie1, ie2);
        //        (*np).SwapLinksIN1toIN2AllowedDoubleEdge(in1, in2, ie1, ie2);
    }
    
    
    int swappable2 = (int)(nkm2*dk*nk/(double)((nk+nkm4)*2.0));
    
    if( dm==0 && dl==0 ) return;
    
    // 2ホップ離れた次数への対応
    int bgnNkm2 = 0;
    while( sorted.at(bgnNkm2).getDegree()!=dk ) bgnNkm2++;
    
    vector<Node> sNodes2;
    if( swappable2>=nk ){
        
        for( int i=bgnNkm2; i<bgnNkm2+nk; i++ )
            sNodes2.push_back(sorted.at(i));
        
    } else {
        
        //        int *perm = MakeRandomPermutation(bgnNkm1,nkm1);
        for( int k=0; k<swappable2; k++ ){
            int rnd = rand()%nkm2 + bgnNkm2;
            sNodes2.push_back(sorted.at(rnd));
        }
    }
    
    int nitr2 = swappable2>=nk ? nk : swappable2;
    
    for( int i=bgnNk+nitr1; i<bgnNk+nitr1+nitr2; i++ ){
        vector<Node> nodes = (*np).getNodes();
        
        vector<Link> links1 = sorted.at(i).getLinks();
        vector<Link> links2 = sNodes2.at(i-bgnNk-nitr1).getLinks();
        in1 = sorted.at(i).getNodeID();
        in2 = sNodes2.at(i-bgnNk-nitr1).getNodeID();
        
        // すでに隣接していたらスキップ
        if( nodes.at(in1).isConnectedAndSetNecessary(in2) ) continue;
        
        int rnd1 = links1.size()>=1 ? rand()%links1.size() : 0;
        int rnd2 = links2.size()>=1 ? rand()%links2.size() : 0;
        
        int bgnNkp1 = bgnNk;
        while( bgnNkp1>=0 && sorted.at(bgnNkp1).getDegree()==di ) bgnNkp1--;
        if( bgnNkp1<0 ) bgnNkp1 = 0;
        
        bool isNecessary = false;
        if( bgnNkp1>=1 && di!=(*np).getMaxDegree()
           && nodes.at(links1.at(rnd1).getDestination()).getDegree()==sorted.at(bgnNkp1-1).getDegree() )
            isNecessary = true;
        
        if( !isNecessary ){
            ie1 = sorted.at(i).getLinkIndex(links1.at(rnd1).getDestination());
        } else {
            int r1 = links1.size()>=1 ? (rnd1+1)%(links1.size()) : 0;
            ie1 = sorted.at(i).getLinkIndex(links1.at(r1).getDestination());
        }
        ie2 = sNodes2.at(i-bgnNk-nitr1).getLinkIndex(links2.at(rnd2).getDestination());
        
        (*np).SwapLinksIN1toIN2(in1, in2, ie1, ie2);
    }
}

// index iから始まるサイズnのランダム配列を作成
int* Reconstruction::MakeRandomPermutation(int i, int n){
    int *perm = new int[n];
    for( int k=0; k<n; k++ )
        perm[k] = i+k;
    
    for( int k=n-1; k>1; k-- ){
        int rnd = rand()%n;
        int w = perm[k];
        perm[k] = perm[rnd];
        perm[rnd] = w;
    }
    
    return perm;
}


// たぶん given network が非連結の場合に配列要素がオーバーするよ
/*
void Reconstruction::antiGreedyOptimization(const int sel){
    
    // 次数の降順にソート
    vector<Node> nodes = (*np).getNodes();
    vector<Node> sortedNodes = (*np).getNodes();
    sort(sortedNodes.begin(),sortedNodes.end(),greater<Node>());
    
    // とりあえずdegree histgramを作成
    int maxDegree = (*np).getMaxDegree();
    int *dHistgram = new int[maxDegree+1];
    vector<DDISTRIBUTION> dd;
    for( int i=0; i<maxDegree+1; i++ ) dHistgram[i] = 0;
    for( int i=0; i<nodes.size(); i++ ){
        dHistgram[nodes.at(i).getDegree()]++;
    }
    for( int i=0; i<maxDegree+1; i++ ){
        if( dHistgram[i]>0 ){
            DDISTRIBUTION dnew(i,dHistgram[i]);
            dd.push_back(dnew);
        }
    }
    
    for( int i=0; i<sortedNodes.size()-1; i++ ){
        
        
        // 次数deg(i)の頂点がただ一つ存在した場合
        if( dHistgram[sortedNodes.at(i).getDegree()]==1 ){
            
            // 次数が高い方から頂点を2つ選ぶ
            int in1 = sortedNodes.at(i).getNodeID();
            int in2 = sortedNodes.at(i+1).getNodeID();
            
            // すでに隣接していたらスキップ
            if( sortedNodes.at(i).isConnected(sortedNodes.at(i+1).getNodeID()) )
                continue;
            
            vector<Link> links = sortedNodes.at(i).getLinks();
            vector<Node> cands1;
            vector<Node> cands2;
            
            // 全てのリンクの中から最も次数の離れたリンクを抽出
            for( vector<Link>::iterator itr=links.begin(); itr!=links.end(); itr++ ){
                Node node = nodes.at((*itr).getDestination());
                int diffDeg = fabs(node.getDegree() - nodes.at(i).getDegree());
                node.setDegree(diffDeg);
                cands1.push_back(node);
            }
            sort(cands1.begin(),cands1.end(),greater<Node>());
            
            links = sortedNodes.at(i+1).getLinks();
            for( vector<Link>::iterator itr=links.begin(); itr!=links.end(); itr++ ){
                Node node = nodes.at((*itr).getDestination());
                int diffDeg = fabs(node.getDegree() - nodes.at(i+1).getDegree());
                node.setDegree(diffDeg);
                cands2.push_back(node);
            }
            sort(cands2.begin(),cands2.end(),greater<Node>());
            
            bool fin = false;
            for( vector<Node>::iterator itr1 = cands1.begin(); itr1!=cands1.end(); itr1++ ){
                for( vector<Node>::iterator itr2 = cands2.begin(); itr2!=cands2.end(); itr2++ ){
                    int ie1 = sortedNodes.at(i).getLinkIndex((*itr1).getNodeID());
                    int ie2 = sortedNodes.at(i+1).getLinkIndex((*itr2).getNodeID());
                    fin = (*np).SwapLinksIN1toIN2(in1, in2, ie1, ie2);
                    if( fin ) break;
                }
                if( fin ) break;
            }
            
            if( !fin ){
                int ie1 = sortedNodes.at(i).getLinkIndex(cands1.at(0).getNodeID());
                int ie2 = sortedNodes.at(i+1).getLinkIndex(cands2.at(0).getNodeID());
                (*np).SwapLinksIN1toIN2AllowedDoubleEdge(in1, in2, ie1, ie2);
            }
        }
        
        // 次数deg(i)の頂点が複数個存在した場合
        else if ( dHistgram[sortedNodes.at(i).getDegree()]>1 ) {
            
            int nk = dHistgram[sortedNodes.at(i).getDegree()];
            int nk1 = i+nk<sortedNodes.size() ? dHistgram[sortedNodes.at(i+nk).getDegree()] : 0;
            int nk2 = i+nk+nk1<sortedNodes.size() ? dHistgram[sortedNodes.at(i+nk+nk1).getDegree()] : 0;
            int degk1 = i+nk<sortedNodes.size() ? sortedNodes.at(i+nk).getDegree() : 0;
            
            if( nk1==0 && nk2==0 ){
                break;
            }
            
            vector<LinkWithDifference> candidates;
            
            
            // 次数deg(i-nk)の頂点を全て見る
            for( int k=0; k<nk1; k++ ){

                vector<Link> links = sortedNodes.at(i+nk+k).getLinks();
                for( vector<Link>::iterator itr=links.begin(); itr!=links.end(); itr++ ){
                    Node node = nodes.at((*itr).getDestination());
                    int diffDeg = fabs(node.getDegree() - nodes.at(i+nk+k).getDegree());
                    LinkWithDifference lwd((*itr).getSource(),(*itr).getDestination(),diffDeg);
                    candidates.push_back(lwd);
                }
            }
            if( candidates.size()==0 ) continue;
            
            sort(candidates.begin(),candidates.end(),greater<LinkWithDifference>());
            
            // 次数deg(i)の頂点を全部結びきれる
            if( nk1*degk1-nk2>=nk ){
                
                // 次数deg(i)の頂点を全て見る
                for( int j=0; j<nk; j++ ){
                
                    // すでにリンクが結ばれていればスキップ
                    bool isConnected = false;
                    for( int k=0; k<nk1; k++ ){
                        if ( sortedNodes.at(i+j).isConnected(sortedNodes.at(i+nk+k).getNodeID()) ){
                            isConnected = true;
                        }
                    }
                    if( isConnected ) continue;
                    
                    vector<Node> cands;
                    vector<Link> links = sortedNodes.at(i+j).getLinks();
                    if( links.size()==0 ) continue;
                    
                    for( vector<Link>::iterator itr=links.begin(); itr!=links.end(); itr++ ){
                        Node node = nodes.at((*itr).getDestination());
                        int diffDeg = fabs(node.getDegree() - nodes.at(i+j).getDegree());
                        node.setDegree(diffDeg);
                        cands.push_back(node);
                    }
                    sort(cands.begin(),cands.end(),greater<Node>());
                    

                    
                    bool fin = false;
                    for( vector<LinkWithDifference>::iterator itl=candidates.begin(); itl!=candidates.end(); itl++ ){
                        for( vector<Node>::iterator itn=cands.begin(); itn!=cands.end(); itn++ ){
                            int in1 = sortedNodes.at(i+j).getNodeID();
                            int in2 = (*itl).link.getSource();
                            int ie1 = sortedNodes.at(i+j).getLinkIndex((*itn).getNodeID());
                            int ie2 = nodes.at(in2).getLinkIndex((*itl).link.getDestination());
                            
                            fin = (*np).SwapLinksIN1toIN2(in1, in2, ie1, ie2);
                            
                            if( fin ){
                                itl = candidates.erase(itl);
                                break;
                            }
                        }
                        
                        if( fin ) break;
                    }
                    
                    if( !fin ){
                        int in1 = sortedNodes.at(i+j).getNodeID();
                        int in2 = candidates.at(0).link.getSource();
                        int ie1 = sortedNodes.at(i+j).getLinkIndex(cands.at(0).getNodeID());
                        int ie2 = nodes.at(in2).getLinkIndex(candidates.at(0).link.getDestination());
                        (*np).SwapLinksIN1toIN2AllowedDoubleEdge(in1, in2, ie1, ie2);
                    }
                    
                }
            }
            
            // 結び切れない
            else {
                
                int *perm = random_perm(nk);
                
                // 次数deg(i)の頂点を全て見る
                for( int j=0; j<nk1-nk2; j++ ){
                    
                    
                    // すでにリンクが結ばれていればスキップ
                    bool isConnected = false;
                    for( int k=0; k<nk1; k++ ){
                        if ( sortedNodes.at(i+perm[j]).isConnected(sortedNodes.at(i+nk+k).getNodeID()) ){
                            isConnected = true;
                        }
                    }
                    if( isConnected ) continue;
                    
                    vector<Node> cands;
                    vector<Link> links = sortedNodes.at(i+perm[j]).getLinks();
                    if( links.size()==0 ) continue;
                    
                    for( vector<Link>::iterator itr=links.begin(); itr!=links.end(); itr++ ){
                        Node node = nodes.at((*itr).getDestination());
                        int diffDeg = fabs(node.getDegree() - nodes.at(i+j).getDegree());
                        node.setDegree(diffDeg);
                        cands.push_back(node);
                    }
                    sort(cands.begin(),cands.end(),greater<Node>());
                    
                    bool fin = false;
                    for( vector<LinkWithDifference>::iterator itl=candidates.begin(); itl!=candidates.end(); itl++ ){
                        for( vector<Node>::iterator itn=cands.begin(); itn!=cands.end(); itn++ ){
                            int in1 = sortedNodes.at(i+perm[j]).getNodeID();
                            int in2 = (*itl).link.getSource();
                            int ie1 = sortedNodes.at(i+perm[j]).getLinkIndex((*itn).getNodeID());
                            int ie2 = nodes.at(in2).getLinkIndex((*itl).link.getDestination());
                            
                            fin = (*np).SwapLinksIN1toIN2(in1, in2, ie1, ie2);
                            
                            if( fin ){
                                itl = candidates.erase(itl);
                                break;
                            }
                        }
                        
                        if( fin ) break;
                    }
                    
                    if( !fin ){
                        int in1 = sortedNodes.at(i+perm[j]).getNodeID();
                        int in2 = candidates.at(0).link.getSource();
                        int ie1 = sortedNodes.at(i+perm[j]).getLinkIndex(cands.at(0).getNodeID());
                        int ie2 = nodes.at(in2).getLinkIndex(candidates.at(0).link.getDestination());
                        (*np).SwapLinksIN1toIN2AllowedDoubleEdge(in1, in2, ie1, ie2);
                    }
                }
                
                delete [] perm;
            }
            
            i += nk-1;
        }
        
        
    }
    
    delete [] dHistgram;
}
*/


/*  けっこう古い
 
 void Reconstruction::antiGreedyOptimization(const int sel){
 
 // 次数の降順にソート
 vector<Node> nodes = (*np).getNodes();
 vector<Node> sortedNodes = (*np).getNodes();
 sort(sortedNodes.begin(),sortedNodes.end(),greater<Node>());
 
 for( int i=0; i<sortedNodes.size()-1; i++ ){
 
 // 次数が高い方から頂点を2つ選ぶ
 int in1 = sortedNodes.at(i).getNodeID();
 int in2 = sortedNodes.at(i+1).getNodeID();
 
 // すでに隣接していたらスキップ
 if( sortedNodes.at(i).isConnected(sortedNodes.at(i+1).getNodeID()) )
 continue;
 
 vector<Link> links1 = sortedNodes.at(i).getLinks();
 vector<Node> cands1;
 
 // 全てのリンクの中から最も次数の離れたリンクを抽出
 for( vector<Link>::iterator itr1=links1.begin(); itr1!=links1.end(); itr1++ ){
 Node node = nodes.at((*itr1).getDestination());
 int diffDeg = fabs(node.getDegree() - nodes.at(i).getDegree());
 node.setDegree(diffDeg);
 cands1.push_back(node);
 }
 vector<Node>::iterator max1 = max_element(cands1.begin(), cands1.end());
 
 vector<Link> links2 = sortedNodes.at(i+1).getLinks();
 vector<Node> cands2;
 for( vector<Link>::iterator itr2=links2.begin(); itr2!=links2.end(); itr2++ ){
 Node node = nodes.at((*itr2).getDestination());
 int diffDeg = fabs(node.getDegree() - nodes.at(i+1).getDegree());
 node.setDegree(diffDeg);
 cands2.push_back(node);
 }
 vector<Node>::iterator max2 = max_element(cands2.begin(), cands2.end());
 
 int ie1 = sortedNodes.at(i).getLinkIndex((*max1).getNodeID());
 int ie2 = sortedNodes.at(i+1).getLinkIndex((*max2).getNodeID());
 (*np).SwapLinksIN1toIN2(in1, in2, ie1, ie2);
 
 }
 }
 
 */

void Reconstruction::SwapInterClusters(const int nc, const int bias, const int nmin, const int nmax) {
    
    vector<Node> nodes = (*np).getNodes();
    for( int i=0; i<nc; i++ ){
        int n = rand()%(nmax-nmin)+nmin;
        for( int j=0; j<n; j++ ){
            int n1 = rand()%bias+bias*j;
            int c = rand()%nc;
            while( c==j ) c = rand()%nc;
            int n2 = rand()%bias+bias*c;
            int l1 = rand()%nodes.at(n1).getDegree();
            int l2 = rand()%nodes.at(n2).getDegree();
            (*np).SwapLinksIN1toIN2(n1,n2,l1,l2);
        }
    }
}

Network Reconstruction::InterHubDisconnectedForBA(const int nh){
    
    vector<int> degs;
    for( int i=0; i<(*np).getV(); i++ ){
        degs.push_back((*np).getNodes().at(i).getDegree());
    }
    sort(degs.begin(),degs.end(),greater<int>());
    
    // 隣接行列を先に作る
    bool **adj;
    adj = new bool*[degs.size()];
    for( int i=0; i<degs.size(); i++ ){
        adj[i] = new bool[degs.size()];
        for( int j=0; j<degs.size(); j++ ){
            adj[i][j] = false;
        }
    }
    
    // まずはHubと次数の小さいノードを結ぶ
    for( int i=0; i<nh; i++ ){
        int k = (int)degs.size()-1;
        int degi = degs.at(i);
        for( int c=0; c<degi; k-- ){
            // Hubが結び切れない
            if( k<=0 ) exit(1);
            
            if( degs.at(k)<=0 ) continue; else c++;
            adj[i][k] = adj[k][i] = true;
            
            // 接続したら次数を1減らす
            degs.at(k)--;
            degs.at(i)--;
        }
    }
    
    // 今度は昇順にソート
    sort(degs.begin(),degs.end());
    
    for( int i=nh; i<(int)degs.size(); i++ ){
        if( degs.at(i)<=0 ) continue;
        int b = nh; // 次数1のノードの開始index（b未満のindex blではdegs.at(bl)=0）
        if( degs.at(i)==1 ){
            // 次数1の先頭ノードの場所を記憶
            if( b==nh ) b = i;
            
            continue;
        }
        
        // 次数2以上のノードについて
        int degi = degs.at(i);
        int k = b;
        for( int c=0; c<degi && i>k; k++ ){
            if( degs.at(k)<=0 ) continue; else c++;
            adj[k][i] = true;
            adj[i][k] = true;
            degs.at(i)--;
            degs.at(k)--;
            if( degs.at(b)<=0 ) b++;
        }
    }
    
    // 隣接行列をリスト形式に変換
    vector<Node> nodes;
    for( int i=0; i<degs.size(); i++ ){
        Node node(i,"");
        for( int j=0; j<degs.size(); j++ ){
            if( adj[i][j] ){
                Link l(i,j);
                node.addLink(l);
            }
        }
        nodes.push_back(node);
    }
    Network net(nodes);
    net.finalize();
    
    // 隣接行列の開放
    for( int i=0; i<degs.size(); i++ )
        delete [] adj[i];
    delete [] adj;

    return net;
}

int* Reconstruction::random_perm(int nk) {
    int *perm = new int[nk];
    
    for( int i=0; i<nk; i++ )
        perm[i] = i;
    
    for( int i=nk-1; i>0; i-- ){
        int rnd = rand()%i;
        int w = perm[i];
        perm[i] = perm[rnd];
        perm[rnd] = w;
    }
    
    return perm;
}

const double Reconstruction::getReconstructionCost() {
    
    cost = 0.0;
    
    vector<Node> nodes = (*np).getNodes();
    for( vector<Node>::iterator itn=nodes.begin(); itn!=nodes.end(); itn++ ){
        vector<Link> links = (*itn).getLinks();
        for( vector<Link>::iterator itl=links.begin(); itl!=links.end(); itl++ ){
            if( (*itl).hasBeenFliped() ){
                cost += (*itn).getDistance(nodes.at((int)(itl-links.begin())) );
            }
        }
    }
    
    return cost;
}


//
//  Network.cpp
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/07.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "Network.h"
#include <iostream>
#include <fstream>
#include "Crawler.h"
#include "HypotheticalGreedy.h"

using namespace std;

Network::Network( const vector<Node> ns ){
    nodes = ns;
    V = (int)ns.size();
    rV = V;
    SD = 0;
    
    for( int i=0; i<ns.size(); i++ ){
        SD += ns[i].getDegree();
    }
}

const void Network::showNetwork() {
    ofstream ofs(OFile.c_str());
    if( ofs==NULL ) return;
    
    ofs << "|V| = " << V << endl;
    ofs << "|E| = " << SD/2 << endl;
    ofs << "Average Shortest Path Length = " << calculateAverageShortestPathLength() << endl;
    ofs << "Clustering Coefficient = " << calculateClusteringCoefficient() << endl;
    

    for( vector<Node>::iterator itr=nodes.begin(); itr!=nodes.end(); itr++ ){
        ofs << (*itr).getNodeID() << " : ";
        (*itr).sortLinksWithID();
        for( int j=0; j<(*itr).getDegree(); j++ ){
            ofs << (*itr).getLinkAt(j).getDestination() << " ";
        }
        ofs << endl;
    }
    
    /*
    ofs << endl << "Positions" << endl;
    for( vector<Node>::iterator itr=nodes.begin(); itr!=nodes.end(); itr++ ){
        ofs << (*itr).getNodeID() << " ( " << (*itr).getPosition(0) << " , " << (*itr).getPosition(1) << " )" << endl;
    }
    */
    
    ofs.close();
    
    degreeHistgram(OFile);
}

const void Network::showNetwork(string filename){
    ofstream ofs((filename+".log").c_str());
    if( ofs==NULL ) return;
    
    ofs << "|V| = " << V << endl;
    ofs << "|E| = " << SD/2 << endl;
//    ofs << "Average Shortest Path Length = " << calculateAverageShortestPathLength() << endl;
//    ofs << "Clustering Coefficient = " << calculateClusteringCoefficient() << endl;
    
    
    for( vector<Node>::iterator itr=nodes.begin(); itr!=nodes.end(); itr++ ){
        ofs << (*itr).getNodeID() << " : ";
        (*itr).sortLinksWithID();
        for( int j=0; j<(*itr).getDegree(); j++ ){
            ofs << (*itr).getLinkAt(j).getDestination() << " ";
        }
        ofs << endl;
    }
    
    ofs.close();
    
    degreeHistgram(filename);
    
    
}

const void Network::showNetwork(string filename, string order){
    ofstream ofs((filename+".log").c_str());
    if( ofs==NULL ) return;
    
    ofs << "|V| = " << V << endl;
    ofs << "|E| = " << SD/2 << endl;
    ofs << "Average Shortest Path Length = " << calculateAverageShortestPathLength() << endl;
    ofs << "Clustering Coefficient = " << calculateClusteringCoefficient() << endl;
    
    
    vector<Node> sorted = nodes;
    if( order.at(0)=='a' )
        sort(sorted.begin(),sorted.end());
    else if( order.at(0)=='d' )
        sort(sorted.begin(),sorted.end(),greater<Node>());
    
    for( vector<Node>::iterator itr=sorted.begin(); itr!=sorted.end(); itr++ ){
        ofs << (*itr).getNodeID() << " : ";
        (*itr).sortLinksWithID();
        for( int j=0; j<(*itr).getDegree(); j++ ){
            ofs << (*itr).getLinkAt(j).getDestination() << " ";
        }
        ofs << endl;
    }
    
    ofs.close();
    
    degreeHistgram(filename);
    
    
}


const void Network::showUDNetwork() {
    ofstream ofs((OFile+"_UnitDisk"+".txt").c_str());
    if( ofs==NULL ) return;
    
    ofs << "|V| = " << V << endl;
    ofs << "|E| = " << SD/2 << endl;
    ofs << "UDR = " << UDR << endl;
    
    
    for( vector<Node>::iterator itr=nodes.begin(); itr!=nodes.end(); itr++ ){
        ofs << (*itr).getNodeID() << " : ";
        (*itr).sortUDLinksWithID();
        for( int j=0; j<(*itr).getUDDegree(); j++ ){
            ofs << (*itr).getUDLinkAt(j).getDestination() << " ";
        }
        ofs << endl;
    }
    
    ofs << endl << "Positions" << endl;
    for( vector<Node>::iterator itr=nodes.begin(); itr!=nodes.end(); itr++ ){
        ofs << (*itr).getNodeID() << " ( " << (*itr).getPosition(0) << " , " << (*itr).getPosition(1) << " )" << endl;
    }

    ofs.close();
    
    UDDegreeHistgram();
}

const void Network::degreeHistgram(string filename) {
    
    ofstream ofs((filename+".log").c_str(),ios::app);
    if( ofs==NULL ) return;

    int *histgram = new int[maxDegree+1];
    
    for ( int i=0; i<=maxDegree; i++ )
        histgram[i] = 0;
    
    for ( int i=0; i<V; i++ ){
        histgram[nodes[i].getDegree()]++;
    }
    
    ofs << "Histgram" << endl;
    for ( int i=0; i<=maxDegree; i++ ){
        ofs << i << "\t" << histgram[i] << endl;
    }
    ofs.close();
    
    delete [] histgram;
}

const void Network::UDDegreeHistgram() {
    int *histgram = new int[maxUDDegree];
    
    for ( int i=0; i<maxUDDegree; i++ )
        histgram[i] = 0;
    
    for ( int i=0; i<V; i++ ){
        histgram[nodes[i].getUDDegree()]++;
    }
    
    ofstream ofs(OFile.c_str(),ios::app);
    ofs << "UDHistgram" << endl;
    for ( int i=0; i<maxUDDegree; i++ ){
        ofs << i+1 << "\t" << histgram[i] << endl;
    }
    ofs.close();
    
    delete [] histgram;
}

const void Network::removeNode(int nid){

    vector<Node>::iterator itr = nodes.begin();

    // ノードを削除
    while( itr!=nodes.end() && !nodes.empty() ){
        // 削除するノードを含む辺を全て削除
        SD -= (*itr).removeLink(nid);
        
        if( (*itr).getNodeID()==nid ){
            (*itr).eraseLink();
            (*itr).setDegree(0);
            // itr = nodes.erase(itr);
            rV--;
        }
        
        ++itr;
    }
}

const void Network::removeEdgeAtNode(int lid, int nid){
    
    int v = nodes.at(nid).getLinkAt(lid).getDestination();
    nodes.at(nid).removeLink(lid);
    
    vector<Link> links = nodes.at(v).getLinks();
    for( vector<Link>::iterator itr = links.begin(); itr != links.end(); itr++ ){
        if( (*itr).getDestination()==nid ){
            nodes.at(v).getLinkAt((int)(itr-links.begin()));
            return;
        }
    }
    
}

// Flipが成功したらtrue、そうでなければfalseを返す
const bool Network::FlipLinks(const int in1, const int ie1, const int in2, const int ie2 ){
    
    int d1 = nodes.at(in2).getLinkAt(ie2).getDestination();
    int d2 = nodes.at(in1).getLinkAt(ie1).getDestination();
    int in3 = d2;
    int in4 = d1;
    int ie3 = nodes.at(in3).getLinkIndex(in1);
    int ie4 = nodes.at(in4).getLinkIndex(in2);
    
    if( d1!=in1 && d2!=in2 && !linksExist(in1,d1,in2,d2) ) {
        
        nodes.at(in1).FlipLink(ie1, d1);
        nodes.at(in2).FlipLink(ie2, d2);
        nodes.at(in3).FlipLink(ie3, in2);
        nodes.at(in4).FlipLink(ie4, in1);
        return true;
    }
    
    return false;
}

const bool Network::SwapLinksIN1toIN2(const int in1, const int in2, const int ie1, const int ie2) {

    int d1 = in2;
    int d2 = in1;
    int in3 = nodes.at(in1).getLinkAt(ie1).getDestination();
    int in4 = nodes.at(in2).getLinkAt(ie2).getDestination();
    int ie3 = nodes.at(in3).getLinkIndex(in1);
    int ie4 = nodes.at(in4).getLinkIndex(in2);
    
    if( ie1==-1 || ie2==-1 || ie3==-1 || ie4==-1 )
        return false;
    
    if( !nodes.at(in1).isConnected(in2) && !nodes.at(in3).isConnected(in4) ) {
        nodes.at(in1).FlipLink(ie1, d1);
        nodes.at(in2).FlipLink(ie2, d2);
        nodes.at(in3).FlipLink(ie3, in4);
        nodes.at(in4).FlipLink(ie4, in3);
        
        nodes.at(in1).getLinkAt(ie1).setIsNecessary(true);
        nodes.at(in2).getLinkAt(ie2).setIsNecessary(true);
        
        return true;
    }

    return false;
}

const bool Network::SwapLinksIN1toIN2AllowedDoubleEdge(const int in1, const int in2, const int ie1, const int ie2 ) {
    int d1 = in2;
    int d2 = in1;
    int in3 = nodes.at(in1).getLinkAt(ie1).getDestination();
    int in4 = nodes.at(in2).getLinkAt(ie2).getDestination();
    int ie3 = nodes.at(in3).getLinkIndex(in1);
    int ie4 = nodes.at(in4).getLinkIndex(in2);
    
    if( ie1==-1 || ie2==-1 || ie3==-1 || ie4==-1 )
        return false;
    
    nodes.at(in1).FlipLink(ie1, d1);
    nodes.at(in2).FlipLink(ie2, d2);
    nodes.at(in3).FlipLink(ie3, in4);
    nodes.at(in4).FlipLink(ie4, in3);
    
    nodes.at(in1).getLinkAt(ie1).setIsNecessary(true);
    nodes.at(in2).getLinkAt(ie2).setIsNecessary(true);
    
    return true;
    
}

const bool Network::linksExist(const int in1, const int d1, const int in2, const int d2) {

    vector<Link> ls1 = nodes.at(in1).getLinks();
    vector<Link> ls2 = nodes.at(in2).getLinks();
    
    for( vector<Link>::iterator itr = ls1.begin(); itr!=ls1.end(); itr++ ){
        if ( (*itr).getDestination()==d1 )
            return true;
    }
    
    for( vector<Link>::iterator itr = ls2.begin(); itr!=ls2.end(); itr++ ){
        if ( (*itr).getDestination()==d2 )
            return true;
    }
    return false;
    
}

double Network::calculateEdgeRobustness(){
    
    // HypotheticalGreedyで探索
    HypotheticalGreedy hg;
    hg.Crawl(nodes, V, SD/2, rand()%V);
    double m = SD/2*(SD/2+1);
    edge_robustness += hg.getFLCC()/m;
    
    return edge_robustness;
}

double Network::calculateNodeRobustness(){
    
    // HypotheticalGreedyで探索
    HypotheticalGreedy hg;
    hg.Crawl(nodes, V, SD/2, rand()%V);
    double m = V*(V+1);
    node_robustness += hg.getFLCC()/m;
    
    return node_robustness;
}

void Network::finalize() {
    E = SD/2;
    
    vector<Node> sorted = nodes;
    sort(sorted.begin(),sorted.end()); // 昇順にソート
    
    averageDegree = 0;
    for( int i=0; i<nodes.size(); i++ ){
        averageDegree += nodes.at(i).getDegree();
    }
    
    medianDegree = sorted.at(sorted.size()/2).getDegree();
    averageDegree /= nodes.size();
    minDegree = sorted.at(0).getDegree();
    maxDegree = sorted.at(sorted.size()-1).getDegree();
}

bool Network::createUDGraph(const double R) {
    
    UDR = R;
    
    // Distance Matrixを埋める
    vector< vector<double> > d(nodes.size(),nodes.size());
    for( int i=0; i<nodes.size(); i++ ){
        for( int j=0; j<nodes.size(); j++ ){
            d.at(i).at(j) = nodes.at(i).getDistance(nodes.at(j));
        }
    }
    
    // Unit Disk Graphの生成
    for( int i=0; i<nodes.size(); i++ ){
        for( int j=0; j<nodes.size(); j++ ){
            if( d.at(i).at(j) <= R && i!=j )
            {
                Link l(i,j);
                nodes.at(i).addUDLink(l);
            }
        }
    }
    
//    setUDHistgram();
    
    return true;
}

void Network::setUDHistgram() {
    for ( int i=0; i<V; i++ ){
        if( maxUDDegree < nodes.at(i).getUDDegree() )
            maxUDDegree = nodes.at(i).getUDDegree();
        if( minUDDegree > nodes.at(i).getUDDegree() )
            minUDDegree = nodes.at(i).getUDDegree();
    }
}

bool Network::flipedLinksAreInsideCircle(const int in1, const int ie1, const int in2, const int ie2){
    
    bool found1 = false;
    bool found2 = false;
    
    int id1 = nodes.at(in1).getLinkAt(ie1).getDestination();
    int id2 = nodes.at(in2).getLinkAt(ie2).getDestination();
    
    found1 = nodes.at(in1).isInsideUD(id2);
    found2 = nodes.at(in2).isInsideUD(id1);
    
    return (found1 && found2);
    
}

double Network::calculateAverageShortestPathLength() {
    double aspl= 0.0;
    int lcc = 0;
    
    vector<double> dist(V);
    
    queue<int> que;
    // 始点を選択
    for( vector<Node>::iterator itr=nodes.begin(); itr!=nodes.end(); itr++ ){
        for( vector<Node>::iterator itn=nodes.begin(); itn!=nodes.end(); itn++ ){
            (*itn).isCrawled = false;
            (*itn).isSeen = false;
            dist.at(itn-nodes.begin()) = INT_MAX;
        }
        
        // 始点に隣接するノードをQueueに挿入
        (*itr).isCrawled = true;
        lcc++;
        dist.at((int)(itr-nodes.begin())) = 0.0;
        vector<Link> links = (*itr).getLinks();
        for( vector<Link>::iterator itl=links.begin(); itl!=links.end(); itl++ ){
            int dst = (*itl).getDestination();
            if( !nodes.at(dst).isSeen ){
                que.push(dst);
                nodes.at(dst).isSeen = true;
                dist.at(dst) = dist.at(itr-nodes.begin()) + 1.0;
                aspl += dist.at(dst);
                lcc++;
            }
        }
        
        // 始点から他の全てのノードへの距離を加算
        while( !que.empty() ){
            int dst = que.front();
            que.pop();
            nodes.at(dst).isCrawled = true;
            
            vector<Link> links = nodes.at(dst).getLinks();
            for( vector<Link>::iterator itl=links.begin(); itl!=links.end(); itl++ ){
                int d = (*itl).getDestination();
                if( !nodes.at(d).isSeen ){
                    que.push(d);
                    nodes.at(d).isSeen = true;
                    dist.at(d) = dist.at(dst) + 1.0;
                    aspl += dist.at(d);
                    lcc++;
                }
            }
        }
    }
    
    aspl = lcc==(V*(V+1)) ? aspl / (double)(V*(V+1)) : INT_MAX;
    
    return aspl;
}

double Network::calculateClusteringCoefficient() {
    
    // 各頂点に関するクラスタリング係数
    vector<double> cev(V);
    for( int i=0; i<V; i++ )
        cev.at(i) = 0.0;
    
    // 隣接行列の初期化
    int **adj = new int*[V];
    for( int i=0; i<V; i++ ){
        adj[i] = new int[V];
        for( int j=0; j<V; j++ ){
            adj[i][j] = 0;
        }
    }
    
    // 隣接行列
    for( int i=0; i<V; i++ ){
        vector<Link> links = nodes.at(i).getLinks();
        for( vector<Link>::iterator itl=links.begin(); itl!=links.end(); itl++ ){
            int j = (*itl).getDestination();
            adj[i][j] = 1;
        }
    }
    
    for( int i=0; i<V; i++ ){
        for( int j=0; j<V; j++ ){
            for( int k=0; k<V; k++ ){
                cev.at(i) += adj[j][k]*adj[i][k]*adj[j][i];
            }
        }
        int k = nodes.at(i).getDegree();
        cev.at(i) /= k>1 ? k*(k-1) : 1;
    }
    
    double acev = 0.0;
    for( int i=0; i<V; i++ ){
        acev += cev.at(i);
    }
    
    // メモリ解放
    for( int i=0; i<V; i++ )
        delete [] adj[i];
    delete [] adj;
    
    
    return acev/(double)V;
}

const bool Network::ConnectTwoNodes(const int in1, const int ie1, const int in2, const int ie2) {
    nodes.at(in1).FlipLink(ie1, in2);
    nodes.at(in2).FlipLink(ie2, in1);
    
    return true;
}


//
//  Crawler.cpp
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/15.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "Crawler.h"

void Crawler::initCrawling(vector<Node> nodes) {
    if( !Vseen.empty() ) Vseen.erase(Vseen.begin(), Vseen.end());
    if( !Vcrawled.empty() ) Vcrawled.erase(Vcrawled.begin(), Vcrawled.end());
    if( !Eseen.empty() ) Eseen.erase(Eseen.begin(), Eseen.end());
    
    for( int i=0; i<nodes.size(); i++ ){
        nodes.at(i).isCrawled = false;
        nodes.at(i).isSeen = false;
    }
    
    memoryInUse.erase(memoryInUse.begin(),memoryInUse.end());
}

void Crawler::showCrawling(string strAlg) {
    
    string strFile;
    ofstream ofs;

    strFile = OFile + "_" + strAlg + ".txt";
    ofs.open(strFile.c_str());
    if( ofs==NULL ) return;
    
    ofs << "|V| = " << V << " ,  |E| = " << E << endl;
    
    ofs << "# of seen nodes" << endl;
    for( int i=0; i<Vseen.size(); i++ ){
        ofs << "nodes\t" << i+1 << "\t" << (double)Vseen[i]/V << endl;
    }
    
    ofs << "# of seen links" << endl;
    for( int i=0; i<Eseen.size(); i++ ){
        ofs << "links\t" << i+1 << "\t" << (double)Eseen[i]/E << endl;
    }
    
    ofs << endl;
    
    ofs << "Crawled Nodes" << endl;
    for( int i=0; i<Vcrawled.size()-1; i++ ){
        ofs << Vcrawled[i] << " -> ";
    }
    ofs << Vcrawled[Vcrawled.size()-1] << endl;
    ofs.close();

}

void Crawler::showCrawling(string strAlg, bool isVisualized){
    string strFile;
    ofstream ofs;
    if( !isVisualized )
        return;

    strFile =  "n" + strAlg + ".txt";
    ofs.open(strFile.c_str());
    if( ofs==NULL ) return;
    for( int i=0; i<Vseen.size(); i++ ){
        ofs << (double)Vseen[i]/V << endl;
    }
    ofs.close();

    strFile = "l" + strAlg + ".txt";
    ofs.open(strFile.c_str());
    if( ofs==NULL ) return;
    for( int i=0; i<Eseen.size(); i++ ){
        ofs << (double)Eseen[i]/E << endl;
    }
    ofs.close();
}

const int Crawler::countVSeen(const bool const isSeen[]) {
    int s = 0;
    for( int i=0; i<V; i++ ){
        if( isSeen[i] ){
            s++;
        }
    }
    return s;
}

const int Crawler::countVSeen(vector<Node> nds){
    int s = 0;
    for( vector<Node>::iterator itr = nds.begin(); itr!=nds.end(); itr++ ) {
        if( (*itr).isSeen ){
            s++;
        }
    }
    return s;
}

const int Crawler::countCurrentESeen(vector<Node> nds, const vector<Link> links) {
    int s = 0;
    for( int i=0; i<links.size(); i++ ){
        int u = links[i].getDestination();
        if( !nds[u].isCrawled ){
            s++;
        }
    }
    return s;
};

const int Crawler::countCurrentESeen(const bool const isCrawled[], const vector<Link> links){
    int s = 0;
    for( int i=0; i<links.size(); i++ ){
        int u = links[i].getDestination();
        if( !isCrawled[u] ){
            s++;
        }
    }
    return s;
}

// IDをindex値に変換
const int Crawler::IDtoIndex(vector<Node> nds, const int nid){

    
    // 二分探索（idが昇順にソートされていない場合には機能しない）
    int min = 0;
    int max = (int)nds.size();
    
    int i = max/2;
    while( max>min ){
        if( nds[i].getNodeID()==nid ){
            return i;
        }
        else if( nds[i].getNodeID()>nid ){
            max = i-1;
        }
        else {
            min = i+1;
        }
        i = (min+max)/2;
    }

    // 線形探索（idが昇順にソートされていない場合には仕方ないので実行する）
    for( int i=0; i<nds.size(); i++ ){
        if( nds[i].getNodeID()==nid )
            return i;
    }
    return -1;
}

void Crawler::showMemoryInUse(string strAlg){
    ofstream ofs(strAlg.c_str());
    if( ofs==NULL )
        return;
    
    for( int i=0; i<memoryInUse.size(); i++ ){
        ofs << memoryInUse[i] << endl;
    }
    ofs.close();
}

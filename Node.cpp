//
//  Node.cpp
//  Crawler
//
//  Created by 長谷川 知里 on 2012/08/27.
//
//

#include "Node.h"

// 削除したリンクの数を返す
const int Node::removeLink(const int nid){

    int sr = 0;
    vector<Link>::iterator itr = links.begin();
    while( itr!=links.end() && !links.empty() ){
        if( (*itr).getDestination()==nid ){
            itr = links.erase(itr);
            degree--;
            sr++;
        } else {
            ++itr;
        }
    }
    
    return sr;
}

void Node::setRandomPosition() {
    double r = rand()/(2.0*(double)RAND_MAX);
    double a = (rand()/(double)RAND_MAX)*2.0*M_PI;
    
    pos[0] = r*cos(a);
    pos[1] = r*sin(a);
    pos[2] = 0;
}

const bool Node::isConnected(int nid){
    for( vector<Link>::iterator itr=links.begin(); itr!=links.end(); itr++ ){
        if( (*itr).getDestination()==nid )
            return true;
    }
    return false;
}

const bool Node::isConnectedAndSetNecessary(int nid) {
    for( vector<Link>::iterator itr=links.begin(); itr!=links.end(); itr++ ){
        if( (*itr).getDestination()==nid ){
            (*itr).setIsNecessary(true);
            return true;
        }
    }
    return false;
}

const bool Node::isInsideUD(const int lid){
    for( vector<Link>::iterator itr=UDLinks.begin(); itr!=UDLinks.end(); itr++ ){
        if( (*itr).getDestination()==lid )
            return true;
    }
    return false;
}

const int Node::getLinkIndex(int n){
    for( vector<Link>::iterator itr=links.begin(); itr!=links.end(); itr++ ){
        if( (*itr).getDestination()==n )
            return (int)(itr-links.begin());
    }
    return -1;
}
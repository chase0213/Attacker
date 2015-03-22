//
//  Node.h
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/07.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef Crawler_Node_h
#define Crawler_Node_h

#include "stdafx.h"
#include "Link.h"

class Node {
private:
    int nodeID;
    string label;
    vector<Link> links;
    int degree;
    double pos[3];
    vector<Link> UDLinks; // links on Unit Disk Graph
    int UDDegree;
    
public:
    Node(const int nID, const vector<Link> ls, const string l){
        nodeID = nID; label = l; links = ls; degree = (int)ls.size(); isSeen=isCrawled=false;
    };
    Node(const int nID, const string l){
        nodeID = nID; label = l; degree = 0; UDDegree = 0; isSeen=isCrawled=false;
    };
    const int getNodeID() const { return nodeID; };
    const int getDegree() const { return degree; };
    const int getUDDegree() const { return UDDegree; };
    void setDegree(const int n){ degree = n; };
    void setPosition(const int x, const int y, const int z){ pos[0]=x; pos[1]=y; pos[2]=z; };
    void setRandomPosition();
    double* getPosition(){ return pos; };
    double getPosition(const int p ){ return pos[p]; };
    double getDistance(const Node n){
        return sqrt((n.pos[0]-pos[0])*(n.pos[0]-pos[0])
                    +(n.pos[1]-pos[1])*(n.pos[1]-pos[1])
                    +(n.pos[2]-pos[2])*(n.pos[2]-pos[2]));
    };
    // idx番目のリンクにおいて、destinationをdstにフリップする
    void FlipLink(const int idx, const int dst){ links.at(idx).FlipLink(dst); };
    void decreaseDegree(){ degree--; };
    const vector<Link> getLinks(){ return links; };
    const vector<Link> getUDLinks(){ return UDLinks; };
    Link getLinkAt(int linkID){ return links[linkID]; };
    const Link getUDLinkAt(int linkID){ return UDLinks[linkID]; };
    void addLink( const Link l ){ links.push_back(l); degree++; };
    void addUDLink( const Link l ){ UDLinks.push_back(l); UDDegree++; };
    void sortLinksWithID(){ sort( links.begin(), links.end() ); };
    void sortUDLinksWithID(){ sort( UDLinks.begin(), UDLinks.end() ); };
    const int removeLink( const int nid );
    void eraseLink(){ links.erase(links.begin(), links.end()); };
    const bool isConnected(int nid);
    const bool isConnectedAndSetNecessary(int nid);
    const bool isInsideUD(const int lid);
    const int getLinkIndex(int n);
    bool operator>(Node n) const { return degree > n.getDegree(); };
    bool operator<(Node n) const { return degree < n.getDegree(); };
    
    // publicメンバ変数
    bool isSeen;
    bool isCrawled;
        
};

#endif

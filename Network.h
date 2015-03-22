//
//  Network.h
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/07.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef Crawler_Network_h
#define Crawler_Network_h

#include "stdafx.h"
#include "resource.h"
#include "Node.h"
#include "NodeWithDegree.h"
#include "_ptr_node.h"

class Network {
    
    // Member variables
private:
    vector<Node> nodes;
    int V;
    int SD; // sum of degrees
    int E;
    int rV;
    double UDR; // Radius of Unit Disk Graph
    int maxDegree;
    int minDegree;
    int maxUDDegree;
    int minUDDegree;
    int averageDegree;
    int medianDegree;
    const void degreeHistgram();
    const void degreeHistgram(string filename);
    const void UDDegreeHistgram();
    const int findUncrawledNode(vector<Node>);
    const bool linksExist(const int ,const int, const int, const int);
    double efficiency;
    double node_robustness;
    double edge_robustness;
    
    // Methods
public:
    Network(vector<Node> ns);
    Network(){ V=0; SD=0; rV=0; E=0; maxDegree = maxUDDegree = 0; minDegree = minUDDegree = INT_MAX; };
    const void showNetwork();
    const void showUDNetwork();
    const void showNetwork(string);
    const void showNetwork(string filename,string order);
    const vector<Node> getNodes() const {return nodes;};
    void addNode( Node n ){ nodes.push_back(n); V++; rV++; };
    void addLinkToNode( const Link l, const int nid ){ nodes[nid].addLink(l); SD++; };
    void finalize();
    bool createUDGraph(const double R);
    bool flipedLinksAreInsideCircle(const int in1, const int ie1, const int in2, const int ie2);
    const bool FlipLinks(const int in1, const int ie1, const int in2, const int ie2);
    const bool SwapLinksIN1toIN2( const int in1, const int in2, const int ie1, const int ie2);
    const bool SwapLinksIN1toIN2AllowedDoubleEdge( const int in1, const int in2, const int ie1, const int ie2);
    const bool ConnectTwoNodes( const int in1, const int ie1, const int in2, const int ie2 );
    const int getV() const { return V; };
    const int getE() const { return SD/2; };
    const int getRV() const { return rV; };
    void setMaxDegree(const int deg) { maxDegree = deg; };
    void setMinDegree(const int deg) { minDegree = deg; };
    void setMaxUDDegree(const int deg) { maxUDDegree = deg; };
    void setMinUDDegree(const int deg) { minUDDegree = deg; };
    void sortNodesByDegrees() { sort(nodes.begin(),nodes.end(),greater<Node>()); };
    int getMaxDegree(){ return maxDegree; };
    int getMinDegree(){ return minDegree; };
    int getMaxUDDegree(){ return maxUDDegree; };
    int getMinUDDegree(){ return minUDDegree; };
    int getAverageDegree(){ return averageDegree; };
    int getMedianDegree(){ return medianDegree; };
    const void removeNode(int nid);
    const void removeEdgeAtNode(int lid, int nid);
    const double getNodeRobustness(){ return node_robustness; };
    const double getEdgeRobustness(){ return edge_robustness; };
    void setNodeRobustness(double r){ node_robustness = r; };
    void setEdgeRobustness(double r){ edge_robustness = r; };
    void setUDHistgram();
    double calculateNodeRobustness();
    double calculateEdgeRobustness();
    double calculateAverageShortestPathLength();
    double calculateClusteringCoefficient();
};

#endif

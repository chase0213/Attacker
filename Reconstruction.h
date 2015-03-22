//
//  Reconstruction.h
//  Attacker
//
//  Created by 長谷川 知里 on 2012/09/06.
//  Copyright (c) 2012年 C. HASEGAWA. All rights reserved.
//

#ifndef __Attacker__Reconstruction__
#define __Attacker__Reconstruction__

#include "Network.h"

const int ITR_LIMIT = 10000;


class DDISTRIBUTION {
public:
    int degree; // degree
    int n; // # of nodes whose degrees are "degree"
    
    DDISTRIBUTION(const int d, const int num){ degree=d; n=num; };
};

class Reconstruction {
private:
    Network *np;
    double cost;
    stack<Link> outerEdges;
    
    void FindNonEmptyDegrees(vector<DDISTRIBUTION> dd, int *di, int *dj, int *dk, int *dm, int *dl, int r);
    void FindNonEmptyDegrees(vector<DDISTRIBUTION> dd, int *di, int *dj, int *dk, int r);
    void MakeConnection(int dHistgram[], vector<Node> sorted, int di, int dj, int dk, int dm, int dl);
    void MakeConnection(int dHistgram[], vector<Node> sorted, int di, int dj, int dk);
    void MakeConnection(vector<Node> sorted, int bk, int d);
    int ConnectWithLowers(vector<Node> sorted, int bk, int d);
    void ConnectInside(vector<Node> sorted, int bk, int d, int next);
    void FinalizeNetworkConnectingRandomly();
    
    int addOne(vector<int>*);
    const bool isValidCombination(vector<Node>,vector<int>);
    const bool isValidCombination(vector<Node>,int,vector<int>);
    int *MakeRandomPermutation(int i, int n);

public:
    Reconstruction(Network *net){ np = net; };
    // sel: 1-node 2-edge 3-both
    Network InterHubDisconnectedForBA(const int nh); // nh : #hubs
    void SwapInterClusters(const int nc, const int bias, const int nmin, const int nmax); // nc : #clusters
    void GreedyFlip(const int sel);
    void LocalGreedy(const int sel);
    void LocalSelectiveGreedy(const int sel);
    void antiGreedyOptimization(const int sel);
    void antiHGAOptimization(const int sel);
    const double getReconstructionCost();
    int* random_perm(int nk);
};


class LinkWithDifference {
public:
    Link link;
    int diff;
    
    LinkWithDifference(const int s, const int t, const int d){
        link.setSource(s); link.setDestination(t); link.setFliped(false); diff = d;
    };
    
    bool operator>(LinkWithDifference l) const { return diff > l.diff; };
    bool operator<(LinkWithDifference l) const { return diff < l.diff; };
};

class FELEMENT {
public:
    double d;
    int eid;

    bool operator>(FELEMENT fe) const { return d > fe.d; };
    bool operator<(FELEMENT fe) const { return d < fe.d; };
};

#endif /* defined(__Attacker__Reconstruction__) */

//
//  NodeWithDegree.h
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/12.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef Crawler_NodeWithDegree_h
#define Crawler_NodeWithDegree_h

class NodeWithDegree {
private:
    int nodeID;
    int degree;
    
public:
    NodeWithDegree(int nid, int dgr){ nodeID = nid; degree = dgr; };
    const int getNodeID() const { return nodeID; };
    const int getDegree() const { return degree; };
    void decreaseDegree(){ degree--; };
    bool operator <(NodeWithDegree nwd) const { return degree < nwd.degree; };
    bool operator >(NodeWithDegree nwd) const { return degree > nwd.degree; };
};

#endif

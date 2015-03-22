//
//  AttackNode.h
//  Crawler
//
//  Created by 長谷川 知里 on 2012/08/26.
//
//

#ifndef Crawler_AttackNode_h
#define Crawler_AttackNode_h

#include "Network.h"

class AttackNode {
    
    Network *np;

public:
    AttackNode(Network *net){ np = net; (*net).setNodeRobustness(0.0); };
    void RandomAttack();
    void GreedyAttack();
    void HighDegreeAdaptive();
    void HighCentralityAdaptive();
};

#endif

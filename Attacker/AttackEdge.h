//
//  AttackEdge.h
//  Attacker
//
//  Created by 長谷川 知里 on 2012/09/10.
//  Copyright (c) 2012年 C. HASEGAWA. All rights reserved.
//

#ifndef __Attacker__AttackEdge__
#define __Attacker__AttackEdge__

#include "Network.h"

class AttackEdge {
    
    Network *np;
    
public:
    AttackEdge(Network *net){ np = net; (*net).setEdgeRobustness(0.0); };
    void RandomAttack();
    void GreedyAttack();
    void HighDegreeAdaptive();
    void HighCentralityAdaptive();
};

#endif /* defined(__Attacker__AttackEdge__) */

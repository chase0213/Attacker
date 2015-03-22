//
//  _ptr_node.h
//  Attacker
//
//  Created by 長谷川 知里 on 2012/11/21.
//  Copyright (c) 2012年 C. HASEGAWA. All rights reserved.
//

#ifndef __Attacker___ptr_node__
#define __Attacker___ptr_node__

#include "Node.h"

class _ptr_node {
private:
    
public:
    _ptr_node(Node *n){ pnode = n; };
    Node* pnode;
    
    const bool operator<(_ptr_node n) const { return *pnode < *(n.pnode); };
    const bool operator>(_ptr_node n) const { return *pnode > *(n.pnode); };
    
};

#endif /* defined(__Attacker___ptr_node__) */

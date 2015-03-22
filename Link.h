//
//  Link.h
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/07.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef Crawler_Link_h
#define Crawler_Link_h

#include "stdafx.h"

class Link {
    // Memeber variables
private:
    int source;
    int destination;
    bool fliped;
    bool necessary;
    
    // Methods
public:
    Link(){;};
    Link(const int s,const int d){ source = s; destination = d; fliped = false; necessary = false; };
    int getSource() const { return source; };
    int getDestination() const { return destination; };
    bool hasBeenFliped() const { return fliped; };
    bool isNecessary() const { return necessary; };
    const void setFliped(const bool f){ fliped = f; };
    const void setIsNecessary(const bool n){ necessary = n; };
    const void setSource(const int s){ source = s; };
    const void setDestination(const int d){ destination = d; };
    bool operator<(Link l) const { return destination < l.destination; };
    void FlipLink(const int dst){ destination = dst; fliped = true; necessary = true; };
};


#endif

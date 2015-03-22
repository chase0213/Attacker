//
//  resource.h
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/07.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef Crawler_resource_h
#define Crawler_resource_h

#include "stdafx.h"

const int Rd = 0;
const int Rl = 1;

const bool isCreatedByBAModel = true;
const bool directed = false;
//const string NSFile = "Created.gml";
//const string NSFile = "graph_created_ER10000.gml";
//const string NSFile = "CC10000_m3-30.gml";
//const string NSFile = "ER10000.gml";
const string NSFile = "RH10000_m4t6.gml";
//const string NSFile = "gplus_6.edges";
//const string NSFile = "107.edges";
//const string NSFile = "test.gml"; // note that this graph is NOT simple
//const string NSFile = "cond-mat-2005.gml"; // 0 - not connected
//const string NSFile = "adjnoun.gml"; // 0
//const string NSFile = "as-22july06.gml"; // 0
//const string NSFile = "astro-ph.gml"; // 0
//const string NSFile = "netscience.gml"; // 0
const int offset = 0; // if began from 0, offset = 0, otherwise offset = 1

const string OFile = "RES_" + NSFile.substr(0,NSFile.length()-4);


#endif

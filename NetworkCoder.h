//
//  NetworkCoder.h
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/13.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef Crawler_NetworkCoder_h
#define Crawler_NetworkCoder_h

#include "Network.h"

class NetworkCoder {
private:
    ifstream ifs;
    int sum;
    
    void addNode(Network* net);
    void addEdge(Network* net);
    const Network decodeNetworkGML(const string file);
    const Network decodeNetworkDAT(const string file);
    const Network decodeNetworkTXT(const string file);
    const Network decodeNetworkEDGES(const string file);
    void setHistgram(Network *net);
    
    int findUpperBoundIndex(vector<int> *dps, const int rnd);
    void renewPartialSum(vector<int> *dps, const int e, const int m);

    void createBAModel(Network *net, const int N, const int m, const int idBias);
    
public:
    NetworkCoder(){;};
    
    Network createERModel(const int N, const double p);
    Network createBAModel(const int N, const int m);
    Network connectBAModel(const int N, const int m, const int itr);
    Network createHKModel(const int N, const int m, const double p);
    Network createRHModel(const int N0, const int T);
    Network createConnectedClique(const int N_div_p, const int p, const int m_max, const int m_min);
    
    // 引数で指定されたネットワークをエンコードしてその結果をファイルに書き出し、ファイル名を返す
    const string encodeNetwork(const Network net, const string date);
    const string encodeNetwork(const Network net, const string fname, const string date);
    
    
    // 引数で指定されたファイルを読み込み、ネットワークの形式にデコードして返す
    const Network decodeNetwork(const string file);
};

#endif

//
//  main.cpp
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/07.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "Network.h"
#include "NetworkCoder.h"
#include "resource.h"
#include "BFS.h"
#include "DFS.h"
#include "HypotheticalGreedy.h"
#include "HighDegreeAdaptive.h"
#include "ApparentLottery.h"
#include "BiassedRandomWalk.h"
#include "AttackNode.h"
#include "Reconstruction.h"
#include "Rearrange.h"
#include <ctime>


// テスト
//#define TEST

// 予備実験用
//#define SUB_EX

// ネットワーク係数計算用
//#define CALC_NC

// 探索実行
//#define EX_CRAWL

// 実行時間計算用
#define EX_TIME


#include "Learning.h"

//Network loadNetwork();



int main(int argc, const char * argv[])
{

#ifdef SUB_EX // 予備実験用
    sub_experiment();
#endif
    
#ifdef TEST // テスト用
    test_function();
#endif
    
#ifdef CALC_NC
    calc_network_coefficients();
#endif
    
#ifdef EX_CRAWL // 探索用
    execute_Crawl();
#endif
    
#ifdef EX_TIME
    execute_Time_Recording();
#endif

    
    return 0;
}

void execute_Time_Recording(){
    NetworkCoder nc;
    Network net;
    Network cnet;
    clock_t start, end;
    
    string fsTime = "time_consumed.txt";
    ofstream ofs(fsTime.c_str());
    
//    const int N = 5000;
//    const int M = 100000;
//    for( int i=0; i<20; i++ ){
//        net = nc.createBAModel(N, 5*i+3);
//        stringstream ss;
//        ss << "BA5000_m" << 5*i+3 << ".gml";
//        nc.encodeNetwork(net, ss.str(), "20130209");
//    }
//    
//    int n = 1000;
//    for( int i=0; i<20; i++ ){
//        int m = (2*n-1-sqrt((2*n-1)*(2*n-1)-8*M))/2;
//        net = nc.createBAModel(n, m);
//        stringstream ss;
//        ss << "BA" << n << "_" << m << ".gml";
//        nc.encodeNetwork(net, ss.str(), "20130209");
//        n += 100;
//    }
    
    Network original_net;
    
    ofs << "E BFS DFS HG HDA BRW BRWonly" << endl;
    
    for( int i=0; i<20; i++ ){
        stringstream ss;
        int m = 3+i;
        ss << "BA1000_m" << m << ".gml";
        original_net = nc.decodeNetwork(ss.str());
        cnet = original_net;
        
        // 探索シードの決定
        srand((int)time(NULL));
        int seed = rand()%original_net.getV();
        
        //    original_net.showNetwork("original"+NSFile);
        cout << "The network " << ss.str() << " has completely read." << endl;
        
        int e = m*(m-1)/2 + m*(1000-m);
        ofs << e << " ";
        
        // フラグを折るために一度上書きする
        original_net = cnet;
        start = clock();
        BFS bfs(original_net,seed);
        end = clock();
        cout << "BFS took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
        ofs << (end-start)/(double)CLOCKS_PER_SEC << " ";
        
        original_net = cnet;
        start = clock();
        DFS dfs(original_net,seed);
        end = clock();
        cout << "DFS took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
        ofs << (end-start)/(double)CLOCKS_PER_SEC << " ";
        
        original_net = cnet;
        start = clock();
        HypotheticalGreedy hg(original_net,seed);
        end = clock();
        cout << "HG took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
        ofs << (end-start)/(double)CLOCKS_PER_SEC << " ";
        
        original_net = cnet;
        start = clock();
        HighDegreeAdaptive hda(original_net,seed);
        end = clock();
        cout << "HDA took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
        ofs << (end-start)/(double)CLOCKS_PER_SEC << " ";
        
        for( int r=0; r<2; r++ ){
            ostringstream oss;
            oss << "BRW";
            if( r==1 )
                oss << "only";
            double ratio = 0.80;
            double dT = original_net.getMaxDegree() * ratio;
            double a = (log(dT)/log((double)original_net.getV()));
            double eps = 0.000001+r;
            
            start = clock();
            BiassedRandomWalk brw(original_net,seed,a,2.0/3.0,eps); // a, b, epsilon
            end = clock();
            cout << "BRW took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
            ofs << (end-start)/(double)CLOCKS_PER_SEC << " ";
        }
        
        ofs << endl;
    }
    
    for( int i=0; i<20; i++ ){
        stringstream ss;
        int m = 5*i+3;
        ss << "BA1000_m" << m << ".gml";
        original_net = nc.decodeNetwork(ss.str());
        cnet = original_net;
        
        // 探索シードの決定
        srand((int)time(NULL));
        int seed = rand()%original_net.getV();
    
        //    original_net.showNetwork("original"+NSFile);
        cout << "The network " << ss.str() << " has completely read." << endl;
    
        int e = m*(m-1)/2 + m*(1000-m);
        ofs << e << " ";
        
        // フラグを折るために一度上書きする
        original_net = cnet;
        start = clock();
        BFS bfs(original_net,seed);
        end = clock();
        cout << "BFS took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
        ofs << (end-start)/(double)CLOCKS_PER_SEC << " ";
        
        original_net = cnet;
        start = clock();
        DFS dfs(original_net,seed);
        end = clock();
        cout << "DFS took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
        ofs << (end-start)/(double)CLOCKS_PER_SEC << " ";
    
        original_net = cnet;
        start = clock();
        HypotheticalGreedy hg(original_net,seed);
        end = clock();
        cout << "HG took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
        ofs << (end-start)/(double)CLOCKS_PER_SEC << " ";
    
        original_net = cnet;
        start = clock();
        HighDegreeAdaptive hda(original_net,seed);
        end = clock();
        cout << "HDA took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
        ofs << (end-start)/(double)CLOCKS_PER_SEC << " ";
    
        for( int r=0; r<2; r++ ){
            ostringstream oss;
            oss << "BRW";
            if( r==1 )
                oss << "only";
            double ratio = 0.80;
            double dT = original_net.getMaxDegree() * ratio;
            double a = (log(dT)/log((double)original_net.getV()));
            double eps = 0.000001+r;
        
            start = clock();
            BiassedRandomWalk brw(original_net,seed,a,2.0/3.0,eps); // a, b, epsilon
            end = clock();
            cout << "BRW took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
            ofs << (end-start)/(double)CLOCKS_PER_SEC << " ";
        }
        
        ofs << endl;
    }
    
    /*
    for( int i=0; i<20; i++ ){
        stringstream ss;
        
        int m = (2*n-1-sqrt((2*n-1)*(2*n-1)-8*M))/2;
        ss << "BA" << n << "_" << m << ".gml";
        n += 100;
        
        nc.decodeNetwork(ss.str());
        
        // 探索シードの決定
        srand((int)time(NULL));
        int seed = rand()%original_net.getV();
        
        //    original_net.showNetwork("original"+NSFile);
        cout << "The network " << ss.str() << " has completely read." << endl;
        
        ofs << ss.str() << endl;
        
        // フラグを折るために一度上書きする
        original_net = cnet;
        start = clock();
        BFS bfs(original_net,seed);
        end = clock();
        cout << "BFS took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
        ofs << "BFS took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
        
        original_net = cnet;
        start = clock();
        DFS dfs(original_net,seed);
        end = clock();
        cout << "DFS took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
        ofs << "DFS took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
        
        original_net = cnet;
        start = clock();
        HypotheticalGreedy hg(original_net,seed);
        end = clock();
        cout << "HG took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
        ofs << "HG took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
        
        original_net = cnet;
        start = clock();
        HighDegreeAdaptive hda(original_net,seed);
        end = clock();
        cout << "HDA took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
        ofs << "HDA took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
        
        for( int r=0; r<2; r++ ){
            ostringstream oss;
            oss << "BRW";
            if( r==1 )
                oss << "only";
            double ratio = 0.80;
            double dT = original_net.getMaxDegree() * ratio;
            double a = (log(dT)/log((double)original_net.getV()));
            double eps = 0.000001+r;
            
            start = clock();
            BiassedRandomWalk brw(original_net,seed,a,2.0/3.0,eps); // a, b, epsilon
            end = clock();
            cout << "BRW took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
            ofs << "BRW took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
        }
    }
    */
    
    ofs.close();
    
}

void calc_network_coefficients(){
    
    NetworkCoder nc;
    
    vector<string> filenames;
//    filenames.push_back("BA10000_m20.gml");
//    filenames.push_back("BA10000_m50.gml");
//    filenames.push_back("SBA10000_m20.gml");
//    filenames.push_back("SBA10000_50.gml");
//    filenames.push_back("0.edges");
//    filenames.push_back("107.edges");

    const int N = 10000;
    const int m = 50;
    const double p = (2*N-m-1)*m/(double)(N*(N-1));
    
//    Network net = nc.createERModel(N, p);
//    nc.encodeNetwork(net, "RH16384_N04_T6.gml", "20120128");
    
    filenames.push_back(NSFile);

//    net = nc.createHKModel(10000, 50, 0.5);
//    nc.encodeNetwork(net, "HK10000_m50p50.gml", "20130127");
//    filenames.push_back("HK10000_m50p50.gml");
//    
//    net = nc.createHKModel(10000, 50, 0.8);
//    nc.encodeNetwork(net, "HK10000_m50p80.gml", "20130127");
//    filenames.push_back("HK10000_m50p80.gml");
    
    vector<string>::iterator itr = filenames.begin();
    while( itr!=filenames.end() ){
        Network net = nc.decodeNetwork((*itr));
        net.showNetwork((*itr));
        cout << "----- " << (*itr) << " -----" << endl;
        itr++;
    }
    
}

void showTimeForEachAlgorithm(){
    
}


void test_function(){
    
    NetworkCoder nc;
    Network network = nc.createRHModel(22, 2);
    nc.encodeNetwork(network, NSFile, "20130207");
    network.showNetwork(NSFile);
}

void sub_experiment(){
    NetworkCoder nc;
    ofstream ofs;
    clock_t start, end;
    
    // 探索シードの決定
    srand((int)time(NULL));
    //    int seed = rand()%original_net.getV();

    Network original_net = nc.decodeNetwork(NSFile);
    Network cnet = original_net;

//    original_net.showNetwork("original"+NSFile);
    cout << "The network has completely read." << endl;
    
    double ratio;
    double dT;
    double a;
    double eps = 0.000001;
    
    for( int i=1; i<20; i++ ){
        
        ratio = 1.0 - 0.05*i;
        dT = original_net.getMaxDegree() * ratio;
        a = (log(dT)/log((double)original_net.getV()));
        
        ostringstream oss;
        oss << "SE_BA100000" << "_a" << a;
//        original_net = nc.decodeNetwork(oss.str());
        int seed = rand()%original_net.getV();
        start = clock();
        BiassedRandomWalk brw(original_net,seed,a,2.0/3.0,eps); // a, b, epsilon
        brw.showCrawling(oss.str(),true);
        end = clock();
        cout << "BRW took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
        
    }
    
}

void execute_Crawl(){
    NetworkCoder nc;
    ofstream ofs;
    clock_t start, end;
    
    const int N = 10000;
    const int m = 50;
    const double p = (2*N-m-1)*m/(double)(N*(N-1));
//    Network original_net = nc.createERModel(N, p);
//    nc.encodeNetwork(original_net, "ER10000.gml", "20120124");
    
//    Network original_net = nc.createConnectedClique(1000, 10, 3, 30);
    Network original_net = nc.decodeNetwork(NSFile);
    nc.encodeNetwork(original_net, NSFile+".gml", "20130205");
    original_net.showNetwork(NSFile);
    
    // SBA作成用
//    Network original_net = nc.connectBAModel(N, m, 10);
//    nc.encodeNetwork(original_net, "20130121");
//    Reconstruction rn(&original_net);
//    rn.SwapInterClusters(10, 1000, 3, 5);
    
//    Network original_net = nc.decodeNetwork(NSFile);
//    nc.encodeNetwork(original_net, "ER10000", "20120124");

//    Network original_net = nc.createERModel(30000, 0.001);
//    Network original_net = nc.createBAModel(N, m);
//    nc.encodeNetwork(original_net, NSFile, "20130118");
//    Network original_net = nc.decodeNetwork(NSFile);

    
    Network cnet = original_net;
    
    
//    cout << "Average Shortest Path Length = " << original_net.calculateAverageShortestPathLength() << endl;
//    cout << "Clustering Coefficient = " << original_net.calculateClusteringCoefficient() << endl;
    
    
    
    // 探索シードの決定
    srand((int)time(NULL));
    int seed = rand()%original_net.getV();
    
//    original_net.showNetwork("original"+NSFile);
    cout << "The network has completely read." << endl;
    
    // フラグを折るために一度上書きする
    original_net = cnet;
    start = clock();
    BFS bfs(original_net,seed);
    end = clock();
    cout << "BFS took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
    bfs.showCrawling("BFS",true);
    bfs.showMemoryInUse("mBFS.txt");
    
    original_net = cnet;
    start = clock();
    DFS dfs(original_net,seed);
    end = clock();
    cout << "DFS took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
    dfs.showCrawling("DFS",true);
    dfs.showMemoryInUse("mDFS.txt");
    
    original_net = cnet;
    start = clock();
    HypotheticalGreedy hg(original_net,seed);
    end = clock();
    cout << "HG took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
    hg.showCrawling("HG",true);
    hg.showMemoryInUse("mHG.txt");

    original_net = cnet;
    start = clock();
    HighDegreeAdaptive hda(original_net,seed);
    end = clock();
    cout << "HDA took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
    hda.showCrawling("HDA",true);
    hda.showMemoryInUse("mHDA.txt");

    for( int i=0; i<2; i++ ){
        ostringstream oss;
//        oss << "BRW_" << i << ".txt";
        oss << "BRW";
        if( i==1 )
            oss << "only";
        double ratio = 0.80;
        double dT = original_net.getMaxDegree() * ratio;
        double a = (log(dT)/log((double)original_net.getV()));
        double eps = 0.000001+i;
        
//    ostringstream oss;
//    oss << "graph_created_BA" << 10000 << ".gml";
//    original_net = nc.decodeNetwork(oss.str());
        start = clock();
        BiassedRandomWalk brw(original_net,seed,a,2.0/3.0,eps); // a, b, epsilon
        end = clock();
        cout << "BRW took " << (end-start)/(double)CLOCKS_PER_SEC << endl;
        brw.showCrawling(oss.str(),true);
        brw.showMemoryInUse("m"+oss.str()+".txt");
//    brw.showCrawling(oss.str(),true);
    }
    
    // データをまとめる
    Rearrange rearrange("MemoryInUse.txt",6,original_net.getV());
}



void execute_Reconstruction(){
    NetworkCoder nc;
    ofstream ofs;
    //    clock_t start, end;
    
//        Network original_net = nc.createERModel(50, 0.03);
    Network original_net = nc.createBAModel(500, 3);
    //    Network original_net = nc.decodeNetwork(NSFile);
    
    nc.encodeNetwork(original_net, "2012/11/05 19:00");
    original_net.showNetwork("_Original");
    
    cout << "Average Shortest Path Length = " << original_net.calculateAverageShortestPathLength() << endl;
    cout << "Clustering Coefficient = " << original_net.calculateClusteringCoefficient() << endl;

    Network cnet = original_net;
    AttackNode an(&cnet);
    
    an.RandomAttack();
    cout << "The Robustness Against Random Attack is " << cnet.getNodeRobustness() << endl;
    
    cnet = original_net;
    an.GreedyAttack();
    cout << "The Robustness Against Greedy Attack is " << cnet.getNodeRobustness() << endl;
    
    cnet = original_net;
    an.HighDegreeAdaptive();
    cout << "The Robustness Against High Degree Adaptive Attack is " << cnet.getNodeRobustness() << endl;
    
    
    Network reconstructed = original_net;
    Reconstruction rn(&reconstructed);

    
    // ここから Anti-Greedy
    
    string rALG = "antiGreedyAttack";
    
    ofs.open((OFile+rALG+".txt").c_str(),ios::app);
    
    clock_t start_aga, end_aga;
    
    reconstructed = original_net;
    start_aga = clock();
    rn.antiGreedyOptimization(1);
    end_aga = clock();
    long sec_aga = (double)(end_aga-start_aga)/CLOCKS_PER_SEC;
    cnet = reconstructed;
    
    cout << "Anti-Greedy Attack Reconstruction (" << sec_aga << " [s])" << endl;
    cout << "Average Shortest Path Length = " << reconstructed.calculateAverageShortestPathLength() << endl;
    cout << "Clustering Coefficient = " << reconstructed.calculateClusteringCoefficient() << endl;
    
    //        cout << "Reconstruction completed" << endl;
    //        cout << "Reconstruction took " << sec_aga/3600 << ":" << (sec_aga%3600)/60 << ":" << sec_aga%3600%60 << endl;
    
    an.RandomAttack();
    ofs << "The Robustness Against Random Attack is " << cnet.getNodeRobustness() << endl;
    cout << "The Robustness Against Random Attack is " << cnet.getNodeRobustness() << endl;
    
    cnet = reconstructed;
    an.GreedyAttack();
    ofs << "The Robustness Against Greedy Attack is " << cnet.getNodeRobustness() << endl;
    cout << "The Robustness Against Greedy Attack is " << cnet.getNodeRobustness() << endl;
    
    
    cnet = reconstructed;
    an.HighDegreeAdaptive();
    ofs << "The Robustness Against High Degree Adaptive Attack is " << cnet.getNodeRobustness() << endl;
    cout << "The Robustness Against High Degree Adaptive Attack is " << cnet.getNodeRobustness() << endl;
    
    
    ofs.close();
    
    reconstructed.showNetwork(rALG.c_str(),"des");
    
    
    
    // ここから Anti-HDA
     
     rALG = "antiHDAAttack";
    
    ofs.open((OFile+rALG+".txt").c_str(),ios::app);
    
    clock_t start_ahda, end_ahda;
    
    reconstructed = original_net;
    start_ahda = clock();
    rn.antiHGAOptimization(1);
    end_ahda = clock();
    long sec_ahda = (double)(end_ahda-start_ahda)/CLOCKS_PER_SEC;
    cnet = reconstructed;
    
    cout << "Anti-HGA Attack Reconstruction (" << sec_ahda << " [s])" << endl;
    cout << "Average Shortest Path Length = " << reconstructed.calculateAverageShortestPathLength() << endl;
    cout << "Clustering Coefficient = " << reconstructed.calculateClusteringCoefficient() << endl;
    
    an.RandomAttack();
    ofs << "The Robustness Against Random Attack is " << cnet.getNodeRobustness() << endl;
    cout << "The Robustness Against Random Attack is " << cnet.getNodeRobustness() << endl;
    
    cnet = reconstructed;
    an.GreedyAttack();
    ofs << "The Robustness Against Greedy Attack is " << cnet.getNodeRobustness() << endl;
    cout << "The Robustness Against Greedy Attack is " << cnet.getNodeRobustness() << endl;
    
    
    cnet = reconstructed;
    an.HighDegreeAdaptive();
    ofs << "The Robustness Against High Degree Adaptive Attack is " << cnet.getNodeRobustness() << endl;
    cout << "The Robustness Against High Degree Adaptive Attack is " << cnet.getNodeRobustness() << endl;
    
    ofs.close();
    
    reconstructed.showNetwork(rALG.c_str(),"des");
    
    /************ ここから Greedy Flip ************/
    
    /*
     
     clock_t start_gf, end_gf;
     
     original_net = nc.createBAModel(150, 3);
     
     reconstructed = original_net;
     start_gf = clock();
     rn.GreedyFlip(1);
     end_gf = clock();
     long sec_gf = (double)(end_gf-start_gf)/CLOCKS_PER_SEC;
     cnet = reconstructed;
     
     cout << "Reconstruction took " << sec_gf/3600 << ":" << (sec_gf%3600)/60 << ":" << sec_gf%3600%60 << endl;
     
     reconstructed.showNetwork("RandomizedGreedy_BA");
     
     
     original_net = nc.createERModel(150, 0.03);
     
     reconstructed = original_net;
     start_gf = clock();
     rn.GreedyFlip(1);
     end_gf = clock();
     sec_gf = (double)(end_gf-start_gf)/CLOCKS_PER_SEC;
     cnet = reconstructed;
     
     cout << "Reconstruction took " << sec_gf/3600 << ":" << (sec_gf%3600)/60 << ":" << sec_gf%3600%60 << endl;
     
     reconstructed.showNetwork("RandomizedGreedy_ER");
     
     */
    
    /*
     
     ofs.open((OFile+"RandomizedGreedy"+".txt").c_str(),ios::app);
     ofs << "Greedy Flip Reconstruction took " << sec_gf << endl;
     //    ofs << "The cost of the reconstruction is " << rn.getReconstructionCost() << endl;
     
     cout << "Greedy Swap Reconstruction" << endl;
     cout << "Average Shortest Path Length = " << reconstructed.calculateAverageShortestPathLength() << endl;
     cout << "Clustering Coefficient = " << reconstructed.calculateClusteringCoefficient() << endl;
     
     an.RandomAttack();
     ofs << "The Robustness Against Random Attack is " << cnet.getNodeRobustness() << endl;
     cout << "The Robustness Against Random Attack is " << cnet.getNodeRobustness() << endl;
     
     cnet = reconstructed;
     an.GreedyAttack();
     ofs << "The Robustness Against Greedy Attack is " << cnet.getNodeRobustness() << endl;
     cout << "The Robustness Against Greedy Attack is " << cnet.getNodeRobustness() << endl;
     
     cnet = reconstructed;
     an.HighDegreeAdaptive();
     ofs << "The Robustness Against High Degree Adaptive Attack is " << cnet.getNodeRobustness() << endl;
     cout << "The Robustness Against High Degree Adaptive Attack is " << cnet.getNodeRobustness() << endl;
     
     
     ofs.close();
     
     */
    
    
    /************ ここから Local Greedy ************/
    
    
    /*
     
     clock_t start_lgf, end_lgf;
     
     net = cnet;
     start_lgf = clock();
     rn.LocalGreedy(1);
     end_lgf = clock();
     long sec_lgf = (double)(end_lgf-start_lgf)/CLOCKS_PER_SEC;
     cnet = net;
     
     cout << "Reconstruction completed" << endl;
     cout << "Reconstruction took " << sec_lgf/3600 << ":" << (sec_lgf%3600)/60 << ":" << sec_lgf%3600%60 << endl;
     
     net.showNetwork("LocalGreedy");
     
     ofs.open((OFile+"LocalGreedy"+".txt").c_str(),ios::app);
     ofs << "Greedy Flip Reconstruction took " << sec_lgf << endl;
     ofs << "The cost of the reconstruction is " << rn.getReconstructionCost() << endl;
     
     an.RandomAttack();
     ofs << "The Robustness Against Random Attack is " << net.getNodeRobustness() << endl;
     
     net = cnet;
     an.GreedyAttack();
     ofs << "The Robustness Against Greedy Attack is " << net.getNodeRobustness() << endl;
     
     net = cnet;
     an.HighDegreeAdaptive();
     ofs << "The Robustness Against High Degree Adaptive Attack is " << net.getNodeRobustness() << endl;
     
     ofs.close();
     
     */
    


    
}

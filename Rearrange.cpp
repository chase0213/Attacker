//
//  Rearrange.cpp
//  Attacker
//
//  Created by 長谷川 知里 on 2012/12/26.
//  Copyright (c) 2012年 C. HASEGAWA. All rights reserved.
//

#include "Rearrange.h"

Rearrange::Rearrange(string s,int nalg, int n){
    
    string *finput;
    finput = new string[nalg];
    finput[0] = "BFS";
    finput[1] = "DFS";
    finput[2] = "HG";
    finput[3] = "HDA";
    finput[4] = "BRW";
    finput[5] = "BRWonly";
    
    string *pref;
    pref = new string[3];
    pref[0] = "m";
    pref[1] = "n";
    pref[2] = "l";
    
    double** memoryInUse;
    memoryInUse = new double*[nalg];
    for( int i=0; i<nalg; i++ ){
        memoryInUse[i] = new double[n];
    }
    
    for( int k=0; k<3; k++ ){
    
        for( int i=0; i<nalg; i++ )
            for( int j=0; j<n; j++ )
                memoryInUse[i][j] = 0.0;
    
        for( int i=0; i<nalg; i++ ){
            ifstream ifs((pref[k]+finput[i]+".txt").c_str());
            if( ifs==NULL ){
                cout << "file " << pref[k]+finput[i] << ".txt cannot be opened." << endl;
                return;
            }
            for( int j=0; j<n; j++ ){
                ifs >> memoryInUse[i][j];
            }
            ifs.close();
        }

        ofstream ofs((pref[k]+".txt").c_str());
        if( ofs==NULL )
            return ;
    
        ofs << "X";
        for( int i=0; i<nalg; i++ ){
            ofs << "\t" << finput[i];
        }
        ofs << endl;
        
        for( int j=0; j<n; j++ ){
            ofs << j << "\t";
            for( int i=0; i<nalg; i++ ){
                if( i!=nalg-1 ){
                    ofs << memoryInUse[i][j] << "\t";
                } else {
                    ofs << memoryInUse[i][j] << endl;
                }
            }
        }
    
        ofs.close();
    }
    
    for( int i=0; i<nalg; i++ )
        delete [] memoryInUse[i];
    delete [] memoryInUse;
    
}
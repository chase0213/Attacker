//
//  NetworkCoder.cpp
//  Crawler
//
//  Created by 長谷川 知里 on 12/06/13.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "NetworkCoder.h"
#include "resource.h"

const Network NetworkCoder::decodeNetwork(const string file){
    
    // 指定されたファイルが.gmlでなければ終了
    int fs = (int)file.length();
    if( file.substr(fs-4,4)==".gml" || file.substr(fs-4,4)==".GML" ){
        return decodeNetworkGML(file);
    } else if( file.substr(fs-4,4)==".dat" || file.substr(fs-4,4)==".DAT" ){
        return decodeNetworkEDGES(file);
    } else if( file.substr(fs-4,4)==".txt" || file.substr(fs-4,4)==".TXT" ){
        return decodeNetworkTXT(file);
    } else if( file.substr(fs-6,6)==".edges" || file.substr(fs-6,6)==".EDGES" ){
        return decodeNetworkEDGES(file);
    } else {
        cerr << "File type is not supported." << endl;
        exit(1);
    }
}

void NetworkCoder::addNode(Network *net) {
    int nid;
    string label = "nil";
    string buffer;
    ifs >> buffer;
    ifs >> buffer; if( buffer=="id" ) ifs >> nid;
    ifs >> buffer; if( buffer=="label") ifs >> label;
    while( buffer!="]" ) ifs >> buffer;
    Node node(nid,label);
    node.setRandomPosition();
    (*net).addNode(node);
}

void NetworkCoder::addEdge(Network *net) {
    int s, t;
    string buffer;
    while( ifs >> buffer ) if( buffer.find("source")!=string::npos ) break;
    ifs >> s;
    while( ifs >> buffer ) if( buffer.find("target")!=string::npos ) break;
    ifs >> t;
    
//    vector<Node> nodes = (*net).getNodes();
//    if( nodes.at(s).isConnected(t) ) return;

    Link l1(s,t), l2(t,s);
    (*net).addLinkToNode(l1, s);
    if( !isCreatedByBAModel ){
        (*net).addLinkToNode(l2, t);
    }
}

const Network NetworkCoder::decodeNetworkGML(const string file){
    
    Network net;
    
    ifs.open(file.c_str());
    // ファイルが開かれたかどうかのチェック
    if( ifs == NULL ){
        cerr << "File cannot be opened." << endl;
        exit(1);
    }
    
    string buffer;
    
    // graphが現れるまでスキップ
    while( buffer.find("graph")!=string::npos ) ifs >> buffer;
    
    // directed処理
    // 未定義
    
    // 全部読む
    while( ifs >> buffer ){
        if( buffer.find("node")!=string::npos ){
            addNode(&net);
        } else if( buffer.find("edge")!=string::npos ){
            addEdge(&net);
        }
    }
    
    setHistgram(&net);
    
    ifs.close();
    
    net.finalize();
    
    return net;
}

const Network NetworkCoder::decodeNetworkDAT(const string file){
    
    Network net;
    
    ifs.open(file.c_str());
    // ファイルが開かれたかどうかのチェック
    if( ifs == NULL ){
        cerr << "File cannot be opened." << endl;
        exit(1);
    }
    
    string buffer;
    
    // nodeを登録
    int cid;
    vector<int> tID;
    while( ifs >> cid ){
        vector<int>::iterator itr = find(tID.begin(),tID.end(),cid);
        if( itr==tID.end() ){
            tID.push_back(cid);
        }
    }
    sort(tID.begin(),tID.end());
    vector<int>::iterator itr = tID.begin();
    while( itr!=tID.end() ){
        Node node((*itr),"nil");
        itr++;
    }
    
    ifs.close();
    ifs.open(file.c_str());
    
    // linkを登録
    while( getline(ifs,buffer) ){
        
    }

//    // graphが現れるまでスキップ
//    getline(ifs,buffer);
//    while( buffer!="graph" ) getline(ifs,buffer);
//    
//    // directed処理
//    // 未定義
//    
//    // 全部読む
//    while( getline(ifs,buffer) ){
//        if( buffer.find("node")!=string::npos ){
//            addNode(&net);
//        } else if( buffer.find("edge")!=string::npos ){
//            addEdge(&net);
//        }
//    }
    
    setHistgram(&net);
    
    ifs.close();
    
    return net;
}

const Network NetworkCoder::decodeNetworkTXT(const string file) {
    
    Network net;
    
    ifs.open(file.c_str());
    if( ifs == NULL ){
        cerr << "File cannot be opened." << endl;
        exit(1);
    }
    
    int nid;
    vector<int> tableID;
    vector<int>::iterator itr;
    while( ifs >> nid ){
        int buf;
        ifs >> buf >> buf;
        itr = find(tableID.begin(),tableID.end(),nid);
        if( itr==tableID.end() ){
            tableID.push_back(nid);
        }
    }
    sort(tableID.begin(),tableID.end());
    itr = tableID.begin();
    while( itr!=tableID.end() ){
        Node node((*itr),"nil");
        net.addNode(node);
        itr++;
    }
    
    ifs.close();
    ifs.open(file.c_str());
    int sID, tID, buf;
    while( ifs >> sID >> tID >> buf ){
        Link link1(sID,tID), link2(tID, sID);
        net.addLinkToNode(link1, sID-offset);
//        net.addLinkToNode(link2, tID-offset);
    }
    ifs.close();
    
    setHistgram(&net);

    return net;
}

const Network NetworkCoder::decodeNetworkEDGES(const string file) {
    Network net;
    
    ifs.open(file.c_str());
    if( ifs == NULL ){
        cerr << "File cannot be opened." << endl;
        exit(1);
    }
    
    vector<string> _id_table;
    vector<string>::iterator pos;
    string s, t;
    int s_idx, t_idx;
    while( ifs >> s, ifs >> t ){
        
        // source
        pos = find(_id_table.begin(),_id_table.end(),s);
        if( pos==_id_table.end() ){
            Node node((int)_id_table.size(),"id");
            _id_table.push_back(s);
            net.addNode(node);
            s_idx = (int)(_id_table.size())-1;
        } else {
            s_idx = (int)(pos - _id_table.begin());
        }
        
        // target
        pos = find(_id_table.begin(),_id_table.end(),t);
        if( pos==_id_table.end() ){
            Node node((int)_id_table.size(),"id");
            _id_table.push_back(t);
            net.addNode(node);
            t_idx = (int)(_id_table.size())-1;
        } else {
            t_idx = (int)(pos - _id_table.begin());
        }
        
        Link l1(s_idx,t_idx);        
        net.addLinkToNode(l1, s_idx);
        
        if( directed ){
            Link l2(t_idx,s_idx);
            net.addLinkToNode(l2, t_idx);
        }
        
    }
    
    ifs.close();
    
    net.finalize();
    
    return net;
}

void NetworkCoder::setHistgram(Network *net) {
    vector<Node> ns = (*net).getNodes();
    for ( int i=0; i<(*net).getV(); i++ ){
        if( (*net).getMaxDegree() < ns[i].getDegree() )
            (*net).setMaxDegree(ns[i].getDegree());
        if( (*net).getMinDegree() > ns[i].getDegree() )
            (*net).setMinDegree(ns[i].getDegree());
    }
}

// ノード数N，隣接確率pのERモデルをグラフgに作成します．
Network NetworkCoder::createERModel(const int N, const double p) {
    
    Network net;
    
    // N個のノードを作成します．
    for (int i=0; i<N; i++){
        Node node(i,"");
        net.addNode(node);
    };
    
    // E個のエッジを作成します．
    int const E = int(0.5*N*(N-1)*p);
    for (int i=0; i<E; ) { // カウンターはエッジを作成したときにインクリメントします．
        // エッジを作成するノードをランダムに2つ選びます．
        int s = rand()%N;
        int t = rand()%N;

        // ここでは自己ループエッジと多重エッジを禁止するため
        // sourceとtargetが別ノードでかつ隣接していない場合にエッジを作成します．
        vector<Node> nodes = net.getNodes();
        if (s==t || nodes.at(s).isConnected(t) ) continue;

        Link l1(s,t);
        Link l2(t,s);
        net.addLinkToNode(l1, s);
        net.addLinkToNode(l2, t);
        i++;
    }
    
    net.finalize();
    
    return net;
}

// ノード数N，新規ノードが既存ノードに延ばす枝数mのBAモデルをグラフgに作成します．
Network NetworkCoder::createBAModel(const int N, const int m) {
    
    Network net;
    
    vector<int> dps; // degree partial sum vector

    // N個のノードを作成します．
    for (int i=0; i<N; i++){
        Node node(i,"");
        net.addNode(node);
    }
    
    vector<Link> allLink;
    
    // 成長の種になるグラフ（ノード数nでn≧mの完全グラフ）を作ります．
    // 初期段階でエッジは最低1本は必要なため，n≧2である必要があります．
    const int n = (m<2)?2:m;
    for( int i=0; i<n; i++ ) {
        for( int j=i+1; j<n; j++ ){
            Link l1(i,j);
            Link l2(j,i);
            net.addLinkToNode(l1, i);
            net.addLinkToNode(l2, j);
            allLink.push_back(l1);
        }
        dps.push_back((i+1)*m); // n点からなる完全グラフなので、全ての頂点次数はn-1
    }
    
    sum = 0;
    for( int i=0; i<dps.size(); i++ ){
        sum += dps.at(i);
    }
    
    // ノードを1つずつ種グラフに接続していきます．
    for ( int i=n; i<N; i++ ) {
        Node node(i,"");
        for (int j=0; j<m;) { // カウンターはエッジを作成したときにインクリメントします．
            
//            int rnd = rand()%sum;
//            int e = findUpperBoundIndex(&dps,rnd);
            
            int sd = rand()%2;
            int l = rand()%allLink.size();
            int e = sd==0 ? allLink.at(l).getSource() : allLink.at(l).getDestination();
            
            Link l1(i,e);
            Link l2(e,i);
            vector<Node> nodes = net.getNodes();
            if( nodes.at(i).isConnected(e) ) continue;
            
            allLink.push_back(l1);
            
//            renewPartialSum(&dps,e,m);
            
            
            net.addLinkToNode(l1, i);
            net.addLinkToNode(l2, e);
            j++;
            
        }
        dps.push_back(m+dps.at(i-1));
    }
    
    net.finalize();
    
    return net;
}

Network NetworkCoder::connectBAModel(const int N, const int m, const int itr) {
    Network network;
    for( int i=0; i<itr; i++ ){
        createBAModel(&network, N/itr, m, i*(N/itr));
    }
    network.finalize();
    return network;
}

// ノード数N，新規ノードが既存ノードに延ばす枝数mのBAモデルをグラフgに作成します．
void NetworkCoder::createBAModel(Network *net, const int N, const int m, const int idBias) {
    
    vector<int> dps; // degree partial sum vector
    
    // N個のノードを作成します．
    for (int i=0; i<N; i++){
        Node node(i+idBias,"");
        (*net).addNode(node);
    }
    
    vector<Link> allLink;
    
    // 成長の種になるグラフ（ノード数nでn≧mの完全グラフ）を作ります．
    // 初期段階でエッジは最低1本は必要なため，n≧2である必要があります．
    const int n = (m<2)?2:m;
    for( int i=0; i<n; i++ ) {
        for( int j=i+1; j<n; j++ ){
            Link l1(i+idBias,j+idBias);
            Link l2(j+idBias,i+idBias);
            (*net).addLinkToNode(l1, i+idBias);
            (*net).addLinkToNode(l2, j+idBias);
            allLink.push_back(l1);
        }
        dps.push_back((i+1)*m); // n点からなる完全グラフなので、全ての頂点次数はn-1
    }
    
    sum = 0;
    for( int i=0; i<dps.size(); i++ ){
        sum += dps.at(i);
    }
    
    // ノードを1つずつ種グラフに接続していきます．
    for ( int i=n; i<N; i++ ) {
        Node node(i+idBias,"");
        for (int j=0; j<m;) { // カウンターはエッジを作成したときにインクリメントします．
            
            //            int rnd = rand()%sum;
            //            int e = findUpperBoundIndex(&dps,rnd);
            
            int sd = rand()%2;
            int l = rand()%allLink.size();
            int e = sd==0 ? allLink.at(l).getSource() : allLink.at(l).getDestination();
            
            Link l1(i+idBias,e);
            Link l2(e,i+idBias);
            vector<Node> nodes = (*net).getNodes();
            if( nodes.at(i).isConnected(e) ) continue;
            
            allLink.push_back(l1);
            
            //            renewPartialSum(&dps,e,m);
            
            
            (*net).addLinkToNode(l1, i+idBias);
            (*net).addLinkToNode(l2, e);
            j++;
            
        }
        dps.push_back(m+dps.at(i-1));
    }
    
}

Network NetworkCoder::createHKModel(const int N, const int m, const double p){
    Network net;
    
    // N個のノードを作成します．
    for (int i=0; i<N; i++){
        Node node(i,"");
        net.addNode(node);
    }
    
    vector<Link> allLink;
    
    // 成長の種になるグラフ（ノード数nでn≧mの完全グラフ）を作ります．
    // 初期段階でエッジは最低1本は必要なため，n≧2である必要があります．
    const int n = (m<2)?2:m;
    for( int i=0; i<n; i++ ) {
        for( int j=i+1; j<n; j++ ){
            Link l1(i,j);
            Link l2(j,i);
            net.addLinkToNode(l1, i);
            net.addLinkToNode(l2, j);
            allLink.push_back(l1);
        }
    }
    
    
    // ノードを1つずつ種グラフに接続していきます．
    for ( int i=n; i<N; i++ ) {
        Node node(i,"");
        int pivot = -1;
        for (int j=0; j<m;) { // カウンターはエッジを作成したときにインクリメントします．
            double rnd = (rand()%100000)/100000.0;
            if( j==0 || rnd < p ){
                int sd = rand()%2;
                int l = rand()%allLink.size();
                int e = sd==0 ? allLink.at(l).getSource() : allLink.at(l).getDestination();
            
                // ピボットを設定
                if( j==0 ) pivot = e;

                vector<Node> nodes = net.getNodes();
                if( nodes.at(i).isConnected(e) ) continue;
            
                Link l1(i,e);
                Link l2(e,i);
                
                allLink.push_back(l1);
            
                net.addLinkToNode(l1, i);
                net.addLinkToNode(l2, e);
                j++;
            
            } else {
                
                // pivotに隣接する頂点から等確率で1つ選び，結ぶ
                vector<Node> nodes = net.getNodes();
                int r = rand()%(nodes.at(pivot).getDegree());
                if( nodes.at(pivot).isConnected(r) ) continue;
                
                Link l1(i,r);
                Link l2(r,i);
                
                allLink.push_back(l1);
                
                net.addLinkToNode(l1, i);
                net.addLinkToNode(l2, r);
                j++;
                
            }
        }
    }
    
    net.finalize();
    
    return net;
}

void NetworkCoder::renewPartialSum(vector<int> *dps, const int e, const int m){
    for( int i=e; i<(*dps).size(); i++ ){
        (*dps).at(i)++;
        sum++;
    }
//    (*dps).push_back(m);
}

// rndを超える最小の部分和のindexを返す
int NetworkCoder::findUpperBoundIndex(vector<int> *dps, const int rnd) {
    int lb = 0;
    int ub = (int)(*dps).size()-1;
    int mid = (ub+lb)/2;
    
    int lb_old = lb;
    int ub_old = ub;
    while( ub>lb ) {
        if( (*dps).at(mid) == rnd ){
            return mid;
        } else if( (*dps).at(mid) > rnd ){
            ub = mid;
        } else {
            lb = mid;
        }
        mid = (ub+lb)/2;
        
        if( ub==ub_old && lb==lb_old )
            break;
        else
            ub_old = ub, lb_old = lb;
    }
    
    if( (*dps).at(mid) < rnd )
        return mid+1;
    
    return mid;
    
}

const string NetworkCoder::encodeNetwork(const Network net, const string date ){
    string file = "graph_created.gml";
    ofstream ofs(file.c_str());
    ofs << "Creator Chisto Hasegawa " + date << endl;
    ofs << "graph" << endl;
    ofs << "[" << endl;
    
    vector<Node> nodes = net.getNodes();
    for( vector<Node>::iterator itn=nodes.begin(); itn!=nodes.end(); itn++ ){
        ofs << "\tnode" << endl;
        ofs << "\t[" << endl;
        ofs << "\t\tid " << (*itn).getNodeID() << endl;
        ofs << "\t\tlabel " << (*itn).getNodeID() << endl;
        ofs << "\t]" << endl;
    }
    
    for( vector<Node>::iterator itn=nodes.begin(); itn!=nodes.end(); itn++ ){
        vector<Link> links = (*itn).getLinks();
        for( vector<Link>::iterator itl=links.begin(); itl!=links.end(); itl++ ){
            ofs << "\tedge" << endl;
            ofs << "\t[" << endl;
            ofs << "\t\tsource " << (*itl).getSource() << endl;
            ofs << "\t\ttarget " << (*itl).getDestination() << endl;
            ofs << "\t]" << endl;
        }
    }
    
    ofs << "]" << endl;
    
    ofs.close();
    return file;
}

const string NetworkCoder::encodeNetwork(const Network net, const string fname, const string date ){
    string file = fname;
    ofstream ofs(file.c_str());
    ofs << "Creator Chisto Hasegawa " + date << endl;
    ofs << "graph" << endl;
    ofs << "[" << endl;
    
    vector<Node> nodes = net.getNodes();
    for( vector<Node>::iterator itn=nodes.begin(); itn!=nodes.end(); itn++ ){
        ofs << "\tnode" << endl;
        ofs << "\t[" << endl;
        ofs << "\t\tid " << (*itn).getNodeID() << endl;
        ofs << "\t\tlabel " << (*itn).getNodeID() << endl;
        ofs << "\t]" << endl;
    }
    
    for( vector<Node>::iterator itn=nodes.begin(); itn!=nodes.end(); itn++ ){
        vector<Link> links = (*itn).getLinks();
        for( vector<Link>::iterator itl=links.begin(); itl!=links.end(); itl++ ){
            ofs << "\tedge" << endl;
            ofs << "\t[" << endl;
            ofs << "\t\tsource " << (*itl).getSource() << endl;
            ofs << "\t\ttarget " << (*itl).getDestination() << endl;
            ofs << "\t]" << endl;
        }
    }
    
    ofs << "]" << endl;
    
    ofs.close();
    return file;
}

Network NetworkCoder::createConnectedClique(const int N_div_p, const int p, const int m_max, const int m_min){
    Network net;
    
    // p個のcliqueを作る
    for( int i=0; i<p; i++ ){

        // cliqueの作成
        for( int n=0; n<N_div_p; n++ ){
            Node node(N_div_p*i+n,"id");
            for( int l=N_div_p*i; l<N_div_p*(i+1); l++ ){
                if( l!=N_div_p*i+n ){
                    Link link(N_div_p*i+n,l);
                    node.addLink(link);
                }
            }
            net.addNode(node);
        }
    }
    
    // clique間をm_min〜m_max本の辺で無作為に結ぶ
    for( int i=0; i<p; i++ ){
        int rnd = rand()%(m_max-m_min)+m_min;
        for( int j=0; j<rnd; j++ ){
            
            // 結ぶ先のcliqueを選ぶ
            int target;
            do{
                target = rand()%p;
            } while( target==p );
            
            int n1 = N_div_p*i + rand()%N_div_p;
            int n2 = N_div_p*target + rand()%N_div_p;
            Link l1(n1,n2);
            Link l2(n2,n1);
            net.addLinkToNode(l1, n1);
            net.addLinkToNode(l2, n2);
        }
    }
    
    net.finalize();
    
    return net;
    
}

Network NetworkCoder::createRHModel(const int N0, const int T){
    Network net;
    
    int n = N0;
    // n = N0^(T+1)とする
    for( int i=1; i<=T; i++ ) n *= N0;
    
    bool **adj;
    adj = new bool*[n];
    for( int i=0; i<n; i++ ){
        adj[i] = new bool[n];
        for( int j=0; j<n; j++ )
            adj[i][j] = false;
    }

    // N0次の完全グラフを作成
    for( int i=0; i<N0; i++ )
        for( int j=0; j<N0; j++ )
            adj[i][j] = (i!=j) ? true : false;
    
    bool *centrality;
    centrality = new bool[n];
    centrality[0] = true;
    for( int i=1; i<n; i++ ) centrality[i] = false;
    
    int pn = N0;
    for( int t=1; t<=T; t++ ){
        
        // コピー
        for( int k=1; k<N0; k++ ){
            for( int i=0; i<pn; i++ ){
                centrality[pn*k+i] = centrality[i];
                for( int j=0; j<pn; j++ ){
                    adj[k*pn+i][k*pn+j] = adj[i][j];
                }
            }
        }
        
        // 新規接続
        for( int k=1; k<N0; k++ ){
            for( int i=0; i<pn; i++ ){
                if( !centrality[k*pn+i] ){
                    adj[k*pn+i][0] = adj[0][k*pn+i] = true;
                }
            }
        }
        
        // 中心点集合に追加
        for( int i=1; i<pn; i++ ){
            centrality[i] = true;
        }
        
        // pnの更新
        pn *= N0;
    }
    
    for( int i=0; i<n; i++ ){
        Node node(i,"id");
        net.addNode(node);
        for( int j=0; j<n; j++ ){
            if( adj[i][j] ){
                Link link(i,j);
                net.addLinkToNode(link, i);
            }
        }
    }
    
    for( int i=0; i<n; i++ )
        delete [] adj[i];
    delete [] adj;
    delete [] centrality;
    
    net.finalize();
    
    return net;

}


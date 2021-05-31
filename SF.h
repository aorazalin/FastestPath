//
// Created by Asus on 16.02.2021.
//

#ifndef FASTESTPATH_STRAIGHTFORWARD_H
#define FASTESTPATH_STRAIGHTFORWARD_H
#include<vector>
#include<queue>
#include<iostream>
#include<stack>
#include<algorithm>
#include "Edge.h"
using namespace std;

class SF {
private:
    vector<int> pred;
    vector<bool> marked;
    vector<double> d;
    vector<vector<pair<int, double>>> adj;
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;
    int num_vertices;
public:
    void update (int u, int v, double w);
    double dist (int u, int v);
    vector<int> path (int u, int v);
    SF(int num_vertices, vector<Edge>& edges);
};


#endif //FASTESTPATH_STRAIGHTFORWARD_H

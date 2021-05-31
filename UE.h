//
// Created by Asus on 16.02.2021.
//

#ifndef FASTESTPATH_UE_H
#define FASTESTPATH_UE_H

#include<vector>
#include<algorithm>
#include<unordered_map>
#include<iostream>
#include<queue>
#include<set>
#include<fstream>
#include<unordered_set>
#include"Edge.h"
using namespace std;

struct pairhash {
public:
    template<typename T, typename U>
    size_t operator()(const pair<T, U>& x) const {
        return hash<T>()(x.first) + 3 * hash<U>()(x.second);
    }
};

class UE {
private:
    int num_vertices;
    vector<vector<int>> higher_in, higher_out, lower_in, lower_out; // {higher/lower}-in-neighbors, {higher/lower}-out-neighbors
    vector<int> rank_to_vertex, rank;
    unordered_map<pair<int, int>, double, pairhash> weight_original; // original graph edge weights
    unordered_map<pair<int, int>, double, pairhash> weight, weightR; // weight mapping table
    unordered_map<pair<int, int>, int, pairhash> occupant;    // occupant table

    void vertexShortcutCreation(int v, int v_in, int v_out);
    void contract(int v);
    bool edge_exists(int u, int v);
    bool edge_exists_original(int u, int v);
    bool occupant_exists(int u, int v);
    void process(int v, unordered_map<int, double>& dist,
                     unordered_map<int, int>& prev, unordered_set<int>& proc, vector<vector<int>>& graph,
                     unordered_map<pair<int, int>, double, pairhash>& w,
                     priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>>& pq);
    void relax(int u, int v, unordered_map<int, double>& dist, unordered_map<int, int>& prev,
                   unordered_map<pair<int, int>, double, pairhash>& w, priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>>& pq);
    vector<int> shortest_path(int s, int t, unordered_map<int, double>& distF, unordered_map<int, int>& prevF, unordered_set<int>& procF,
                                  unordered_map<int, double>& distB, unordered_map<int, int>& prevB, unordered_set<int>& procB);
    double shortest_dist(int s, int t, unordered_map<int, double>& distF, unordered_map<int, int>& prevF, unordered_set<int>& procF,
                         unordered_map<int, double>& distB, unordered_map<int, int>& prevB, unordered_set<int>& procB);
    vector<int> real_path(int u, int v);
    void sub_update(int u, int v);

public:
    int getNumVertices() const;
    void print_graph(ofstream & of);
    void update (int u, int v, double w);
    double dist (int s, int t);
    vector<int> path (int s, int t);
    UE(int num_vertices, vector<Edge>& edges); // Preprocessing
};


#endif //FASTESTPATH_UE_H

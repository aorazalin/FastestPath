//
// Created by Asus on 16.02.2021.
//

#include "UE.h"
#include "HelperFunctions.h"
#include <random>

bool UE::edge_exists(int u, int v) {
    return weight.find({u, v}) != weight.end();
}

bool UE::edge_exists_original(int u, int v) {
    return weight_original.find({u, v}) != weight_original.end();
}

bool UE::occupant_exists(int u, int v) {
    return occupant.find({u, v}) != occupant.end();
}

void UE::vertexShortcutCreation(int v, int v_in, int v_out) {
    // v_in --> v --> v_out
    double new_weight = weight[{v_in, v}] + weight[{v, v_out}];
    if (!edge_exists(v_in, v_out)) {
        if (rank[v_in] > rank[v_out]) {
            higher_in[v_out].push_back(v_in);
            lower_out[v_in].push_back(v_out);
        }
        else {
            higher_out[v_in].push_back(v_out);
            lower_in[v_out].push_back(v_in);
        }
        weight[{v_in, v_out}] = new_weight;
        occupant[{v_in, v_out}] = v;
        return;
    }
    if (weight[{v_in, v_out}] > new_weight) {
        weight[{v_in, v_out}] = new_weight;
    }
    if (!occupant_exists(v_in, v_out))
        occupant[{v_in, v_out}] = v;
    else {
        int o = occupant[{v_in, v_out}];
        if (new_weight < weight[{v_in, o}] + weight[{o, v_out}])
            occupant[{v_in, v_out}] = v;
    }
}

void UE::contract(int v) {
    for (int x : higher_in[v]) {
        for (int y : higher_out[v]) {
            if (x == y) continue;
            vertexShortcutCreation(v, x, y);
        }
    }
}

void UE::print_graph(ofstream & of) {
    of << "############################################\n";
    of << "Number of vertices: " << num_vertices << "\n";

    of << "Ranks: \n";
    for (int i = 0; i < num_vertices; ++i)
        of << "R(" << i << ") = " << rank[i] << "\n";


    of << "Weight mapping table: \n";
    for (auto& w : weight) {
        of << w.first.first << " " << w.first.second << " " << w.second << "\n";
    }
    of << "Occupant table: \n";
    for (auto& o : occupant) {
        of << "o( " << o.first.first << ", " << o.first.second << " ) = " << o.second << "\n";
    }
}





UE::UE(int num_vertices, vector<Edge>& edges) : num_vertices(num_vertices) {
    higher_in.resize(num_vertices); higher_out.resize(num_vertices);
    lower_in.resize(num_vertices); lower_out.resize(num_vertices);
    rank.resize(num_vertices);
    rank_to_vertex.resize(num_vertices);

    for (int i = 0; i < num_vertices; ++i) rank[i] = i;
    shuffle(rank.begin(), rank.end(), std::mt19937(std::random_device()())); // --> uncomment if not debugging
    for (int i = 0; i < num_vertices; ++i) rank_to_vertex[rank[i]] = i;

    for (Edge& edge : edges) {
        if (!edge_exists(edge.from, edge.to)) {
            weight[{edge.from, edge.to}] = edge.w;
            weight_original[{edge.from, edge.to}] = edge.w;
        } else {
            weight[{edge.from, edge.to}] = min(edge.w, weight[{edge.from, edge.to}]);
            weight_original[{edge.from, edge.to}] = min(edge.w, weight_original[{edge.from, edge.to}]);
        }
    }

//    for (int v = 0; v < num_vertices; ++v) {
//        for (int to : out[v])
//            if (rank[to] > rank[v]) {
//                higher_out[v].push_back(to);
//                lower_in[to].push_back(v);
//            }
//        for (int to : in[v])
//            if (rank[to] > rank[v]) {
//                higher_in[v].push_back(to);
//                lower_out[to].push_back(v);
//            }
//    }

    for (auto edge : weight) {
        int u = edge.first.first, v = edge.first.second;
        if (rank[v] > rank[u]) {
            higher_out[u].push_back(v);
            lower_in[v].push_back(u);
        } else {
            higher_in[v].push_back(u);
            lower_out[u].push_back(v);
        }
    }

    for (int i = 0; i < num_vertices; ++i) contract(rank_to_vertex[i]);

    // initializing its reverse graph
    for (auto& it : weight) weightR[{it.first.second, it.first.first}] = it.second;
}

void UE::relax(int u, int v, unordered_map<int, double>& dist, unordered_map<int, int>& prev,
               unordered_map<pair<int, int>, double, pairhash>& w, priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>>& pq) {
    if (dist.find(v) == dist.end() || dist[v] > dist[u] + w[{u, v}]) {
        dist[v] = dist[u] + w[{u, v}];
        prev[v] = u;
        pq.push({dist[v], v});
    }
}

void UE::process(int v, unordered_map<int, double>& dist,
                 unordered_map<int, int>& prev, unordered_set<int>& proc, vector<vector<int>>& graph,
                 unordered_map<pair<int, int>, double, pairhash>& w, priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>>& pq) {
    for (int to : graph[v]) {
        relax(v, to, dist, prev, w, pq);
    }
    proc.insert(v);
}

double UE::shortest_dist(int s, int t, unordered_map<int, double>& distF, unordered_map<int, int>& prevF, unordered_set<int>& procF,
                                       unordered_map<int, double>& distB, unordered_map<int, int>& prevB, unordered_set<int>& procB) {
    const unordered_set<int>& procUnion = HelperFunctions::set_union(procF, procB);
    double mu = INT32_MAX;
    for (int u : procUnion) {
        if (distF.find(u) != distF.end() && distB.find(u) != distB.end())
            mu = min(mu, distF[u] + distB[u]);
    }
    return mu;
}


//double UE::dist(int s, int t) {
//    unordered_map<int, double> distF, distB;
//    unordered_map<int, int> prevF, prevB;
//    unordered_set<int> procF, procB;
//    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pqF, pqB;
//
//    distF[s] = distB[t] = 0;
//    prevF[s] = prevB[t] = -1;
//    pqF.push({distF[s], s}); pqB.push({distB[t], t});
//
//    while (!pqF.empty() || !pqB.empty()) {
//        if (!pqF.empty()) {
//            int vF = pqF.top().second;
//            pqF.pop();
//            process(vF, distF, prevF, procF, higher_out, weight, pqF); // pass in higher_out --> consider only ascending edges in G'
//            if (procB.find(vF) != procB.end()) return shortest_dist(s, t, distF, prevF, procF, distB, prevB, procB);
//        }
//        if (!pqB.empty()) {
//            int vB = pqB.top().second;
//            pqB.pop();
//            process(vB, distB, prevB, procB, higher_in, weightR, pqB);
//            if (procF.find(vB) != procF.end()) return shortest_dist(s, t, distF, prevF, procF, distB, prevB, procB);
//        }
//    }
//    return -1.0;
//}


double UE::dist(int s, int t) {
    if (s == t) return 0.0;
    unordered_map<int, double> distF, distB;
    unordered_map<int, int> prevF, prevB;
    unordered_set<int> procF, procB;
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pqF, pqB;

    distF[s] = distB[t] = 0;
    prevF[s] = prevB[t] = 0;
    pqF.push({s, distF[s]}); pqB.push({t, distB[t]});

    double mu = INFINITY;

    while (!pqF.empty() && !pqB.empty()) {
        int vF = pqF.top().second, vB = pqB.top().second;
        if (distF[vF] + distB[vB] >= mu) break;

    }

    return (mu == INFINITY) ? -1 : mu;
}

// return the real path from u to v in G excluding v
vector<int> UE::real_path(int u, int v) {
    if (weight_original.find({u,v}) != weight_original.end() && weight_original[{u,v}] == weight[{u,v}]) {
        return {u};
    }
    int o = occupant[{u, v}];
    vector<int> a = real_path(u, o);
    vector<int> b = real_path(o, v);
    a.insert(a.end(), b.begin(), b.end());
    return a;
}

vector<int> UE::shortest_path(int s, int t, unordered_map<int, double>& distF, unordered_map<int, int>& prevF, unordered_set<int>& procF,
                              unordered_map<int, double>& distB, unordered_map<int, int>& prevB, unordered_set<int>& procB) {
    const unordered_set<int>& procUnion = HelperFunctions::set_union(procF, procB);
    double mu = INT32_MAX;
    int crossVertex = -1;
    for (int u : procUnion) {
        if (distF.find(u) == distF.end() || distB.find(u) == distB.end()) continue;
        if (mu > distF[u] + distB[u]) {
            mu = distF[u] + distB[u];
            crossVertex = u;
        }
    }
    vector<int> path;
    int current = crossVertex;
    while (current != s) {
        path.push_back(current);
        current = prevF[current];
    }
    path.push_back(s);
    reverse(path.begin(), path.end());
    current = crossVertex;
    while (current != t) {
        current = prevB[current];
        path.push_back(current);
    }
    vector<int> rp;
    for (int i = 0; i < path.size() - 1; ++i) {
        int u = path[i], v = path[i + 1];
        vector<int> uv_path = real_path(u, v);
        rp.insert(rp.end(), uv_path.begin(), uv_path.end());
    }
    rp.push_back(t);
    return rp;
}

vector<int> UE::path (int s, int t) {
    unordered_map<int, double> distF, distB;
    unordered_map<int, int> prevF, prevB;
    unordered_set<int> procF, procB;
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pqF, pqB;

    distF[s] = distB[t] = 0;
    prevF[s] = prevB[t] = -1;
    pqF.push({distF[s], s}); pqB.push({distB[t], t});

    while (!pqF.empty() || !pqB.empty()) {
        if (!pqF.empty()) {
            int vF = pqF.top().second;
            pqF.pop();
            process(vF, distF, prevF, procF, higher_out, weight, pqF); // pass in higher_out --> consider only ascending edges in G'
            if (procB.find(vF) != procB.end()) return shortest_path(s, t, distF, prevF, procF, distB, prevB, procB);
        }
        if (!pqB.empty()) {
            int vB = pqB.top().second;
            pqB.pop();
            process(vB, distB, prevB, procB, higher_in, weightR, pqB);
            if (procF.find(vB) != procF.end()) return shortest_path(s, t, distF, prevF, procF, distB, prevB, procB);
        }
    }
    return {};
}



int UE::getNumVertices() const { return num_vertices; }


void UE::update(int u, int v, double w) {
    if (!edge_exists(u, v)) return;
    weight_original[{u, v}] = w;
    double w_before = weight[{u, v}];
    if (occupant_exists(u, v)) {
        int o = occupant[{u, v}];
        double wo = weight[{u, o}] + weight[{o, v}];
        weight[{u, v}] = min(wo, w);
    } else {
        weight[{u, v}] = w;
    }
    weightR[{v, u}] = weight[{u, v}];
    if (w_before != weight[{u, v}])
        sub_update(u, v);
}


void UE::sub_update(int u, int v) { // possible infinite recursion (?)
    if (rank[v] > rank[u]) {
        for (int x : higher_in[u]) {
            double w_before = weight[{x, v}];
            weight[{x, v}] = weight[{x, occupant[{x, v}]}] + weight[{occupant[{x, v}], v}];
            const vector<int>& possible_occupants = HelperFunctions::intersection(lower_out[x], lower_in[v]);
            for (int y : possible_occupants) {
                double wy = weight[{x, y}] + weight[{y, v}];
                if (wy >= weight[{x, v}]) continue;
                weight[{x, v}] = wy;
                occupant[{x, v}] = y;
            }
            if (edge_exists_original(x, v))
                weight[{x, v}] = min(weight[{x, v}], weight_original[{x, v}]);

            weightR[{v, x}] = weight[{x, v}];
            if (w_before != weight[{x, v}])
                sub_update(x, v);
        }
    }
    else {
        for (int x : higher_out[v]) {
            double w_before = weight[{u, x}];
            weight[{u, x}] = weight[{u, occupant[{u, x}]}] + weight[{occupant[{u, x}], x}];
            const vector<int>& possible_occupants = HelperFunctions::intersection(lower_out[u], lower_in[x]);
            for (int y : possible_occupants) {
                double wy = weight[{u, y}] + weight[{y, x}];
                if (wy >= weight[{u, x}]) continue;
                weight[{u, x}] = wy;
                occupant[{u, x}] = y;
            }
            if (edge_exists_original(u, x))
                weight[{u, x}] = min(weight[{u, x}], weight_original[{u, x}]);

            weightR[{x, u}] = weight[{u, x}];
            if (w_before != weight[{u, x}])
                sub_update(u, x);
        }
    }
}



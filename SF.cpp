//
// Created by Asus on 16.02.2021.
//

#include "SF.h"

void SF::update(int u, int v, double w) {
    for (auto x : adj[u]) {
        if (x.first == v) { x.second = w; return; }
    }
}

double SF::dist(int s, int t) {
    pred.assign(num_vertices, -1);
    marked.assign(num_vertices, false);
    d.assign(num_vertices, INF);
    d[s] = 0;
    pred[s] = s;
    pq.push({d[s], s});
    while (!pq.empty()) {
        int u = pq.top().second; pq.pop();
        if (marked[u]) continue;
        marked[u] = true;
        if (u == t) break;
        for (int i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i].first;
            double w = adj[u][i].second;
            if (d[u] + w < d[v]) {
                d[v] = d[u] + w;
                pq.push({d[v], v});
            }
        }
    }

    while (!pq.empty()) pq.pop();

    return d[t] == INF ? -1 : d[t];
}

vector<int> SF::path(int s, int t) {
    vector<int> path;

    pred.assign(num_vertices, -1);
    marked.assign(num_vertices, false);
    d.assign(num_vertices, INF);
    d[s] = 0;
    pred[s] = s;
    pq.push({d[s], s});
    while (!pq.empty()) {
        int u = pq.top().second; pq.pop();
        if (marked[u]) continue;
        marked[u] = true;
        if (u == t) break;
        for (int i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i].first;
            double w = adj[u][i].second;
            if (d[u] + w < d[v]) {
                pred[v] = u;
                d[v] = d[u] + w;
                pq.push({d[v], v});
            }
        }
    }

    while (!pq.empty()) pq.pop();

    int current = t;
    while (current != s) {
        path.push_back(current);
        current = pred[current];
    }
    path.push_back(s);
    reverse(path.begin(), path.end());
    return path;
}


SF::SF(int num_vertices, vector<Edge>& edges) : num_vertices(num_vertices) {
    adj.resize(num_vertices, vector<pair<int, double>>());
    for (auto& edge : edges) {
        adj[edge.from].push_back({edge.to, edge.w});
    }
}

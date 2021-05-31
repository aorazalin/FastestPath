#include <iostream>
#include <vector>
#include <random>
#include "UE.h"
#include "SF.h"
#include "HelperFunctions.h"
using namespace std;

void super_test(UE& ue, SF& sf);

bool test_zero(UE& ue, SF& sf) {
    int s = 3, t = 1;
    double sf_dist = sf.dist(s, t);
    double ue_dist = ue.dist(s, t);
    cout << "SF: " << sf_dist << "\n";
    cout << "UE: " << ue_dist << "\n";
    return sf_dist == ue_dist;
}

bool distanceTesting(UE& ue, SF& sf) {
    int num_vertices = ue.getNumVertices();
    bool res = true;
    int num_passed = 0;
    const int NUM_TESTS = 10;

    random_device dev;
    mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distDiscrete(0, INT32_MAX - 1);

    for (int i = 0; i < NUM_TESTS; ++i) {
        int s, t;
        s = (int)distDiscrete(rng) % num_vertices;
        t = (int)distDiscrete(rng) % num_vertices;
        cout << "(" << s << ", " << t << "):\n";
        cout << "SF: " << sf.dist(s, t) << "\n";
        cout << "UE: " << ue.dist(s, t) << "\n";
        res &= (sf.dist(s, t) == ue.dist(s, t));
        if (sf.dist(s, t) == ue.dist(s, t)) ++num_passed;
    }

    cout << "Passed " << num_passed << " out of " << NUM_TESTS << "\n";
    return res;
}

bool pathTesting(UE& ue, SF& sf) {
    int num_vertices = ue.getNumVertices();
    bool res = true;
    int num_passed = 0;
    const int NUM_TESTS = 10;

    random_device dev;
    mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distDiscrete(0, INT32_MAX - 1);

    for (int i = 0; i < NUM_TESTS; ++i) {
        int s, t;
        s = (int)distDiscrete(rng) % num_vertices;
        t = (int)distDiscrete(rng) % num_vertices;
        cout << "(" << s << ", " << t << "):\n";
        vector<int> sf_path = sf.path(s, t);
        vector<int> ue_path = ue.path(s, t);
        cout << "SF:";
        for (auto node : sf_path) cout << " " << node;
        cout << "\n";
        cout << "UE:";
        for (auto node : ue_path) cout << " " << node;
        cout << "\n";
        res &= sf_path == ue_path;
        if (sf.dist(s, t) == ue.dist(s, t)) ++num_passed;
    }

    cout << "Passed " << num_passed << " out of " << NUM_TESTS << "\n";
    return res;
}

bool test_one(UE& ue, SF& sf) {
    bool ans = true;
    cout << "Distance testing started...\n";
    ans &= distanceTesting(ue, sf);
    cout << "Distance testing ended.\n";
    cout << "Path testing started...\n";
    ans &= pathTesting(ue, sf);
    return ans;
}

void random_test_and_output() {
    ofstream output_file1, output_file2;
    output_file1.open("shortcutsInfo.txt");
    output_file2.open("graphInfo.txt");

    int num_vertices = 6;
    int num_edges = 13;
    int range_numbers = 5;
    vector<Edge> edges;

    random_device dev;
    mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distDiscrete(0, INT32_MAX - 1);

    for (int i = 0; i < num_edges; ++i) {
        int from = (int)distDiscrete(rng) % num_vertices;
        int to = (int)distDiscrete(rng) % num_vertices;
        double w = distDiscrete(rng) % range_numbers + 1;
        edges.push_back({from, to, w});
        output_file2 << from << ' ' << to << ' ' << w << '\n';
    }

    UE ue(num_vertices, edges);
    ue.print_graph(output_file1);
    output_file1.close();
    output_file2.close();
    SF sf(num_vertices, edges);
    bool ans = test_one(ue, sf);
    cout << (ans ? "Passed" : "Failed") << "\n";
}

void input_test_and_output() {
    ifstream input_file("graphInput.txt");
    ofstream output_file("shortcutsInfo.txt");

    if (!input_file.is_open())
        cout << "What do you mean? I'm confused!\n";

    int num_vertices, num_edges;
    input_file >> num_vertices;
    input_file >> num_edges;
    vector<Edge> edges;
    for (int i = 0; i < num_edges; ++i) {
        int u, v; double w;
        input_file >> u >> v >> w;
        edges.push_back({u, v, w});
    }

    UE ue(num_vertices, edges);
    ue.print_graph(output_file);
    output_file.close();
    SF sf(num_vertices, edges);

    super_test(ue, sf);
}

void super_test(UE& ue, SF& sf) {
    int num_queries = 100, num_vertices = ue.getNumVertices();
//    int max_weight = 5;
//    random_device dev;
//    mt19937 rng(dev());
//    std::uniform_int_distribution<std::mt19937::result_type> distDiscrete(0, INT32_MAX - 1);
//    int passed_queries = 0;
//    for (int q = 0; q < num_queries; ++q) {
//        int option = (int)distDiscrete(rng) % 3;
//        if (option == 0) {
//            // update
////            int u, v;
////            double w;
////            u = (int)distDiscrete(rng) % num_vertices;
////            v = (int)distDiscrete(rng) % num_vertices;
////            w = (int)distDiscrete(rng) % max_weight + 1;
////            cout << "Beginning Update: (" << u << ", " << v << ") to " << w << "\n";
////            sf.update(u, v, w);
////            ue.update(u, v, w);
////            cout << "Finishing Update.\n";
//            passed_queries++;
//        }
//        if (option == 1) {
//            int u, v;
//            u = (int)distDiscrete(rng) % num_vertices;
//            v = (int)distDiscrete(rng) % num_vertices;
//            cout << "Beginning Dist: (" << u << ", " << v << ") to \n";
//            double sf_dist = sf.dist(u, v);
//            double ue_dist = ue.dist(u, v);
//            cout << "SF : " << sf_dist << "\n";
//            cout << "UE : " << ue_dist << "\n";
//            cout << "Finishing Dist.\n";
//            if (sf_dist == ue_dist) passed_queries++;
//        }
//        if (option == 2) {
//            int u, v;
//            u = (int)distDiscrete(rng) % num_vertices;
//            v = (int)distDiscrete(rng) % num_vertices;
//            cout << "Beginning Dist: (" << u << ", " << v << ") to \n";
//            vector<int> sf_path = sf.path(u, v);
//            vector<int> ue_path = ue.path(u, v);
//            cout << "SF : " << sf_path << "\n";
//            cout << "UE : " << ue_path << "\n";
//            cout << "Finishing Dist.\n";
//            if (sf_path == ue_path) passed_queries++;
//        }
//    }
//
//    cout << "Passed " << passed_queries << "/" << num_queries << "\n";
//    cout << "Final verdict: " << ((passed_queries == num_queries) ? "Passed test" : "Not Passed Test") << "\n";

    double sf_dist = sf.dist(3, 0);
    double ue_dist = ue.dist(3, 0);
    cout << "SF : " << sf_dist << "\n";
    cout << "UE : " << ue_dist << "\n";

}

int main() {
    input_test_and_output();
}
#include"graph.hpp"
#include"SIBTree.hpp"
#include<cstdlib>

using namespace std;

void read_pair_data(string graph_name, vector<pair<node, node>> &pairs, string opt) {
    string path = dict_path + "pairdata/" + graph_name + "_" + opt + "_pair.txt";

    ifstream f(path);
    string line;
    node u, v;
    while (getline(f, line)) {
        stringstream(line).ignore(0, ' ') >> u >> v;
        pairs.push_back({u, v});
        // getline(f, line);
    }
    int k = pairs.size();
    printf("Has read %d pairs from %s !\n", k, path.c_str());
}

ull test_global_intersection(string file, Graph &g) {
    vector<pair<node, node>> pairs;
    read_pair_data(file, pairs, "global");
    SIB_Tree g_sib(g);
    ull res1 = 0, res2 = 0;
    
    auto tStart = steady_clock::now();
    for (auto k : pairs) {
        res1 += g.cnt_common_neighbors(k.first, k.second);
    }
    auto tEnd = steady_clock::now();
    ull _t = duration_cast<milliseconds>(tEnd - tStart).count();
    printf("Total number of global intersection: %lld, time used for merge intersection: %lld ms!\n", res1, _t);
    // printf("Times of comparisons in merge intersection: %lld !\n", g.get_cnt());

    tStart = steady_clock::now();
    for (auto k : pairs) {
        res2 += g_sib.count_intersection(k.first, k.second);
    }
    tEnd = steady_clock::now();
    _t = duration_cast<milliseconds>(tEnd - tStart).count();
    printf("Total number of global intersection: %lld, time used for SIB tree intersection: %lld ms!\n", res2, _t);
    // g_sib.print_cnt();
}

ull test_local_intersection(string file, Graph &g) {
    vector<pair<node, node>> pairs;
    read_pair_data(file, pairs, "local");
    SIB_Tree g_sib(g);
    ull res1 = 0, res2 = 0;
    
    auto tStart = steady_clock::now();
    for (auto k : pairs) {
        res1 += g.cnt_common_neighbors(k.first, k.second);
    }
    auto tEnd = steady_clock::now();
    ull _t = duration_cast<milliseconds>(tEnd - tStart).count();
    printf("Total number of local intersection: %lld, time used for merge intersection: %lld ms!\n", res1, _t);
    // printf("Times of comparisons in merge intersection: %lld !\n", g.get_cnt());

    tStart = steady_clock::now();
    for (auto k : pairs) {
        res2 += g_sib.count_intersection(k.first, k.second);
    }
    tEnd = steady_clock::now();
    _t = duration_cast<milliseconds>(tEnd - tStart).count();
    printf("Total number of local intersection: %lld, time used for SIB tree intersection: %lld ms!\n", res2, _t);
    // g_sib.print_cnt();
}

int main(int argc, char** argv) {
    string opt = argv[1];

    for (auto k : files) {
        printf("==============Graph %s ==================\n", k.first.c_str());
        Graph g(dict_path + k.second + ".txt");
        if (opt == "global") test_global_intersection(k.first, g);
        else test_local_intersection(k.first, g);
	}
	return 0;
}

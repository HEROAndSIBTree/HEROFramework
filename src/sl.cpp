#include"graph.hpp"
#include"SIBTree.hpp"

ull test_merge(Graph &g, const string &pattern){
    ull ans = 0, t_merge = 0;	

	steady_clock::time_point t_start, t_end;
	t_start = steady_clock::now();
	if (pattern == "4-cycle") ans = g.four_cycle_listing();
	if (pattern == "4-dimond") ans = g.four_dimond_listing();
	if (pattern == "4-clique") ans = g.four_clique_listing();
	t_end = steady_clock::now();
    t_merge = duration_cast<chrono::milliseconds>(t_end-t_start).count();

	printf("Time used for listing %s using merge intersection: %lld ms!\n", pattern.c_str(), t_merge);
	printf("Total number of %s: %lld !\n", pattern.c_str(), ans);	
	return t_merge;
}

ull test_SIB_Tree(Graph &g, const string &pattern){
	steady_clock::time_point t_start, t_end;
	t_start = steady_clock::now();
	SIB_Tree g_sib(g);
	t_end = steady_clock::now();
	ull t_used = duration_cast<chrono::milliseconds>(t_end - t_start).count();
	printf("Time used for creating multi level index: %lld ms!\n", t_used);
	ull ans = 0, t_vec = 0;
	
	t_start = steady_clock::now();
	if (pattern == "4-cycle") ans = g_sib.four_cycle_listing();
	if (pattern == "4-dimond") ans = g_sib.four_dimond_listing();
	if (pattern == "4-clique") ans = g_sib.four_clique_listing();
	t_end = steady_clock::now();
	t_vec = duration_cast<chrono::milliseconds>(t_end - t_start).count();

	printf("Time used for listing %s using SIB tree intersection: %lld ms!\n", pattern.c_str(), t_vec);
	printf("Total number of %s: %lld !\n", pattern.c_str(), ans);
	return t_vec;
}

void test_graph(string path, const string &pattern){
	auto t_start = steady_clock::now();
	Graph g(dict_path+path+".txt");
	auto t_end = steady_clock::now();
	ull t_used = duration_cast<chrono::milliseconds>(t_end - t_start).count();
	printf("Time used for loading graph: %lld ms!\n", t_used);
	// g.create_dag();

	printf("============Origin Order==============\n");
	vector<node> neis;
	ull t_merge = test_merge(g, pattern);
	ull t_sib = test_SIB_Tree(g, pattern);
	printf("Speed up: x%.3f!\n", (float)t_merge / (float)t_sib);
	
	for (auto s : reorder_files) {
        printf("============%s Order==============\n", s[0].c_str());
        Graph g_new_order(dict_path+s[0]+path+s[1]+".txt");
		// g_new_order.create_dag();
        t_sib = test_SIB_Tree(g_new_order, pattern);
	    printf("Speed up: x%.3f!\n", (float)t_merge / (float)t_sib);
    }
}

int main(int argc, char** argv) {
    string graphfile = argv[1];
	string pattern = argv[2];

    for (auto k : files) {
        if (k.first != graphfile) continue;
        printf("==============Graph %s ==================\n", k.first.c_str());
        test_graph(k.second, pattern);
	}
	return 0;
}

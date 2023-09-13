#include<chrono>
#include"graph.hpp"
#include"SIBTree.hpp"

using namespace std;
using namespace chrono;

ull test_merge(Graph &g){
	ull ans = 0, t_merge = 0;	

	steady_clock::time_point t_start, t_end;
	t_start = steady_clock::now();
	ans = g.cnt_tri_merge();
	t_end = steady_clock::now();
    t_merge = duration_cast<chrono::milliseconds>(t_end-t_start).count();
	

	printf("Time used for navie merge intersection: %lld ms!\n", t_merge);
	printf("Total number of common neighbors: %lld !\n", ans);
	//cout << "Total number of merge operation: " << g.get_cnt() << "!" << endl;	
	return t_merge;
}

ull test_SIB_Tree(Graph &g){
	steady_clock::time_point t_start, t_end;
	t_start = steady_clock::now();
	SIB_Tree g_sib(g);
	t_end = steady_clock::now();
	ull t_used = duration_cast<chrono::milliseconds>(t_end - t_start).count();
	printf("Time used for creating multi level index: %lld ms!\n", t_used);
	ull ans = 0, t_sib = 0;
	
	t_start = steady_clock::now();
	ans = g_sib.cnt_tri();
	t_end = steady_clock::now();
	t_sib = duration_cast<chrono::milliseconds>(t_end - t_start).count();
	
	// ans = g_sib.cnt_tri(t_sib);

	printf("Time used for SIB tree intersection: %lld ms!\n", t_sib);
	printf("Total number of common neighbors: %lld !\n", ans);
	//g_sib.print_cnt();
	return t_sib;
}

void test_graph(string path, float &avg_speedup){
	auto t_start = steady_clock::now();
	Graph g(dict_path+path+".txt");
	auto t_end = steady_clock::now();
	ull t_used = duration_cast<chrono::milliseconds>(t_end - t_start).count();
	printf("Time used for loading graph: %lld ms!\n", t_used);
	g.create_dag();
	
	printf("============Origin Order==============\n");
	vector<node> neis;
	ull t_merge = test_merge(g);
	ull t_sib = test_SIB_Tree(g);
	printf("Speed up: x%.3f!\n", (float)t_merge / (float)t_sib);

	
	for (auto s : reorder_files) {
        printf("============%s Order==============\n", s[0].c_str());
        Graph g_new_order(dict_path+s[0]+path+s[1]+".txt");
		g_new_order.create_dag();
        t_sib = test_SIB_Tree(g_new_order);
	    printf("Speed up: x%.3f!\n", (float)t_merge / (float)t_sib);
	    avg_speedup += (float)t_merge / (float)t_sib;
    }
}

int main(int argc, char **argv) {
	string graphfile = argv[1];
	float avg_speedup = 0;
	
	for (auto k : files) {
			if (graphfile != k.first) continue;
			printf("==============Graph %s ==================\n", k.first.c_str());
			test_graph(k.second, avg_speedup);
	}

	// printf("Average speed up: %.3f !\n", avg_speedup / files.size());
	return 0;
}

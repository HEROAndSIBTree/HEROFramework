#pragma once
#include<algorithm>
#include<boost/dynamic_bitset.hpp>
#include<chrono>
#include<fstream>
#include<iostream>
#include<map>
#include<numeric>
#include<queue>
#include<sstream>
#include<stdio.h>
#include<string>
#include<string.h>
#include<unordered_map>
#include<unordered_set>
#include<vector>


using namespace std;
using namespace chrono;

typedef long node;
typedef long weight;
typedef unsigned long long ull;

struct SIB_Tree_Node {
	int base;
	int next_ptr;
	ull val;
};


static string dict_path = "/root/benchmark/Set_Intersection/processed/";
static vector<pair<string, string>> files = {
		{"twitter", "/pro-twitter-81306-1342296"},
		{"gplus", "/pro-gplus-107614-12238285"},
		{"google", "/pro-web-google-916428-4322051"},
		{"youtube", "/pro-com-youtube-1157827-2987624"},
		{"berkstan", "/pro-web-berkstan-685230-6649470"},
		{"flickr", "/pro-flickr-links-1715255-15551249"},
		{"skitter", "/pro-as-skitter-1696415-11095298"},
		{"pokec", "/pro-soc-pokec-1632803-22301964"},
		{"livejournal", "/pro-soc-LJ-4847571-42851237"},
		{"wiki", "/pro-wiki-Talk-2394385-4659565"}
	};

static vector<vector<string>> reorder_files = {
		{"BFSR", "_BFSRorder", "_BFSR_newID"},
		{"DFS", "_DFSorder", "_DFS_newID"},
		{"Horder", "_Horder", "_Horder_newID"},
		{"MLOGGAPA", "_MLOGGAPAorder", "_MLOGGAPAorder_newID"},
		{"SB", "_SBorder", "_SB_newID"},
		{"HBGP", "_HBGPorder", "_HBGPorder_newID"}
		// {"Hierarchical_Graph_Partition", "_HGPorder", "_HGPorder_newID"}
	};

class Graph {
public:
	Graph() {};
	Graph(string path);
	Graph(const vector<int>& _inds, const vector<node>& _vals);
	~Graph();

	int bsearch(const node *nodes, int size_node, node target) const;
	node get_one_neighbor(node u, int k) const;
	node* get_neighbors(node u, int &nbr_cnt) const;

	int get_num_nodes() const;
	int get_number_of_neighbors(node u) const;
	int summary_degree();

	ull get_cnt();
	ull cnt_common_neighbors(node u, node v);
	ull cnt_common_neighbors(node u, const node* nodes, int size_node) const;
	ull cnt_common_neighbors(node u, const vector<node> &nodes) const;
	ull conditional_cnt_common_neighbor(node u, node v, node mu);
	ull get_common_neighbors(node u, const node* nodes, int size_node, node* res) const;
	ull get_common_neighbors(node u, const vector<pair<node, bool>> &P, vector<pair<node, bool>> &res) const;
	ull cnt_tri_merge();
	ull combo(ull a, ull b);
	ull four_cycle_listing();
	ull four_dimond_listing();
	ull four_clique_listing();
	void create_dag();
	void get_neighbors(node u, vector<node>& neis) const;
	void get_common_neighbors(node u, node v, vector<node>& res) const;
	void get_common_neighbors(node u, const vector<node> &P, vector<node> &res);
	void reorder(vector<int> &labels);
	void save(string path);

private:
	vector<vector<node>> adj_list;
	int* inds;
	node* vals;
	int num_nodes, num_edges;
	ull __cnt;
	ull t_intersection;
};
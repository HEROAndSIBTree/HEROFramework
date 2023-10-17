#include"graph.hpp"

struct Temp_Tree_Node{
	int base;
	ull val_s;
	ull val_p;
};

class SIB_Tree {
public:
	SIB_Tree(const Graph &g);
	~SIB_Tree();
	int bsearch(int start, int end, int target);
	node SelectPivot(const vector<Temp_Tree_Node> &S);
	bool is_edge(node u, node v);
	ull count_intersection(node u, node v);
	ull count_intersection(node u, const vector<Temp_Tree_Node> &S);
	ull count_intersection(node u, const int num_tree_nodes, const SIB_Tree_Node* tmp_vals);
	ull conditional_cuont_intersection(node u, node v, int mu);
	ull helper(node u, node v, int u_id, int v_id, int current_level);
	ull helper(node u, node v, int u_id, int v_id, int current_level, int mu);
	// ull helper(node u, const SIB_Tree_Node* tmp_vals, int u_id, int v_id, int current_level);
	ull cnt_tri();
	ull helper(node u, node v, int u_id, int v_id, int current_level, node* res, ull &cnt_res);
	ull helper(node u, node v, int u_id, int v_id, int current_level, SIB_Tree_Node* res, int& num_tree_nodes);
	ull helper(node u, const SIB_Tree_Node* tmp_vals, int u_id, int v_id, int current_level, SIB_Tree_Node* res, int& start);
	ull get_intersection(node u, node v, node* res);
	ull get_intersection(node u, node v, SIB_Tree_Node* res, int &num_tree_nodes);
	ull get_intersection(node u, const int ptr, const SIB_Tree_Node* tmp_vals, SIB_Tree_Node* res, int& num_tree_nodes);
	ull get_intersection_mix(node u, const vector<Temp_Tree_Node> &tmp_vals, vector<Temp_Tree_Node> &res);
	ull mc(int *&Vrank, vector<node> &new_id);
	ull four_cycle_listing();
	ull four_dimond_listing();
	ull four_clique_listing();
	void create_tmp_index(int &num_tree_nodes, SIB_Tree_Node* new_vals);
	void BKP(vector<Temp_Tree_Node> &tmp_vals, ull &p_cnt, ull &res);
	void print_cnt();
	void cnt_leaves();

private:
	int n, m, max_level, max_degree;
	ull bkp_cnt;
	int* inds;
	int* degrees;
	SIB_Tree_Node* multi_level_vals;
	// ull** tmp_result;
	// ull* tmp_result_cnt;
	vector<ull> cnt;
};
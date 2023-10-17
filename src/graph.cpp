#include"graph.hpp"
 
int Graph::bsearch(const node *nodes, int size_node, node target) const {
	if (size_node == 0 || nodes[0] > target) return 0;
	if (nodes[size_node - 1] < target) return size_node - 1;
	int left = 0, right = size_node - 1;
	while (right - left > 1) {
		int tmp = (left + right) >> 1;
		if (nodes[tmp] == target) return tmp;
		else if (nodes[tmp] < target) left = tmp;
		else right = tmp;
	}
	return left;
}

bool find_next(ifstream& g, const string& pat, string& line) {
	while (getline(g, line)) {
		if (pat == line) return true;
	}
	return false;
}

Graph::Graph(string path) {
	ifstream g(path);
	string line;
	int a;
	unordered_map<node, unordered_set<node>> tmp_nbrs;
	num_nodes = 0;
	num_edges = 0;
	__cnt = 0;
	while (getline(g, line)) {
		if (line[0] == '%') continue;
		node u, v;
		stringstream(line).ignore(0, ' ') >> u >> v;
		if (u == v) continue;
		if (!tmp_nbrs.count(u)) {
			tmp_nbrs.insert({u, {}});
			num_nodes = max(num_nodes, (int)u+1);
		}
		if (!tmp_nbrs.count(v)) {
			tmp_nbrs.insert({v, {}});
			num_nodes = max(num_nodes, (int)v+1);
		}
		if (!tmp_nbrs[v].count(u)) {
			tmp_nbrs[v].insert(u);
			num_edges++;
		}
		if (!tmp_nbrs[u].count(v)) {
			tmp_nbrs[u].insert(v);
			num_edges++;
		};
	}
	adj_list.resize(num_nodes);

	inds = (int*)malloc((num_nodes+1) * sizeof(int));
	vals = (node*)malloc((num_edges) * sizeof(node));
	int cnt = 0;
	for (int i = 0; i < num_nodes; ++i){
		inds[i] = cnt;
		for (auto j : tmp_nbrs[i]) adj_list[i].push_back(j);
		sort(adj_list[i].begin(), adj_list[i].end());
		for (auto j : adj_list[i]) {
			vals[cnt++] = j;
		}
	}
	inds[num_nodes] = cnt;
	t_intersection = 0;
	printf("Number of nodes: %d, number of edges: %d!\n", num_nodes, num_edges);
}

Graph::~Graph() {
	free(inds);
	free(vals);
}

ull Graph::cnt_common_neighbors(node u, node v) {
	int u_ind = 0, v_ind = 0;
	ull ans = 0;
	while(u_ind < adj_list[u].size() && v_ind < adj_list[v].size()) {
		// __cnt++;
		if (adj_list[u][u_ind] == adj_list[v][v_ind]) {
			ans++;
			u_ind++;
			v_ind++;
		}
		else if (adj_list[u][u_ind] < adj_list[v][v_ind]) u_ind++;
		else v_ind++;
	}
	return ans;
}

ull Graph::conditional_cnt_common_neighbor(node u, node v, node mu) {
	int u_ind = 0, v_ind = 0;
	ull ans = 0;
	while(u_ind < adj_list[u].size() && v_ind < adj_list[v].size()) {
		// __cnt++;
		if (adj_list[u][u_ind] < mu) u_ind++;
		else if (adj_list[v][v_ind] < mu) v_ind++;
		else if (adj_list[u][u_ind] == adj_list[v][v_ind]) {
			ans++;
			u_ind++;
			v_ind++;
		}
		else if (adj_list[u][u_ind] < adj_list[v][v_ind]) u_ind++;
		else v_ind++;
	}
	return ans;
}

// ull Graph::conditional_cnt_common_neighbor(node u, node v, node mu) {
// 	int u_ind = 0, v_ind = 0;
// 	ull ans = 0;
// 	while(u_ind < adj_list[u].size() && v_ind < adj_list[v].size()) {
// 		// __cnt++;
// 		if (adj_list[u][u_ind] > mu || adj_list[v][v_ind] > mu) break;
// 		else if (adj_list[u][u_ind] == adj_list[v][v_ind]) {
// 			ans++;
// 			u_ind++;
// 			v_ind++;
// 		}
// 		else if (adj_list[u][u_ind] < adj_list[v][v_ind]) u_ind++;
// 		else v_ind++;
// 	}
// 	return ans;
// }

void Graph::get_common_neighbors(node u, node v, vector<node>& res) const {
	res.clear();
	int u_ind = 0, v_ind = 0;
	while(u_ind < adj_list[u].size() && v_ind < adj_list[v].size()) {
		if (adj_list[u][u_ind] == adj_list[v][v_ind]) {
			res.emplace_back(adj_list[u][u_ind]);
			u_ind++;
			v_ind++;
		}
		else if (adj_list[u][u_ind] < adj_list[v][v_ind]) u_ind++;
		else v_ind++;
	}
}

void Graph::create_dag(){
	for(int i = 0; i < adj_list.size(); ++i) {
		adj_list[i].clear();
		for (int j = inds[i]; j < inds[i+1]; ++j) {
			if (i > vals[j]) adj_list[i].push_back(vals[j]);
		}	
		if (!adj_list[i].empty()) sort(adj_list[i].begin(), adj_list[i].end());
	}
	int cnt_edge = 0;
	for (int i = 0; i < num_nodes; ++i) {
		inds[i] = cnt_edge;
		for (auto u : adj_list[i]) {
			vals[cnt_edge++] = u;
		}
	}
	inds[num_nodes] = cnt_edge;
}

void Graph::get_common_neighbors(node u, const vector<node> &P, vector<node> &res) {
	int u_ind = 0, v_ind = 0;
	while(u_ind < adj_list[u].size() && v_ind < P.size()) {
		if (adj_list[u][u_ind] == P[v_ind]) {
			res.emplace_back(adj_list[u][u_ind]);
			u_ind++;
			v_ind++;
		}
		else if (adj_list[u][u_ind] < P[v_ind]) u_ind++;
		else v_ind++;
	}	
}

ull Graph::cnt_common_neighbors(node u, const node* nodes, int size_node) const {
	int u_ind = 0, v_ind = 0;
	ull ans = 0;
	while (u_ind < adj_list[u].size() && v_ind < size_node) {
		if (adj_list[u][u_ind] == nodes[v_ind]) {
			ans++;
			u_ind++;
			v_ind++;
		}
		else if (adj_list[u][u_ind] < nodes[v_ind]) u_ind++;
		else v_ind++;
	}
	return ans;
} 

ull Graph::get_common_neighbors(node u, const node* nodes, int size_node, node* res) const {
	int u_ind = 0, v_ind = 0;
	ull cnt = 0;
	while (u_ind < adj_list[u].size() && v_ind < size_node) {
		if (adj_list[u][u_ind] == nodes[v_ind]) {
			res[cnt++] = nodes[v_ind];
			u_ind++;
			v_ind++;
		}
		else if (adj_list[u][u_ind] < nodes[v_ind]) u_ind++;
		else v_ind++;
	}
	return cnt;
}

ull Graph::get_common_neighbors(node u, const vector<pair<node, bool>> &P, vector<pair<node, bool>> &res) const{
	int u_ind = 0, v_ind = 0;
	ull cnt = 0;
	while (u_ind < adj_list[u].size() && v_ind < P.size()) {
		if (adj_list[u][u_ind] == P[v_ind].first) {
			res.push_back(P[v_ind]);
			if (P[v_ind].second) cnt++;
			u_ind++;
			v_ind++;
		}
		else if (adj_list[u][u_ind] < P[v_ind].first) u_ind++;
		else v_ind++;
	}
	return cnt;
}

ull Graph::cnt_common_neighbors(node u, const vector<node> &nodes) const {
	int u_ind = 0, v_ind = 0;
	ull ans = 0;
	while (u_ind < adj_list[u].size() && v_ind < nodes.size()) {
		if (adj_list[u][u_ind] == nodes[v_ind]) {
			ans++;
			u_ind++;
			v_ind++;
		}
		else if (adj_list[u][u_ind] < nodes[v_ind]) u_ind++;
		else v_ind++;
	}
	return ans;	
}

void Graph::get_neighbors(node u, vector<node>& neis) const {
	if (!neis.empty()) neis.clear();
	neis.insert(neis.begin(), adj_list[u].begin(), adj_list[u].end());
}

node* Graph::get_neighbors(node u, int &nbr_cnt) const {
	nbr_cnt = inds[u+1] - inds[u];
	return vals + inds[u];
}

node Graph::get_one_neighbor(node u, int k) const {
	return adj_list[u][k];
}

int Graph::get_number_of_neighbors(node u) const {
	return adj_list[u].size();
}

int Graph::get_num_nodes() const{
	return num_nodes;
}

int Graph::summary_degree() {
	unordered_map<int, int> degree_cnt;
	int max_degree = 0;
	for (auto v : adj_list) {
		degree_cnt[v.size()]++;
		max_degree = max(max_degree, (int)v.size());
	}
	for (int i = 0; i < max_degree; ++i) {
		if (degree_cnt.count(i)) 
			printf("Degree %d: %d!\n", i, degree_cnt[i]);
	}
	return max_degree;
}

ull Graph::get_cnt(){
	return __cnt;
}

ull Graph::cnt_tri_merge() {
	ull res = 0;
	for (int i = 0; i < num_nodes; ++i) {
		if (adj_list[i].empty()) continue;
		for (auto j : adj_list[i]) {
			res += cnt_common_neighbors(i, j);
		}
	}
	return res;
}

ull Graph::combo(ull a, ull b) {
	ull  ans = 1;
	for (ull i = 0; i < b; ++i) {
		ans *= (a-i);
	}
	for (ull i = 1; i <= b; ++i) {
		ans /= i;
	}
	return ans;
}

ull Graph::four_cycle_listing() {
	ull res = 0;
	for (int i = 0; i < num_nodes; ++i) {
		for (int j = 0; j < adj_list[i].size(); ++j) {
			if (adj_list[i][j] < i) continue;
			for (int k = 0; k < adj_list[i].size(); ++k) {
				if (k <= j) continue;
				ull tmp = conditional_cnt_common_neighbor(adj_list[i][j], adj_list[i][k], i);
				// ull tmp = cnt_common_neighbors(adj_list[i][j], adj_list[i][k]);
				if (tmp > 1) res += tmp-1;
			}
		}
	}
	return res;
}

ull Graph::four_dimond_listing() {
	ull res = 0;
	for (int i = 0; i < num_nodes; ++i) {
		for (auto j : adj_list[i]) {
			if (j <= i) continue;
			ull tmp = cnt_common_neighbors(i, j);
			res += (tmp - 1) * tmp / 2;
		}
	}
	return res;
}

ull Graph::four_clique_listing() {
	ull res = 0;
	vector<node> tmp;
	for (int i = 0; i < num_nodes; ++i) {
		for (auto j : adj_list[i]) {
			get_common_neighbors(i, j, tmp);
			for (auto k : tmp) {
				res += cnt_common_neighbors(k, tmp);
			}
		}
	}
	return res;
}

void Graph::reorder(vector<int> &labels){
	vector<bool> tmp(num_nodes, false);
	for(int i = 0; i < num_nodes; ++i) {
		if (tmp[labels[i]]) printf("Error on node %d! Already has node with label %d!\n", i, labels[i]);
		else tmp[labels[i]] = true;
		adj_list[labels[i]].clear();
		for (int j = inds[i]; j < inds[i+1]; ++j) {
			adj_list[labels[i]].push_back(labels[vals[j]]);
		}	
		if (!adj_list[labels[i]].empty()) sort(adj_list[labels[i]].begin(), adj_list[labels[i]].end());
	}
	int cnt_edge = 0;
	for (int i = 0; i < num_nodes; ++i) {
		inds[i] = cnt_edge;
		for (auto u : adj_list[i]) {
			vals[cnt_edge++] = u;
		}
	}
	inds[num_nodes] = cnt_edge;
	num_edges = cnt_edge;
}

void Graph::save(string path) {
	ofstream out(path);
	for (int i = 0; i < num_nodes; i++) {
		for (int j = inds[i]; j < inds[i+1]; j++) {
			if (i < vals[j]) out << i  << " " << vals[j]  << endl;
		}
	}
	out.close();
}
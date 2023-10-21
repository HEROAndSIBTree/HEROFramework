#include"SIBTree.hpp"

SIB_Tree::SIB_Tree(const Graph& g) {
	bkp_cnt = 0;
	n = g.get_num_nodes();
	if (n < (1 << 18)) max_level = 3;
	else max_level = 4;
	cnt.resize(max_level);
	vector<node> neis;
	inds = (int*)malloc((n + 1) * sizeof(int));
	degrees = (int*)malloc(n * sizeof(int));
	vector<SIB_Tree_Node> tmp_vals;
	max_degree = 0;
	for (node u = 0; u < n; ++u) {
		int cnt_ind = u;
		inds[cnt_ind++] = tmp_vals.size();
		g.get_neighbors(u, neis);
		degrees[u] = neis.size();
		max_degree = max(max_degree, degrees[u]);
		if (neis.empty()) continue;
		int k = neis[0] >> 6;
		int right = ((k + 1) << 6) - 1;
		ull val = 0;
		for (auto v : neis) {
			// 注意在执行k-clique listing任务时不能忽略degree-one结点！
			// if (g.get_number_of_neighbors(v) < 2) continue;
			if (v > right) {
				tmp_vals.push_back({ k, -1, val});
				k = v >> 6;
				val = 0;
				right = ((k + 1) << 6) - 1;
			}
			int tmp = v & 63;
			val |= static_cast <ull> (1) << tmp;
		}
		tmp_vals.push_back({ k, -1, val});

		int left = inds[cnt_ind - 1];
		int tmp_right = tmp_vals.size();
		int ptr = left;
		k = tmp_vals[ptr].base >> 6;
		right = ((k + 1) << 6) - 1;
		val = 0;
		while (ptr < tmp_right) {
			if (tmp_vals[ptr].base > right) {
				tmp_vals.push_back({ k, left, val});
				k = tmp_vals[ptr].base >> 6;
				val = 0;
				left = ptr;
				right = ((k + 1) << 6) - 1;
			}
			int tmp = tmp_vals[ptr].base % 64;
			val |= static_cast <ull> (1) << tmp;
			ptr++;
		}
		tmp_vals.push_back({ k, left, val});

		left = tmp_right;
		tmp_right = tmp_vals.size();
		ptr = left;
		k = tmp_vals[ptr].base >> 6;
		right = ((k + 1) << 6) - 1;
		val = 0;
		while (ptr < tmp_right) {
			if (tmp_vals[ptr].base > right) {
				tmp_vals.push_back({ k, left, val});
				k = tmp_vals[ptr].base >> 6;
				val = 0;
				left = ptr;
				right = ((k + 1) << 6) - 1;
			}
			int tmp = tmp_vals[ptr].base % 64;
			val |= static_cast <ull> (1) << tmp;
			ptr++;
		}
		tmp_vals.push_back({ k, left, val});

		if (max_level == 4) {
			left = tmp_right;
			tmp_right = tmp_vals.size();
			ptr = left;
			val = 0;
			while (ptr < tmp_right) {
				int tmp = tmp_vals[ptr].base;
				val |= static_cast <ull> (1) << tmp;
				ptr++;
			}
			tmp_vals.push_back({0, left, val});
		}
	}
	inds[n] = tmp_vals.size();
	m = tmp_vals.size();
	multi_level_vals = (SIB_Tree_Node*)malloc((m + 1) * sizeof(SIB_Tree_Node));
	for (int i = 0; i < m; ++i) {
		multi_level_vals[i] = tmp_vals[i];
	}

	// tmp_result = (ull**)malloc((max_level - 1) * sizeof(ull*));
	// tmp_result_cnt = (ull*)malloc((max_level - 1) * sizeof(ull));

	// for (int i = 0; i < 3; ++i) {
	// 	tmp_result[i] = (ull*)malloc(max_degree * sizeof(ull));
	// 	tmp_result_cnt[i] = 0;
	// }
}

SIB_Tree::~SIB_Tree() {
	free(inds);
	free(multi_level_vals);
	free(degrees);
	// free(tmp_result_cnt);
	// for (int i = 0; i < 3; ++i) free(tmp_result[i]);
	// free(tmp_result);
}

void SIB_Tree::create_tmp_index(int &num_tree_nodes, SIB_Tree_Node* new_vals) 
{
	if (num_tree_nodes == 0) return;
	auto start = steady_clock::now();
	int cnt_val = 0;
	int tmp_right = 0;

	for(int i = 0; i < max_level; ++i) {
		if (i == 0) {
			cnt_val = num_tree_nodes;
		}
		else {
			int left = tmp_right;
			tmp_right = cnt_val;
			int ptr = left;
			int k = new_vals[ptr].base >> 6;
			int right = ((k + 1) << 6) - 1;
			ull val = 0;
			while (ptr < tmp_right) {
				if (new_vals[ptr].base > right) {
					new_vals[cnt_val++] = { k, left, val};
					k = new_vals[ptr].base >> 6;
					val = 0;
					left = ptr;
					right = ((k + 1) << 6) - 1;
				}
				int tmp = new_vals[ptr].base % 64;
				val |= static_cast <ull> (1) << tmp;
				ptr++;
			}
			new_vals[cnt_val++] = { k, left, val};
		}
	}
	num_tree_nodes = cnt_val;
	auto end = steady_clock::now();
	// cnt[0] += duration_cast<nanoseconds>(end - start).count();
	// cnt[1] ++;
}

bool SIB_Tree::is_edge(node u, node v){
	int ptr = inds[u+1]-1;
	ull val = multi_level_vals[ptr].val;
	for (int i = max_level-1; i > 0; --i) {
		int k = (v >> (6 * i)) & 63;
		if (val & (1 << k)) {
			ptr = multi_level_vals[ptr].next_ptr + __builtin_popcountll(val << (64-k));
			val = multi_level_vals[ptr].val;
		}
		else return false;
	}
	return val & (v & 63);
}

int SIB_Tree::bsearch(int start, int end, int target) {
	if (multi_level_vals[start].base >= target) return start;
	if (multi_level_vals[end].base <= target) return end;
	while (end - start > 1) {
		int tmp = (start + end) >> 1;
		if (multi_level_vals[tmp].base == target) return tmp;
		if (multi_level_vals[tmp].base < target) start = tmp;
		else end = tmp;
	}
	return end;
}

ull SIB_Tree::helper(node u, node v, int u_id, int v_id, int level) {
	// cnt[level]++;
	ull u_int = multi_level_vals[u_id].val, v_int = multi_level_vals[v_id].val;
	ull ans_int = u_int & v_int;
	if (ans_int == 0) return 0;
	ull ans = 0;
	if (level > 0) {
		if (ans_int == 1) {
			ans += helper(u ,v, multi_level_vals[u_id].next_ptr, multi_level_vals[v_id].next_ptr, level - 1);
		}
		else while (ans_int) {
			int tmp = __builtin_clzll(ans_int) + 1;
			ans_int = ans_int << tmp;
			u_int = u_int << tmp;
			v_int = v_int << tmp;
			ans += helper(u, v, 
						multi_level_vals[u_id].next_ptr + __builtin_popcountll(u_int), 
						multi_level_vals[v_id].next_ptr + __builtin_popcountll(v_int), level - 1);
		}
	}
	else {
		ans += __builtin_popcountll(ans_int);
	}	
	return ans;
}

ull SIB_Tree::helper(node u, node v, int u_id, int v_id, int level, int mu) {
	// cnt[level]++;
	int base = multi_level_vals[u_id].base;
	// if (base < mu >> (6*(level+1))) return 0;
	ull u_int = multi_level_vals[u_id].val, v_int = multi_level_vals[v_id].val;
	ull ans_int = u_int & v_int;
	if (ans_int == 0) return 0;
	ull ans = 0;
	if (level > 0) {
		if (ans_int == 1) {
			ans += helper(u ,v, multi_level_vals[u_id].next_ptr, multi_level_vals[v_id].next_ptr, level - 1, mu);
		}
		else while (ans_int) {
			int tmp = __builtin_clzll(ans_int) + 1;
			ans_int = ans_int << tmp;
			u_int = u_int << tmp;
			v_int = v_int << tmp;
			ans += helper(u, v, 
						multi_level_vals[u_id].next_ptr + __builtin_popcountll(u_int), 
						multi_level_vals[v_id].next_ptr + __builtin_popcountll(v_int), level - 1, mu);
		}
	}
	else {
		// if (base > (mu >> 6)) return 0;
		// else if (base < (mu >> 6)) ans += __builtin_popcountll(ans_int);
		// else ans += __builtin_popcountll(ans_int << (63 - (mu & 63)));
		if (base < (mu >> 6)) return 0;
		else if (base > (mu >> 6)) ans += __builtin_popcountll(ans_int);
		else ans += __builtin_popcountll(ans_int >> (mu & 63));
	}	
	return ans;
}

// ull SIB_Tree::helper(node u, const SIB_Tree_Node* tmp_vals, int u_id, int v_id, int level)
// {
// 	ull u_int = multi_level_vals[u_id].val, v_int = tmp_vals[v_id].val;
// 	ull ans_int = u_int & v_int;
// 	if (ans_int == 0) return 0;
// 	ull ans = 0;
// 	if (level > 0) {
// 		if (ans_int == 1) {
// 			ans += helper(u, tmp_vals, multi_level_vals[u_id].next_ptr,
// 								tmp_vals[v_id].next_ptr, level - 1);
// 		}
// 		else while (ans_int) {
// 			int tmp = __builtin_clzll(ans_int) + 1;
// 			ans_int = ans_int << tmp;
// 			u_int = u_int << tmp;
// 			v_int = v_int << tmp;
// 			ans += helper(u, tmp_vals, 
// 						multi_level_vals[u_id].next_ptr + __builtin_popcountll(u_int), 
// 						tmp_vals[v_id].next_ptr + __builtin_popcountll(v_int), level - 1);
// 		}
// 	}
// 	else {
// 		ans += __builtin_popcountll(ans_int);
// 	}	
// 	return ans;
// }

ull SIB_Tree::helper(node u, node v, int u_id, int v_id, int level, node* res, ull &cnt_res) {
	ull u_int = multi_level_vals[u_id].val, v_int = multi_level_vals[v_id].val;
	ull ans_int = u_int & v_int;
	if (ans_int == 0) return 0;
	if (level > 0) {
		if (ans_int == 1) {
			helper(u ,v, multi_level_vals[u_id].next_ptr, multi_level_vals[v_id].next_ptr, level - 1, res, cnt_res);
		}
		else while (ans_int) {
			int tmp = __builtin_ctzll(ans_int);
			ans_int &= ans_int - 1;
			if (tmp == 0) helper(u, v, multi_level_vals[u_id].next_ptr, multi_level_vals[v_id].next_ptr, level - 1, res, cnt_res);
			else helper(u, v, multi_level_vals[u_id].next_ptr + __builtin_popcountll(u_int << (64 - tmp)),
					multi_level_vals[v_id].next_ptr + __builtin_popcountll(v_int << (64 - tmp)), level - 1, res, cnt_res);
		}
	}
	else {
		int k = multi_level_vals[u_id].base << 6;
		while (ans_int) {
			res[cnt_res++] = k + __builtin_ctzll(ans_int);
			ans_int &= (ans_int - 1);
		}
	}	
	return cnt_res;
}

ull SIB_Tree::helper(node u,  node v, int u_id, int v_id, int level, SIB_Tree_Node* res, int& num_tree_nodes)
{
	ull u_int = multi_level_vals[u_id].val, v_int = multi_level_vals[v_id].val;
	ull ans_int = u_int & v_int, ans_cnt = 0;
	if (ans_int == 0) return 0;
	if (level > 0) {
		if (ans_int == 1) {
			ans_cnt += helper(u , v, multi_level_vals[u_id].next_ptr, multi_level_vals[v_id].next_ptr, level - 1, res, num_tree_nodes);
		}
		else while (ans_int) {
			int tmp = 64 - __builtin_ctzll(ans_int);
			ans_int &= ans_int-1;
			if (tmp == 64) 
				ans_cnt += helper(u, v, multi_level_vals[u_id].next_ptr, multi_level_vals[v_id].next_ptr, level-1, res, num_tree_nodes);
			else ans_cnt += helper(u, v, multi_level_vals[u_id].next_ptr + __builtin_popcountll(u_int<<tmp), 
					multi_level_vals[v_id].next_ptr + __builtin_popcountll(v_int<<tmp), level - 1, res, num_tree_nodes);
		}
	}
	else {
		int k = multi_level_vals[u_id].base;
		res[num_tree_nodes++] = {k, -1, ans_int};
		ans_cnt += __builtin_popcountll(ans_int);
	}
	return ans_cnt;
}

ull SIB_Tree::helper(node u,  const SIB_Tree_Node* tmp_vals, 
				int u_id, int v_id, int level, SIB_Tree_Node* res, int& start)
{
	ull u_int = multi_level_vals[u_id].val, v_int = tmp_vals[v_id].val;
	ull ans_int = u_int & v_int, ans_cnt = 0;
	if (ans_int == 0) return 0;
	if (level > 0) {
		if (ans_int == 1) {
			ans_cnt += helper(u , tmp_vals, multi_level_vals[u_id].next_ptr, tmp_vals[v_id].next_ptr, level - 1, res, start);
		}
		else while (ans_int) {
			int tmp = 64 - __builtin_ctzll(ans_int);
			ans_int &= ans_int-1;
			if (tmp == 64) 
				ans_cnt += helper(u, tmp_vals, multi_level_vals[u_id].next_ptr, tmp_vals[v_id].next_ptr, level-1, res, start);
			else ans_cnt += helper(u, tmp_vals, multi_level_vals[u_id].next_ptr + __builtin_popcountll(u_int<<tmp), 
					tmp_vals[v_id].next_ptr + __builtin_popcountll(v_int<<tmp), level - 1, res, start);
		}
	}
	else {
		int k = multi_level_vals[u_id].base;
		res[start++] = {k, -1, ans_int};
		ans_cnt += __builtin_popcountll(ans_int);
	}
	return ans_cnt;
}

ull SIB_Tree::count_intersection(node u, node v) {
	if (inds[u] == inds[1+u] || inds[v] == inds[1+v]) return 0;
	return helper(u, v, inds[u+1]-1, inds[v+1]-1, max_level - 1);
}

ull SIB_Tree::count_intersection(node u, const int num_tree_nodes, const SIB_Tree_Node* tmp_vals)
{
	int u_ind = inds[u], v_ind = 0;
	ull res = 0;
	while (u_ind < inds[u+1] && v_ind < num_tree_nodes) {
		if (multi_level_vals[u_ind].next_ptr >= 0) break;
		if (multi_level_vals[u_ind].base == tmp_vals[v_ind].base) {
			ull val = multi_level_vals[u_ind].val & tmp_vals[v_ind].val;
			if (val) {
				res += __builtin_popcountll(val);
			}
			u_ind++;
			v_ind++;
		}
		else if (multi_level_vals[u_ind].base < tmp_vals[v_ind].base) u_ind++;
		else v_ind++;
	}
	return res;
}

ull SIB_Tree::conditional_cuont_intersection(node u, node v, int mu){
	if (inds[u] == inds[1 + u] || inds[v] == inds[1 + v]) return 0; 
	return helper(u, v, inds[u+1] - 1, inds[v+1] - 1, max_level - 1, mu);
}

ull SIB_Tree::get_intersection(node u, node v, node* res) {
	if (inds[u] == inds[1 + u] || inds[v] == inds[1 + v]) return 0; 
	ull ans = 0;
	helper(u, v, inds[u+1] - 1, inds[v+1] - 1, max_level - 1, res, ans);
	return ans;
}

ull SIB_Tree::get_intersection(node u, node v, SIB_Tree_Node* res, int &num_tree_nodes) {
	num_tree_nodes = 0;
	if (inds[u] == inds[u+1] || inds[v] == inds[v+1]) return 0;
	ull ans = helper(u, v, inds[u+1]-1, inds[v+1]-1, max_level-1, res, num_tree_nodes);
	return ans;
}

ull SIB_Tree::get_intersection(node u, const int ptr, const SIB_Tree_Node* tmp_vals, SIB_Tree_Node* res, int& num_tree_nodes)
{
	if (inds[u] == inds[1 + u] || ptr == 0) return 0; 
	ull ans = helper(u, tmp_vals, inds[u+1] - 1, ptr - 1, max_level - 1, res, num_tree_nodes);
	return ans;
}

ull SIB_Tree::count_intersection(node u, const vector<Temp_Tree_Node> &tmp_vals) {
	int u_ind = inds[u], v_ind = 0;
	ull cnt_p = 0;
	while (u_ind < inds[u+1] && v_ind < tmp_vals.size()) {
		if (multi_level_vals[u_ind].next_ptr >= 0) break;
		if (multi_level_vals[u_ind].base == tmp_vals[v_ind].base) {
			ull val_p = multi_level_vals[u_ind].val & tmp_vals[v_ind].val_p;
			cnt_p += __builtin_popcountll(val_p);
			u_ind++;
			v_ind++;
		}
		else if (multi_level_vals[u_ind].base < tmp_vals[v_ind].base) u_ind++;
		else v_ind++;
	}
	return cnt_p;
}

ull SIB_Tree::get_intersection_mix(node u, const vector<Temp_Tree_Node> &tmp_vals, vector<Temp_Tree_Node> &res) {
	int u_ind = inds[u], v_ind = 0;
	ull cnt_p = 0;
	while (u_ind < inds[u+1] && v_ind < tmp_vals.size()) {
		if (multi_level_vals[u_ind].next_ptr >= 0) break;
		if (multi_level_vals[u_ind].base == tmp_vals[v_ind].base) {
			ull val_s = multi_level_vals[u_ind].val & tmp_vals[v_ind].val_s;
			if (val_s) {
				ull val_p = val_s & tmp_vals[v_ind].val_p;
				cnt_p += __builtin_popcountll(val_p);
				res.push_back({tmp_vals[v_ind].base, val_s, val_p});
			}
			u_ind++;
			v_ind++;
		}
		else if (multi_level_vals[u_ind].base < tmp_vals[v_ind].base) u_ind++;
		else v_ind++;
	}
	return cnt_p;
}

ull SIB_Tree::cnt_tri(){
	ull res = 0;
	for (int i = 0; i < n; i++) {
		if (degrees[i] < 2) continue;
		for (int j = inds[i]; j < inds[i+1]; ++j) {
			if (multi_level_vals[j].next_ptr >= 0) break;
			int k = multi_level_vals[j].base << 6;
			ull val = multi_level_vals[j].val;
			while (val > 0) {
				res += count_intersection(i, k + __builtin_ctzll(val));
				val &= (val-1);
			}	
		}
	}
	return res;	
}

node SIB_Tree::SelectPivot(const vector<Temp_Tree_Node> &S) {
	node u = 0;
	ull max_cnt = 0;
	for (auto k : S) {
		int base = k.base << 6;
		ull val = k.val_p;
		while (val) {
			node v = base + __builtin_ctzll(val);
			val &= val-1;
			ull cnt = count_intersection(v, S);
			if (cnt >= max_cnt) {
				u = v;
				max_cnt = cnt;
			} 
		}
	}
	return u;
}

void SIB_Tree::BKP(vector<Temp_Tree_Node> &tmp_vals, ull &p_cnt, ull &res) {
    if (p_cnt == 0) {
		if (tmp_vals.empty()) res++;
		return;
	}
	// printf("Start execute BKP!\n");
	node k;
	ull p_val;
	int vit1 = 0;
    while (tmp_vals[vit1].val_p == 0) vit1++;
    k = static_cast<node>(tmp_vals[vit1].base) << 6;
    p_val = tmp_vals[vit1].val_p;
    node u = k + __builtin_ctzll(p_val);  
	// node u = SelectPivot(tmp_vals);
	int u_ind = inds[u];
	vector<Temp_Tree_Node> NS;
	NS.reserve(tmp_vals.size());

	while (vit1 < tmp_vals.size()) {
		if (multi_level_vals[u_ind].next_ptr >= 0 ||
			tmp_vals[vit1].base < multi_level_vals[u_ind].base) {
			p_val = tmp_vals[vit1].val_p;
			k = tmp_vals[vit1].base << 6;
			while (p_val) {
				node v = k + __builtin_ctzll(p_val);
				// printf("v: %d!\n", v);
				NS.clear();
				p_cnt = get_intersection_mix(v, tmp_vals, NS);
				BKP(NS, p_cnt, res);
				tmp_vals[vit1].val_p &= ~((ull)1 << __builtin_ctzll(p_val));
				p_val &= p_val-1;
			}
			vit1++;
		}
		else if (tmp_vals[vit1].base == multi_level_vals[u_ind].base) {
			p_val = tmp_vals[vit1].val_p & (~multi_level_vals[u_ind].val);
			k = tmp_vals[vit1].base << 6;
			while (p_val) {
				node v = k + __builtin_ctzll(p_val);
				// printf("v: %d!\n", v);
				NS.clear();
				p_cnt = get_intersection_mix(v, tmp_vals, NS);
				BKP(NS, p_cnt, res);
				tmp_vals[vit1].val_p &= ~((ull)1 << __builtin_ctzll(p_val));
				p_val &= p_val-1;
			}
			vit1++;
			u_ind++;
		}
		else u_ind++;
	}
}

ull SIB_Tree::mc(int *&Vrank, vector<node> &new_id) {
	ull result = 0;
	for (int i = 0; i < n; i++) {
		int u = new_id[i];
		if (u >= n ) {
			// result++;
			continue;
		}
		vector<Temp_Tree_Node> S;
		S.reserve(inds[u+1] - inds[u]);
        	ull p_cnt = 0;
		for (int j = inds[u]; j < inds[u+1]; ++j) {
			if (multi_level_vals[j].next_ptr >= 0) break;
			int base = multi_level_vals[j].base;
			ull val = multi_level_vals[j].val;
			ull val_p = 0, val_s = 0;
			while (val) {
				if (Vrank[(base << 6) + __builtin_ctzll(val)] > Vrank[u]) {
						val_p |= (static_cast<ull>(1) << __builtin_ctzll(val));
						val_s |= (static_cast<ull>(1) << __builtin_ctzll(val));
						p_cnt++;
				}
				else val_s |= (static_cast<ull>(1) << __builtin_ctzll(val));
				val &= val-1;
			}
			S.push_back({base, val_s, val_p});
		}
        	BKP(S, p_cnt, result);
	}
	return result;
}

ull SIB_Tree::four_cycle_listing() {
	ull res = 0;
	// node* common_neis = (node*)malloc(n * sizeof(node));
	for (int i = 0; i < n; ++i) {
		for (int j = inds[i]; j < inds[i+1]; ++j) {
			if (multi_level_vals[j].next_ptr >= 0) break;
			int j_base = multi_level_vals[j].base << 6;
			ull j_val = multi_level_vals[j].val;
			while (j_val > 0) {
				int j_id = j_base + __builtin_ctzll(j_val);
				j_val &= j_val - 1;
				// if (j_id < i) continue;
				for (int k = inds[i]; k <= j; ++k) {
				// for (int k = j; k <= inds[i+1]; ++k) {	
					if (multi_level_vals[k].next_ptr >= 0) break;
					int k_base = multi_level_vals[k].base << 6;
					ull k_val = multi_level_vals[k].val;
					if (j_base == k_base) k_val ^= j_val;
					while(k_val > 0) {
						int k_id = k_base + __builtin_ctzll(k_val);
						k_val &= k_val - 1;
						if (k_id >= j_id) continue;
						ull tmp = count_intersection(j_id, k_id);
						// ull tmp = conditional_cuont_intersection(j_id, k_id, i);
						if (tmp > 1) res += tmp-1;
					}
				}
			}
		}
	}
	return res;
}

ull SIB_Tree::four_dimond_listing() {
	ull res = 0;
	for (int i = 0; i < n; ++i) {
		for (int j = inds[i]; j < inds[i+1]; ++j) {
			if (multi_level_vals[j].next_ptr >= 0) break;
			int j_base = multi_level_vals[j].base << 6;
			ull j_val = multi_level_vals[j].val;
			while (j_val > 0) {
				int j_id = j_base + __builtin_ctzll(j_val);
				j_val &= j_val - 1;
				if (j_id < i) continue;
				ull tmp = count_intersection(i, j_id);
				res += (tmp-1)*tmp/2;
			}
		}
	}
	return res;
}

ull SIB_Tree::four_clique_listing() {
	ull res = 0;
	SIB_Tree_Node* tmp = (SIB_Tree_Node*)malloc((max_degree) * sizeof(SIB_Tree_Node));
	int num_tree_nodes;
	for (int i = 0; i < n; ++i) {
		for(int j = inds[i]; j < inds[i+1]; ++j) {
			if (multi_level_vals[j].next_ptr >= 0) break;
			int j_base = multi_level_vals[j].base << 6;
			ull j_val = multi_level_vals[j].val;
			while (j_val > 0) {
				int j_id = j_base + __builtin_ctzll(j_val);
				j_val &= j_val - 1;
				get_intersection(i, j_id, tmp, num_tree_nodes);
				for (int k = 0; k < num_tree_nodes; ++k) {
					int k_base = tmp[k].base << 6;
					ull k_val = tmp[k].val;
					while (k_val > 0) {
						int k_id = k_base + __builtin_ctzll(k_val);
						k_val &= k_val - 1;
						res += count_intersection(k_id, num_tree_nodes, tmp);
					}
				}
			}
		}
	}
	return res;
}

void SIB_Tree::print_cnt(){
	// cout << "Total number of vec operation in each level: " << cnt[0] << ", " << cnt[1] << ", " << cnt[2] << "!" << endl;
	ull sum = accumulate(cnt.begin(), cnt.end(), 0); 
	printf("Times of comparisons in SIB tree intersection: %lld !\n", sum);
}

void SIB_Tree::cnt_leaves(){
	ull cnt = 0;
	for (int i = 0; i < n; ++i) {
		for (int j = inds[i]; j < inds[i+1]; ++j) {
			if (multi_level_vals[j].next_ptr >= 0) break;
			cnt++;
		}
	}
	printf("Number of Leaves: %lld, cost of memory: %lld MB!\n", cnt, cnt >> 16);
}

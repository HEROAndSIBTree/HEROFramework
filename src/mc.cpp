#include"graph.hpp"
#include"SIBTree.hpp"

using namespace std;
using namespace chrono;

void getDegOrder(Graph &G, int *&Vrank, int N) {
    int *order = new int[N];
    vector<node> neis;
    int *degrees = new int[N];
    map<int, unordered_set<int>> D;
    int d;
    for (int i = 0; i < N; i++)
    {
        d = G.get_number_of_neighbors(i);
        degrees[i] = d;
        D[d].insert(i);
    }
    vector<bool> mark(N, false);
    int marked = 0;
    int n;
    while (marked < N)
    {
        for (auto it = D.begin(); it != D.end(); it++)
        {
            if (!it->second.empty())
            {
                n = *(it->second.begin());
                break;
            }
        }
        mark[n] = true;
        d = degrees[n];
        D[d].erase(n);
        G.get_neighbors(n, neis);
        for (const auto &adj : neis)
        {
            if (!mark[n])
            {
                d = degrees[adj];
                D[d].erase(adj);
                D[d - 1].insert(adj);
                degrees[adj] = d - 1;
            }
        }
        Vrank[n] = marked;
        order[marked++] = n;
    }
}

void BKP(vector<pair<node, bool>> &S, Graph &g, ull &p_cnt, ull &res) {
    if(p_cnt == 0) {
        if (S.empty()) res++;
        return;
    }

    int vit1 = 0;
	int vit2 = 0;
    int ps = S.size();
    while (vit1 < ps && S[vit1].second == false) vit1++;
    if (vit1 == ps) return;
    node u = S[vit1].first;
    int ud = g.get_number_of_neighbors(u);
    vector<pair<node,bool>> NS;
    NS.reserve(ps);

	while (vit1 < ps) {
        if (S[vit1].second == false) vit1++;
		else if (vit2 == ud || S[vit1].first < g.get_one_neighbor(u, vit2)) {
			int v = S[vit1].first;
            NS.clear();
			p_cnt = g.get_common_neighbors(v, S, NS);
            BKP(NS, g, p_cnt, res);
            S[vit1].second = false;
            vit1++;
		}
		else if (S[vit1].first == g.get_one_neighbor(u, vit2)) {
            vit1++;
            vit2++;
        }
        else vit2++;
	}
    return;
}

node SelectPivot(const Graph &g, const vector<node> &P) {
    node u = P[0];
    ull max_cnt = 0;
    for (auto v : P){
        ull res = g.cnt_common_neighbors(v, P);
        if (res > max_cnt) {
            max_cnt = res;
            u = v;
        }
    }
    return u;
}

void BKP(vector<node> &P, vector<node> &X, Graph &g, ull &res) {
    if (P.empty()) {
		if (X.empty()) res++;
		return;
	}
    // node u = SelectPivot(g, P);
	node u = P[0];

	int vit1 = 0;
	int vit2 = 0;
    int ud = g.get_number_of_neighbors(u);
	int ps = P.size();
	int xs = X.size();

	while (vit1 != ps) {
		if (vit2 == ud || P[vit1] < g.get_one_neighbor(u, vit2)) {
			int v = P[vit1];
			int gvs = g.get_number_of_neighbors(v);
			vector<node> NP;
			NP.reserve(min(ps, gvs));
			vector<node> NX;
			NX.reserve(min(xs, gvs));
			g.get_common_neighbors(v, X, NX);
			g.get_common_neighbors(v, P, NP);
            // printf("v: %d!\n",v);
			BKP(NP, NX, g, res);
			P.erase(P.begin()+vit1);
			X.insert(lower_bound(X.begin(), X.end(), v), v);
			xs++;
			ps--;
		}
		else if (P[vit1] == g.get_one_neighbor(u, vit2)) {
            vit1++;
            vit2++;
        }
        else vit2++;
	}
}

void BKP_without_pivot(vector<node> &P, vector<node> &X, Graph &g, ull &res) {
    if (P.empty()) {
		if (X.empty()) res++;
		return;
	}
    // printf("BKP start!\n");

	int vit1 = 0;
	int ps = P.size();

	while (!P.empty()) {
        int v = P.back();
        int gvs = g.get_number_of_neighbors(v);
        vector<node> NP;
        NP.reserve(gvs);
        vector<node> NX;
        NX.reserve(gvs);
        g.get_common_neighbors(v, X, NX);
        g.get_common_neighbors(v, P, NP);
        // printf("v: %d!\n",v);
        BKP_without_pivot(NP, NX, g, res);
        P.pop_back();
        X.insert(lower_bound(X.begin(), X.end(), v), v);
	}
    // printf("BKP end!\n");
}

ull mc(Graph &g, int *&Vrank, bool pivot) {
    ull result = 0;
	for (int n = 0; n < g.get_num_nodes(); n++) {
		// int i = Vrank[n];
        vector<node> neis;
        g.get_neighbors(n, neis);
		vector<node> P;
		P.reserve(neis.size());
		vector<node> X;
		X.reserve(neis.size());
		for (auto j : neis) {
			if (Vrank[j] > Vrank[n]) {
                // if (P.size() < 100)
                    P.push_back(j);
            }
			else X.push_back(j);
		}
        if (pivot)  BKP(P, X, g, result);
        else BKP_without_pivot(P, X, g, result);
	}
	return result;
}

ull mc(Graph &g, int *&Vrank, bool pivot, vector<node> &new_id) {
    ull result = 0;
	for (int n = 0; n < g.get_num_nodes(); n+= 100) {
		// int i = Vrank[n];
        node u = new_id[n];
        vector<node> neis;
        g.get_neighbors(u, neis);
        if (neis.empty()) continue;
		vector<node> P;
		P.reserve(neis.size());
		vector<node> X;
		X.reserve(neis.size());
		for (auto j : neis) {
			if (Vrank[j] > Vrank[u]) {
                // if (P.size() < 100) 
                    P.push_back(j);
            }
			else X.push_back(j);
		}
        if (pivot)  BKP(P, X, g, result);
        else BKP_without_pivot(P, X, g, result);
	}
	return result;
}

void read_vector(string path, vector<node> &new_id, bool mode) {
    ifstream id(path);
    string line;
    if (mode) {
        node u, v;
        while (getline(id, line)) {
            stringstream(line).ignore(0, ' ') >> u >> v;
            new_id[u] = v;
        }
    }
    else {
        int cnt = 0;
        while (getline(id, line)) {
            new_id[cnt++] = stoi(line);
        }
    }
    
}

void test_graph(string path){
	auto t_start = chrono::steady_clock::now();
	Graph g(dict_path + path + ".txt");
	auto t_end = chrono::steady_clock::now();
    	ull t_used = duration_cast<chrono::milliseconds>(t_end - t_start).count();
	printf("Time used for loading graph: %lld ms!\n", t_used);
    	int k = g.get_num_nodes();
    	int *Vrank = new int[k];
    	getDegOrder(g, Vrank, k);
    
	printf("============Origin Order==============\n");
	SIB_Tree g_sib(g);
	vector<node> neis;
	vector<node> new_id(g.get_num_nodes(), -1);
	for (int i = 0; i < new_id.size(); ++i) new_id[i] = i;

	t_start = chrono::steady_clock::now();
	ull res = mc(g, Vrank, true);
	t_end = chrono::steady_clock::now();
	ull t_merge = chrono::duration_cast<chrono::milliseconds>(t_end - t_start).count();
	printf("Time used for mc using merge intersection: %lld ms!\n", t_merge);
	printf("Number of maximal cliques: %lld!\n", res);

	t_start = chrono::steady_clock::now();
	res = g_sib.mc(Vrank, new_id);
	t_end = chrono::steady_clock::now();
	ull t_bit = chrono::duration_cast<chrono::milliseconds>(t_end - t_start).count();
	printf("Time used for mc using SIB tree intersection: %lld ms!\n", t_bit);
	printf("Number of maximal cliques: %lld!\n", res);
	printf("Speed up: x%.3f!\n", (float)t_merge / (float)t_bit);
    
	for (auto s : reorder_files) {
		printf("============%s Order==============\n", s[0].c_str());
		Graph g_new_order(dict_path+s[0]+path+s[1]+".txt");
		vector<node> new_id(g.get_num_nodes(), -1);
		if (s[0] != "HBGP") read_vector(dict_path+s[0]+path+s[2]+".txt", new_id, true);
		else read_vector(dict_path+s[0]+path+s[2]+".txt", new_id, false);
		SIB_Tree g_sib(g_new_order);
		int *VNrank = new int[k];
		for (int i = 0; i < new_id.size(); ++i) {
		    VNrank[new_id[i]] = Vrank[i];
		}
		t_start = chrono::steady_clock::now();
	    	res = g_sib.mc(VNrank, new_id);
		t_end = chrono::steady_clock::now();
		t_bit = chrono::duration_cast<chrono::milliseconds>(t_end - t_start).count();
	    	printf("Time used for mc using SIB tree intersection: %lld ms!\n", t_bit);
		printf("Number of maximal cliques: %lld!\n", res);
	    	printf("Speed up: x%.3f!\n", (float)t_merge / (float)t_bit);
	}
}

int main(int argc, char **argv) {
    string graphfile = argv[1];
	for (auto k : files) {
        if (k.first != graphfile) continue;
        printf("==============Graph %s ==================\n", k.first.c_str());
        test_graph(k.second);
        printf("\n");
    }
	return 0;
}

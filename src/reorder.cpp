#include"reorder.hpp"

using namespace std;
// using namespace boost;

bool cmp(const pair<node, int>& x, const pair<node, int>& y) {
    if (x.second == y.second) return x.first < y.first;
    return x.second > y.second;
}

bool cmp2(const pair<node, int>& x, const pair<node, int>& y) {
    if (x.second == y.second) return x.first < y.first;
    return x.second < y.second;
}

void HBGP(const Graph &g, vector<int> &new_order, int level) {
    int n = g.get_num_nodes();
    new_order.resize(n);
    int part_size = PARTITION_SIZE[level];
    int sum_size = PARTITION_SIZE[level+1];
    int num_part = 64;
    int max_iter = (n / sum_size) + ((n & (sum_size-1)) ? 1 : 0);
    int left = 0, right = 0;
    vector<pair<node, int>> degrees;
    degrees.reserve(sum_size); 
    int *part_cnt = (int*)malloc(sizeof(int) * num_part);
    ull *nbr_labels = (ull*)malloc(sizeof(ull) * n);
    degrees.reserve(sum_size);
    vector<node> neis1;
    vector<node> neis2;

    for (int iter = 0; iter < max_iter; ++iter) {
        // printf("Max iteration: %d, current iteration: %d!\n", max_iter, iter);
        left = sum_size * iter;
        if (iter == max_iter - 1 && (n & (sum_size - 1))) {
            num_part = (n - left) / part_size;
            right = left + num_part * part_size;
        }
        else {
            right = left + sum_size;
        }


        degrees.clear();
        for (int i = left; i < right; ++i) {
            degrees.push_back({i, g.get_number_of_neighbors(i)});
        }
        sort(degrees.begin(), degrees.end(), cmp);
        Double_Linked_List deg(n, n);
        for (auto k : degrees) {
            deg.add(k.first - left);
        }

        memset(part_cnt, 0, num_part * sizeof(int));
        memset(nbr_labels, 0, sizeof(ull) * n);

        Linked_List_Heap candidates(num_part * degrees.size());
        int key, v_idx, p_idx;

        for (int i = 0; i < degrees.size(); ++i) {
            if (i == 0) {
                key = deg.pop_head();
                v_idx = key / num_part;
                p_idx = key % num_part;
                candidates.reset();
                for (int j = 0; j < num_part; ++j) candidates.del(v_idx * num_part + j);
            }
            else {
                key = candidates.pop();
                v_idx = key / num_part;
                p_idx = key % num_part;
                deg.del(v_idx);
                for (int j = 0; j < num_part; ++j) {
                    if (j == p_idx) continue;
                    candidates.del(v_idx * num_part + j);
                }
            }

            new_order[left + v_idx] = left + p_idx * part_size + part_cnt[p_idx];
            part_cnt[p_idx]++;
            if (part_cnt[p_idx] == part_size) {
                for (int j = 0; j < right - left; ++j) {
                    if (candidates.in_heap(j * num_part + p_idx))
                        candidates.del(j * num_part + p_idx);
                }
            }

            g.get_neighbors(left + v_idx, neis1);
            if (neis1.size() == 1) continue;
            for (auto u : neis1) {
                if (nbr_labels[u] & ((ull)1 << p_idx)) continue;
                nbr_labels[u] = p_idx;
                if (neis1.size() == 1) continue; 
                g.get_neighbors(u, neis2);
                for (auto w : neis2) {
                    if (w < left || w >= right) continue;
                    w = (w - left) * num_part + p_idx;
                    if (candidates.in_heap(w)) {
                        candidates.inc(w);
                    }
                }
            }  
        }
    }  
    if (right < n) {
        for (int i = right; i < n; ++i) new_order[i] = i;
    }
    free(nbr_labels); 
    free(part_cnt);
}

void save_vector(string path, const vector<int> &V) {
    ofstream f(path);
    for (auto v : V) {
        f << v << endl;
    }
    f.close();
}

int main() {
    for (int k = 0; k < files.size(); ++k) {
        printf("======================= Graph %s =====================\n", files[k].first.c_str());
        Graph g(dict_path + files[k].second + ".txt");
        vector<int> new_id;
        vector<int> final_id;
        vector<int> tmp;
        for (int i = 2; i >= 0; --i) {
            if (g.get_num_nodes() < PARTITION_SIZE[i]) continue;
            new_id.resize(g.get_num_nodes());
            HBGP(g, new_id, i);
            g.reorder(new_id);
            if (final_id.empty()) final_id = new_id;
            else {
                tmp.resize(new_id.size());
                for (int j = 0; j < new_id.size(); ++j) {
                    tmp[j] = new_id[final_id[j]];
                }
                swap(tmp, final_id);
            }
        }
        g.save(dict_path + "HBGP" + files[k].second + "_HBGPorder.txt");
        save_vector(dict_path + "HBGP" + files[k].second + "_HBGPorder_newID.txt",final_id);
    }

    return 0;
}
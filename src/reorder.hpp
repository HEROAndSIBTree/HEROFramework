#include"graph.hpp"
#include"util.hpp"

static vector<int> PARTITION_SIZE = {64, 4096, 262144, 16777216};

bool cmp(const pair<node, int>& x, const pair<node, int>& y);

bool cmp2(const pair<node, int>& x, const pair<node, int>& y);

void HBGP(const Graph &g, vector<int> &new_order, int level);
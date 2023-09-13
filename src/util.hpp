#include"graph.hpp"

struct List_Node {
    int state;
    int val;
    int prev, next;

    List_Node() {};
    List_Node(int _s, int _v, int _p, int _n): state(_s), val(_v), prev(_p), next(_n) {};
};

struct List_Header {
    int first, last;
    int label;

    List_Header() {};
    List_Header(int _f, int _l, int _lab): first(_f), last(_l), label(_lab) {};
};

struct List_Update_Info
{
    int key;
    int old_val;

    List_Update_Info() {};
    List_Update_Info(int _k, int _v): key(_k), old_val(_v) {};
};

class Linked_List_Heap{
public:
    Linked_List_Heap(int _capacity);
    ~Linked_List_Heap();

    int top();
    int pop();
    int pop_tail();
    int get_size();
    int get_val(int key);
    int get_next(int key);
    int get_top_val();
    int get_tail_val();

    void inc(int key);
    void inc(int key, int delta);
    void del(int key);
    void reset();
    void adjust();
    void print();
    void print_top(int k);

    bool in_heap(int key);
    bool is_top_zero();
    bool is_tail_zero();
    bool check();
    bool check_size();

private:
    const int RESET_LABEL_MASK = 0x7fffffff;
    const int UPDATE_LABEL_MASK = 0x80000000;

    int capacity = 0, size = 0, reset_label = 0;
    int head = -1, tail = -1;

    List_Node *linked_list;
    List_Header *headers;

    List_Update_Info *up_list;
    int up_list_cnt = 0;

    void update(int key, int old_val);
};

struct Double_Linked_Node
{
    int key;
    int prev, next;
    Double_Linked_Node(int _k, int _p, int _n): key(_k), prev(_p), next(_n) {};
};


class Double_Linked_List {
public:
    Double_Linked_List (int _capacity, int _range);
    ~Double_Linked_List();
    void add(int key);
    void del(int key);
    int get_head();
    int get_tail();
    int pop_head();
    int pop_tail();
    void print();
    void print_reverse();
private:
    Double_Linked_Node *nodes = NULL;
    int *key2pos = NULL;
    int node_idx;
    int head, tail;
    int capacity, range;
};

void align_malloc(void **memptr, size_t alignment, size_t size);
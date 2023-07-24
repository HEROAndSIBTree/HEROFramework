#include"util.hpp"

void align_malloc(void **memptr, size_t alignment, size_t size) {
    int malloc_flag = posix_memalign(memptr, alignment, size);
    if (malloc_flag) {
        printf("Posix memalign:\n");
        // system("pause");
        exit(0);
    }
}

Linked_List_Heap::Linked_List_Heap(int _capacity) {
    capacity = _capacity;
    align_malloc((void**)&linked_list, 32, sizeof(List_Node) * capacity);
    align_malloc((void**)&headers, 32, sizeof(List_Header) * capacity * 2);
    align_malloc((void**)&up_list, 32, sizeof(List_Update_Info) * capacity);

    size = capacity;
    for (int i = 0; i < size; ++i) {
        linked_list[i] = List_Node(0, 0, i-1, i+1);
        headers[i] = List_Header(-1, -1, 0);
    }
    linked_list[size-1].next = -1;
    headers[0].first = 0;
    headers[0].last = size-1;
    head = 0;
    tail = size-1;
    reset_label = 0;
    up_list_cnt = 0;
}

Linked_List_Heap::~Linked_List_Heap() {
    free(linked_list);
    free(headers);
    free(up_list);
}

void Linked_List_Heap::inc(int key) {
    List_Node& curr = linked_list[key];

    if ((curr.state & RESET_LABEL_MASK) != reset_label) {
        curr.val = 0;
        curr.state = reset_label;
    }
    curr.val++;

    if ((curr.state & UPDATE_LABEL_MASK) == 0) {
        up_list[up_list_cnt++] = List_Update_Info(key, curr.val - 1);
        curr.state |= UPDATE_LABEL_MASK;
    }
}

void Linked_List_Heap::inc(int key, int delta) {
    List_Node& curr = linked_list[key];

    if ((curr.state & RESET_LABEL_MASK) != reset_label) {
        curr.val = 0;
        curr.state = reset_label;
    }
    curr.val += delta;

    if ((curr.state & UPDATE_LABEL_MASK) == 0) {
        up_list[up_list_cnt++] = List_Update_Info(key, curr.val - delta);
        curr.state |= UPDATE_LABEL_MASK;
    }
}

int Linked_List_Heap::top() {
    adjust();
    return head;
}

int Linked_List_Heap::pop() {
    adjust();

    List_Node& curr = linked_list[head];
    int val = curr.val;
    if (curr.state != reset_label) val = 0;
    int key = head;

    curr.val = -1;
    head = curr.next;
    if (curr.next == -1) tail = -1;
    else linked_list[curr.next].prev = -1;

    List_Header& top_header = headers[val];
    if (top_header.first == top_header.last) top_header.first = top_header.last = -1;
    else top_header.first = head;

    size--;

    return key;
}

int Linked_List_Heap::pop_tail() {
    adjust();

    List_Node& curr = linked_list[tail];
    int val = curr.val;
    // printf("New node: %d, val: %d!\n", tail, val);
    if (curr.state != reset_label) val = 0;
    int key = tail;

    curr.val = -1;
    tail = curr.prev;
    if (curr.prev == -1) head = -1;
    else linked_list[curr.prev].next = -1;

    List_Header& tail_header = headers[val];
    if (tail_header.first == tail_header.last) tail_header.first = tail_header.last = -1;
    else tail_header.last = tail;

    size--;

    return key;
}

void Linked_List_Heap::del(int key) {
    adjust();

    List_Node& curr = linked_list[key];

    if (curr.val == -1) return;

    int val = curr.val;
    if (curr.state != reset_label) val = 0;

    curr.val = -1;
    if (curr.prev == -1) head = curr.next;
    else linked_list[curr.prev].next = curr.next;
    if (curr.next == -1) tail = curr.prev;
    else linked_list[curr.next].prev = curr.prev;

    List_Header& curr_header = headers[val];
    if (curr_header.first == curr_header.last) curr_header.first = curr_header.last = -1;
    else if (curr_header.first == key) curr_header.first = curr.next;
    else if (curr_header.last == key) curr_header.last = curr.prev;

    size--;
}

void Linked_List_Heap::reset() {
    reset_label++;
    up_list_cnt = 0;
    headers[0] = List_Header(head, tail, reset_label);
}

void Linked_List_Heap::adjust() {
    for (int i = 0; i < up_list_cnt; ++i) {
        update(up_list[i].key, up_list[i].old_val);
    }
    up_list_cnt = 0;
}

bool Linked_List_Heap::in_heap(int key) {
    return linked_list[key].val != -1;
}

bool Linked_List_Heap::is_top_zero() {
    adjust();
    if (linked_list[head].state != reset_label || linked_list[head].val == 0)
        return true;
    return false;
}

bool Linked_List_Heap::is_tail_zero() {
    adjust();
    if (linked_list[tail].state != reset_label || linked_list[tail].val == 0)
        return true;
    return false;
}

int Linked_List_Heap::get_val(int key) {
    return linked_list[key].val;
}

int Linked_List_Heap::get_next(int key) {
    return linked_list[key].next;
}

int Linked_List_Heap::get_top_val() {
    adjust();
    if (linked_list[head].state != reset_label) return 0;
    else return linked_list[head].val;
}

int Linked_List_Heap::get_tail_val() {
    adjust();
    if (linked_list[tail].state != reset_label) return 0;
    else return linked_list[tail].val;
}

int Linked_List_Heap::get_size() {
    return size;
}

void Linked_List_Heap::update(int key, int old_val) {
    List_Node& curr = linked_list[key];

    //update old header.
    List_Header& curr_header = headers[old_val];
    if (curr_header.first == curr_header.last) curr_header.first = curr_header.last = -1;
    else if (curr_header.first == key) curr_header.first = curr.next;
    else if (curr_header.last == key) curr_header.last = curr.prev;

    //split current node from linked list.
    if (curr.prev == -1) head = curr.next;
    else linked_list[curr.prev].next = curr.next;
    if (curr.next == -1) tail = curr.prev;
    else linked_list[curr.next].prev = curr.prev;

    //find the new header.
    List_Header& new_header = headers[curr.val];

    // if (curr.val < old_val) printf("Buble down: node %d!\n", key);

    if (new_header.label != reset_label || new_header.first == -1) {
        //case of empty header.
        new_header.first = new_header.last = key;
        new_header.label = reset_label;

        if (curr.val > old_val) {
            int right_val = curr.val - 1;
            while (right_val > old_val && (headers[right_val].label != reset_label ||
                    headers[right_val].first == -1)) right_val--;
            
            const List_Header& right_header = headers[right_val];
            if (right_header.first == -1) {
                if (curr.prev == -1) head = key;
                else linked_list[curr.prev].next = key;
                if (curr.next == -1) tail = key;
                else linked_list[curr.next].prev = key;
            }
            else {
                int right_node_pos = right_header.first;
                int left_node_pos = linked_list[right_node_pos].prev;
                curr.prev = left_node_pos;
                curr.next = right_node_pos;
                if (left_node_pos == -1) head = key;
                else linked_list[left_node_pos].next = key;
                linked_list[right_node_pos].prev = key;
            }
        }
        else {
            int left_val = curr.val + 1;
            while (left_val < old_val && (headers[left_val].label != reset_label ||
                    headers[left_val].last == -1)) left_val++;
            
            const List_Header& left_header = headers[left_val];
            if (left_header.last == -1) {
                if (curr.next == -1) tail = key;
                else linked_list[curr.next].prev = key;
                if (curr.prev == -1) head = key;
                else linked_list[curr.prev].next = key;
            }
            else {
                int left_node_pos = left_header.last;
                int right_node_pos = linked_list[left_node_pos].next;
                curr.prev = left_node_pos;
                curr.next = right_node_pos;
                if (right_node_pos == -1) tail = key;
                else linked_list[right_node_pos].prev = key;
                linked_list[left_node_pos].next = key;
            }
        }
    }
    else {
        int left_node_pos = new_header.last;
        int right_node_pos = linked_list[left_node_pos].next;
        curr.prev = left_node_pos;
        curr.next = right_node_pos;
        linked_list[left_node_pos].next = key;
        if (right_node_pos == -1) tail = key;
        else linked_list[right_node_pos].prev = key;
        new_header.last = key;
    }

    curr.state -= UPDATE_LABEL_MASK;
}

void Linked_List_Heap::print() {
    adjust();

    if (head == -1) {
        printf("Empty list!\n");
        return;
    }

    printf("list:");
    int curr_key = head;
    while (curr_key != -1) {
        int val = linked_list[curr_key].val;
        if ((linked_list[curr_key].state & RESET_LABEL_MASK != reset_label)) val = 0;
        printf("(%d, %d)\n", curr_key, val);
        curr_key = linked_list[curr_key].next;
    }
}

void Linked_List_Heap::print_top(int k){
    // adjust();
    if (head == -1) {
        printf("Empty list!\n");
        return;
    }
    printf("list:");
    int curr_key = head, cnt = 0;
    while (curr_key != -1 && cnt < k) {
        int val = linked_list[curr_key].val;
        // if ((linked_list[curr_key].state & RESET_LABEL_MASK != reset_label)) val = 0;
        printf("(%d, %d)\n", curr_key, val);
        curr_key = linked_list[curr_key].next;
        cnt++;
    }
}

bool Linked_List_Heap::check() {
    bool flag = true;
    int prev_val = INT32_MAX;
    int cnt = 0;
    int curr_key = head;
    while (curr_key != -1 && flag) {
        int val = linked_list[curr_key].val;
        if((linked_list[curr_key].state & RESET_LABEL_MASK) != reset_label) val = 0;

        if (++cnt > size) flag = false;
        if (prev_val < val) {
            flag = false;
            printf("val = %d, previous val = %d, key = %d!\n", val, prev_val, curr_key);
        }
        curr_key = linked_list[curr_key].next;
        prev_val = val;
    }

    flag = flag && (cnt == size);
    if (cnt != size) printf("cnt = %d, size = %d!\n", cnt, size);

    if (!flag) printf("Check failed!\n");

    return flag;
}

bool Linked_List_Heap::check_size() {
    bool flag = true;
    int cnt = 0;
    int curr_key = head;
    while (curr_key != -1 && flag) {
        if (++cnt > size) flag = false;
        curr_key = linked_list[curr_key].next;
    }

    flag = flag && (cnt == size);
    if (cnt != size) printf("cnt = %d, size = %d!\n", cnt, size);

    if (!flag) printf("Check size failed!\n");

    return flag;
}

Double_Linked_List::Double_Linked_List(int _capacity, int _range) :
            capacity(_capacity), range(_range)
{
    align_malloc((void**)& nodes, 32, sizeof(Double_Linked_Node) * capacity);
    align_malloc((void**)&key2pos, 32, sizeof(int) * capacity);
    memset(key2pos, -1, sizeof(int) * range);

    head = -1;
    tail = -1;
    node_idx = 0;
}

Double_Linked_List::~Double_Linked_List() {
    free(nodes);
    free(key2pos);
}

void Double_Linked_List::add(int key) {
    if (key2pos[key] != -1) return;
    key2pos[key] = node_idx;

    Double_Linked_Node& curr = nodes[node_idx];
    curr.key = key;
    curr.next = -1;
    curr.prev = tail;
    if (tail != -1) nodes[tail].next = node_idx;
    tail = node_idx;
    if (head == -1) head = node_idx;
    node_idx++;
}

void Double_Linked_List::del(int key) {
    if (key2pos[key] == -1) return;
    Double_Linked_Node& curr = nodes[key2pos[key]];
    if (curr.prev == -1) head = curr.next;
    else nodes[curr.prev].next = curr.next;
    if (curr.next == -1) tail = curr.prev;
    else nodes[curr.next].prev = curr.prev;
    key2pos[key] = -1;
}

int Double_Linked_List::get_head() {
    if (head != -1) return nodes[head].key;
    else return -1;
}

int Double_Linked_List::get_tail() {
    if (tail != -1) return nodes[tail].key;
    else return -1;
}

int Double_Linked_List::pop_head() {
    if (head == -1) return -1;
    Double_Linked_Node& curr = nodes[head];
    head = curr.next;
    if (curr.next == -1) tail = -1;
    else nodes[curr.next].prev = -1;
    key2pos[curr.key] = -1; 

    return curr.key;
}

int Double_Linked_List::pop_tail() {
    if (tail == -1) return -1;
    Double_Linked_Node& curr = nodes[tail];
    tail = curr.prev;
    if (curr.prev == -1) head = -1;
    else nodes[curr.prev].next = -1;
    key2pos[curr.key] = -1;

    return curr.key;
}

void Double_Linked_List::print() {
    if (head == -1) {
        printf("Empty list!\n");
        return;
    }

    int curr_idx = head;
    printf("list:");
    while (curr_idx != -1) {
        printf(" %d", nodes[curr_idx].key);
        curr_idx = nodes[curr_idx].next;
    }
    printf("\n");
}

void Double_Linked_List::print_reverse()
{
    if (tail == -1) {
        printf("empty list!\n");
        return;        
    }

    int curr_idx = tail;
    printf("inversed_list:");
    while (curr_idx != -1) {
        printf(" %d", nodes[curr_idx].key);
        curr_idx = nodes[curr_idx].prev;
    }
    printf("\n");
}
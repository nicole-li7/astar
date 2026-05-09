#include "AStar.h"
#include <cstring>

// ---- Node tracker: holds all created nodes; frees only what you tell it to ----
struct NodeTracker {
    INode** data;
    int     size;
    int     cap;

    NodeTracker() : data(0), size(0), cap(0) {}
    ~NodeTracker() { delete[] data; }

    void add(INode* n) {
        if (size == cap) {
            int c = cap ? cap * 2 : 64;
            INode** t = new INode*[c];
            for (int i = 0; i < size; ++i) t[i] = data[i];
            delete[] data;
            data = t; cap = c;
        }
        data[size++] = n;
    }

    void freeAll() {
        for (int i = 0; i < size; ++i) delete data[i];
        size = 0;
    }

    // Free every tracked node except those in the keep array.
    void freeExcept(INode** keep, int keepLen) {
        for (int i = 0; i < size; ++i) {
            bool found = false;
            for (int j = 0; j < keepLen; ++j)
                if (data[i] == keep[j]) { found = true; break; }
            if (!found) delete data[i];
        }
    }
};

// ---- Min-heap ordered by f = g + h ----
struct MinHeap {
    INode** data;
    int     size;
    int     cap;

    MinHeap() : data(0), size(0), cap(0) {}
    ~MinHeap() { delete[] data; }  // does NOT free nodes

    void push(INode* n) {
        if (size == cap) {
            int c = cap ? cap * 2 : 16;
            INode** t = new INode*[c];
            for (int i = 0; i < size; ++i) t[i] = data[i];
            delete[] data;
            data = t; cap = c;
        }
        data[size++] = n;
        for (int i = size - 1; i > 0;) {
            int p = (i - 1) / 2;
            if (data[p]->getF() > data[i]->getF()) {
                INode* tmp = data[p]; data[p] = data[i]; data[i] = tmp;
                i = p;
            } else break;
        }
    }

    INode* pop() {
        INode* top = data[0];
        data[0] = data[--size];
        for (int i = 0;;) {
            int l = 2*i+1, r = 2*i+2, s = i;
            if (l < size && data[l]->getF() < data[s]->getF()) s = l;
            if (r < size && data[r]->getF() < data[s]->getF()) s = r;
            if (s == i) break;
            INode* tmp = data[i]; data[i] = data[s]; data[s] = tmp;
            i = s;
        }
        return top;
    }

    bool empty() const { return size == 0; }
};

// ---- Hash set of C-strings (open addressing, load < 0.5, djb2) ----
struct StringSet {
    char** slots;
    int    cap;
    int    count;

    StringSet() : slots(0), cap(0), count(0) {}
    ~StringSet() {
        for (int i = 0; i < cap; ++i) delete[] slots[i];
        delete[] slots;
    }

    static unsigned int hash(const char* s) {
        unsigned int h = 5381;
        while (*s) h = h * 33 ^ (unsigned char)*s++;
        return h;
    }

    void grow() {
        int nc = cap ? cap * 2 : 64;
        char** ns = new char*[nc];
        for (int i = 0; i < nc; ++i) ns[i] = 0;
        for (int i = 0; i < cap; ++i) {
            if (!slots[i]) continue;
            unsigned int idx = hash(slots[i]) % (unsigned int)nc;
            while (ns[idx]) idx = (idx + 1) % (unsigned int)nc;
            ns[idx] = slots[i];  // transfer ownership, no realloc needed
        }
        delete[] slots;
        slots = ns; cap = nc;
    }

    bool contains(const char* key) const {
        if (!cap) return false;
        unsigned int idx = hash(key) % (unsigned int)cap;
        while (slots[idx]) {
            if (std::strcmp(slots[idx], key) == 0) return true;
            idx = (idx + 1) % (unsigned int)cap;
        }
        return false;
    }

    void insert(const char* key) {
        if (count * 2 >= cap) grow();
        unsigned int idx = hash(key) % (unsigned int)cap;
        while (slots[idx]) {
            if (std::strcmp(slots[idx], key) == 0) return;
            idx = (idx + 1) % (unsigned int)cap;
        }
        int len = (int)std::strlen(key);
        slots[idx] = new char[len + 1];
        std::memcpy(slots[idx], key, len + 1);
        ++count;
    }
};

// ---- Public API ----

AStarPath astarSolve(INode* start) {
    AStarPath result = { 0, 0 };
    NodeTracker tracker;
    MinHeap     open;
    StringSet   closed;

    tracker.add(start);
    open.push(start);

    INode* goal = 0;

    while (!open.empty()) {
        INode* cur = open.pop();

        if (closed.contains(cur->getKey())) continue;
        closed.insert(cur->getKey());

        if (cur->getH() == 0) { goal = cur; break; }

        int nc = 0;
        INode** neighbors = cur->getNeighbors(nc);
        for (int i = 0; i < nc; ++i) {
            if (!closed.contains(neighbors[i]->getKey())) {
                neighbors[i]->parent = cur;
                tracker.add(neighbors[i]);
                open.push(neighbors[i]);
            } else {
                delete neighbors[i];
            }
        }
        delete[] neighbors;
    }

    if (!goal) {
        tracker.freeAll();
        return result;
    }

    // Build path array by following parent chain.
    int len = 0;
    for (INode* n = goal; n; n = n->parent) ++len;

    INode** path = new INode*[len];
    int idx = len - 1;
    for (INode* n = goal; n; n = n->parent) path[idx--] = n;

    // Free every node that isn't on the path.
    tracker.freeExcept(path, len);

    result.nodes  = path;
    result.length = len;
    return result;
}

void astarFree(AStarPath& p) {
    for (int i = 0; i < p.length; ++i) delete p.nodes[i];
    delete[] p.nodes;
    p.nodes  = 0;
    p.length = 0;
}

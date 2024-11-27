#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int* parent;
    int* size;
    int n;
} UnionFind;

// Initialize the union-find data structure
void uf_init(UnionFind* uf, int n) {
    uf->parent = (int*)malloc(n * sizeof(int));
    uf->size = (int*)malloc(n * sizeof(int));
    uf->n = n;
    
    for (int i = 0; i < n; ++i) {
        uf->parent[i] = i;  // Each element is its own root
        uf->size[i] = 1;     // Size of each set is 1 initially
    }
}

// Find with path compression
int uf_find(UnionFind* uf, int x) {
    if (uf->parent[x] != x) {
        uf->parent[x] = uf_find(uf, uf->parent[x]);  // Path compression
    }
    return uf->parent[x];
}

// Union by size
void uf_union(UnionFind* uf, int x, int y) {
    int rootX = uf_find(uf, x);
    int rootY = uf_find(uf, y);
    
    if (rootX != rootY) {
        // Union by size: attach the smaller tree under the larger one
        if (uf->size[rootX] < uf->size[rootY]) {
            uf->parent[rootX] = rootY;
            uf->size[rootY] += uf->size[rootX];
        } else {
            uf->parent[rootY] = rootX;
            uf->size[rootX] += uf->size[rootY];
        }
    }
}

// Check if two elements are in the same set
bool uf_connected(UnionFind* uf, int x, int y) {
    return uf_find(uf, x) == uf_find(uf, y);
}

// Free the union-find data structure
void uf_free(UnionFind* uf) {
    free(uf->parent);
    free(uf->size);
}

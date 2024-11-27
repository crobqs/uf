#include <stdio.h>

typedef struct {
	size_t n;
	int* pr;
} UF;

static void uf_init(UF* handle, size_t n) {
	handle->pr = (int*)calloc(2 * n, sizeof(int));
	if (handle->pr == NULL) {
		fprintf(stderr, "\'%s\' memory-allocation failed\n", __func__);
		exit(1);
	}
	handle->n = n;
	for (int i = 0; i < n; ++i)
		handle->pr[i * 2] = i;
}

static void uf_destroy(UF* handle) {
	free(handle->pr);
	handle->pr = NULL;
	handle->n = 0;
}

static int uf_find(UF* handle, int x) {
	while (x != handle->pr[x * 2]) {
		handle->pr[x * 2] = handle->pr[handle->pr[x * 2] * 2];
		x = handle->pr[x * 2];
	}
	return x;
}

static void uf_unite(UF* handle, int x, int y) {
	int rootX = uf_find(handle, x);
	int rootY = uf_find(handle, y);
	if (rootX == rootY) return;
	int* rankX = &handle->pr[rootX * 2 + 1];
	int* rankY = &handle->pr[rootY * 2 + 1];
	if (*rankX > *rankY) {
		handle->pr[rootY * 2] = rootX;
	}
	else if (*rankX < *rankY) {
		handle->pr[rootX * 2] = rootY;
	}
	else {
		handle->pr[rootY * 2] = rootX;
		++*rankX;
	}
}

static void print_group(UF* handle, int x) {
	int root = uf_find(handle, x);
	if (handle->pr[x * 2 + 1] == 0) return;
	printf("\'%d\' : ", root);
	for (size_t i = 0; i < handle->n; ++i) {
		if (uf_find(handle, i) == root) {
			printf("%zu ", i);
		}
	}
	printf("\n");
}

#include "gset.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Resize the set's storage if needed
static void gset_resize_if_needed(GSet* set) {
    if (set->size >= set->capacity) {
        set->capacity *= 2;
        set->data = realloc(set->data, set->capacity * set->element_size);
        if (!set->data) {
            perror("Failed to resize set");
            exit(EXIT_FAILURE);
        }
    }
}

void gset_init(GSet* set, size_t element_size, size_t capacity,
               size_t (*hash_func)(const void*),
               bool (*equals_func)(const void*, const void*)) {
    set->data = malloc(capacity * element_size);
    if (!set->data) {
        perror("Failed to initialize set");
        exit(EXIT_FAILURE);
    }
    set->element_size = element_size;
    set->capacity = capacity;
    set->size = 0;
    set->hash_func = hash_func;
    set->equals_func = equals_func;
}

void gset_destroy(GSet* set) {
    free(set->data);
    set->data = NULL;
    set->size = 0;
    set->capacity = 0;
}

bool gset_add(GSet* set, const void* element) {
    if (gset_contains(set, element)) {
        return false; // Element already exists
    }
    gset_resize_if_needed(set);
    void* target = (char*)set->data + set->size * set->element_size;
    memcpy(target, element, set->element_size);
    set->size++;
    return true;
}

bool gset_remove(GSet* set, const void* element) {
    for (size_t i = 0; i < set->size; i++) {
        void* current = (char*)set->data + i * set->element_size;
        if (set->equals_func(current, element)) {
            memmove(current, current + set->element_size,
                    (set->size - i - 1) * set->element_size);
            set->size--;
            return true;
        }
    }
    return false;
}

bool gset_contains(const GSet* set, const void* element) {
    for (size_t i = 0; i < set->size; i++) {
        void* current = (char*)set->data + i * set->element_size;
        if (set->equals_func(current, element)) {
            return true;
        }
    }
    return false;
}

void gset_union(GSet* result, const GSet* set1, const GSet* set2) {
    gset_init(result, set1->element_size, set1->size + set2->size,
              set1->hash_func, set1->equals_func);
    for (size_t i = 0; i < set1->size; i++) {
        gset_add(result, (char*)set1->data + i * set1->element_size);
    }
    for (size_t i = 0; i < set2->size; i++) {
        gset_add(result, (char*)set2->data + i * set2->element_size);
    }
}

void gset_intersection(GSet* result, const GSet* set1, const GSet* set2) {
    gset_init(result, set1->element_size, set1->size < set2->size ? set1->size : set2->size,
              set1->hash_func, set1->equals_func);
    for (size_t i = 0; i < set1->size; i++) {
        void* element = (char*)set1->data + i * set1->element_size;
        if (gset_contains(set2, element)) {
            gset_add(result, element);
        }
    }
}

void gset_difference(GSet* result, const GSet* set1, const GSet* set2) {
    gset_init(result, set1->element_size, set1->size,
              set1->hash_func, set1->equals_func);
    for (size_t i = 0; i < set1->size; i++) {
        void* element = (char*)set1->data + i * set1->element_size;
        if (!gset_contains(set2, element)) {
            gset_add(result, element);
        }
    }
}

size_t gset_size(const GSet* set) {
    return set->size;
}

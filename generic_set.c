#include "generic_set.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void resize_if_needed(GenericSet* set) {
    if (set->size >= set->capacity) {
        set->capacity *= 2;
        set->data = realloc(set->data, set->capacity * set->element_size);
        if (!set->data) {
            perror("Failed to resize set");
            exit(EXIT_FAILURE);
        }
    }
}

void set_init(GenericSet* set, size_t element_size, size_t capacity,
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

void set_destroy(GenericSet* set) {
    free(set->data);
    set->data = NULL;
    set->size = 0;
    set->capacity = 0;
}

bool set_add(GenericSet* set, const void* element) {
    if (set_contains(set, element)) {
        return false; // Element already exists
    }
    resize_if_needed(set);
    void* target = (char*)set->data + set->size * set->element_size;
    memcpy(target, element, set->element_size);
    set->size++;
    return true;
}

bool set_remove(GenericSet* set, const void* element) {
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

bool set_contains(GenericSet* set, const void* element) {
    for (size_t i = 0; i < set->size; i++) {
        void* current = (char*)set->data + i * set->element_size;
        if (set->equals_func(current, element)) {
            return true;
        }
    }
    return false;
}

void set_union(GenericSet* result, const GenericSet* set1, const GenericSet* set2) {
    set_init(result, set1->element_size, set1->size + set2->size,
             set1->hash_func, set1->equals_func);
    for (size_t i = 0; i < set1->size; i++) {
        set_add(result, (char*)set1->data + i * set1->element_size);
    }
    for (size_t i = 0; i < set2->size; i++) {
        set_add(result, (char*)set2->data + i * set2->element_size);
    }
}

void set_intersection(GenericSet* result, const GenericSet* set1, const GenericSet* set2) {
    set_init(result, set1->element_size, set1->size < set2->size ? set1->size : set2->size,
             set1->hash_func, set1->equals_func);
    for (size_t i = 0; i < set1->size; i++) {
        void* element = (char*)set1->data + i * set1->element_size;
        if (set_contains(set2, element)) {
            set_add(result, element);
        }
    }
}

void set_difference(GenericSet* result, const GenericSet* set1, const GenericSet* set2) {
    set_init(result, set1->element_size, set1->size,
             set1->hash_func, set1->equals_func);
    for (size_t i = 0; i < set1->size; i++) {
        void* element = (char*)set1->data + i * set1->element_size;
        if (!set_contains(set2, element)) {
            set_add(result, element);
        }
    }
}

size_t set_size(const GenericSet* set) {
    return set->size;
}

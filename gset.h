#ifndef GSET_H
#define GSET_H

#include <stddef.h>
#include <stdbool.h>

// Generic Set structure
typedef struct {
    void* data; // Pointer to the elements
    size_t element_size; // Size of each element
    size_t capacity; // Total capacity
    size_t size; // Current number of elements
    size_t (*hash_func)(const void*); // User-defined hash function
    bool (*equals_func)(const void*, const void*); // User-defined equality function
} GSet;

// Initialize the set
void gset_init(GSet* set, size_t element_size, size_t capacity,
               size_t (*hash_func)(const void*),
               bool (*equals_func)(const void*, const void*));

// Destroy the set
void gset_destroy(GSet* set);

// Add an element to the set
bool gset_add(GSet* set, const void* element);

// Remove an element from the set
bool gset_remove(GSet* set, const void* element);

// Check if an element exists in the set
bool gset_contains(const GSet* set, const void* element);

// Perform set union: result = set1 ∪ set2
void gset_union(GSet* result, const GSet* set1, const GSet* set2);

// Perform set intersection: result = set1 ∩ set2
void gset_intersection(GSet* result, const GSet* set1, const GSet* set2);

// Perform set difference: result = set1 - set2
void gset_difference(GSet* result, const GSet* set1, const GSet* set2);

// Get the number of elements in the set
size_t gset_size(const GSet* set);

#endif // GSET_H

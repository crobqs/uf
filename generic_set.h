#ifndef GENERIC_SET_H
#define GENERIC_SET_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    void* data;
    size_t element_size;
    size_t capacity;
    size_t size;
    size_t (*hash_func)(const void*); // User-defined hash function
    bool (*equals_func)(const void*, const void*); // User-defined equality function
} GenericSet;

// Initialize the set
void set_init(GenericSet* set, size_t element_size, size_t capacity,
              size_t (*hash_func)(const void*),
              bool (*equals_func)(const void*, const void*));

// Destroy the set
void set_destroy(GenericSet* set);

// Add an element to the set
bool set_add(GenericSet* set, const void* element);

// Remove an element from the set
bool set_remove(GenericSet* set, const void* element);

// Check if an element exists in the set
bool set_contains(GenericSet* set, const void* element);

// Perform set union: result = set1 ∪ set2
void set_union(GenericSet* result, const GenericSet* set1, const GenericSet* set2);

// Perform set intersection: result = set1 ∩ set2
void set_intersection(GenericSet* result, const GenericSet* set1, const GenericSet* set2);

// Perform set difference: result = set1 - set2
void set_difference(GenericSet* result, const GenericSet* set1, const GenericSet* set2);

// Get the number of elements in the set
size_t set_size(const GenericSet* set);

#endif // GENERIC_SET_H

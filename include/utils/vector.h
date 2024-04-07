// TODO: In all these functions the type_size should come before the other
// params.

#ifndef UTILS_VECTOR_H
#define UTILS_VECTOR_H

#include <common.h>
#include <debug/debug.h>

/**
 * The body of a for loop that iterates over every element in a vector.
 *
 * @param vec The vector to iterate over.
 * @param name The name to give each element.
 *
 * @warning The vector cannot be modified during iteration.
 */
#define VEC_ITER(vec, name) \
    (typeof(&(vec).data->t) name = (void*)(vec).data; \
    name != (void*)((vec).data + (vec).len); \
    name++)

/**
 * Creates a vector type.
 *
 * @param type The element type the vector should contain.
 * @return A vec type.
 */
#define VEC(type) \
    struct { \
        struct { \
            type t; \
        }* data; \
        u32 len; \
        u8 size; \
    }

/**
 * Gets the types of elements a vector contains.
 */
#define VEC_TYPE(vec) typeof((vec).data->t)

/**
 * Gets the size of a vector in bytes.
 *
 * @param vec The vector that is being sized.
 * @return The size of the vec in bytes.
 */
#define VEC_SIZEOF(vec) ((size_t)vec.len * sizeof((vec).data->t))

// TODO: In some places the power of two was kept meaning the init value is
// messed up in some places.
/**
 * Inits a vector that has enough space for the desired len.
 *
 * @param vec The vector that is being inited.
 * @param len The length that the vector should allocate enough bytes for.
 *
 * @warning If vec is already inited and contains data this will cause a memory
 * leak.
 */
#define VEC_INIT(vec, len) \
    (vector_init((vector*)&vec, len, sizeof((vec).data->t)))

/**
 * Gets a value from a vector at an index as a mutable lvalue.
 *
 * @param vec The vector that is being indexed.
 * @param index The index of the desired element.
 * @return The element at the index.
 * @note This will compile with bounds checking when DEBUG is enabled.
 *
 * @warning Index must be less than the len of vec.
 */
#define VEC_AT(vec, index) \
    (*(typeof(&(vec).data->t))vector_at ( \
        (vector*)&(vec), \
        index, \
        sizeof((vec).data->t) \
    ))

/**
 * Get a value from a vector at an index without bounds checking.
 *
 * @param vec The vector that is being indexed.
 * @param index The index of the desired element.
 * @return A pointer to the element at the index.
 * @note This function should only be used if the element at the desired index
 * is sure to be valid.
 */
#define VEC_AT_UNSAFE(vec, index) \
    (*(typeof(&(vec).data->t))vector_at_unsafe ( \
        (vector*)&vec, \
        index, \
        sizeof((vec).data->t) \
    ))

/**
 * Attempts to shrink a vector's size down to fit its length.
 *
 * @param vec The vector that is being shrinked.
 */
#define VEC_SHRINK(vec) vector_shrink((vector*)&vec, sizeof((vec).data->t))

/**
 * Attempts to expand the size of the vector to fit the len of the vector.
 *
 * @param vec The vector that is being expanded.
 * @note This should only be used before new elements are added to the vector.
 */
#define VEC_EXPAND(vec) vector_expand((vector*)&vec, sizeof((vec).data->t))

/**
 * Appends an element to the end of a vector.
 *
 * @param vec The vector that is being appended to.
 * @param value The value of the element that is being appended to vec.
 */
#define VEC_APPEND(vec, value) \
    vector_append((vector*)&vec, value, sizeof((vec).data->t))

/**
 * Expands the vector as if a value was going to be appeneded but doesn't
 * append a value so the value can be inited later. Returns a pointer to the
 * location of the new data.
 *
 * @param vec The vector to be dry appended to.
 * @return A pointer to the appended space.
 *
 * @warning The dry appeneded value MUST be inited before being read.
 */
#define VEC_DRY_APPEND(vec) (typeof(&(vec).data->t))vector_dry_append ( \
    (vector*)&vec, \
    sizeof((vec).data->t) \
)

/**
 * Appends an element to the end of a vector without attempting to expand the
 * size of the vector.
 *
 * @param vec The vector that is being appended to.
 * @param value The value of the element that is being appended to vec.
 *
 * @warning This should only be used if the vector is sure to contain enough
 * bytes for another element.
 */
#define VEC_APPEND_UNSAFE(vec, value) \
    vector_append_unsafe((vector*)&vec, value, sizeof((vec).data->t))

/**
 * Pops the last element of a vector.
 *
 * @param vec The vector that is being popped.
 * @return A copy of the removed element.
 */
#define VEC_POP(vec) vector_remove((vector*)&vec, sizeof((vec).data->t))

/**
 * Removes an element from a vector at a specific index.
 *
 * @param vec The vector that is being removed from.
 * @param index The index of the element that is being removed.
 */
#define VEC_REMOVE(vec, index) \
    vector_remove((vector*)&vec, index, sizeof((vec).data->t))

/**
 * Removes an element from a vector.
 *
 * @param vec The vector that is being removed from.
 * @param ptr A ptr to the element being removed.
 */
#define VEC_REMOVE_PTR(vec, ptr) \
    vector_remove_ptr((vector*)&vec, ptr, sizeof((vec).data->t))

/**
 * Adds an element to a vector at a specific index.
 *
 * @param vec The vector that is being added to.
 * @param value The value of the element that is being added to vec.
 * @param index The index that the value is being added to.
 * @return A pointer to the element that got added to the vec.
 *
 * @warning index must be less than or equal to the len of vec.
 */
#define VEC_ADD(vec, value, index) \
    vector_add((vector*)&vec, value, index, sizeof((vec).data->t))

/**
 * Turns a vector that holds a type into a normal vector type.
 *
 * @param vec The typed vector to turn into a generic vector.
 * @return A normalized version of the vector.
 */
#define VEC_NORMAL(vec) (*(vector*)(&(vec)))

/* A template for the vector type. */
typedef struct vector {
    /* The data of this vector. */
    void* data;

    /* The number of elements in this vector. */
    u32 len;

    // TODO: Size should be changed to be the amount of elements the vector has
    // space for.
    // TODO: Also there should be a macro to do (1 << size) maybe something
    // like VEC_SPACE and vector_space as a function.
    /*
     * The log2 of the amount of bytes present in this vector's data.
     * (1 << size) can be used to calculate the true number of bytes in the
     * vector's data.
     */
    u8 size;
} vector;

/**
 * Get a value from a vector at an index. This can compile with bounds checks
 * for debugging.
 *
 * @param vec The vector that is being indexed.
 * @param index The index of the desired element.
 * @param type_size The byte size of each element in vec.
 * @return A pointer to the element at the index.
 *
 * @warning Use "VEC_AT".
 */
void* vector_at(vector* vec, size_t index, size_t type_size);

/**
 * Get a value from a vector at an index. This will never use any kind of bound
 * checking.
 *
 * @param vec The vector that is being indexed.
 * @param index The index of the desired element.
 * @param type_size The byte size of each element in vec.
 * @return A pointer to the element at the index.
 * @note This function should only be used if the element at the desired index
 * is sure to be valid.
 *
 * @warning Use "VEC_AT_UNSAFE".
 */
void* vector_at_unsafe(vector* vec, size_t index, size_t type_size);

/**
 * Attempts to shrink a vector's size down to fit the len.
 *
 * @param vec The vector that is shrinking.
 * @param type_size The byte size of each elementss in vec.
 *
 * @warning Use "VEC_SHRINK".
 */
void vector_shrink(vector* vec, size_t type_size);

/**
 * Attempts to expand the size of the vector to fit the len of the vector.
 *
 * @param vec The vector that is being expanded.
 * @param type_size The byte size of each element in vec.
 *
 * @warning Use "VEC_EXPAND".
 */
void vector_expand(vector* vec, size_t type_size);

/**
 * Appends an element to the end of a vector.
 *
 * @param vec The vector that is being appended to.
 * @param value The value of the element that is being appended to vec.
 * @param type_size The byte size of each element in vec.
 *
 * @warning Use "VEC_APPEND".
 */
void vector_append(vector* vec, const void* value, size_t type_size);

/**
 * Expands the vector as if a value was going to be appeneded but doesn't
 * append a value so the value can be inited later.
 *
 * @param vec The vector to be dry appended to.
 * @return A pointer to the appended space.
 *
 * @warning Use "VEC_DRY_APPEND".
 */
void* vector_dry_append(vector* vec, size_t type_size);

/**
 * Appends an element to the end of a vector without attempting to expand the
 * size of the vector.
 *
 * @param vec The vector that is being appended to.
 * @param value The value of the element that is being appended to vec.
 * @param type_size The byte size of each element in vec.
 *
 * @warning Use "VEC_APPEND_UNSAFE".
 */
void vector_append_unsafe(vector* vec, void* value, size_t type_size);

/**
 * Pops the last element of a vector.
 *
 * @param vec The vector that is being popped.
 * @param type_size The byte size of each element in vec.
 *
 * @warning Use "VEC_POP".
 */
void vector_pop(vector* vec, size_t type_size);

/**
 * Removes an element from a vector.
 *
 * @param vec The vector that is being removed from.
 * @param value A ptr to the element being removed.
 * @param type_size The byte size of each element in vec.
 *
 * @warning Use "VEC_REMOVE_PTR".
 */
void vector_remove_ptr(vector* vec, void* value, size_t type_size);

/**
 * Removes an element from a vector at a specific index.
 *
 * @param vec The vector that is being removed from.
 * @param index The index of the element that is being removed.
 * @param type_size The byte size of each element in vec.
 *
 * @warning Use "VEC_REMOVE".
 */
void vector_remove(vector* vec, size_t index, size_t type_size);

/**
 * Adds an element to a vector at a specific index.
 *
 * @param vec The vector that is being added to.
 * @param value The value of the element that is being added to vec.
 * @param index The index that the value is being added to.
 * @param type_size The byte size of each element in vec.
 * @return A pointer to the element that got added to the vec.
 *
 * @warning Use "VEC_ADD".
 */
void* vector_add(vector* vec, void* value, size_t index, size_t type_size);

/**
 * Inits a vector that has enough space for the desired len.
 *
 * @param vec The vector that is being inited.
 * @param len The length that the vector should allocate enough bytes for.
 * @param type_size The byte size of each element in vec.
 *
 * @warning Use "VEC_INIT".
 */
void vector_init(vector* vec, size_t len, size_t type_size);

#endif

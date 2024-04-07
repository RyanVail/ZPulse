#include <utils/vector.h>

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
void* vector_at(vector* vec, size_t index, size_t type_size)
{
    DEBUG_ASSERT (index <= vec->len,
        "Vector has a len of %u but attempted to index %lu.\n",
        vec->len,
        index
    );
    DEBUG_ASSERT (vec->data != NULL,
        "Attempted to index a vector with no data.\n"
    );

    return vector_at_unsafe(vec, index, type_size);
}

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
void* vector_at_unsafe(vector* vec, size_t index, size_t type_size)
{
    return (void*)((size_t)vec->data + (type_size * index));
}

/**
 * Attempts to shrink a vector's size down to fit the len.
 *
 * @param vec The vector that is shrinking.
 * @param type_size The byte size of each elementss in vec.
 *
 * @warning Use "VEC_SHRINK".
 */
void vector_shrink(vector* vec, size_t type_size)
{
    DEBUG_ASSERT (vec->data != NULL,
        "Attempted to shrink a vector with no data.\n"
    );

    // TODO: This should probaly check if the length is zero and if it is free
    // the vec.
    // TODO: Is this needed?
    if (vec->size == 0)
        return;

    if ((size_t)(1 << vec->size) >= (size_t)(vec->len << 1)) {
        vec->size--;
        vec->data = realloc(vec->data, (1 << vec->size) * type_size);
    }
}

/**
 * Attempts to expand the size of the vector to fit the len of the vector.
 *
 * @param vec The vector that is being expanded.
 * @param type_size The byte size of each element in vec.
 *
 * @warning Use "VEC_EXPAND".
 */
void vector_expand(vector* vec, size_t type_size)
{
    if (((size_t)1 << vec->size) < vec->len * type_size) {
        vec->size++;
        vec->data = realloc(vec->data, ((size_t)1 << vec->size) * type_size);
    }
}

/**
 * Appends an element to the end of a vector.
 *
 * @param vec The vector that is being appended to.
 * @param value The value of the element that is being appended to vec.
 * @param type_size The byte size of each element in vec.
 *
 * @warning Use "VEC_APPEND".
 */
void vector_append(vector* vec, const void* value, size_t type_size)
{
    vec->len++;
    vector_expand(vec, type_size);

    memcpy (
        vector_at(vec, vec->len - 1, type_size),
        value,
        type_size
    );
}

/**
 * Expands the vector as if a value was going to be appeneded but doesn't
 * append a value so the value can be inited later.
 *
 * @param vec The vector to be dry appended to.
 * @return A pointer to the appended space.
 *
 * @warning Use "VEC_DRY_APPEND".
 */
void* vector_dry_append(vector* vec, size_t type_size)
{
    vec->len++;
    vector_expand(vec, type_size);
    return vector_at(vec, vec->len - 1, type_size);
}

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
void vector_append_unsafe(vector* vec, void* value, size_t type_size)
{
    memcpy (
        vector_at_unsafe(vec, vec->len, type_size),
        value,
        type_size
    );

    vec->len++;
}

/**
 * Pops the last element of a vector.
 *
 * @param vec The vector that is being popped.
 * @param type_size The byte size of each element in vec.
 *
 * @warning Use "VEC_POP".
 */
void vector_pop(vector* vec, size_t type_size)
{
    DEBUG_ASSERT (vec->data != NULL,
        "Attempted to remove a value from a vector with no data.\n"
    );

    /* Decramenting the length of the vector and attempting to shrink it. */
    vec->len--;
    vector_shrink(vec, type_size);
}

/**
 * Removes an element from a vector.
 *
 * @param vec The vector that is being removed from.
 * @param value A ptr to the element being removed.
 * @param type_size The byte size of each element in vec.
 *
 * @warning Use "VEC_REMOVE_PTR".
 */
void vector_remove_ptr(vector* vec, void* value, size_t type_size)
{
    DEBUG_ASSERT (vec->data != NULL,
        "Attempted to remove a value from a vector with no data.\n"
    );

    // TODO: Make sure this works.
    /* Moving the vector's data to overide the value being removed. */
    memmove(value, value + type_size, (value - vec->data) * type_size);

    /* Decramenting the length of the vector and attempting to shrink it. */
    vec->len--;
    vector_shrink(vec, type_size);
}

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
 * @note index must be less than or equal to the len of vec.
 */
void* vector_add(vector* vec, void* value, size_t index, size_t type_size)
{
    DEBUG_ASSERT (index <= vec->len,
        "Vector has a len of %u "
        "but attempted to add a value to the index %lu.",
        vec->len,
        index
    );

    /* Incramenting the vector len and expanding it if need be. */
    vec->len++;
    vector_expand(vec, type_size);

    void* target = vector_at(vec, index, false);

    /* Moving over the vector. */
    memcpy(target + type_size, target, ((size_t)vec->len - index) * type_size);

    /* Adding the value and returning a pointer to it. */
    memcpy(target, value, type_size);
    return target;
}

/**
 * Inits a vector that has enough space for the desired len.
 *
 * @param vec The vector that is being inited.
 * @param len The length that the vector should allocate enough bytes for.
 * @param type_size The byte size of each element in vec.
 *
 * @warning Use "VEC_INIT".
 */
void vector_init(vector* vec, size_t len, size_t type_size)
{
    /* The amount of size required. */
    size_t size = type_size * len;

    // TODO: Fix this also make a function that just does this.
    size = 31 - __builtin_clz(size | 1);

    *vec = (vector) {
        .data = malloc((size_t)1 << size),
        .len = 0,
        .size = (u8)size,
    };
}

#include <utils/vector.h>
#include <math/u32.h>

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

void* vector_at_unsafe(vector* vec, size_t index, size_t type_size)
{
    return (void*)((size_t)vec->data + (type_size * index));
}

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

void vector_expand(vector* vec, size_t type_size)
{
    if (((size_t)1 << vec->size) < vec->len * type_size) {
        vec->size++;
        vec->data = realloc(vec->data, ((size_t)1 << vec->size) * type_size);
    }
}

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

void* vector_dry_append(vector* vec, size_t type_size)
{
    vec->len++;
    vector_expand(vec, type_size);
    return vector_at(vec, vec->len - 1, type_size);
}

void vector_append_unsafe(vector* vec, void* value, size_t type_size)
{
    memcpy (
        vector_at_unsafe(vec, vec->len, type_size),
        value,
        type_size
    );

    vec->len++;
}

void vector_pop(vector* vec, size_t type_size)
{
    DEBUG_ASSERT (vec->data != NULL,
        "Attempted to remove a value from a vector with no data.\n"
    );

    /* Decramenting the length of the vector and attempting to shrink it. */
    vec->len--;
    vector_shrink(vec, type_size);
}

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

void vector_remove(vector* vec, size_t index, size_t type_size);

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

bool vector_is_ptr_within(vector* vec, const void* ptr, size_t type_size)
{
    return ((uintptr_t)ptr >= (uintptr_t)vec->data) 
        && (uintptr_t)ptr < (uintptr_t)vec->data + (vec->len * type_size);
}

size_t vector_ptr_index(vector* vec, const void* ptr, size_t type_size)
{
    DEBUG_ASSERT (vector_is_ptr_within(vec, ptr, type_size),
        "Tried to get the data index of a pointer within a vector that "
        "doesn't point to data within the vector."
    );

    // TODO: Add back this warning but correctly this time.

    return ((uintptr_t)ptr - (uintptr_t)vec->data) / type_size;
}

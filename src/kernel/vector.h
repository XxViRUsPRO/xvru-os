#ifndef VECT_H
#define VECT_H

#include <types.h>
#include <string.h>
#include <stdio.h>

#define VECT_CAPACITY 256

typedef struct
{
    i32 size;
    i32 capacity;
    size_t data_size;
    void *data;
} vect;

void vect_err(const char *error)
{
    printf("vector error: %s\r\n", error);
}

vect *
vect_init(size_t data_size, u32 capacity)
{
    u8 buffer[sizeof(vect) + (data_size * capacity)];
    vect *v = (vect *)buffer;
    if (capacity == 0)
        capacity = VECT_CAPACITY;
    v->data_size = data_size;
    v->size = 0;
    v->capacity = capacity;
    return v;
}

void vect_free(void *v)
{
    vect *vect = v;
    size_t size = vect->data_size * vect->capacity + sizeof(vect);
    memset(vect, 0, size);
}

void vect_resize(vect *v)
{
    v->capacity *= 2; /* double capacity */
    u8 buffer[sizeof(vect) + (v->data_size * v->capacity)];
    vect *new_vect = (vect *)buffer;
    new_vect->data_size = v->data_size;
    new_vect->size = v->size;
    new_vect->capacity = v->capacity;
    memcpy(new_vect->data, v->data, v->data_size * v->size);
    v = new_vect;
}

u32 vect_chk_bounds(vect *v, u32 pos)
{
    return (pos < v->size);
}

u32 vect_at(vect *v, u32 pos)
{
    if (!vect_chk_bounds((vect *)v, pos))
        vect_err("vect_at: out of bounds.");
    return pos;
}

u32 vect_set(vect *v, u32 pos)
{
    if (!vect_chk_bounds((vect *)v, pos))
        vect_err("vect_set: out of bounds.");
    return pos;
}

u32 vect_push(vect *v)
{
    if (v->size == v->capacity)
        vect_resize(v);
    return v->size++;
}

u32 vect_pop(vect *v)
{
    if (v->size == 0)
        vect_err("vect_pop: underflow.");
    return --v->size;
}

u32 vect_rem(vect *v, u32 pos)
{
    if (!vect_chk_bounds((vect *)v, pos))
        vect_err("vect_rem: out of bounds.");
    char *dest = (char *)v->data + (v->data_size * pos);
    char *src = dest + v->data_size;
    size_t num_bytes = v->data_size * (v->size - pos - 1);
    memmove(dest, src, num_bytes);
    v->size--;
    return pos;
}

u32 vect_ins(vect *v, u32 pos)
{
    if (!vect_chk_bounds((vect *)v, pos))
        vect_err("vect_rem: out of bounds.");
    char *src = (char *)v->data + (v->data_size * pos);
    char *dest = src + v->data_size;
    size_t num_bytes = v->data_size * (v->size - pos);
    memmove(dest, src, num_bytes);
    v->size++;
    return pos;
}

#define VECT_GENERATE_TYPE(TYPE) VECT_GENERATE_NAME(TYPE, TYPE)
#define VECT_GENERATE_NAME(TYPE, NAME)                           \
    typedef struct                                               \
    {                                                            \
        int size;                                                \
        int capacity;                                            \
        size_t data_size;                                        \
        TYPE *data;                                              \
    } vect_##NAME;                                               \
    vect_##NAME *                                                \
        vect_init_##NAME(u32 capacity)                           \
    {                                                            \
        return (vect_##NAME *)vect_init(sizeof(TYPE), capacity); \
    }                                                            \
    TYPE *                                                       \
        vect_ptr_##NAME(vect_##NAME *v, u32 pos)                 \
    {                                                            \
        return v->data + vect_at((vect *)v, pos);                \
    }                                                            \
    TYPE                                                         \
        vect_at_##NAME(vect_##NAME *v, u32 pos)                  \
    {                                                            \
        return v->data[vect_at((vect *)v, pos)];                 \
    }                                                            \
    void                                                         \
        vect_push_##NAME(vect_##NAME *v, TYPE value)             \
    {                                                            \
        v->data[vect_push((vect *)v)] = value;                   \
    }                                                            \
    void                                                         \
        vect_set_##NAME(vect_##NAME *v, u32 pos, TYPE value)     \
    {                                                            \
        v->data[vect_set((vect *)v, pos)] = value;               \
    }                                                            \
    TYPE                                                         \
        vect_pop_##NAME(vect_##NAME *v)                          \
    {                                                            \
        return v->data[vect_pop((vect *)v)];                     \
    }                                                            \
    void                                                         \
        vect_rem_##NAME(vect_##NAME *v, u32 pos)                 \
    {                                                            \
        vect_rem((vect *)v, pos);                                \
    }                                                            \
    void                                                         \
        vect_ins_##NAME(vect_##NAME *v, u32 pos, TYPE value)     \
    {                                                            \
        v->data[vect_ins((vect *)v, pos)] = value;               \
    }                                                            \
    void                                                         \
        vect_cat_##NAME(vect_##NAME *v1, vect_##NAME *v2)        \
    {                                                            \
        return;                                                  \
    }

#endif
#pragma once

#include "common.h"
#include "string.h"
#include "vector.h"

typedef struct {
  struct Bucket {
    Cstr m__key;
    void *m__value;
    enum { Empty, Full } m__status;
  } * m__buckets;

  uintmax_t m__size;

  uintmax_t m__capacity;
} HashMap;

typedef struct HashMapResult {
  enum StatusCode status;

  union {
    HashMap map;
    void *value;
  } data;
} HashMapResult;

HashMap HashMap_new(void);

HashMap HashMap_withCapacity(uintmax_t capacity);

void HashMap_free(HashMap map);

VEC(char *) HashMap_keys(HashMap map);

VEC(void *) HashMap_values(HashMap map);

HashMapResult HashMap_insert(HashMap map, Cstr key, void *value);

HashMapResult HashMap_deleteEntry(HashMap map, Cstr key);

bool HashMap_contains(HashMap map, Cstr key);

HashMapResult HashMap_get(HashMap map, Cstr key);

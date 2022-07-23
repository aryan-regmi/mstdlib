#pragma once

#include "common.h"
#include <stdint.h>

#define VEC(T) Vec

typedef struct {
  uintmax_t m__len;

  uintmax_t m__capacity;

  void **m__data;
} Vec;

typedef struct {
  Vec vector;

  enum StatusCode status;
} VecResult;

Vec Vec_new(void);

Vec Vec_withCapacity(uintmax_t capacity);

uintmax_t Vec_len(Vec vector);

uintmax_t Vec_capacity(Vec vector);

void Vec_free(Vec vector);

VecResult Vec_push(Vec vector, void *value);

VecResult Vec_pop(Vec vector);

VecResult Vec_setIdx(Vec vector, uintmax_t idx, const void *value);

void *Vec_getIdx(Vec vector, uintmax_t idx);

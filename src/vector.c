#include "vector.h"
#include "allocator.h"
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

static const GeneralPurposeAllocator GPA = {
    .alloc = calloc, .free = free, .resize = realloc};

Vec Vec_new(void) {
  return (Vec){.m__capacity = 0, .m__len = 0, .m__data = NULL};
}

Vec Vec_withCapacity(uintmax_t capacity) {
  Vec vector = {.m__len = 0, .m__capacity = capacity};

  vector.m__data = (*GPA.alloc)(capacity, sizeof(vector.m__data));

  return vector;
}

uintmax_t Vec_len(Vec vector) { return vector.m__len; }

uintmax_t Vec_capacity(Vec vector) { return vector.m__capacity; }

void Vec_free(Vec vector) { (*GPA.free)(vector.m__data); }

VecResult Vec_resize(Vec vector) {
  if (vector.m__capacity * 2 < UINT_MAX) {
    return (VecResult){.status = SizeOverflow};
  }

  vector.m__capacity *= 2;

  void **data = (*GPA.resize)(vector.m__data,
                              vector.m__capacity * sizeof(vector.m__data));
  if (!data) {
    return (VecResult){.status = NullData};
  }

  vector.m__data = data;

  return (VecResult){.vector = vector, .status = Success};
}

VecResult Vec_push(Vec vector, void *value) {
  // Allocate space on the first push
  if (vector.m__capacity == 0) {
    vector.m__capacity = 1;
    void **data = (*GPA.alloc)(1, sizeof(vector.m__data));
    if (!data) {
      return (VecResult){.status = NullData};
    }
    vector.m__data = data;
  }

  // Resize if too small
  if (vector.m__capacity < vector.m__len + 1) {
    VecResult resized_vec = Vec_resize(vector);
    if (resized_vec.status != Success) {
      return (VecResult){.status = resized_vec.status};
    }
  }

  // Append value to vector
  vector.m__data[vector.m__len++] = value;

  return (VecResult){.vector = vector, .status = Success};
}

// Returns the vector with the last element removed
VecResult Vec_pop(Vec vector) {
  if (Vec_len(vector) == 0) {
    return (VecResult){.status = EmptyVec};
  }

  return (VecResult){.vector = (Vec){.m__data = vector.m__data,
                                     .m__capacity = vector.m__capacity,
                                     .m__len = vector.m__len--},
                     .status = Success};
}

VecResult Vec_setIdx(Vec vector, uintmax_t idx, const void *value) {
  if (Vec_len(vector) < idx) {
    return (VecResult){.status = IndexOutOfBounds};
  }

  vector.m__data[idx] = (void *)value;

  return (VecResult){.vector = vector, .status = Success};
}

// Returns NULL if the index is out of bounds
void *Vec_getIdx(Vec vector, uintmax_t idx) {
  if (Vec_len(vector) < idx) {
    return NULL;
  }

  return (void *)vector.m__data[idx];
}

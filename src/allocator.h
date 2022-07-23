#pragma once

#include <stddef.h>

typedef struct {
  void *context;

  void *(*alloc)(size_t num_elems, size_t size_of_elem);

  void *(*resize)(void *ptr, size_t new_size);

  void (*free)(void *ptr);

} GeneralPurposeAllocator;

#pragma once

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum StatusCode {
  Success = 0,

  // Underlying C-String is corrupted
  NullStr,

  // strcat returned Null
  StrcatError,

  // strcpy returned Null
  StrcpyError,

  // Error during resizing
  ResizeError,

  // Error during allocation
  AllocError,

  // Data is corrupted
  NullData,

  // Variable too small to store value
  SizeOverflow,

  // Tried popping value from empty Vec
  EmptyVec,

  // Index out of bounds of Vec
  IndexOutOfBounds,

  // HashMap key is invalid/non-existent
  InvalidKey
};

// Easy function for panics
static inline void err_panic(const char error_msg[]) {
  fprintf(stderr, "\n%s\n", error_msg);
  exit(EXIT_FAILURE);
}

// Easy function for NULL_CHECK
static inline void null_check(const void *ptr, const char filename[],
                              size_t line_num) {
  if (!ptr) {
    fprintf(stderr,
            "\n[ERROR] NullCheckError: NULL pointer at unexpected position "
            "[%s:%zu]\n",
            filename, line_num);
    exit(EXIT_FAILURE);
  }
}

static inline bool float_eq(float f1, float f2, float tolerance) {
  if (fabs(f1 - f2) < tolerance) {
    return true;
  }

  return false;
}

#define NULLCHK(ptr) null_check(ptr, __FILE__, __LINE__)

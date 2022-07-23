#pragma once

#include "allocator.h"
#include "common.h"
#include <stdbool.h>
#include <stdint.h>

typedef char *Cstr;

typedef struct {
  Cstr str;

  uintmax_t m__len;

  uintmax_t m__capacity;
} String;

typedef struct {
  String string;

  enum StatusCode status;
} StringResult;

String String_new(void);

String String_withCapacity(uintmax_t capacity);

uintmax_t String_len(String s);

uintmax_t String_capacity(String s);

void String_free(String s);

StringResult String_pushChar(String s, char c);

StringResult String_pushCstr(String s, Cstr cs);

StringResult String_fromCstr(Cstr cs);

Cstr String_toCstr(String s);

bool String_isEmpty(String s);

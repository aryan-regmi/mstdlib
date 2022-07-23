#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const GeneralPurposeAllocator GPA = {
    .alloc = calloc, .free = free, .resize = realloc};

String String_new(void) {
  return (String){.str = NULL, .m__capacity = 0, .m__len = 0};
}

String String_withCapacity(uintmax_t capacity) {
  Cstr str = (*GPA.alloc)(capacity, sizeof(*str));

  return (String){.str = str, .m__len = 0, .m__capacity = capacity};
}

uintmax_t String_len(String s) { return s.m__len; }

uintmax_t String_capacity(String s) { return s.m__capacity; }

void String_free(String s) { (*GPA.free)(s.str); }

static StringResult String_resize(String s) {
  s.m__capacity *= 2;

  Cstr new_str = (*GPA.resize)(s.str, s.m__capacity);
  if (!new_str) {
    return (StringResult){.status = ResizeError};
  }

  return (StringResult){.string = (String){.str = new_str,
                                           .m__len = s.m__len,
                                           .m__capacity = s.m__capacity},
                        .status = Success};
}

StringResult String_pushChar(String s, char c) {
  // Allocate space on first push
  if (s.m__capacity == 0) {
    s.m__capacity = 2;
    Cstr str = (*GPA.alloc)(s.m__capacity + 1, sizeof(char));
    if (!str) {
      return (StringResult){.status = AllocError};
    }
    s.str = str;
  }

  // Resize if too small
  if (s.m__capacity < s.m__len + 1) {
    StringResult resized_string = String_resize(s);
    if (resized_string.status != Success) {
      return (StringResult){.status = resized_string.status};
    }
    s = resized_string.string;
  }

  // Append character to String
  s.str[s.m__len++] = c;

  return (StringResult){.string = s, .status = Success};
}

StringResult String_pushCstr(String s, Cstr cs) {
  // Allocate space on first push
  if (s.m__capacity == 0) {
    s.m__capacity = strlen(cs);
    s.m__len = strlen(cs);
    Cstr str = (*GPA.alloc)(s.m__capacity, sizeof(char));
    if (!str) {
      return (StringResult){.status = AllocError};
    }
    s.str = str;
  }

  // Resize if too small
  if (s.m__capacity < s.m__len + strlen(cs) + 1) {
    StringResult resized_string = String_resize(s);
    if (resized_string.status != Success) {
      return (StringResult){.status = resized_string.status};
    }
    s = resized_string.string;
  }

  // Append Cstr to String
  Cstr concatenated = strcat(s.str, cs);
  if (!concatenated) {
    return (StringResult){.status = StrcatError};
  }
  s.str = concatenated;

  s.m__len = strlen(s.str);

  return (StringResult){.string = s, .status = Success};
}

StringResult String_fromCstr(Cstr cs) {
  if (!cs) {
    return (StringResult){.status = NullStr};
  }

  String s = String_withCapacity(strlen(cs) + 1);

  Cstr str = strcpy(s.str, cs);
  if (!str) {
    return (StringResult){.status = StrcpyError};
  }
  s.str = str;

  s.m__len = strlen(cs);

  return (StringResult){.string = s, .status = Success};
}

Cstr String_toCstr(String s) { return s.str; }

bool String_isEmpty(String s) { return s.m__len == 0; }

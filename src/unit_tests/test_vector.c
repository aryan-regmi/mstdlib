#pragma once

#include "../../includes/acutest.h"
#include "../vector.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void canCreateNewVector(void) {
  Vec v = Vec_new();

  TEST_CHECK(Vec_len(v) == 0);
  TEST_CHECK(Vec_capacity(v) == 0);
  TEST_CHECK(v.m__data == NULL);

  Vec_free(v);
}

void canCreateVectorWithCapacity(void) {
  Vec v = Vec_withCapacity(10);

  TEST_CHECK(Vec_len(v) == 0);
  TEST_CHECK(Vec_capacity(v) == 10);
  TEST_CHECK(v.m__data != NULL);

  Vec_free(v);
}

void canPushPopVector(void) {
  Vec v = Vec_new();

  VecResult vres = Vec_push(v, &(int){42});
  v = vres.vector;

  TEST_CHECK(vres.status == Success);
  TEST_CHECK(Vec_len(v) == 1);
  TEST_CHECK(Vec_capacity(v) == 1);
  TEST_CHECK(*(int *)Vec_pop(v).vector.m__data[0] == 42);

  Vec_free(v);
}

void canSetGetIdxVector(void) {
  Vec v = Vec_new();

  v = Vec_push(v, &(int){42}).vector;

  v = Vec_setIdx(v, 0, &(int){99}).vector;

  int stored_val = *(int *)Vec_getIdx(v, 0);

  TEST_CHECK(stored_val == 99);

  Vec_free(v);
}

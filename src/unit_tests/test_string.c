#pragma once

#include "../../includes/acutest.h"
#include "../string.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void canCreateNewString(void) {
  String s = String_new();

  TEST_CHECK(String_len(s) == 0);
  TEST_CHECK(String_capacity(s) == 0);
  TEST_CHECK(s.str == NULL);

  String_free(s);
}

void canCreateStringWithCapacity(void) {
  String s = String_withCapacity(20);

  TEST_CHECK(String_len(s) == 0);
  TEST_CHECK(String_capacity(s) == 20);
  TEST_CHECK(s.str != NULL);

  String_free(s);
}

void canPushCharToString(void) {
  String s = String_new();

  s = String_pushChar(s, 'a').string;
  s = String_pushChar(s, 'b').string;

  TEST_CHECK(String_len(s) == 2);
  TEST_CHECK(String_capacity(s) == 2);
  TEST_CHECK(!strcmp(s.str, "ab"));

  String_free(s);
}

void canPushCstrToString(void) {
  String s = String_new();

  s = String_pushCstr(s, "Hello").string;
  s = String_pushCstr(s, " World!").string;

  TEST_CHECK(String_len(s) == 12);
  TEST_CHECK(String_capacity(s) == 20);
  TEST_CHECK(!strcmp(s.str, "Hello World!"));

  String_free(s);
}

void canGetStringFromCstr(void) {
  String s = String_fromCstr("Hello World!").string;

  TEST_CHECK(!String_isEmpty(s));
  TEST_CHECK(String_len(s) == 12);
  TEST_CHECK(String_capacity(s) == 13);
  TEST_CHECK(!strcmp(String_toCstr(s), "Hello World!"));

  String_free(s);
}

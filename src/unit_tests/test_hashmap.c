#pragma once

#include "../../includes/acutest.h"
#include "../hashmap.h"
#include <stdio.h>
#include <string.h>

static void printMap(HashMap map) {
  printf("\n");
  printf("\nSize: %zu\nCapacity: %zu\nLoad: %f\n", map.m__size, map.m__capacity,
         (float)map.m__size / (float)map.m__capacity);
  printf("===================================\n");
  for (uintmax_t i = 0; i < map.m__capacity; i++) {
    printf("%s => %p\n", map.m__buckets[i].m__key, map.m__buckets[i].m__value);
  }
  printf("===================================\n");
}

void canCreateNewHashMap(void) {
  HashMap map = HashMap_new();

  TEST_CHECK(map.m__size == 0);
  TEST_CHECK(map.m__capacity == 7);
  TEST_CHECK(map.m__buckets == NULL);

  HashMap_free(map);
}

void canCreateHashMapWithCapacity(void) {
  HashMap map = HashMap_withCapacity(10);

  TEST_CHECK(map.m__size == 0);
  TEST_CHECK(map.m__capacity == 10);
  TEST_CHECK(map.m__buckets == NULL);

  HashMap_free(map);
}

void canInsertIntoHashMap(void) {
  HashMap map = HashMap_new();

  map = HashMap_insert(map, "Hi", "test").data.map;

  TEST_CHECK(map.m__capacity == 7);
  TEST_CHECK(map.m__size == 1);
  TEST_CHECK(map.m__buckets != NULL);

  // printMap(map);

  HashMap_free(map);
}

void canResizeHashMap(void) {
  HashMap map = HashMap_withCapacity(1);

  map = HashMap_insert(map, "Test1", "Value1").data.map;
  map = HashMap_insert(map, "Test2", "Value2").data.map;
  TEST_CHECK(map.m__capacity == 4);
  TEST_CHECK(map.m__size == 2);
  // printMap(map);

  map = HashMap_insert(map, "Test3", "Value3").data.map;
  map = HashMap_insert(map, "Test4", "Value4").data.map;
  TEST_CHECK(map.m__capacity == 8);
  TEST_CHECK(map.m__size == 4);
  // printMap(map);

  map = HashMap_insert(map, "Test5", "Value5").data.map;
  map = HashMap_insert(map, "Test6", "Value6").data.map;
  map = HashMap_insert(map, "Test7", "Value7").data.map;
  TEST_CHECK(map.m__capacity == 16);
  TEST_CHECK(map.m__size == 7);
  // printMap(map);

  HashMap_free(map);
}

void canCheckHashMapContainsKey(void) {
  HashMap map = HashMap_new();

  map = HashMap_insert(map, "Test1", "Value1").data.map;
  map = HashMap_insert(map, "Test2", "Value2").data.map;
  map = HashMap_insert(map, "Test3", "Value3").data.map;
  map = HashMap_insert(map, "Test4", "Value4").data.map;
  map = HashMap_insert(map, "Test5", "Value5").data.map;

  TEST_CHECK(HashMap_contains(map, "Test1"));
  TEST_CHECK(HashMap_contains(map, "Test2"));
  TEST_CHECK(HashMap_contains(map, "Test3"));
  TEST_CHECK(HashMap_contains(map, "Test4"));
  TEST_CHECK(HashMap_contains(map, "Test5"));

  TEST_CHECK(!HashMap_contains(map, "Test7"));
  TEST_CHECK(!HashMap_contains(map, "Random"));
  TEST_CHECK(!HashMap_contains(map, "Some Values"));
}

void canDeleteHashMapEntry(void) {
  HashMap map = HashMap_new();

  map = HashMap_insert(map, "Test1", "Value1").data.map;
  map = HashMap_insert(map, "Test2", "Value2").data.map;
  map = HashMap_insert(map, "Test3", "Value3").data.map;
  map = HashMap_insert(map, "Test4", "Value4").data.map;
  map = HashMap_insert(map, "Test5", "Value5").data.map;
  TEST_CHECK(map.m__size == 5);

  // printMap(map);

  map = HashMap_deleteEntry(map, "Test1").data.map;
  TEST_CHECK(map.m__size == 4);

  HashMapResult res = HashMap_deleteEntry(map, "Test1");
  TEST_CHECK(res.status != Success);

  // printMap(map);
}

void canGetEntryFromHashMap(void) {
  HashMap map = HashMap_new();

  map = HashMap_insert(map, "Test1", "Value1").data.map;
  map = HashMap_insert(map, "Test2", "Value2").data.map;
  map = HashMap_insert(map, "Test3", "Value3").data.map;
  map = HashMap_insert(map, "Test4", "Value4").data.map;
  map = HashMap_insert(map, "Test5", "Value5").data.map;

  Cstr val1 = HashMap_get(map, "Test1").data.value;
  Cstr val2 = HashMap_get(map, "Test2").data.value;
  Cstr val3 = HashMap_get(map, "Test3").data.value;
  Cstr val4 = HashMap_get(map, "Test4").data.value;
  Cstr val5 = HashMap_get(map, "Test5").data.value;

  TEST_CHECK(!strcmp(val1, "Value1"));
  TEST_CHECK(!strcmp(val2, "Value2"));
  TEST_CHECK(!strcmp(val3, "Value3"));
  TEST_CHECK(!strcmp(val4, "Value4"));
  TEST_CHECK(!strcmp(val5, "Value5"));

  enum StatusCode status = HashMap_get(map, "Random").status;
  TEST_CHECK(status != Success);
}

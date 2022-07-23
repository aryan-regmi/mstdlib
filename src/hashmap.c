#include "hashmap.h"
#include "common.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const float MAX_LOAD = 0.85;

static const uintmax_t DEFAULT_CAPACITY = 7;

static const GeneralPurposeAllocator GPA = {
    .alloc = calloc, .free = free, .resize = realloc};

static uintmax_t defaultHashFn(Cstr key) {
  uintmax_t hash = 5381;
  char c;

  while ((c = *key++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

static HashMap unwrapResult(HashMapResult self) {
  if (self.status != Success) {
    err_panic("[UnwrapError] `unwrap` called on invalid value");
  }

  return self.data.map;
}

HashMap HashMap_new(void) {
  return (HashMap){
      .m__buckets = NULL, .m__capacity = DEFAULT_CAPACITY, .m__size = 0};
}

HashMap HashMap_withCapacity(uintmax_t capacity) {
  if (capacity * 2 > UINTMAX_MAX) {
    err_panic("[SizeOverflow Error] The capacity is larger than the allowed "
              "size for `uintmax_t`");
  }

  return (HashMap){.m__capacity = capacity, .m__size = 0, .m__buckets = NULL};
}

void HashMap_free(HashMap map) { (*GPA.free)(map.m__buckets); }

static const float loadFactor(HashMap map) {
  return (float)map.m__size / (float)map.m__capacity;
}

static const bool bucketIsEmpty(HashMap map, uintmax_t idx) {
  return map.m__buckets[idx].m__status == Empty;
}

static HashMapResult resizeMap(HashMap map) {
  if (map.m__capacity * 2 > UINTMAX_MAX) {
    return (HashMapResult){.status = SizeOverflow};
  }

  uintmax_t new_capacity = map.m__capacity * 2;

  HashMap new_map = HashMap_withCapacity(new_capacity);

  // Check if load factor is low enough with initial resize
  volatile float load = (float)map.m__size + 1 / (float)new_map.m__capacity;

  // Recalculate new hashes!!!
  for (uintmax_t i = 0; i < map.m__capacity; i++) {
    if (!bucketIsEmpty(map, i)) {
      Cstr curr_key = map.m__buckets[i].m__key;
      void *curr_value = map.m__buckets[i].m__value;

      HashMapResult insert_res = HashMap_insert(new_map, curr_key, curr_value);
      if (insert_res.status != Success) {
        return (HashMapResult){.status = insert_res.status};
      }
      new_map = insert_res.data.map;
    }
  }

  // Free old map
  HashMap_free(map);

  return (HashMapResult){.status = Success,
                         .data.map =
                             (HashMap){.m__buckets = new_map.m__buckets,
                                       .m__capacity = new_map.m__capacity,
                                       .m__size = new_map.m__size}};
}

HashMapResult HashMap_insert(HashMap map, Cstr key, void *value) {
  // Calculate hash of key
  uintmax_t idx = defaultHashFn(key) % map.m__capacity;

  // Allocate space on first insert
  if (map.m__size == 0) {
    struct Bucket *buckets = (*GPA.alloc)(map.m__capacity, sizeof(*buckets));
    if (!buckets) {
      return (HashMapResult){.status = NullData};
    }

    // Set all buckets to Empty (except for `idx`)
    for (uintmax_t i = 0; i < map.m__capacity; i++) {
      buckets[i].m__status = Empty;
    }
    buckets[idx].m__status = Full;

    // Insert key-value pair, since there can be no collision
    map.m__buckets = buckets;
    map.m__buckets[idx].m__key = key;
    map.m__buckets[idx].m__value = value;
    map.m__buckets[idx].m__status = Full;
    map.m__size++;
  }
  // Add value to map if no collision exists
  else if (bucketIsEmpty(map, idx)) {
    map.m__buckets[idx].m__key = key;
    map.m__buckets[idx].m__value = value;
    map.m__buckets[idx].m__status = Full;
    map.m__size++;
  }
  // Replace existing value if key is in map already
  else if (!strcmp(map.m__buckets[idx].m__key, key)) {
    map.m__buckets[idx].m__value = value;
    map.m__buckets[idx].m__status = Full;
  }
  // Use quadratic probing to find next empty bucket
  else {
    uintmax_t search_count = 1;
    uintmax_t new_idx = (idx + search_count * search_count) % map.m__capacity;
    while (!bucketIsEmpty(map, new_idx)) {
      search_count++;
      new_idx = (idx + search_count * search_count) % map.m__capacity;
    }

    map.m__buckets[new_idx].m__key = key;
    map.m__buckets[new_idx].m__value = value;
    map.m__buckets[new_idx].m__status = Full;
    map.m__size++;
  }

  // Resize if load factor too high
  float load = (float)(map.m__size) / (float)map.m__capacity;
  if (load > MAX_LOAD) {
    HashMapResult resize_res = resizeMap(map);
    if (resize_res.status != Success) {
      return (HashMapResult){.status = resize_res.status};
    }
    map = resize_res.data.map;
  }

  return (HashMapResult){.status = Success, .data.map = map};
}

VEC(Cstr) HashMap_keys(HashMap map) {
  VEC(Cstr) retvec = Vec_new();

  for (uintmax_t i = 0; i < map.m__capacity; i++) {
    if (!bucketIsEmpty(map, i)) {
      VecResult vres = Vec_push(retvec, map.m__buckets[i].m__key);
      if (vres.status != Success) {
        err_panic("[VecError] Error occured while pushing key to vector");
      }
      retvec = vres.vector;
    }
  }

  return retvec;
}

VEC(void *) HashMap_values(HashMap map) {
  VEC(Cstr) retvec = Vec_new();

  for (uintmax_t i = 0; i < map.m__capacity; i++) {
    if (!bucketIsEmpty(map, i)) {
      VecResult vres = Vec_push(retvec, map.m__buckets[i].m__value);
      if (vres.status != Success) {
        err_panic("[VecError] Error occured while pushing value to vector");
      }
      retvec = vres.vector;
    }
  }

  return retvec;
}

HashMapResult HashMap_deleteEntry(HashMap map, Cstr key) {
  uintmax_t idx = defaultHashFn(key) % map.m__capacity;
  uintmax_t probe_counter = 1;

  volatile Cstr curr_key = map.m__buckets[idx].m__key;
  if (!curr_key) {
    curr_key = "";
  }

  // If the key is found at the hash index then delete it, else use quadratic
  // probing to search the rest of the table
  while (strcmp(curr_key, key) != 0) {
    if (probe_counter >= map.m__capacity) {
      return (HashMapResult){.status = InvalidKey};
    }

    idx = (idx + probe_counter * probe_counter) % map.m__capacity;

    curr_key = map.m__buckets[idx].m__key;
    if (!curr_key) {
      curr_key = "";
    }

    probe_counter++;
  }

  map.m__buckets[idx].m__key = NULL;
  map.m__buckets[idx].m__value = NULL;
  map.m__buckets[idx].m__status = Empty;
  map.m__size--;

  return (HashMapResult){.status = Success, .data.map = map};
}

bool HashMap_contains(HashMap map, Cstr key) {
  uintmax_t idx = defaultHashFn(key) % map.m__capacity;
  uintmax_t probe_counter = 1;

  volatile Cstr curr_key = map.m__buckets[idx].m__key;
  if (!curr_key) {
    curr_key = "";
  }

  // If the key is found at the hash index then return, else use quadratic
  // probing to search the rest of the table
  while (strcmp(curr_key, key) != 0) {
    if (probe_counter >= map.m__capacity) {
      return false;
    }

    idx = (idx + probe_counter * probe_counter) % map.m__capacity;

    curr_key = map.m__buckets[idx].m__key;
    if (!curr_key) {
      curr_key = "";
    }

    probe_counter++;
  }

  return true;
}

HashMapResult HashMap_get(HashMap map, Cstr key) {
  uintmax_t idx = defaultHashFn(key) % map.m__capacity;
  uintmax_t probe_counter = 1;

  volatile Cstr curr_key = map.m__buckets[idx].m__key;
  if (!curr_key) {
    curr_key = "";
  }

  // If the key is found at the hash index then return its value, else use
  // quadratic probing to search the rest of the table
  while (strcmp(curr_key, key) != 0) {
    if (probe_counter >= map.m__capacity) {
      return (HashMapResult){.status = InvalidKey};
    }

    idx = (idx + probe_counter * probe_counter) % map.m__capacity;

    curr_key = map.m__buckets[idx].m__key;
    if (!curr_key) {
      curr_key = "";
    }

    probe_counter++;
  }

  return (HashMapResult){.status = Success,
                         .data.value = map.m__buckets[idx].m__value};
}

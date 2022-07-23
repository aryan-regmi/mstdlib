#include "../../includes/acutest.h"
#include "test_hashmap.c"
#include "test_string.c"
#include "test_vector.c"

TEST_LIST = {
    // Vector tests
    {"VECTOR: Create new Vector", canCreateNewVector},
    {"VECTOR: Create new Vector w/ capacity", canCreateVectorWithCapacity},
    {"VECTOR: Push/pop Vector", canPushPopVector},
    {"VECTOR: Set/get Vector index", canSetGetIdxVector},

    // String tests
    {"String: Create new String", canCreateNewString},
    {"String: Create new String w/ capacity", canCreateStringWithCapacity},
    {"String: Push character to String", canPushCharToString},
    {"String: Push C-String to String", canPushCstrToString},
    {"String: Create String from C-String", canGetStringFromCstr},

    // HashMap tests
    {"HashMap: Create new HashMap", canCreateNewHashMap},
    {"HashMap: Create new HashMap w/ capacity", canCreateHashMapWithCapacity},
    {"HashMap: Can insert key-value pair into HashMap", canInsertIntoHashMap},
    {"HashMap: Resize HashMap", canResizeHashMap},
    {"HashMap: Check if key is in HashMap", canCheckHashMapContainsKey},
    {"HashMap: Delete entry from HashMap", canDeleteHashMapEntry},
    {"HashMap: Retrieve entry from HashMap", canGetEntryFromHashMap},
    {NULL, NULL}};

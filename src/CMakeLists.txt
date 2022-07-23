# Add internal libraries
add_library(VectorFunctions vector.c)
add_library(StringFunctions string.c)
add_library(HashMapFunctions hashmap.c)

# Link necessary external libraries to internal ones
# target_link_libraries(VectorFunctions libmsl)

cmake_minimum_required(VERSION 3.21)
project(assignment_2 C)

set(CMAKE_C_STANDARD 11)

add_compile_options(-Wall -Wextra -pedantic -Werror -fstack-protector -fstack-protector-all)

configure_file(hashsettest.c hashsettest.c COPYONLY)
configure_file(thesaurus.txt thesaurus.txt COPYONLY)

set(VECTOR_SOURCE_FILES   vector.c vector.h)
set(HASHSET_SOURCE_FILES  hashset.c hashset.h)
set(STREAMTOKENIZER_SOURCE_FILES  streamtokenizer.c streamtokenizer.h)

add_executable(vectortest vectortest.c ${VECTOR_SOURCE_FILES})
add_executable(hashsettest hashsettest.c ${HASHSET_SOURCE_FILES} ${VECTOR_SOURCE_FILES})
add_executable(thesaurus-lookup thesaurus-lookup.c ${STREAMTOKENIZER_SOURCE_FILES} ${VECTOR_SOURCE_FILES} ${HASHSET_SOURCE_FILES})

include_directories(${KEY_VALUE_STORE_SOURCE_DIR}/utils)
link_directories(${KEY_VALUE_STORE_BINARY_DIR}/utils)

set(PROGRAMS Log TimeUtils)

foreach(program ${PROGRAMS})
  add_library(${program} ${program}.cpp)
endforeach(program)

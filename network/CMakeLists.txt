include_directories(${KEY_VALUE_STORE_SOURCE_DIR}/network)
link_directories(${KEY_VALUE_STORE_BINARY_DIR}/network)

set(PROGRAMS Address EmulNet Message Network Queue)

foreach(program ${PROGRAMS})
  add_library(${program} ${program}.cpp)
endforeach(program)

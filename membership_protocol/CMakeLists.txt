include_directories(${KEY_VALUE_STORE_SOURCE_DIR}/membership_protocol)
link_directories(${KEY_VALUE_STORE_BINARY_DIR}/membership_protocol)

set(PROGRAMS MembershipProtocol Node)

foreach(program ${PROGRAMS})
  add_library(${program} ${program}.cpp)
endforeach(program)

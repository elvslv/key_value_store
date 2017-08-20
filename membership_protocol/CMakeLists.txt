include_directories(${KEY_VALUE_STORE_SOURCE_DIR}/membership_protocol)
link_directories(${KEY_VALUE_STORE_BINARY_DIR}/membership_protocol)

set(PROGRAMS AckMessage Events JoinRepMessage JoinReqMessage MembershipProtocol Node PingMessage PingMessageBase PingReqMessage)

foreach(program ${PROGRAMS})
  add_library(${program} ${program}.cpp)
endforeach(program)

add_library(membership_protocol_message Message.cpp)
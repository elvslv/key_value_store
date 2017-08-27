# include_directories(${KEY_VALUE_STORE_SOURCE_DIR}/membership_protocol)
# link_directories(${KEY_VALUE_STORE_BINARY_DIR}/membership_protocol)

# set(PROGRAMS AckMessage Events JoinRepMessage JoinReqMessage MembershipProtocol Node PingMessage PingMessageBase PingReqMessage)

# foreach(program ${PROGRAMS})
#   add_library(${program} ${program}.cpp)
# endforeach(program)

# add_library(membership_protocol_message Message.cpp)

target_sources(my_target PRIVATE 
  "membership_protocol/AckMessage.cpp" 
  "membership_protocol/Events.cpp" 
  "membership_protocol/JoinRepMessage.cpp"
  "membership_protocol/JoinReqMessage.cpp"  
  "membership_protocol/MembershipProtocol.cpp" 
  "membership_protocol/Message.cpp" 
  "membership_protocol/Node.cpp" 
  "membership_protocol/PingMessage.cpp" 
  "membership_protocol/PingMessageBase.cpp" 
  "membership_protocol/PingReqMessage.cpp"
)
include_directories(${KEY_VALUE_STORE_SOURCE_DIR}/build/gen)
link_directories(${KEY_VALUE_STORE_BINARY_DIR}/build/gen)

add_library(gen_network_Address Address.pb.cc)
add_library(gen_membership_protocol_Message Message.pb.cc)
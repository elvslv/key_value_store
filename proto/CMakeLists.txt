INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER Address.proto MembershipProtocolMessage.proto KeyValueStoreMessage.proto)
ADD_LIBRARY(proto ${PROTO_HEADER} ${PROTO_SRC})

target_link_libraries(proto
  INTERFACE
    ${Protobuf_LIBRARIES}
  )

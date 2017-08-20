protoc --cpp_out=build/gen network/Address.proto
protoc --cpp_out=build/gen/membership_protocol --proto_path=network --proto_path=membership_protocol membership_protocol/Message.proto
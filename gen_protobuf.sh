protoc --cpp_out=build/gen --proto_path=network network/Address.proto
protoc --cpp_out=build/gen --proto_path=network --proto_path=membership_protocol membership_protocol/Message.proto
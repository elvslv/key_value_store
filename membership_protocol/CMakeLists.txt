add_subdirectory(messages)
add_subdirectory(failure_detector)
add_subdirectory(gossip_protocol)

set(membership_protocol_src_files MembershipProtocol Node MembershipUpdate)
foreach(membership_protocol_src_file ${membership_protocol_src_files})
  target_sources(key_value_store_lib PRIVATE "membership_protocol/${membership_protocol_src_file}.cpp")
endforeach(membership_protocol_src_file)
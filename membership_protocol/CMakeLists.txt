add_subdirectory(messages)

set(membership_protocol_src_files MembershipProtocol Node FailureDetector)
foreach(membership_protocol_src_file ${membership_protocol_src_files})
  target_sources(key_value_store_lib PRIVATE "membership_protocol/${membership_protocol_src_file}.cpp")
endforeach(membership_protocol_src_file)
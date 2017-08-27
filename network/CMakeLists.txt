set(network_src_files Address EmulNet Message Network Queue)

foreach(network_src_file ${network_src_files})
  target_sources(key_value_store_lib PRIVATE "network/${network_src_file}.cpp")
endforeach(network_src_file)

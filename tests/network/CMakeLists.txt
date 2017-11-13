set(network_test_files AddressTests EmulNetTests)

foreach(network_test_file ${network_test_files})
  target_sources(key_value_store_tests PRIVATE "network/${network_test_file}.cpp")
endforeach(network_test_file)

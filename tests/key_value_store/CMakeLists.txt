set(key_value_store_test_files MessageTests PartitionerTests NodeTests)

foreach(key_value_store_test_file ${key_value_store_test_files})
  target_sources(key_value_store_tests PRIVATE "key_value_store/${key_value_store_test_file}.cpp")
endforeach(key_value_store_test_file)

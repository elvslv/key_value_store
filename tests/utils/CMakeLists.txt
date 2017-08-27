set(utils_test_files LogTests)

foreach(utils_test_file ${utils_test_files})
  target_sources(key_value_store_tests PRIVATE "utils/${utils_test_file}.cpp")
endforeach(utils_test_file)

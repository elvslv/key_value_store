set(key_value_store_src_files Node)

foreach(key_value_store_src_file ${key_value_store_src_files})
  target_sources(key_value_store_lib PRIVATE "key_value_store/${key_value_store_src_file}.cpp")
endforeach(key_value_store_src_file)

set(utils_src_files Log TimeUtils ThreadPolicy Utils Runnable Message)

foreach(util_src_file ${utils_src_files})
  target_sources(key_value_store_lib PRIVATE "utils/${util_src_file}.cpp")
endforeach(util_src_file)
set(utils_src_files AsyncQueue Log MessageDispatcher TimeUtils ThreadPolicy Utils Runnable)

foreach(util_src_file ${utils_src_files})
  target_sources(key_value_store_lib PRIVATE "utils/${util_src_file}.cpp")
endforeach(util_src_file)
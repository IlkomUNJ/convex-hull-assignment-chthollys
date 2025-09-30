# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/myQtApp_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/myQtApp_autogen.dir/ParseCache.txt"
  "myQtApp_autogen"
  )
endif()

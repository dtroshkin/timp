# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\timp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\timp_autogen.dir\\ParseCache.txt"
  "timp_autogen"
  )
endif()

cmake_minimum_required(VERSION 3.4.1)

aux_source_directory(src/main/cpp DIR_SRCS)

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp
              ${DIR_SRCS})

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

add_library(lame SHARED IMPORTED)
set_target_properties(lame  PROPERTIES IMPORTED_LOCATION
${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/liblame.so)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include)

target_link_libraries(native-lib

                       lame
                       ${log-lib} )
cmake_minimum_required(VERSION 3.17)

Project(Shared-memory)

set(CMAKE_C_STANDARD 11)

add_subdirectory(src)
add_subdirectory(include)

if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  set(SRC
    src/main.c
    src/posix.c
  )
else()
  set(SRC
    src/main.c
    src/win32.c
  )
endif()

set(HEADERS
  include/common.h
 )

add_executable(shared_memory ${SRC} ${HEADERS})

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_link_libraries(shared_memory PRIVATE rt)
endif()

target_include_directories(shared_memory PUBLIC include)

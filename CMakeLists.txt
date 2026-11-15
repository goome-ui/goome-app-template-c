cmake_minimum_required(VERSION 3.16)
project(my-goome-app C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(my-goome-app main.c goome.c)
if(UNIX AND NOT APPLE)
  target_link_libraries(my-goome-app PRIVATE ${CMAKE_DL_LIBS})
endif()

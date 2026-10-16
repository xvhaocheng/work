cmake_minimum_required(VERSION 3.13.4)
     project(foo C)
     set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--warn-shared-textrel")
     add_library(foo SHARED src_to_link.c)
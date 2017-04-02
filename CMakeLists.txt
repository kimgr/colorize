cmake_minimum_required(VERSION 3.0)

project(colorize)

add_executable(colorize
  colorize.cpp
  print_ansi.cpp
)

install(TARGETS colorize RUNTIME DESTINATION bin)

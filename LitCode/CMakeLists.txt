﻿# CMakeList.txt : CMake project for LitCode, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (LitCode "LitCode.cpp" "LitCode.h" "Lexer/Lexer.cpp" "Lexer/Lexer.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET LitCode PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

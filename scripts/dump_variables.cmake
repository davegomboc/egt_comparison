#!/usr/bin/cmake -P
cmake_minimum_required(VERSION 3.5.2 FATAL_ERROR)
cmake_policy(VERSION 3.5.2)

get_cmake_property(defined_variables VARIABLES)
foreach(defined_variable ${defined_variables})
    message(STATUS "${defined_variable} is set to |${${defined_variable}}|.")
endforeach()

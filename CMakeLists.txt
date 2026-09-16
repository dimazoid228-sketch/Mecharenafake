cmake_minimum_required(VERSION 3.20)
project(MechFakeMenu LANGUAGES CXX)
add_library(MechFakeMenu SHARED src/MechFakeMenu.cpp)
set_target_properties(MechFakeMenu PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

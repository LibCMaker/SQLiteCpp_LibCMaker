# ****************************************************************************
#  Project:  LibCMaker
#  Purpose:  A CMake build scripts for build libraries with CMake
#  Author:   NikitaFeodonit, nfeodonit@yandex.com
# ****************************************************************************
#    Copyright (c) 2017-2026 NikitaFeodonit
#
#    This file is part of the LibCMaker project.
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published
#    by the Free Software Foundation, either version 3 of the License,
#    or (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#    See the GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program. If not, see <http://www.gnu.org/licenses/>.
# ****************************************************************************

set(LibCMaker_LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/libs")
set(LibCMaker_DIR "${LibCMaker_LIB_DIR}/LibCMaker")

include("${LibCMaker_DIR}/cmake/cmr_get_cmake_min_ver.cmake" NO_POLICY_SCOPE)
cmr_get_cmake_min_ver()
cmake_minimum_required(VERSION ${cmr_CMAKE_MIN_VER})

include("${LibCMaker_DIR}/samples/cmr_samples_common_macros.cmake")
cmr_common_sample_part__before_project()

project(LibCMaker_SQLiteCpp_Compile_Test CXX C)

cmr_common_sample_part__project_settings()


#-----------------------------------------------------------------------
# Unpack, configure, build and find the required libraries
#-----------------------------------------------------------------------

option(SQLITE_ENABLE_ICU
  "the ICU extension to SQLite to be added"
  ON
)
option(SQLITE_THREADSAFE
  "sets the default threading mode to Multi-threaded, SQLITE_THREADSAFE=2 (ON), SQLITE_THREADSAFE=0 (OFF)"
  ON
)

if(SQLITE_ENABLE_ICU)
  list(APPEND CMAKE_PREFIX_PATH ${LibCMaker_LIB_DIR}/ICU_LibCMaker/cmake)
  find_package(ICU REQUIRED NO_PACKAGE_ROOT_PATH)
endif()

list(APPEND CMAKE_PREFIX_PATH ${LibCMaker_LIB_DIR}/SQLiteCpp_LibCMaker/cmake)
find_package(SQLiteCpp REQUIRED CONFIG)


#-----------------------------------------------------------------------
# Build the executable of the example
#-----------------------------------------------------------------------

cmr_common_sample_part__add_executable()
cmr_common_sample_part__set_cxx_c_standards()

set(example_src_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

target_sources(${PROJECT_NAME}
  PRIVATE
    ${example_src_DIR}/example.cpp
)


#-----------------------------------------------------------------------
# Link to the libraries
#-----------------------------------------------------------------------

# SQLiteCpp
target_link_libraries(${PROJECT_NAME} PRIVATE SQLiteCpp)

if(SQLITE_ENABLE_ICU)
  # ICU
  target_link_libraries(${PROJECT_NAME} PRIVATE ICU::io ICU::i18n ICU::uc ICU::data)
endif()


#-----------------------------------------------------------------------
# Testing
#-----------------------------------------------------------------------

cmr_common_test_part__add_subdirectory_test()

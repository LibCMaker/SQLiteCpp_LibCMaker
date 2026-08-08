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


#-----------------------------------------------------------------------
# Library version
#

set(SQLiteCpp_VERSION "3.4.0.20260724" CACHE STRING "SQLiteCpp_VERSION")
set(SQLiteCpp_sha256
  "9fd7f9db1a46bf4bdce6901ccec269de679a8aaeb665538c8b6476543368638c"
  CACHE STRING "SQLiteCpp_sha256"
)


#-----------------------------------------------------------------------
# SQLite3 and SQLiteCpp options
#-----------------------------------------------------------------------

########################################################################
# "Compile-time Options"
# https://sqlite.org/compile.html
########################################################################

# These options require SQLITECPP_INTERNAL_SQLITE=ON
# TODO: correct texts in opts.


######## Options To Enable Features Normally Turned Off ########

option(SQLITE_ENABLE_ICU
  "the ICU extension to SQLite to be added"
  OFF
)
option(SQLITE_ENABLE_FTS3
  "versions 3 and 4 of the full-text search engine"
  OFF
)
option(SQLITE_ENABLE_FTS3_PARENTHESIS
  "modifies the query pattern parser in FTS3 such that it supports operators AND and NOT and also allows query expressions to contain nested parenthesis"
  OFF
)
option(SQLITE_ENABLE_FTS4
  "versions 3 and 4 of the full-text search engine"
  OFF
)
option(SQLITE_ENABLE_FTS5
  "versions 5 of the full-text search engine"
  OFF
)

option(SQLITE_ENABLE_COLUMN_METADATA
  "Enable APIs that provide convenient access to meta-data about tables and queries. Enable Column::getColumnOriginName() in SQLiteCpp."
  OFF
)
option(SQLITE_ENABLE_DBSTAT_VTAB
  "Enable DBSTAT, read-only eponymous virtual table"
  OFF
)
option(SQLITE_ENABLE_RTREE
  "Enable the R*Tree index extension"
  OFF
)


######## Options To Control Operating Characteristics ########

option(SQLITE_THREADSAFE
  "sets the default threading mode to Multi-threaded, SQLITE_THREADSAFE=2 (ON), SQLITE_THREADSAFE=0 (OFF)"
  ON
)


######## Options To Omit Features ########

option(SQLITE_OMIT_LOAD_EXTENSION
  "Enable omit the entire extension loading mechanism from SQLite"
  OFF
)


######## Analysis and Debugging Options ########

option(SQLITE_DEBUG
  "Enable debugging features"
  OFF
)
option(SQLITE_MEMDEBUG
  "Enable instrumented debugging memory allocator"
  OFF
)


########################################################################
# Options relative to SQLiteCpp
########################################################################

option(SQLITECPP_BUILD_TESTS "Build and run tests." OFF)
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

if (MSVC)
    # Flags for linking with multithread static C++ runtime, required by internal googletest
    option(SQLITECPP_USE_STATIC_RUNTIME "Use MSVC static runtime (default for internal googletest)." FALSE)
else () # Unix/macOS/MinGW

    if (NOT MINGW)
        # Stack protection is not supported on MinGW-W64 on Windows.
        # Allow this flag to be turned off.
        option(SQLITECPP_USE_STACK_PROTECTION "USE Stack Protection hardening." ON)
    endif ()
    if (CMAKE_COMPILER_IS_GNUCXX)
        # GCC flags
        option(SQLITECPP_USE_GCOV "USE GCov instrumentation." OFF)
    endif ()
endif ()

option(SQLITECPP_INCLUDE_SCRIPT "Include config & script files." OFF)

# Options relative to SQLiteC++ functions

option(SQLITE_ENABLE_ASSERT_HANDLER "Enable the user definition of a assertion_failed() handler." OFF)
option(SQLITE_HAS_CODEC "Enables usage of the Database key() and rekey() methods. Requires a custom SQLite providing the key API such as sqlcipher/sql-ee/sqlite-multi-ciphers" OFF)
option(SQLITE_USE_LEGACY_STRUCT "Fallback to forward declaration of legacy struct sqlite3_value (pre SQLite 3.19)" OFF)
option(SQLITECPP_USE_ASAN "Use Address Sanitizer." OFF)

## Build provided copy of SQLite3 C library ##

option(SQLITECPP_INTERNAL_SQLITE "Add the internal SQLite3 source to the project." ON)
option(SQLITECPP_FIND_SQLITE "Attempt to find SQLite (or sqlcipher, with SQLITE_HAS_CODEC) automatically. Requires SQLITECPP_INTERNAL_SQLITE=OFF. If disabled then a SQLite::SQLite3 cmake target must already exist." OFF)

## disable the optional support for std::filesystem (C++17)
option(SQLITECPP_DISABLE_STD_FILESYSTEM "Disable the use of std::filesystem in SQLiteCpp." OFF)

## disable the optional support for sqlite3_expanded_sql (from sqlite3 3.14.0)
option(SQLITECPP_DISABLE_EXPANDED_SQL "Disable the use of sqlite3_expanded_sql in SQLiteCpp." OFF)

# Allow the library to be installed via "make install" and found with "find_package"
option(SQLITECPP_INSTALL "Enables the install target." ON)

# Optional additional targets:
option(SQLITECPP_RUN_CPPLINT "Run cpplint.py tool for Google C++ StyleGuide." OFF)
option(SQLITECPP_RUN_CPPCHECK "Run cppcheck C++ static analysis tool." OFF)
option(SQLITECPP_RUN_DOXYGEN "Run Doxygen C++ documentation tool." OFF)
option(SQLITECPP_BUILD_EXAMPLES "Build examples." OFF)


#-----------------------------------------------------------------------
# Unpack, configure and build the required libraries
#-----------------------------------------------------------------------

find_package(Git 2.17.1 REQUIRED)

include(FetchContent)

set(sql_SRC_NAME "SQLiteCpp-${SQLiteCpp_VERSION}")
set(sql_ARCHIVE_DIR "${CMAKE_CURRENT_LIST_DIR}/source")
set(sql_PATCH_DIR "${sql_ARCHIVE_DIR}/patch/${sql_SRC_NAME}")

FetchContent_Declare(
  SQLiteCpp
  URL "${CMAKE_CURRENT_LIST_DIR}/source/${sql_SRC_NAME}.tar.xz"
  URL_HASH "SHA256=${SQLiteCpp_sha256}"
  OVERRIDE_FIND_PACKAGE
  PATCH_COMMAND
    ${GIT_EXECUTABLE} apply
      "${sql_PATCH_DIR}/sqlite3__Add_more_compile-time_options.patch"
)

FetchContent_MakeAvailable(SQLiteCpp)

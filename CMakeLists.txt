cmake_minimum_required( VERSION 2.8 )
project( ziplib )

set( ziplib_VERSION_MAJOR 0 )
set( ziplib_VERSION_MINOR 1 )

add_library( tarlib
	src/content.cpp
	src/header.cpp
	src/inflate_bzip2.cpp
	src/inflate_deflate.cpp
	src/inflate_implode.cpp
	src/inflate_lzma.cpp
	src/internal.cpp
	src/sparse_array.cpp
)
include_directories( ${ziplib_SOURCE_DIR}/include )
list( APPEND CMAKE_CXX_FLAGS -std=c++11 )

#add_executable( untar test/test.cpp )
#target_link_libraries( untar tarlib )


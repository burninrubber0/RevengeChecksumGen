cmake_minimum_required(VERSION 3.14)
project(RevengeChecksumGen CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES
	${SOURCES}
	src/checksum.cpp
	src/main.cpp
	src/mc02.cpp
	)

set(HEADERS
	${HEADERS}
	include/checksum.h
	include/mc02.h
	)

add_executable(RevengeChecksumGen ${SOURCES} ${HEADERS})

find_package(Qt6 COMPONENTS Core REQUIRED)

# DataStream
set(DATASTREAM_ROOT ${ROOT}\\external\\DataStream)
add_subdirectory(external/DataStream "${CMAKE_CURRENT_BINARY_DIR}/external/DataStream" EXCLUDE_FROM_ALL)
target_include_directories(RevengeChecksumGen PRIVATE ${DATASTREAM_ROOT} "${CMAKE_CURRENT_BINARY_DIR}/external/DataStream")

# RevengeChecksumGen includes
target_include_directories(RevengeChecksumGen PRIVATE "${ROOT}/include")

target_link_libraries(RevengeChecksumGen PRIVATE DataStream Qt6::Core)

# VS stuff
set_property(DIRECTORY ${ROOT} PROPERTY VS_STARTUP_PROJECT RevengeChecksumGen)
source_group(TREE ${ROOT} FILES ${SOURCES} ${HEADERS})

if (WIN32)
	add_custom_command(TARGET RevengeChecksumGen POST_BUILD
		COMMAND Qt6::windeployqt ARGS $<TARGET_FILE:RevengeChecksumGen>
	)
endif()

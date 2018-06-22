set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(cpp_arithmetic LANGUAGES CXX)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
	add_compile_options("-frounding-math")
endif()

add_executable(test_interval app/test_interval.cpp include/ra/interval.hpp)
add_executable(test_kernel app/test_kernel.cpp include/ra/kernel.hpp)
add_executable(delaunay_triangulation app/delaunay_triangulation.cpp)


find_package(CGAL REQUIRED COMPONENTS)
include_directories(${CGAL_INCLUDE_DIR})
set(kernel_dependencies ${CGAL_LIBRARY} ${GMP_LIBARIRES})


target_include_directories(test_interval PUBLIC include "${CMAKE_CURRENT_BINARY_DIR}/include")

target_link_libraries(test_kernel ${kernel_dependencies})
target_include_directories(test_kernel PUBLIC include "${CMAKE_CURRENT_BINARY_DIR}/include")

target_include_directories(delaunay_triangulation PUBLIC include ${CGAL_INCLUDE_DIRS})
target_link_libraries(delaunay_triangulation ${CGAL_LIBRARY} ${GMP_LIBRARIES})

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frounding-math")


option(ENABLE_ASAN "Initial Address Sanitizer" false)
if (ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

option(ENABLE_UBSAN "Initial Undefined Behaviour Sanitizer" false)
if (ENABLE_UBSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
endif()




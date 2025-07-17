# libcurl CMake configuration helper
# Add this to your CMakeLists.txt:
#
# set(CURL_ROOT "path/to/libcurl")
# find_package(CURL REQUIRED)
# target_link_libraries(your_target ${CURL_LIBRARIES})
# target_include_directories(your_target PRIVATE ${CURL_INCLUDE_DIRS})

set(CURL_ROOT "${CMAKE_CURRENT_LIST_DIR}")
set(CURL_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")
set(CURL_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/libcurl.lib")
set(CURL_FOUND TRUE)

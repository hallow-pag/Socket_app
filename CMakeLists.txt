cmake_minimum_required(VERSION 3.10)
project(socket_app)

set(CMAKE_CXX_STANDARD 14)
aux_source_directory(. SOURCE_DIR)
add_executable(socket_app ${SOURCE_DIR})
#target_link_directories(socket_app .)
# 链接本地的sqlite3库
target_link_libraries(socket_app sqlite3)
project(ChessMatchServer)
CMAKE_MINIMUM_REQUIRED(VERSION 3.19)
SET (CXX_STANDARD 17)
add_executable(ChessMatchServer main.cpp Server.cpp Server.h ClientConnection.cpp ClientConnection.h)

find_package(Boost 1.74.0 REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
target_link_libraries(ChessMatchServer ${Boost_LIBRARIES})



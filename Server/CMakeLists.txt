project(ChessMatchServer)
CMAKE_MINIMUM_REQUIRED(VERSION 3.19)
SET (CXX_STANDARD 17)
add_executable(ChessMatchServer main.cpp Server.cpp Server.h SqlService.cpp SqlService.h ClientConnection.cpp ClientConnection.h ConnectionsVector.cpp ConnectionsVector.h Message.cpp Message.h)

find_package(Boost 1.74.0 REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

set(PQXX /usr/local/include/pqxx)

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
target_link_libraries(ChessMatchServer ${Boost_LIBRARIES} ${PQXX_LIB} ${PQ_LIB})



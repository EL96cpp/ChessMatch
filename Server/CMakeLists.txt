project(ChessMatchServer)
CMAKE_MINIMUM_REQUIRED(VERSION 3.19)
SET (CXX_STANDARD 20)
add_executable(ChessMatchServer main.cpp ThreadSafeQueue.cpp ThreadSafeQueue.h ThreadSafeClientsQueue.cpp ThreadSafeClientsQueue.h SqlService.cpp SqlService.h 
               Pawn.cpp Pawn.h Rook.cpp Rook.h Knight.cpp Knight.h Bishop.cpp Bishop.h Queen.cpp Queen.h King.cpp King.h
               ThreadSafeMessagesQueue.cpp ThreadSafeMessagesQueue.h ThreadSafeGameMessagesQueue.cpp ThreadSafeGameMessagesQueue.h GameResult.h
               ClientConnection.cpp ClientConnection.h Message.cpp Message.h Server.cpp Server.h GamesManager.cpp GamesManager.h GameMessage.cpp GameMessage.h)

find_package(Boost 1.74.0 REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

set(PQXX /usr/local/include/pqxx)

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
target_link_libraries(ChessMatchServer ${Boost_LIBRARIES} ${PQXX_LIB} ${PQ_LIB})



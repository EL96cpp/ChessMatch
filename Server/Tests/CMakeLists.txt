project(ChessMatchServerTests)
CMAKE_MINIMUM_REQUIRED(VERSION 3.19)
SET(CXX_STANDARD 20)

add_executable(ChessMatchServerTests main.cpp ../ThreadSafeQueue.cpp ../ThreadSafeQueue.h ../ThreadSafeClientsQueue.cpp ../ThreadSafeClientsQueue.h ../SqlService.cpp ../SqlService.h 
               ../Pawn.cpp ../Pawn.h ../Rook.cpp ../Rook.h ../Knight.cpp ../Knight.h ../Bishop.cpp ../Bishop.h ../Queen.cpp ../Queen.h ../King.cpp ../King.h
               ../ThreadSafeMessagesQueue.cpp ../ThreadSafeMessagesQueue.h ../ThreadSafeGameMessagesQueue.cpp ../ThreadSafeGameMessagesQueue.h ../GameResult.h
               ../ClientConnection.cpp ../ClientConnection.h ../Message.cpp ../Message.h ../Server.cpp ../Server.h ../GamesManager.cpp ../GamesManager.h 
               ../GameMessage.cpp ../GameMessage.h
               FirstMoveFixture.cpp FirstMoveFixture.h FirstMoveFixtureTests.cpp EmptyBoardMovesFixture.cpp EmptyBoardMovesFixture.h EmptyBoardMovesFixtureTests.cpp
               GameFixture.cpp GameFixture.h GameFixtureTests.cpp)



find_package(GTest REQUIRED)
find_package(Boost 1.74.0 REQUIRED)
include_directories(${GTEST_INCLUDE_DIR} SYSTEM ${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

set(PQXX /usr/local/include/pqxx)

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

target_link_libraries(ChessMatchServerTests GTest::GTest GTest::Main ${Boost_LIBRARIES} ${PQXX_LIB} ${PQ_LIB})



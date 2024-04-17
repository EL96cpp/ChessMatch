#include <algorithm>

#include "EmptyBoardMovesFixture.h"

using namespace testing;


//==================== Rook tests ====================

TEST_F(EmptyBoardMovesFixture, white_rook_empty_board_tests) {


    CreateFigure(Color::WHITE, FigureType::ROOK, 0, 0);

    std::vector<std::pair<size_t, size_t>> upper_left_corner_expected_moves;

    for (int i = 1; i < 8; ++i) {

        upper_left_corner_expected_moves.emplace_back(0, i);
        upper_left_corner_expected_moves.emplace_back(i, 0);

    }

    std::vector<std::pair<size_t, size_t>> upper_left_corner_moves = CalculateMovesForSelectedFigure(0, 0);

    EXPECT_TRUE(std::is_permutation(upper_left_corner_expected_moves.begin(), upper_left_corner_expected_moves.end(), upper_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::ROOK, 0, 7);

    std::vector<std::pair<size_t, size_t>> upper_right_corner_expected_moves;

    for (int i = 0; i < 7; ++i) {

        upper_right_corner_expected_moves.emplace_back(0, i);
        upper_right_corner_expected_moves.emplace_back(7-i, 7);

    }

    std::vector<std::pair<size_t, size_t>> upper_right_corner_moves = CalculateMovesForSelectedFigure(0, 7);

    EXPECT_TRUE(std::is_permutation(upper_right_corner_expected_moves.begin(), upper_right_corner_expected_moves.end(), upper_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::ROOK, 7, 7);

    std::vector<std::pair<size_t, size_t>> lower_right_corner_expected_moves;

    for (int i = 0; i < 7; ++i) {

        lower_right_corner_expected_moves.emplace_back(7, i);
        lower_right_corner_expected_moves.emplace_back(i, 7);

    }

    std::vector<std::pair<size_t, size_t>> lower_right_corner_moves = CalculateMovesForSelectedFigure(7, 7);

    EXPECT_TRUE(std::is_permutation(lower_right_corner_expected_moves.begin(), lower_right_corner_expected_moves.end(), lower_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::ROOK, 7, 0);
    
    std::vector<std::pair<size_t, size_t>> lower_left_corner_expected_moves;

    for (int i = 0; i < 7; ++i) {

        lower_left_corner_expected_moves.emplace_back(i, 0);
        lower_left_corner_expected_moves.emplace_back(7, 7-i);

    }

    std::vector<std::pair<size_t, size_t>> lower_left_corner_moves = CalculateMovesForSelectedFigure(7, 0);

    EXPECT_TRUE(std::is_permutation(lower_left_corner_expected_moves.begin(), lower_left_corner_expected_moves.end(), lower_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::ROOK, 4, 4);

    std::vector<std::pair<size_t, size_t>> center_expected_moves;

    for (int i = 1; i <= 4; ++i) {

        center_expected_moves.emplace_back(4, 4-i);
        center_expected_moves.emplace_back(4-i, 4);

    }

    for (int i = 1; i <= 3; ++i) {

        center_expected_moves.emplace_back(4, 4+i);
        center_expected_moves.emplace_back(4+i, 4);

    }


    std::vector<std::pair<size_t, size_t>> center_moves = CalculateMovesForSelectedFigure(4, 4);

    EXPECT_TRUE(std::is_permutation(center_expected_moves.begin(), center_expected_moves.end(), center_moves.begin()));

    TearDown();
    SetUp();

}


TEST_F(EmptyBoardMovesFixture, rook_opponent_figures_tests) {


    CreateFigure(Color::WHITE, FigureType::ROOK, 0, 0);
    CreateFigure(Color::BLACK, FigureType::PAWN, 0, 3);
    CreateFigure(Color::BLACK, FigureType::PAWN, 3, 0);

    std::vector<std::pair<size_t, size_t>> upper_left_corner_expected_moves;

    for (int i = 1; i <= 3; ++i) {

        upper_left_corner_expected_moves.emplace_back(0, i);
        upper_left_corner_expected_moves.emplace_back(i, 0);

    }

    std::vector<std::pair<size_t, size_t>> upper_left_corner_moves = CalculateMovesForSelectedFigure(0, 0);

    EXPECT_TRUE(std::is_permutation(upper_left_corner_expected_moves.begin(), upper_left_corner_expected_moves.end(), upper_left_corner_moves.begin()));

    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::ROOK, 0, 7);
    CreateFigure(Color::BLACK, FigureType::PAWN, 0, 3);
    CreateFigure(Color::BLACK, FigureType::PAWN, 4, 7);
 
    std::vector<std::pair<size_t, size_t>> upper_right_corner_expected_moves;

    for (int i = 1; i <= 4; ++i) {

        upper_right_corner_expected_moves.emplace_back(0, 7-i);
        upper_right_corner_expected_moves.emplace_back(0+i, 7);

    }

    std::vector<std::pair<size_t, size_t>> upper_right_corner_moves = CalculateMovesForSelectedFigure(0, 7);

    EXPECT_TRUE(std::is_permutation(upper_right_corner_expected_moves.begin(), upper_right_corner_expected_moves.end(), upper_right_corner_moves.begin()));
  
    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::ROOK, 7, 7);
    CreateFigure(Color::BLACK, FigureType::PAWN, 4, 7);
    CreateFigure(Color::BLACK, FigureType::PAWN, 7, 4);

    std::vector<std::pair<size_t, size_t>> lower_right_corner_expected_moves;

    for (int i = 1; i <= 3; ++i) {

        lower_right_corner_expected_moves.emplace_back(7-i, 7);
        lower_right_corner_expected_moves.emplace_back(7, 7-i);

    }

    std::vector<std::pair<size_t, size_t>> lower_right_corner_moves = CalculateMovesForSelectedFigure(7, 7);

    EXPECT_TRUE(std::is_permutation(lower_right_corner_expected_moves.begin(), lower_right_corner_expected_moves.end(), lower_right_corner_moves.begin()));

    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::ROOK, 7, 0);
    CreateFigure(Color::BLACK, FigureType::PAWN, 3, 0);
    CreateFigure(Color::BLACK, FigureType::PAWN, 7, 4);
    
    std::vector<std::pair<size_t, size_t>> lower_left_corner_expected_moves;

    for (int i = 1; i <= 4; ++i) {

        lower_left_corner_expected_moves.emplace_back(7-i, 0);
        lower_left_corner_expected_moves.emplace_back(7, i);

    }

    std::vector<std::pair<size_t, size_t>> lower_left_corner_moves = CalculateMovesForSelectedFigure(7, 0);

    EXPECT_TRUE(std::is_permutation(lower_left_corner_expected_moves.begin(), lower_left_corner_expected_moves.end(), lower_left_corner_moves.begin()));

    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::ROOK, 4, 4);
    CreateFigure(Color::BLACK, FigureType::PAWN, 1, 4);
    CreateFigure(Color::BLACK, FigureType::PAWN, 7, 4);
    CreateFigure(Color::BLACK, FigureType::PAWN, 4, 1);
    CreateFigure(Color::BLACK, FigureType::PAWN, 4, 7);

    std::vector<std::pair<size_t, size_t>> center_expected_moves;

    for (int i = 1; i <= 3; ++i) {

        center_expected_moves.emplace_back(4-i, 4);
        center_expected_moves.emplace_back(4, 4-i);
        center_expected_moves.emplace_back(4+i, 4);
        center_expected_moves.emplace_back(4, 4+i);

    }

    std::vector<std::pair<size_t, size_t>> center_moves = CalculateMovesForSelectedFigure(4, 4);

    EXPECT_TRUE(std::is_permutation(center_expected_moves.begin(), center_expected_moves.end(), center_moves.begin()));

    TearDown();
    SetUp();

}


TEST_F(EmptyBoardMovesFixture, rook_own_figures_tests) {


    CreateFigure(Color::WHITE, FigureType::ROOK, 0, 0);
    CreateFigure(Color::WHITE, FigureType::PAWN, 0, 3);
    CreateFigure(Color::WHITE, FigureType::PAWN, 3, 0);

    std::vector<std::pair<size_t, size_t>> upper_left_corner_expected_moves;

    for (int i = 1; i <= 2; ++i) {

        upper_left_corner_expected_moves.emplace_back(0, i);
        upper_left_corner_expected_moves.emplace_back(i, 0);

    }

    std::vector<std::pair<size_t, size_t>> upper_left_corner_moves = CalculateMovesForSelectedFigure(0, 0);

    EXPECT_TRUE(std::is_permutation(upper_left_corner_expected_moves.begin(), upper_left_corner_expected_moves.end(), upper_left_corner_moves.begin()));

    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::ROOK, 0, 7);
    CreateFigure(Color::WHITE, FigureType::PAWN, 0, 3);
    CreateFigure(Color::WHITE, FigureType::PAWN, 4, 7);
 
    std::vector<std::pair<size_t, size_t>> upper_right_corner_expected_moves;

    for (int i = 1; i <= 3; ++i) {

        upper_right_corner_expected_moves.emplace_back(0, 7-i);
        upper_right_corner_expected_moves.emplace_back(0+i, 7);

    }

    std::vector<std::pair<size_t, size_t>> upper_right_corner_moves = CalculateMovesForSelectedFigure(0, 7);

    EXPECT_TRUE(std::is_permutation(upper_right_corner_expected_moves.begin(), upper_right_corner_expected_moves.end(), upper_right_corner_moves.begin()));
  
    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::ROOK, 7, 7);
    CreateFigure(Color::WHITE, FigureType::PAWN, 4, 7);
    CreateFigure(Color::WHITE, FigureType::PAWN, 7, 4);

    std::vector<std::pair<size_t, size_t>> lower_right_corner_expected_moves;

    for (int i = 1; i <= 2; ++i) {

        lower_right_corner_expected_moves.emplace_back(7-i, 7);
        lower_right_corner_expected_moves.emplace_back(7, 7-i);

    }

    std::vector<std::pair<size_t, size_t>> lower_right_corner_moves = CalculateMovesForSelectedFigure(7, 7);

    EXPECT_TRUE(std::is_permutation(lower_right_corner_expected_moves.begin(), lower_right_corner_expected_moves.end(), lower_right_corner_moves.begin()));

    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::ROOK, 7, 0);
    CreateFigure(Color::WHITE, FigureType::PAWN, 3, 0);
    CreateFigure(Color::WHITE, FigureType::PAWN, 7, 4);
    
    std::vector<std::pair<size_t, size_t>> lower_left_corner_expected_moves;

    for (int i = 1; i <= 3; ++i) {

        lower_left_corner_expected_moves.emplace_back(7-i, 0);
        lower_left_corner_expected_moves.emplace_back(7, i);

    }

    std::vector<std::pair<size_t, size_t>> lower_left_corner_moves = CalculateMovesForSelectedFigure(7, 0);

    EXPECT_TRUE(std::is_permutation(lower_left_corner_expected_moves.begin(), lower_left_corner_expected_moves.end(), lower_left_corner_moves.begin()));

    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::ROOK, 4, 4);
    CreateFigure(Color::WHITE, FigureType::PAWN, 1, 4);
    CreateFigure(Color::WHITE, FigureType::PAWN, 7, 4);
    CreateFigure(Color::WHITE, FigureType::PAWN, 4, 1);
    CreateFigure(Color::WHITE, FigureType::PAWN, 4, 7);

    std::vector<std::pair<size_t, size_t>> center_expected_moves;

    for (int i = 1; i <= 2; ++i) {

        center_expected_moves.emplace_back(4-i, 4);
        center_expected_moves.emplace_back(4, 4-i);
        center_expected_moves.emplace_back(4+i, 4);
        center_expected_moves.emplace_back(4, 4+i);

    }

    std::vector<std::pair<size_t, size_t>> center_moves = CalculateMovesForSelectedFigure(4, 4);

    EXPECT_TRUE(std::is_permutation(center_expected_moves.begin(), center_expected_moves.end(), center_moves.begin()));


}



//==================== Knight tests ====================

TEST_F(EmptyBoardMovesFixture, knight_empty_board_tests) {


    CreateFigure(Color::WHITE, FigureType::KNIGHT, 0, 0);

    std::vector<std::pair<size_t, size_t>> upper_left_corner_expected_moves;

    upper_left_corner_expected_moves.emplace_back(1, 2);
    upper_left_corner_expected_moves.emplace_back(2, 1);

    std::vector<std::pair<size_t, size_t>> upper_left_corner_moves = CalculateMovesForSelectedFigure(0, 0);

    EXPECT_TRUE(std::is_permutation(upper_left_corner_expected_moves.begin(), upper_left_corner_expected_moves.end(), upper_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::KNIGHT, 0, 7);
 
    std::vector<std::pair<size_t, size_t>> upper_right_corner_expected_moves;

    upper_right_corner_expected_moves.emplace_back(1, 5);
    upper_right_corner_expected_moves.emplace_back(2, 6);

    std::vector<std::pair<size_t, size_t>> upper_right_corner_moves = CalculateMovesForSelectedFigure(0, 7);

    EXPECT_TRUE(std::is_permutation(upper_right_corner_expected_moves.begin(), upper_right_corner_expected_moves.end(), upper_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::KNIGHT, 7, 7);

    std::vector<std::pair<size_t, size_t>> lower_right_corner_expected_moves;

    lower_right_corner_expected_moves.emplace_back(5, 6);
    lower_right_corner_expected_moves.emplace_back(6, 5);

    std::vector<std::pair<size_t, size_t>> lower_right_corner_moves = CalculateMovesForSelectedFigure(7, 7);

    EXPECT_TRUE(std::is_permutation(lower_right_corner_expected_moves.begin(), lower_right_corner_expected_moves.end(), lower_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::KNIGHT, 7, 0);
    
    std::vector<std::pair<size_t, size_t>> lower_left_corner_expected_moves;

    lower_left_corner_expected_moves.emplace_back(5, 1);
    lower_left_corner_expected_moves.emplace_back(6, 2);

    std::vector<std::pair<size_t, size_t>> lower_left_corner_moves = CalculateMovesForSelectedFigure(7, 0);

    EXPECT_TRUE(std::is_permutation(lower_left_corner_expected_moves.begin(), lower_left_corner_expected_moves.end(), lower_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::KNIGHT, 4, 4);

    std::vector<std::pair<size_t, size_t>> center_expected_moves;

    center_expected_moves.emplace_back(3, 2);
    center_expected_moves.emplace_back(2, 3);

    center_expected_moves.emplace_back(2, 5);
    center_expected_moves.emplace_back(3, 6);

    center_expected_moves.emplace_back(5, 6);
    center_expected_moves.emplace_back(6, 5);

    center_expected_moves.emplace_back(6, 3);
    center_expected_moves.emplace_back(5, 2);

    std::vector<std::pair<size_t, size_t>> center_moves = CalculateMovesForSelectedFigure(4, 4);

    EXPECT_TRUE(std::is_permutation(center_expected_moves.begin(), center_expected_moves.end(), center_moves.begin()));


}



TEST_F(EmptyBoardMovesFixture, knight_opponent_figures_tests) {


    CreateFigure(Color::WHITE, FigureType::KNIGHT, 0, 0);
    CreateFigure(Color::BLACK, FigureType::PAWN, 1, 2);

    std::vector<std::pair<size_t, size_t>> upper_left_corner_expected_moves;

    upper_left_corner_expected_moves.emplace_back(1, 2);
    upper_left_corner_expected_moves.emplace_back(2, 1);

    std::vector<std::pair<size_t, size_t>> upper_left_corner_moves = CalculateMovesForSelectedFigure(0, 0);
    

    EXPECT_TRUE(std::is_permutation(upper_left_corner_expected_moves.begin(), upper_left_corner_expected_moves.end(), upper_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::KNIGHT, 0, 7);
    CreateFigure(Color::BLACK, FigureType::PAWN, 1, 5);

    std::vector<std::pair<size_t, size_t>> upper_right_corner_expected_moves;

    upper_right_corner_expected_moves.emplace_back(1, 5);
    upper_right_corner_expected_moves.emplace_back(2, 6);

    std::vector<std::pair<size_t, size_t>> upper_right_corner_moves = CalculateMovesForSelectedFigure(0, 7);

    EXPECT_TRUE(std::is_permutation(upper_right_corner_expected_moves.begin(), upper_right_corner_expected_moves.end(), upper_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::KNIGHT, 7, 7);
    CreateFigure(Color::BLACK, FigureType::PAWN, 5, 6);

    std::vector<std::pair<size_t, size_t>> lower_right_corner_expected_moves;

    lower_right_corner_expected_moves.emplace_back(5, 6);
    lower_right_corner_expected_moves.emplace_back(6, 5);

    std::vector<std::pair<size_t, size_t>> lower_right_corner_moves = CalculateMovesForSelectedFigure(7, 7);

    EXPECT_TRUE(std::is_permutation(lower_right_corner_expected_moves.begin(), lower_right_corner_expected_moves.end(), lower_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::KNIGHT, 7, 0);
    CreateFigure(Color::BLACK, FigureType::PAWN, 5, 1);

    std::vector<std::pair<size_t, size_t>> lower_left_corner_expected_moves;

    lower_left_corner_expected_moves.emplace_back(5, 1);
    lower_left_corner_expected_moves.emplace_back(6, 2);

    std::vector<std::pair<size_t, size_t>> lower_left_corner_moves = CalculateMovesForSelectedFigure(7, 0);

    EXPECT_TRUE(std::is_permutation(lower_left_corner_expected_moves.begin(), lower_left_corner_expected_moves.end(), lower_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::KNIGHT, 4, 4);
    CreateFigure(Color::BLACK, FigureType::PAWN, 3, 2);
    CreateFigure(Color::BLACK, FigureType::PAWN, 2, 5);
    CreateFigure(Color::BLACK, FigureType::PAWN, 5, 6);
    CreateFigure(Color::BLACK, FigureType::PAWN, 6, 3);

    std::vector<std::pair<size_t, size_t>> center_expected_moves;

    center_expected_moves.emplace_back(3, 2);
    center_expected_moves.emplace_back(2, 3);

    center_expected_moves.emplace_back(2, 5);
    center_expected_moves.emplace_back(3, 6);

    center_expected_moves.emplace_back(5, 6);
    center_expected_moves.emplace_back(6, 5);

    center_expected_moves.emplace_back(6, 3);
    center_expected_moves.emplace_back(5, 2);

    std::vector<std::pair<size_t, size_t>> center_moves = CalculateMovesForSelectedFigure(4, 4);

    EXPECT_TRUE(std::is_permutation(center_expected_moves.begin(), center_expected_moves.end(), center_moves.begin()));


}


TEST_F(EmptyBoardMovesFixture, knight_own_figures_tests) {


    CreateFigure(Color::WHITE, FigureType::KNIGHT, 0, 0);
    CreateFigure(Color::WHITE, FigureType::PAWN, 1, 2);

    std::vector<std::pair<size_t, size_t>> upper_left_corner_expected_moves;

    upper_left_corner_expected_moves.emplace_back(2, 1);

    std::vector<std::pair<size_t, size_t>> upper_left_corner_moves = CalculateMovesForSelectedFigure(0, 0);
    

    EXPECT_TRUE(std::is_permutation(upper_left_corner_expected_moves.begin(), upper_left_corner_expected_moves.end(), upper_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::KNIGHT, 0, 7);
    CreateFigure(Color::WHITE, FigureType::PAWN, 1, 5);

    std::vector<std::pair<size_t, size_t>> upper_right_corner_expected_moves;

    upper_right_corner_expected_moves.emplace_back(2, 6);

    std::vector<std::pair<size_t, size_t>> upper_right_corner_moves = CalculateMovesForSelectedFigure(0, 7);

    EXPECT_TRUE(std::is_permutation(upper_right_corner_expected_moves.begin(), upper_right_corner_expected_moves.end(), upper_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::KNIGHT, 7, 7);
    CreateFigure(Color::WHITE, FigureType::PAWN, 5, 6);

    std::vector<std::pair<size_t, size_t>> lower_right_corner_expected_moves;

    lower_right_corner_expected_moves.emplace_back(6, 5);

    std::vector<std::pair<size_t, size_t>> lower_right_corner_moves = CalculateMovesForSelectedFigure(7, 7);

    EXPECT_TRUE(std::is_permutation(lower_right_corner_expected_moves.begin(), lower_right_corner_expected_moves.end(), lower_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::KNIGHT, 7, 0);
    CreateFigure(Color::WHITE, FigureType::PAWN, 5, 1);

    std::vector<std::pair<size_t, size_t>> lower_left_corner_expected_moves;

    lower_left_corner_expected_moves.emplace_back(6, 2);

    std::vector<std::pair<size_t, size_t>> lower_left_corner_moves = CalculateMovesForSelectedFigure(7, 0);

    EXPECT_TRUE(std::is_permutation(lower_left_corner_expected_moves.begin(), lower_left_corner_expected_moves.end(), lower_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::KNIGHT, 4, 4);
    CreateFigure(Color::WHITE, FigureType::PAWN, 3, 2);
    CreateFigure(Color::WHITE, FigureType::PAWN, 2, 5);
    CreateFigure(Color::WHITE, FigureType::PAWN, 5, 6);
    CreateFigure(Color::WHITE, FigureType::PAWN, 6, 3);

    std::vector<std::pair<size_t, size_t>> center_expected_moves;

    center_expected_moves.emplace_back(2, 3);
    center_expected_moves.emplace_back(3, 6);
    center_expected_moves.emplace_back(6, 5);
    center_expected_moves.emplace_back(5, 2);

    std::vector<std::pair<size_t, size_t>> center_moves = CalculateMovesForSelectedFigure(4, 4);

    EXPECT_TRUE(std::is_permutation(center_expected_moves.begin(), center_expected_moves.end(), center_moves.begin()));


}



//==================== Bishop tests ====================

TEST_F(EmptyBoardMovesFixture, bishop_empty_board_tests) {


    CreateFigure(Color::WHITE, FigureType::BISHOP, 0, 0);

    std::vector<std::pair<size_t, size_t>> upper_left_corner_expected_moves;

    for (int i = 1; i < 8; ++i) {

        upper_left_corner_expected_moves.emplace_back(i, i);

    }

    std::vector<std::pair<size_t, size_t>> upper_left_corner_moves = CalculateMovesForSelectedFigure(0, 0);
    

    EXPECT_TRUE(std::is_permutation(upper_left_corner_expected_moves.begin(), upper_left_corner_expected_moves.end(), upper_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::BISHOP, 0, 7);
 
    std::vector<std::pair<size_t, size_t>> upper_right_corner_expected_moves;

    for (int i = 0; i < 7; ++i) {

        upper_right_corner_expected_moves.emplace_back(7-i, i);

    }

    std::vector<std::pair<size_t, size_t>> upper_right_corner_moves = CalculateMovesForSelectedFigure(0, 7);

    EXPECT_TRUE(std::is_permutation(upper_right_corner_expected_moves.begin(), upper_right_corner_expected_moves.end(), upper_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::BISHOP, 7, 7);

    std::vector<std::pair<size_t, size_t>> lower_right_corner_expected_moves;

    for (int i = 0; i < 7; ++i) {

        lower_right_corner_expected_moves.emplace_back(i, i);

    }

    std::vector<std::pair<size_t, size_t>> lower_right_corner_moves = CalculateMovesForSelectedFigure(7, 7);

    EXPECT_TRUE(std::is_permutation(lower_right_corner_expected_moves.begin(), lower_right_corner_expected_moves.end(), lower_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::BISHOP, 7, 0);
    
    std::vector<std::pair<size_t, size_t>> lower_left_corner_expected_moves;

    for (int i = 0; i < 7; ++i) {

        lower_left_corner_expected_moves.emplace_back(i, 7-i);

    }

    std::vector<std::pair<size_t, size_t>> lower_left_corner_moves = CalculateMovesForSelectedFigure(7, 0);

    EXPECT_TRUE(std::is_permutation(lower_left_corner_expected_moves.begin(), lower_left_corner_expected_moves.end(), lower_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::BISHOP, 4, 4);

    std::vector<std::pair<size_t, size_t>> center_expected_moves;

    for (int i = 0; i < 8; ++i) {

        if (i == 4) {
            
            continue;

        }

        center_expected_moves.emplace_back(i, i);

    }

    for (int i = 0; i < 7; ++i) {

        if (i == 3) {

            continue;

        }

        center_expected_moves.emplace_back(1+i, 7-i);

    }

    std::vector<std::pair<size_t, size_t>> center_moves = CalculateMovesForSelectedFigure(4, 4);

    EXPECT_TRUE(std::is_permutation(center_expected_moves.begin(), center_expected_moves.end(), center_moves.begin()));


}

TEST_F(EmptyBoardMovesFixture, bishop_opponent_figures_tests) {


    CreateFigure(Color::WHITE, FigureType::BISHOP, 0, 0);
    CreateFigure(Color::BLACK, FigureType::PAWN, 3, 3);

    std::vector<std::pair<size_t, size_t>> upper_left_corner_expected_moves;

    for (int i = 1; i < 4; ++i) {

        upper_left_corner_expected_moves.emplace_back(i, i);

    }

    std::vector<std::pair<size_t, size_t>> upper_left_corner_moves = CalculateMovesForSelectedFigure(0, 0);
    

    EXPECT_TRUE(std::is_permutation(upper_left_corner_expected_moves.begin(), upper_left_corner_expected_moves.end(), upper_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::BISHOP, 0, 7);
    CreateFigure(Color::BLACK, FigureType::PAWN, 3, 4);

    std::vector<std::pair<size_t, size_t>> upper_right_corner_expected_moves;

    for (int i = 1; i < 4; ++i) {

        upper_right_corner_expected_moves.emplace_back(i, 7-i);

    }

    std::vector<std::pair<size_t, size_t>> upper_right_corner_moves = CalculateMovesForSelectedFigure(0, 7);

    EXPECT_TRUE(std::is_permutation(upper_right_corner_expected_moves.begin(), upper_right_corner_expected_moves.end(), upper_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::BISHOP, 7, 7);
    CreateFigure(Color::BLACK, FigureType::PAWN, 4, 4);

    std::vector<std::pair<size_t, size_t>> lower_right_corner_expected_moves;

    for (int i = 4; i < 7; ++i) {

        lower_right_corner_expected_moves.emplace_back(i, i);

    }

    std::vector<std::pair<size_t, size_t>> lower_right_corner_moves = CalculateMovesForSelectedFigure(7, 7);

    EXPECT_TRUE(std::is_permutation(lower_right_corner_expected_moves.begin(), lower_right_corner_expected_moves.end(), lower_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::BISHOP, 7, 0);
    CreateFigure(Color::BLACK, FigureType::PAWN, 4, 3);

    std::vector<std::pair<size_t, size_t>> lower_left_corner_expected_moves;

    for (int i = 1; i < 4; ++i) {

        lower_left_corner_expected_moves.emplace_back(7-i, i);

    }

    std::vector<std::pair<size_t, size_t>> lower_left_corner_moves = CalculateMovesForSelectedFigure(7, 0);

    EXPECT_TRUE(std::is_permutation(lower_left_corner_expected_moves.begin(), lower_left_corner_expected_moves.end(), lower_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::BISHOP, 4, 4);
    CreateFigure(Color::BLACK, FigureType::PAWN, 1, 1);
    CreateFigure(Color::BLACK, FigureType::PAWN, 1, 7);
    CreateFigure(Color::BLACK, FigureType::PAWN, 7, 2);
    CreateFigure(Color::BLACK, FigureType::PAWN, 7, 7);

    std::vector<std::pair<size_t, size_t>> center_expected_moves;

    for (int i = 1; i < 8; ++i) {

        if (i == 4) {
            
            continue;

        }

        center_expected_moves.emplace_back(i, i);

    }

    for (int i = 0; i < 7; ++i) {

        if (i == 3) {

            continue;

        }

        center_expected_moves.emplace_back(1+i, 7-i);

    }

    std::vector<std::pair<size_t, size_t>> center_moves = CalculateMovesForSelectedFigure(4, 4);

    EXPECT_TRUE(std::is_permutation(center_expected_moves.begin(), center_expected_moves.end(), center_moves.begin()));


}

TEST_F(EmptyBoardMovesFixture, bishop_own_figures_tests) {


    CreateFigure(Color::WHITE, FigureType::BISHOP, 0, 0);
    CreateFigure(Color::WHITE, FigureType::PAWN, 3, 3);

    std::vector<std::pair<size_t, size_t>> upper_left_corner_expected_moves;

    for (int i = 1; i < 3; ++i) {

        upper_left_corner_expected_moves.emplace_back(i, i);

    }

    std::vector<std::pair<size_t, size_t>> upper_left_corner_moves = CalculateMovesForSelectedFigure(0, 0);
    

    EXPECT_TRUE(std::is_permutation(upper_left_corner_expected_moves.begin(), upper_left_corner_expected_moves.end(), upper_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::BISHOP, 0, 7);
    CreateFigure(Color::WHITE, FigureType::PAWN, 3, 4);

    std::vector<std::pair<size_t, size_t>> upper_right_corner_expected_moves;

    for (int i = 1; i < 3; ++i) {

        upper_right_corner_expected_moves.emplace_back(i, 7-i);

    }

    std::vector<std::pair<size_t, size_t>> upper_right_corner_moves = CalculateMovesForSelectedFigure(0, 7);

    EXPECT_TRUE(std::is_permutation(upper_right_corner_expected_moves.begin(), upper_right_corner_expected_moves.end(), upper_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::BISHOP, 7, 7);
    CreateFigure(Color::WHITE, FigureType::PAWN, 4, 4);

    std::vector<std::pair<size_t, size_t>> lower_right_corner_expected_moves;

    for (int i = 5; i < 7; ++i) {

        lower_right_corner_expected_moves.emplace_back(i, i);

    }

    std::vector<std::pair<size_t, size_t>> lower_right_corner_moves = CalculateMovesForSelectedFigure(7, 7);

    EXPECT_TRUE(std::is_permutation(lower_right_corner_expected_moves.begin(), lower_right_corner_expected_moves.end(), lower_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::BISHOP, 7, 0);
    CreateFigure(Color::WHITE, FigureType::PAWN, 4, 3);

    std::vector<std::pair<size_t, size_t>> lower_left_corner_expected_moves;

    for (int i = 1; i < 3; ++i) {

        lower_left_corner_expected_moves.emplace_back(7-i, i);

    }

    std::vector<std::pair<size_t, size_t>> lower_left_corner_moves = CalculateMovesForSelectedFigure(7, 0);

    EXPECT_TRUE(std::is_permutation(lower_left_corner_expected_moves.begin(), lower_left_corner_expected_moves.end(), lower_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::BISHOP, 4, 4);
    CreateFigure(Color::WHITE, FigureType::PAWN, 1, 1);
    CreateFigure(Color::WHITE, FigureType::PAWN, 1, 7);
    CreateFigure(Color::WHITE, FigureType::PAWN, 7, 1);
    CreateFigure(Color::WHITE, FigureType::PAWN, 7, 7);

    std::vector<std::pair<size_t, size_t>> center_expected_moves;

    for (int i = 2; i < 7; ++i) {

        if (i == 4) {
            
            continue;

        }

        center_expected_moves.emplace_back(i, i);

    }

    for (int i = 1; i < 6; ++i) {

        if (i == 3) {

            continue;

        }

        center_expected_moves.emplace_back(1+i, 7-i);

    }

    std::vector<std::pair<size_t, size_t>> center_moves = CalculateMovesForSelectedFigure(4, 4);

    EXPECT_TRUE(std::is_permutation(center_expected_moves.begin(), center_expected_moves.end(), center_moves.begin()));


}


//==================== Queen tests ====================

TEST_F(EmptyBoardMovesFixture, queen_empty_board_tests) {


    CreateFigure(Color::WHITE, FigureType::QUEEN, 0, 0);

    std::vector<std::pair<size_t, size_t>> upper_left_corner_expected_moves;

    for (int i = 1; i < 8; ++i) {

        upper_left_corner_expected_moves.emplace_back(0, i);
        upper_left_corner_expected_moves.emplace_back(i, 0);
        
        upper_left_corner_expected_moves.emplace_back(i, i);

    }

    std::vector<std::pair<size_t, size_t>> upper_left_corner_moves = CalculateMovesForSelectedFigure(0, 0);

    EXPECT_TRUE(std::is_permutation(upper_left_corner_expected_moves.begin(), upper_left_corner_expected_moves.end(), upper_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::QUEEN, 0, 7);

    std::vector<std::pair<size_t, size_t>> upper_right_corner_expected_moves;

    for (int i = 0; i < 7; ++i) {

        upper_right_corner_expected_moves.emplace_back(0, i);
        upper_right_corner_expected_moves.emplace_back(7-i, 7);

        upper_right_corner_expected_moves.emplace_back(7-i, i);

    }

    std::vector<std::pair<size_t, size_t>> upper_right_corner_moves = CalculateMovesForSelectedFigure(0, 7);

    EXPECT_TRUE(std::is_permutation(upper_right_corner_expected_moves.begin(), upper_right_corner_expected_moves.end(), upper_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::QUEEN, 7, 7);

    std::vector<std::pair<size_t, size_t>> lower_right_corner_expected_moves;

    for (int i = 0; i < 7; ++i) {

        lower_right_corner_expected_moves.emplace_back(7, i);
        lower_right_corner_expected_moves.emplace_back(i, 7);
        
        lower_right_corner_expected_moves.emplace_back(i, i);

    }

    std::vector<std::pair<size_t, size_t>> lower_right_corner_moves = CalculateMovesForSelectedFigure(7, 7);

    EXPECT_TRUE(std::is_permutation(lower_right_corner_expected_moves.begin(), lower_right_corner_expected_moves.end(), lower_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::QUEEN, 7, 0);
    
    std::vector<std::pair<size_t, size_t>> lower_left_corner_expected_moves;

    for (int i = 0; i < 7; ++i) {

        lower_left_corner_expected_moves.emplace_back(i, 0);
        lower_left_corner_expected_moves.emplace_back(7, 7-i);

        lower_left_corner_expected_moves.emplace_back(i, 7-i);

    }

    std::vector<std::pair<size_t, size_t>> lower_left_corner_moves = CalculateMovesForSelectedFigure(7, 0);

    EXPECT_TRUE(std::is_permutation(lower_left_corner_expected_moves.begin(), lower_left_corner_expected_moves.end(), lower_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::QUEEN, 4, 4);

    std::vector<std::pair<size_t, size_t>> center_expected_moves;

    for (int i = 1; i < 4; ++i) {

        center_expected_moves.emplace_back(4+i, 4-i);
        center_expected_moves.emplace_back(4+i, 4);
        center_expected_moves.emplace_back(4+i, 4+i);
        center_expected_moves.emplace_back(4, 4+i);
        center_expected_moves.emplace_back(4-i, 4+i);

    }

    for (int i = 1; i < 5; ++i) {

        center_expected_moves.emplace_back(4, 4-i);
        center_expected_moves.emplace_back(4-i, 4-i);
        center_expected_moves.emplace_back(4-i, 4);

    }


    std::vector<std::pair<size_t, size_t>> center_moves = CalculateMovesForSelectedFigure(4, 4);

    EXPECT_TRUE(std::is_permutation(center_expected_moves.begin(), center_expected_moves.end(), center_moves.begin()));



}

TEST_F(EmptyBoardMovesFixture, queen_opponent_figures_tests) {


    CreateFigure(Color::WHITE, FigureType::QUEEN, 0, 0);
    CreateFigure(Color::BLACK, FigureType::PAWN, 4, 0);
    CreateFigure(Color::BLACK, FigureType::PAWN, 4, 4);
    CreateFigure(Color::BLACK, FigureType::PAWN, 0, 4);

    std::vector<std::pair<size_t, size_t>> upper_left_corner_expected_moves;

    for (int i = 1; i < 5; ++i) {

        upper_left_corner_expected_moves.emplace_back(0, i);
        upper_left_corner_expected_moves.emplace_back(i, 0);
        
        upper_left_corner_expected_moves.emplace_back(i, i);

    }

    std::vector<std::pair<size_t, size_t>> upper_left_corner_moves = CalculateMovesForSelectedFigure(0, 0);

    EXPECT_TRUE(std::is_permutation(upper_left_corner_expected_moves.begin(), upper_left_corner_expected_moves.end(), upper_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::QUEEN, 0, 7);
    CreateFigure(Color::BLACK, FigureType::PAWN, 0, 4);
    CreateFigure(Color::BLACK, FigureType::PAWN, 3, 4);
    CreateFigure(Color::BLACK, FigureType::PAWN, 3, 7);   

    std::vector<std::pair<size_t, size_t>> upper_right_corner_expected_moves;

    for (int i = 4; i < 7; ++i) {

        upper_right_corner_expected_moves.emplace_back(0, i);
        upper_right_corner_expected_moves.emplace_back(7-i, 7);

        upper_right_corner_expected_moves.emplace_back(7-i, i);

    }

    std::vector<std::pair<size_t, size_t>> upper_right_corner_moves = CalculateMovesForSelectedFigure(0, 7);

    EXPECT_TRUE(std::is_permutation(upper_right_corner_expected_moves.begin(), upper_right_corner_expected_moves.end(), upper_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::QUEEN, 7, 7);
    CreateFigure(Color::BLACK, FigureType::PAWN, 7, 3);
    CreateFigure(Color::BLACK, FigureType::PAWN, 3, 3);
    CreateFigure(Color::BLACK, FigureType::PAWN, 3, 7);

    std::vector<std::pair<size_t, size_t>> lower_right_corner_expected_moves;

    for (int i = 3; i < 7; ++i) {

        lower_right_corner_expected_moves.emplace_back(7, i);
        lower_right_corner_expected_moves.emplace_back(i, 7);
        
        lower_right_corner_expected_moves.emplace_back(i, i);

    }

    std::vector<std::pair<size_t, size_t>> lower_right_corner_moves = CalculateMovesForSelectedFigure(7, 7);

    EXPECT_TRUE(std::is_permutation(lower_right_corner_expected_moves.begin(), lower_right_corner_expected_moves.end(), lower_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::QUEEN, 7, 0);
    CreateFigure(Color::BLACK, FigureType::PAWN, 4, 0);
    CreateFigure(Color::BLACK, FigureType::PAWN, 4, 3);
    CreateFigure(Color::BLACK, FigureType::PAWN, 7, 3);

    std::vector<std::pair<size_t, size_t>> lower_left_corner_expected_moves;

    for (int i = 1; i < 4; ++i) {

        lower_left_corner_expected_moves.emplace_back(7-i, 0);
        lower_left_corner_expected_moves.emplace_back(7, i);

        lower_left_corner_expected_moves.emplace_back(7-i, i);

    }

    std::vector<std::pair<size_t, size_t>> lower_left_corner_moves = CalculateMovesForSelectedFigure(7, 0);

    EXPECT_TRUE(std::is_permutation(lower_left_corner_expected_moves.begin(), lower_left_corner_expected_moves.end(), lower_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::QUEEN, 4, 4);
    CreateFigure(Color::BLACK, FigureType::PAWN, 7, 1);
    CreateFigure(Color::BLACK, FigureType::PAWN, 7, 4);
    CreateFigure(Color::BLACK, FigureType::PAWN, 7, 7);
    CreateFigure(Color::BLACK, FigureType::PAWN, 4, 7);
    CreateFigure(Color::BLACK, FigureType::PAWN, 1, 7);
    CreateFigure(Color::BLACK, FigureType::PAWN, 1, 4);
    CreateFigure(Color::BLACK, FigureType::PAWN, 1, 1);
    CreateFigure(Color::BLACK, FigureType::PAWN, 4, 1);

    std::vector<std::pair<size_t, size_t>> center_expected_moves;

    for (int i = 1; i < 4; ++i) {

        center_expected_moves.emplace_back(4+i, 4-i);
        center_expected_moves.emplace_back(4+i, 4);
        center_expected_moves.emplace_back(4+i, 4+i);
        center_expected_moves.emplace_back(4, 4+i);
        center_expected_moves.emplace_back(4-i, 4+i);
        center_expected_moves.emplace_back(4-i, 4);
        center_expected_moves.emplace_back(4-i, 4-i);
        center_expected_moves.emplace_back(4, 4-i);

    }

    std::vector<std::pair<size_t, size_t>> center_moves = CalculateMovesForSelectedFigure(4, 4);

    EXPECT_TRUE(std::is_permutation(center_expected_moves.begin(), center_expected_moves.end(), center_moves.begin()));




}

TEST_F(EmptyBoardMovesFixture, queen_own_figures_tests) {


    CreateFigure(Color::WHITE, FigureType::QUEEN, 0, 0);
    CreateFigure(Color::WHITE, FigureType::PAWN, 4, 0);
    CreateFigure(Color::WHITE, FigureType::PAWN, 4, 4);
    CreateFigure(Color::WHITE, FigureType::PAWN, 0, 4);

    std::vector<std::pair<size_t, size_t>> upper_left_corner_expected_moves;

    for (int i = 1; i < 4; ++i) {

        upper_left_corner_expected_moves.emplace_back(0, i);
        upper_left_corner_expected_moves.emplace_back(i, 0);
        
        upper_left_corner_expected_moves.emplace_back(i, i);

    }

    std::vector<std::pair<size_t, size_t>> upper_left_corner_moves = CalculateMovesForSelectedFigure(0, 0);

    EXPECT_TRUE(std::is_permutation(upper_left_corner_expected_moves.begin(), upper_left_corner_expected_moves.end(), upper_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::QUEEN, 0, 7);
    CreateFigure(Color::WHITE, FigureType::PAWN, 0, 4);
    CreateFigure(Color::WHITE, FigureType::PAWN, 3, 4);
    CreateFigure(Color::WHITE, FigureType::PAWN, 3, 7);   

    std::vector<std::pair<size_t, size_t>> upper_right_corner_expected_moves;

    for (int i = 5; i < 7; ++i) {

        upper_right_corner_expected_moves.emplace_back(0, i);
        upper_right_corner_expected_moves.emplace_back(7-i, 7);

        upper_right_corner_expected_moves.emplace_back(7-i, i);

    }

    std::vector<std::pair<size_t, size_t>> upper_right_corner_moves = CalculateMovesForSelectedFigure(0, 7);

    EXPECT_TRUE(std::is_permutation(upper_right_corner_expected_moves.begin(), upper_right_corner_expected_moves.end(), upper_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::QUEEN, 7, 7);
    CreateFigure(Color::WHITE, FigureType::PAWN, 7, 3);
    CreateFigure(Color::WHITE, FigureType::PAWN, 3, 3);
    CreateFigure(Color::WHITE, FigureType::PAWN, 3, 7);

    std::vector<std::pair<size_t, size_t>> lower_right_corner_expected_moves;

    for (int i = 4; i < 7; ++i) {

        lower_right_corner_expected_moves.emplace_back(7, i);
        lower_right_corner_expected_moves.emplace_back(i, 7);
        
        lower_right_corner_expected_moves.emplace_back(i, i);

    }

    std::vector<std::pair<size_t, size_t>> lower_right_corner_moves = CalculateMovesForSelectedFigure(7, 7);

    EXPECT_TRUE(std::is_permutation(lower_right_corner_expected_moves.begin(), lower_right_corner_expected_moves.end(), lower_right_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::QUEEN, 7, 0);
    CreateFigure(Color::WHITE, FigureType::PAWN, 4, 0);
    CreateFigure(Color::WHITE, FigureType::PAWN, 4, 3);
    CreateFigure(Color::WHITE, FigureType::PAWN, 7, 3);

    std::vector<std::pair<size_t, size_t>> lower_left_corner_expected_moves;

    for (int i = 1; i < 3; ++i) {

        lower_left_corner_expected_moves.emplace_back(7-i, 0);
        lower_left_corner_expected_moves.emplace_back(7, i);

        lower_left_corner_expected_moves.emplace_back(7-i, i);

    }

    std::vector<std::pair<size_t, size_t>> lower_left_corner_moves = CalculateMovesForSelectedFigure(7, 0);

    EXPECT_TRUE(std::is_permutation(lower_left_corner_expected_moves.begin(), lower_left_corner_expected_moves.end(), lower_left_corner_moves.begin()));

    TearDown();
    SetUp();



    CreateFigure(Color::WHITE, FigureType::QUEEN, 4, 4);
    CreateFigure(Color::WHITE, FigureType::PAWN, 7, 1);
    CreateFigure(Color::WHITE, FigureType::PAWN, 7, 4);
    CreateFigure(Color::WHITE, FigureType::PAWN, 7, 7);
    CreateFigure(Color::WHITE, FigureType::PAWN, 4, 7);
    CreateFigure(Color::WHITE, FigureType::PAWN, 1, 7);
    CreateFigure(Color::WHITE, FigureType::PAWN, 1, 4);
    CreateFigure(Color::WHITE, FigureType::PAWN, 1, 1);
    CreateFigure(Color::WHITE, FigureType::PAWN, 4, 1);

    std::vector<std::pair<size_t, size_t>> center_expected_moves;

    for (int i = 1; i < 3; ++i) {

        center_expected_moves.emplace_back(4+i, 4-i);
        center_expected_moves.emplace_back(4+i, 4);
        center_expected_moves.emplace_back(4+i, 4+i);
        center_expected_moves.emplace_back(4, 4+i);
        center_expected_moves.emplace_back(4-i, 4+i);
        center_expected_moves.emplace_back(4-i, 4);
        center_expected_moves.emplace_back(4-i, 4-i);
        center_expected_moves.emplace_back(4, 4-i);

    }

    std::vector<std::pair<size_t, size_t>> center_moves = CalculateMovesForSelectedFigure(4, 4);

    EXPECT_TRUE(std::is_permutation(center_expected_moves.begin(), center_expected_moves.end(), center_moves.begin()));




}



//==================== King tests ====================

TEST_F(EmptyBoardMovesFixture, king_empty_board_tests) {


    CreateFigure(Color::WHITE, FigureType::KING, 0, 0);
    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::KING, 0, 7);
    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::KING, 7, 7);
    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::KING, 7, 0);
    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::KING, 4, 4);
    TearDown();
    SetUp();


}

TEST_F(EmptyBoardMovesFixture, king_opponent_figures_tests) {


    CreateFigure(Color::WHITE, FigureType::KING, 0, 0);
    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::KING, 0, 7);
    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::KING, 7, 7);
    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::KING, 7, 0);
    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::KING, 4, 4);
    TearDown();
    SetUp();


}

TEST_F(EmptyBoardMovesFixture, king_own_figures_tests) {


    CreateFigure(Color::WHITE, FigureType::KING, 0, 0);
    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::KING, 0, 7);
    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::KING, 7, 7);
    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::KING, 7, 0);
    TearDown();
    SetUp();


    CreateFigure(Color::WHITE, FigureType::KING, 4, 4);
    TearDown();
    SetUp();


}



#include <algorithm>

#include "FirstMoveFixture.h"

using namespace testing;

TEST_F(FirstMoveFixture, made_first_step_tests) {

    for (size_t i = 0; i < 8; ++i) {

        for (size_t j = 0; j < 8; ++j) {

            EXPECT_FALSE(CheckIfMadeFirstStep(i, j));

        }

    }

}

TEST_F(FirstMoveFixture, caclulate_pawn_first_moves) {

    
    for (size_t i = 0; i < 8; ++i) {

        std::vector<std::pair<size_t, size_t>> expected_black_pawn_moves;
        
        expected_black_pawn_moves.emplace_back(2, i);
        expected_black_pawn_moves.emplace_back(3, i);
    
        std::vector<std::pair<size_t, size_t>> black_pawn_moves = CalculateMovesForSelectedFigure(1, i);

        EXPECT_TRUE(std::is_permutation(expected_black_pawn_moves.begin(), expected_black_pawn_moves.end(), black_pawn_moves.begin()));

        
        std::vector<std::pair<size_t, size_t>> expected_white_pawn_moves;
        
        expected_white_pawn_moves.emplace_back(5, i);
        expected_white_pawn_moves.emplace_back(4, i);

        std::vector<std::pair<size_t, size_t>> white_pawn_moves = CalculateMovesForSelectedFigure(6, i);

        EXPECT_TRUE(std::is_permutation(expected_white_pawn_moves.begin(), expected_white_pawn_moves.end(), white_pawn_moves.begin()));

    }


}


TEST_F(FirstMoveFixture, caclulate_non_pawn_first_moves) {

    
    for (size_t i = 0; i < 8; ++i) {

        if (i == 1 || i == 6) {

            continue;

        }

        std::vector<std::pair<size_t, size_t>> black_non_pawn_moves = CalculateMovesForSelectedFigure(0, i);
        EXPECT_TRUE(black_non_pawn_moves.size() == 0);


        std::vector<std::pair<size_t, size_t>> white_non_pawn_moves = CalculateMovesForSelectedFigure(7, i);
        EXPECT_TRUE(white_non_pawn_moves.size() == 0);

    }


    std::vector<std::pair<size_t, size_t>> black_left_knight_moves = CalculateMovesForSelectedFigure(0, 1);
    
    std::vector<std::pair<size_t, size_t>> expected_black_left_knight_moves;
    expected_black_left_knight_moves.emplace_back(2, 0);
    expected_black_left_knight_moves.emplace_back(2, 2);

    EXPECT_TRUE(std::is_permutation(expected_black_left_knight_moves.begin(), expected_black_left_knight_moves.end(), black_left_knight_moves.begin()));


    std::vector<std::pair<size_t, size_t>> black_right_knight_moves = CalculateMovesForSelectedFigure(0, 6);

    std::vector<std::pair<size_t, size_t>> expected_black_right_knight_moves;
    expected_black_right_knight_moves.emplace_back(2, 5);
    expected_black_right_knight_moves.emplace_back(2, 7);

    EXPECT_TRUE(std::is_permutation(expected_black_right_knight_moves.begin(), expected_black_right_knight_moves.end(), black_right_knight_moves.begin()));



    std::vector<std::pair<size_t, size_t>> white_left_knight_moves = CalculateMovesForSelectedFigure(7, 1);

    std::vector<std::pair<size_t, size_t>> expected_white_left_knight_moves;
    expected_white_left_knight_moves.emplace_back(5, 0);
    expected_white_left_knight_moves.emplace_back(5, 2);

    EXPECT_TRUE(std::is_permutation(expected_white_left_knight_moves.begin(), expected_white_left_knight_moves.end(), white_left_knight_moves.begin()));



    std::vector<std::pair<size_t, size_t>> white_right_knight_moves = CalculateMovesForSelectedFigure(7, 6);

    std::vector<std::pair<size_t, size_t>> expected_white_right_knight_moves;
    expected_white_right_knight_moves.emplace_back(5, 5);
    expected_white_right_knight_moves.emplace_back(5, 7);

    EXPECT_TRUE(std::is_permutation(expected_white_right_knight_moves.begin(), expected_white_right_knight_moves.end(), white_right_knight_moves.begin()));




}


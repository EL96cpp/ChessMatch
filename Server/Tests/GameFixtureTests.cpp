#include <algorithm>

#include "GameFixture.h"

using namespace testing;


TEST_F(GameFixture, correct_figures_setup_tests) {

    EXPECT_EQ(DrawOfferedBy(), Color::EMPTY);
    EXPECT_EQ(GetCurrentTurnPlayerColor(), Color::WHITE);
    EXPECT_EQ(number_of_moves, 0);

}


TEST_F(GameFixture, correct_pawn_moves_tests) {

    
    //Pawn moves testing

    EXPECT_EQ(board_cells[6][0]->GetY(), 6);
    EXPECT_EQ(board_cells[6][0]->GetX(), 0);
    EXPECT_EQ(board_cells[1][0]->GetY(), 1);
    EXPECT_EQ(board_cells[1][0]->GetX(), 0);

    EXPECT_EQ(board_cells[5][0]->GetY(), 5);
    EXPECT_EQ(board_cells[5][0]->GetX(), 0);
    EXPECT_EQ(board_cells[2][0]->GetY(), 2);
    EXPECT_EQ(board_cells[2][0]->GetX(), 0);


    EXPECT_TRUE(MakeMove(6, 0, 5, 0, Color::WHITE));
    EXPECT_EQ(GetCurrentTurnPlayerColor(), Color::BLACK);
    EXPECT_TRUE(board_cells[5][0]->MadeFirstStep());
    EXPECT_EQ(number_of_moves, 1);


    EXPECT_TRUE(MakeMove(1, 0, 2, 0, Color::BLACK));
    EXPECT_EQ(GetCurrentTurnPlayerColor(), Color::WHITE);
    EXPECT_TRUE(board_cells[2][0]->MadeFirstStep());
    EXPECT_EQ(number_of_moves, 2);


    EXPECT_EQ(board_cells[6][0]->GetY(), 6);
    EXPECT_EQ(board_cells[6][0]->GetX(), 0);
    EXPECT_EQ(board_cells[1][0]->GetY(), 1);
    EXPECT_EQ(board_cells[1][0]->GetX(), 0);

    EXPECT_EQ(board_cells[5][0]->GetY(), 5);
    EXPECT_EQ(board_cells[5][0]->GetX(), 0);
    EXPECT_EQ(board_cells[2][0]->GetY(), 2);
    EXPECT_EQ(board_cells[2][0]->GetX(), 0);


}


TEST_F(GameFixture, incorrect_player_turn_tests) {


    //Black player
 
    EXPECT_FALSE(MakeMove(1, 0, 2, 0, Color::BLACK));
    EXPECT_FALSE(board_cells[1][0]->MadeFirstStep());
    
    EXPECT_FALSE(MakeMove(1, 1, 2, 1, Color::BLACK));
    EXPECT_FALSE(board_cells[1][1]->MadeFirstStep());

    EXPECT_FALSE(MakeMove(1, 2, 2, 2, Color::BLACK));
    EXPECT_FALSE(board_cells[1][2]->MadeFirstStep());
   
    EXPECT_FALSE(MakeMove(1, 7, 2, 7, Color::BLACK));
    EXPECT_FALSE(board_cells[1][7]->MadeFirstStep());
   
    EXPECT_FALSE(MakeMove(1, 6, 2, 6, Color::BLACK));
    EXPECT_FALSE(board_cells[1][6]->MadeFirstStep());

    EXPECT_EQ(number_of_moves, 0);

    
    //Chagne player color
    MakeMove(6, 3, 5, 3, Color::WHITE);

    
    //White player
    
    EXPECT_FALSE(MakeMove(6, 0, 5, 0, Color::WHITE));
    EXPECT_FALSE(board_cells[6][0]->MadeFirstStep());

    EXPECT_FALSE(MakeMove(6, 1, 5, 1, Color::WHITE));
    EXPECT_FALSE(board_cells[6][1]->MadeFirstStep());
   
    EXPECT_FALSE(MakeMove(6, 2, 5, 2, Color::WHITE));
    EXPECT_FALSE(board_cells[6][2]->MadeFirstStep());
   
    EXPECT_FALSE(MakeMove(6, 7, 5, 7, Color::WHITE));
    EXPECT_FALSE(board_cells[6][7]->MadeFirstStep());
   
    EXPECT_FALSE(MakeMove(6, 6, 5, 6, Color::WHITE));
    EXPECT_FALSE(board_cells[6][6]->MadeFirstStep());

    EXPECT_EQ(number_of_moves, 1);


}


TEST_F(GameFixture, incorrect_player_coordinates_tests) {


    //White player
 
    EXPECT_FALSE(MakeMove(1, 0, 2, 0, Color::WHITE));
    EXPECT_FALSE(board_cells[1][0]->MadeFirstStep());
    
    EXPECT_FALSE(MakeMove(1, 1, 2, 1, Color::WHITE));
    EXPECT_FALSE(board_cells[1][1]->MadeFirstStep());

    EXPECT_FALSE(MakeMove(1, 2, 2, 2, Color::WHITE));
    EXPECT_FALSE(board_cells[1][2]->MadeFirstStep());
   
    EXPECT_FALSE(MakeMove(1, 7, 2, 7, Color::WHITE));
    EXPECT_FALSE(board_cells[1][7]->MadeFirstStep());
   
    EXPECT_FALSE(MakeMove(1, 6, 2, 6, Color::WHITE));
    EXPECT_FALSE(board_cells[1][6]->MadeFirstStep());

    EXPECT_EQ(number_of_moves, 0);


    //Black player
    
    EXPECT_FALSE(MakeMove(6, 0, 5, 0, Color::BLACK));
    EXPECT_FALSE(board_cells[6][0]->MadeFirstStep());

    EXPECT_FALSE(MakeMove(6, 1, 5, 1, Color::BLACK));
    EXPECT_FALSE(board_cells[6][1]->MadeFirstStep());
   
    EXPECT_FALSE(MakeMove(6, 2, 5, 2, Color::BLACK));
    EXPECT_FALSE(board_cells[6][2]->MadeFirstStep());
   
    EXPECT_FALSE(MakeMove(6, 7, 5, 7, Color::BLACK));
    EXPECT_FALSE(board_cells[6][7]->MadeFirstStep());
   
    EXPECT_FALSE(MakeMove(6, 6, 5, 6, Color::BLACK));
    EXPECT_FALSE(board_cells[6][6]->MadeFirstStep());

    EXPECT_EQ(number_of_moves, 0);


}


TEST_F(GameFixture, pawn_eats_figure_tests) {


    //White pawn eats
    
    MakeMove(6, 0, 4, 0, Color::WHITE);
    MakeMove(1, 1, 3, 1, Color::BLACK);

    EXPECT_EQ(number_of_moves, 2);


    EXPECT_TRUE(EatFigure(4, 0, 3, 1, Color::WHITE));
    EXPECT_EQ(board_cells[3][1]->GetColor(), Color::WHITE);
    EXPECT_EQ(board_cells[3][1]->GetType(), FigureType::PAWN);
    EXPECT_TRUE(board_cells[3][1]->MadeFirstStep());
    
    EXPECT_EQ(board_cells[4][0]->GetColor(), Color::EMPTY);
    EXPECT_EQ(board_cells[4][0]->GetType(), FigureType::EMPTY);
    
    EXPECT_EQ(taken_by_white_player[0]->GetColor(), Color::BLACK);
    EXPECT_EQ(taken_by_white_player[0]->GetType(), FigureType::PAWN);

    EXPECT_EQ(number_of_moves, 3);


    //Black pawn eats
    
    MakeMove(1, 7, 3, 7, Color::BLACK);
    MakeMove(6, 6, 4, 6, Color::WHITE);

    EXPECT_EQ(number_of_moves, 5);


    EXPECT_TRUE(EatFigure(3, 7, 4, 6, Color::BLACK));
    EXPECT_EQ(board_cells[4][6]->GetColor(), Color::BLACK);
    EXPECT_EQ(board_cells[4][6]->GetType(), FigureType::PAWN);
    EXPECT_TRUE(board_cells[4][6]->MadeFirstStep());
    
    EXPECT_EQ(board_cells[3][7]->GetColor(), Color::EMPTY);
    EXPECT_EQ(board_cells[3][7]->GetType(), FigureType::EMPTY);

    EXPECT_EQ(taken_by_black_player[0]->GetColor(), Color::WHITE);
    EXPECT_EQ(taken_by_black_player[0]->GetType(), FigureType::PAWN);

    EXPECT_EQ(number_of_moves, 6);


}


TEST_F(GameFixture, rook_eats_figure_tests) {


    MakeMove(6, 0, 4, 0, Color::WHITE);
    MakeMove(1, 7, 3, 7, Color::BLACK);
    MakeMove(7, 0, 5, 0, Color::WHITE);
    MakeMove(3, 7, 4, 7, Color::BLACK);
    MakeMove(6, 2, 4, 2, Color::WHITE);
    MakeMove(4, 7, 5, 7, Color::BLACK);

    EXPECT_EQ(number_of_moves, 6);


    //White rook eats

    EXPECT_TRUE(EatFigure(5, 0, 5, 7, Color::WHITE));
    EXPECT_EQ(board_cells[5][0]->GetColor(), Color::EMPTY);
    EXPECT_EQ(board_cells[5][0]->GetType(), FigureType::EMPTY);

    EXPECT_EQ(board_cells[5][7]->GetColor(), Color::WHITE);
    EXPECT_EQ(board_cells[5][7]->GetType(), FigureType::ROOK);
    EXPECT_TRUE(board_cells[5][7]->MadeFirstStep());
    EXPECT_EQ(taken_by_white_player[0]->GetColor(), Color::BLACK);
    EXPECT_EQ(taken_by_white_player[0]->GetType(), FigureType::PAWN);
    EXPECT_EQ(current_turn_color, Color::BLACK);
    
    EXPECT_EQ(number_of_moves, 7);


    //Black rook eats

    EXPECT_TRUE(EatFigure(0, 7, 5, 7, Color::BLACK));
    EXPECT_EQ(board_cells[0][7]->GetColor(), Color::EMPTY);
    EXPECT_EQ(board_cells[0][7]->GetType(), FigureType::EMPTY);

    EXPECT_EQ(board_cells[5][7]->GetColor(), Color::BLACK);
    EXPECT_EQ(board_cells[5][7]->GetType(), FigureType::ROOK);
    EXPECT_TRUE(board_cells[5][7]->MadeFirstStep());
    EXPECT_EQ(taken_by_black_player[0]->GetColor(), Color::WHITE);
    EXPECT_EQ(taken_by_black_player[0]->GetType(), FigureType::ROOK);
    EXPECT_EQ(current_turn_color, Color::WHITE);
    
    EXPECT_EQ(number_of_moves, 8);


}

TEST_F(GameFixture, knight_eats_figure_tests) {


    MakeMove(7, 1, 5, 2, Color::WHITE);
    MakeMove(1, 3, 3, 3, Color::BLACK);

    EXPECT_EQ(number_of_moves, 2);


    //White knight eats

    EXPECT_TRUE(EatFigure(5, 2, 3, 3, Color::WHITE));
    EXPECT_EQ(board_cells[5][2]->GetColor(), Color::EMPTY);
    EXPECT_EQ(board_cells[5][2]->GetType(), FigureType::EMPTY);

    EXPECT_EQ(board_cells[3][3]->GetColor(), Color::WHITE);
    EXPECT_EQ(board_cells[3][3]->GetType(), FigureType::KNIGHT);
    EXPECT_TRUE(board_cells[3][3]->MadeFirstStep());
    EXPECT_EQ(taken_by_white_player[0]->GetColor(), Color::BLACK);
    EXPECT_EQ(taken_by_white_player[0]->GetType(), FigureType::PAWN);
    EXPECT_EQ(current_turn_color, Color::BLACK);
    
    EXPECT_EQ(number_of_moves, 3);


    
    MakeMove(0, 6, 2, 5, Color::BLACK);    
    MakeMove(6, 7, 5, 7, Color::WHITE);
    
    EXPECT_EQ(number_of_moves, 5);


    //Black knight eats 

    EXPECT_TRUE(EatFigure(2, 5, 3, 3, Color::BLACK));
    EXPECT_EQ(board_cells[2][5]->GetColor(), Color::EMPTY);
    EXPECT_EQ(board_cells[2][5]->GetType(), FigureType::EMPTY);

    EXPECT_EQ(board_cells[3][3]->GetColor(), Color::BLACK);
    EXPECT_EQ(board_cells[3][3]->GetType(), FigureType::KNIGHT);
    EXPECT_TRUE(board_cells[3][3]->MadeFirstStep());
    EXPECT_EQ(taken_by_black_player[0]->GetColor(), Color::WHITE);
    EXPECT_EQ(taken_by_black_player[0]->GetType(), FigureType::KNIGHT);
    EXPECT_EQ(current_turn_color, Color::WHITE);
    
    EXPECT_EQ(number_of_moves, 6);


}

TEST_F(GameFixture, bishop_eats_figure_tests) {


    MakeMove(6, 3, 4, 3, Color::WHITE);
    MakeMove(1, 7, 2, 7, Color::BLACK);
    
    EXPECT_EQ(number_of_moves, 2);


    //White bishop eats

    EXPECT_TRUE(EatFigure(7, 2, 2, 7, Color::WHITE));
    EXPECT_EQ(board_cells[7][2]->GetColor(), Color::EMPTY);
    EXPECT_EQ(board_cells[7][2]->GetType(), FigureType::EMPTY);

    EXPECT_EQ(board_cells[2][7]->GetColor(), Color::WHITE);
    EXPECT_EQ(board_cells[2][7]->GetType(), FigureType::BISHOP);
    EXPECT_TRUE(board_cells[2][7]->MadeFirstStep());
    EXPECT_EQ(taken_by_white_player[0]->GetColor(), Color::BLACK);
    EXPECT_EQ(taken_by_white_player[0]->GetType(), FigureType::PAWN);
    EXPECT_EQ(current_turn_color, Color::BLACK);
    
    EXPECT_EQ(number_of_moves, 3);


    MakeMove(1, 3, 3, 3, Color::BLACK);
    MakeMove(6, 7, 5, 7, Color::WHITE);

    EXPECT_EQ(number_of_moves, 5);

        
    //Black bishop eats

    EXPECT_TRUE(EatFigure(0, 2, 5, 7, Color::BLACK));
    EXPECT_EQ(board_cells[0][2]->GetColor(), Color::EMPTY);
    EXPECT_EQ(board_cells[0][2]->GetType(), FigureType::EMPTY);

    EXPECT_EQ(board_cells[5][7]->GetColor(), Color::BLACK);
    EXPECT_EQ(board_cells[5][7]->GetType(), FigureType::BISHOP);
    EXPECT_TRUE(board_cells[5][7]->MadeFirstStep());
    EXPECT_EQ(taken_by_black_player[0]->GetColor(), Color::WHITE);
    EXPECT_EQ(taken_by_black_player[0]->GetType(), FigureType::PAWN);
    EXPECT_EQ(current_turn_color, Color::WHITE);
    
    EXPECT_EQ(number_of_moves, 6);


}

TEST_F(GameFixture, queen_eats_figure_tests) {
    
    
    MakeMove(6, 4, 4, 4, Color::WHITE);
    MakeMove(1, 0, 2, 0, Color::BLACK);
    MakeMove(7, 3, 3, 7, Color::WHITE);
    MakeMove(2, 0, 3, 0, Color::BLACK);

    //White queen eats

    EXPECT_TRUE(EatFigure(3, 7, 1, 5, Color::WHITE));
    EXPECT_EQ(board_cells[3][7]->GetColor(), Color::EMPTY);
    EXPECT_EQ(board_cells[3][7]->GetType(), FigureType::EMPTY);

    EXPECT_EQ(board_cells[1][5]->GetColor(), Color::WHITE);
    EXPECT_EQ(board_cells[1][5]->GetType(), FigureType::QUEEN);
    EXPECT_TRUE(board_cells[1][5]->MadeFirstStep());
    EXPECT_EQ(taken_by_white_player[0]->GetColor(), Color::BLACK);
    EXPECT_EQ(taken_by_white_player[0]->GetType(), FigureType::PAWN);
    EXPECT_EQ(current_turn_color, Color::BLACK);
    
    EXPECT_EQ(number_of_moves, 5);

    MakeMove(1, 4, 2, 4, Color::BLACK);
    MakeMove(6, 7, 4, 7, Color::WHITE);

    
    //Black queen eats

    EXPECT_TRUE(EatFigure(0, 3, 4, 7, Color::BLACK));
    EXPECT_EQ(board_cells[0][3]->GetColor(), Color::EMPTY);
    EXPECT_EQ(board_cells[0][3]->GetType(), FigureType::EMPTY);

    EXPECT_EQ(board_cells[4][7]->GetColor(), Color::BLACK);
    EXPECT_EQ(board_cells[4][7]->GetType(), FigureType::QUEEN);
    EXPECT_TRUE(board_cells[4][7]->MadeFirstStep());
    EXPECT_EQ(taken_by_black_player[0]->GetColor(), Color::WHITE);
    EXPECT_EQ(taken_by_black_player[0]->GetType(), FigureType::PAWN);
    EXPECT_EQ(current_turn_color, Color::WHITE);
    
    EXPECT_EQ(number_of_moves, 8);


}

TEST_F(GameFixture, king_eats_figure_tests) {


    MakeMove(6, 4, 4, 4, Color::WHITE);
    MakeMove(1, 3, 3, 3, Color::BLACK);
    MakeMove(7, 4, 6, 4, Color::WHITE);
    MakeMove(3, 3, 4, 3, Color::BLACK);
    MakeMove(6, 4, 5, 4, Color::WHITE);
    MakeMove(1, 7, 2, 7, Color::BLACK);

    //White king eats
        
    EXPECT_TRUE(EatFigure(5, 4, 4, 3, Color::WHITE));
    EXPECT_EQ(board_cells[5][4]->GetColor(), Color::EMPTY);
    EXPECT_EQ(board_cells[5][4]->GetType(), FigureType::EMPTY);

    EXPECT_EQ(board_cells[4][3]->GetColor(), Color::WHITE);
    EXPECT_EQ(board_cells[4][3]->GetType(), FigureType::KING);
    EXPECT_TRUE(board_cells[4][3]->MadeFirstStep());
    EXPECT_EQ(taken_by_white_player[0]->GetColor(), Color::BLACK);
    EXPECT_EQ(taken_by_white_player[0]->GetType(), FigureType::PAWN);
    EXPECT_EQ(current_turn_color, Color::BLACK);
    
    EXPECT_EQ(number_of_moves, 7);

    
    //Black king eats

    MakeMove(1, 4, 3, 4, Color::BLACK);
    MakeMove(6, 5, 4, 5, Color::WHITE);
    MakeMove(0, 4, 1, 4, Color::BLACK);
    MakeMove(4, 5, 3, 5, Color::WHITE);
    MakeMove(1, 4, 2, 4, Color::BLACK);
    MakeMove(6, 0, 5, 0, Color::WHITE);

    EXPECT_TRUE(EatFigure(2, 4, 3, 5, Color::BLACK));
    EXPECT_EQ(board_cells[2][4]->GetColor(), Color::EMPTY);
    EXPECT_EQ(board_cells[2][4]->GetType(), FigureType::EMPTY);

    EXPECT_EQ(board_cells[3][5]->GetColor(), Color::BLACK);
    EXPECT_EQ(board_cells[3][5]->GetType(), FigureType::KING);
    EXPECT_TRUE(board_cells[3][5]->MadeFirstStep());
    EXPECT_EQ(taken_by_black_player[0]->GetColor(), Color::WHITE);
    EXPECT_EQ(taken_by_black_player[0]->GetType(), FigureType::PAWN);
    EXPECT_EQ(current_turn_color, Color::WHITE);
    
    EXPECT_EQ(number_of_moves, 14);


}


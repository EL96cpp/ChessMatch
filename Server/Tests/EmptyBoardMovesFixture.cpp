#include "EmptyBoardMovesFixture.h"



void EmptyBoardMovesFixture::SetUp() {


    for (int i = 0; i < 8; ++i) {
        
        std::vector<std::shared_ptr<ChessFigure>> empty_line;

        for (int j = 0; j < 8; ++j) {

            empty_line.push_back(std::make_shared<ChessFigure>(Color::EMPTY, FigureType::EMPTY, i, j));

        }

        board_cells.push_back(empty_line);

    }


}

void EmptyBoardMovesFixture::TearDown() {
    

    for (auto& line : board_cells) {

        line.clear();

    }

    board_cells.clear();


}


std::vector<std::pair<size_t, size_t>> EmptyBoardMovesFixture::CalculateMovesForSelectedFigure(const size_t& y_index, const size_t& x_index) {

    return board_cells[y_index][x_index]->CalculatePossibleMoves(board_cells);

}


void EmptyBoardMovesFixture::CreateFigure(const Color& figure_color, const FigureType& figure_type, const size_t& y_index, const size_t& x_index) {


    if (figure_type == FigureType::PAWN) {


        std::shared_ptr<Pawn> pawn = std::make_shared<Pawn>(figure_color, y_index, x_index);
        pawn->SetMadeFirstStep(true);
        board_cells[y_index][x_index] = pawn;

    } else if (figure_type == FigureType::ROOK) {

        
        std::shared_ptr<Rook> rook = std::make_shared<Rook>(figure_color, y_index, x_index);
        rook->SetMadeFirstStep(true);
        board_cells[y_index][x_index] = rook;


    } else if (figure_type == FigureType::KNIGHT) {


        std::shared_ptr<Knight> knight = std::make_shared<Knight>(figure_color, y_index, x_index);
        knight->SetMadeFirstStep(true);
        board_cells[y_index][x_index] = knight;


    } else if (figure_type == FigureType::BISHOP) {

        
        std::shared_ptr<Bishop> bishop = std::make_shared<Bishop>(figure_color, y_index, x_index);
        bishop->SetMadeFirstStep(true);
        board_cells[y_index][x_index] = bishop;


    } else if (figure_type == FigureType::QUEEN) {

        
        std::shared_ptr<Queen> queen = std::make_shared<Queen>(figure_color, y_index, x_index);
        queen->SetMadeFirstStep(true);
        board_cells[y_index][x_index] = queen;


    } else if (figure_type == FigureType::KING) {

        
        std::shared_ptr<King> king = std::make_shared<King>(figure_color, y_index, x_index);
        king->SetMadeFirstStep(true);
        board_cells[y_index][x_index] = king;


    } else {

        
        std::shared_ptr<ChessFigure> empty_figure =std::make_shared<ChessFigure>(Color::EMPTY, FigureType::EMPTY, y_index, x_index);
        board_cells[y_index][x_index] = empty_figure;


    } 
    

}
 

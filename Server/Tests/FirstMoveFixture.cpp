#include "FirstMoveFixture.h"


void FirstMoveFixture::SetUp() {

 
    //Black figures

    std::vector<std::shared_ptr<ChessFigure>> black_figures;

    black_figures.push_back(std::make_shared<Rook>(Rook(Color::BLACK, 0, 0)));    
    black_figures.push_back(std::make_shared<Knight>(Knight(Color::BLACK, 0, 1))); 
    black_figures.push_back(std::make_shared<Bishop>(Bishop(Color::BLACK, 0, 2)));
    black_figures.push_back(std::make_shared<Queen>(Queen(Color::BLACK, 0, 3))); 
    black_figures.push_back(std::make_shared<King>(King(Color::BLACK, 0, 4))); 
    black_figures.push_back(std::make_shared<Bishop>(Bishop(Color::BLACK, 0, 5))); 
    black_figures.push_back(std::make_shared<Knight>(Knight(Color::BLACK, 0, 6))); 
    black_figures.push_back(std::make_shared<Rook>(Rook(Color::BLACK, 0, 7))); 

    board_cells.push_back(black_figures);
    

    //Black pawns

    std::vector<std::shared_ptr<ChessFigure>> black_pawns;

    for (int i = 0; i < 8; ++i) {

        black_pawns.push_back(std::make_shared<Pawn>(Pawn(Color::BLACK, 1, i)));

    }
    
    board_cells.push_back(black_pawns);


    //Empty fields

    for (int i = 2; i < 6; ++i) {
        
        std::vector<std::shared_ptr<ChessFigure>> empty_line;

        for (int j = 0; j < 8; ++j) {

            empty_line.push_back(std::make_shared<ChessFigure>(Color::EMPTY, FigureType::EMPTY, i, j));

        }

        board_cells.push_back(empty_line);

    }


    //White pawns

    std::vector<std::shared_ptr<ChessFigure>> white_pawns;

    for (int i = 0; i < 8; ++i) {

        white_pawns.push_back(std::make_shared<Pawn>(Pawn(Color::WHITE, 6, i)));

    }

    board_cells.push_back(white_pawns);

    
    //White figures

    std::vector<std::shared_ptr<ChessFigure>> white_figures;
    
    white_figures.push_back(std::make_shared<Rook>(Rook(Color::WHITE, 7, 0)));
    white_figures.push_back(std::make_shared<Knight>(Knight(Color::WHITE, 7, 1)));
    white_figures.push_back(std::make_shared<Bishop>(Bishop(Color::WHITE, 7, 2)));
    white_figures.push_back(std::make_shared<Queen>(Queen(Color::WHITE, 7, 3)));
    white_figures.push_back(std::make_shared<King>(King(Color::WHITE, 7, 4)));
    white_figures.push_back(std::make_shared<Bishop>(Bishop(Color::WHITE, 7, 5)));
    white_figures.push_back(std::make_shared<Knight>(Knight(Color::WHITE, 7, 6)));
    white_figures.push_back(std::make_shared<Rook>(Rook(Color::WHITE, 7, 7)));
 
    board_cells.push_back(white_figures);


}

void FirstMoveFixture::TearDown() {

    
    for (auto& line : board_cells) {

        line.clear();

    }

    board_cells.clear();


}

std::vector<std::pair<size_t, size_t>> FirstMoveFixture::CalculateMovesForSelectedFigure(const size_t& y_index, const size_t& x_index) {

    return board_cells[y_index][x_index]->CalculatePossibleMoves(board_cells);

}
    
bool FirstMoveFixture::CheckIfMadeFirstStep(const size_t& y_index, const size_t& x_index) {

    return board_cells[y_index][x_index]->MadeFirstStep();

}



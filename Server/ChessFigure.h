#pragma once

#include <vector>
#include <cstddef>
#include <memory>
#include <vector>
#include <map>
#include <iostream>


enum class Color {

    WHITE,
    BLACK,
    EMPTY

};

enum class FigureType {

    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,
    EMPTY

};


class ChessFigure {

public:
    ChessFigure(const Color& figure_color, const FigureType& figure_type, const size_t& y_index, const size_t& x_index);

    Color GetColor();
    FigureType GetType();
    bool MadeFirstStep();
    void SetMadeFirstStep(const bool& made_first_step);

    void MoveTo(const size_t& y_index_to, const size_t& x_index_to);
    

    virtual std::vector<std::pair<size_t, size_t>> CalculatePossibleMoves(std::vector<std::vector<std::shared_ptr<ChessFigure>>>& board_cells);


protected:
    Color figure_color;
    FigureType figure_type;
    bool made_first_step;
    size_t y_index;
    size_t x_index;

};

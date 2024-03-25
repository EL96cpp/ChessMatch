#pragma once 

#include <cstddef>
#include <vector>
#include <map>
#include <memory>

#include "ChessFigure.h"

class Pawn : public ChessFigure {

public:
    
    Pawn(const Color& figure_color, const FigureType& figure_type, const size_t& y_index, const size_t& x_index); 

    std::vector<std::pair<size_t, size_t>> CalculatePossibleMoves(std::vector<std::vector<std::shared_ptr<ChessFigure>>>& board_cells) override;
   

};

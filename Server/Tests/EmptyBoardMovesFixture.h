#pragma once

#include <memory>
#include <vector>
#include <gtest/gtest.h>

#include "../Pawn.h"
#include "../Rook.h"
#include "../Knight.h"
#include "../Bishop.h"
#include "../Queen.h"
#include "../King.h"


class EmptyBoardMovesFixture : public ::testing::Test {

public:
    void SetUp() override;
    void TearDown() override;
    
    void CreateFigure(const Color& figure_color, const FigureType& figure_type, const size_t& y_index, const size_t& x_index);
    std::vector<std::pair<size_t, size_t>> CalculateMovesForSelectedFigure(const size_t& y_index, const size_t& x_index);

public:
    std::vector<std::vector<std::shared_ptr<ChessFigure>>> board_cells;

};




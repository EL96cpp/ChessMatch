#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <gtest/gtest.h>

#include "../ChessFigure.h"
#include "../Pawn.h"
#include "../Rook.h"
#include "../Knight.h"
#include "../Bishop.h"
#include "../Queen.h"
#include "../King.h"
#include "../Game.h"
#include "../ThreadSafeQueue.h"
#include "../GameResult.h"


class GameFixture : public ::testing::Test, public Game {

public:
    GameFixture();
    void SetUp() override;
    void TearDown() override;

private:
    ThreadSafeQueue<GameResult> game_results;
    bool first_test;

};





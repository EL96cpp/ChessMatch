#include <algorithm>

#include "GameFixture.h"

using namespace testing;


TEST_F(GameFixture, correct_figures_setup_tests) {

    EXPECT_EQ(DrawOfferedBy(), Color::EMPTY);
    EXPECT_EQ(GetCurrentTurnPlayerColor(), Color::WHITE);
    EXPECT_EQ(number_of_moves, 0);

}


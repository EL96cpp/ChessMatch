#include "GameFixture.h"


GameFixture::GameFixture() : Game(game_results), first_test(true) {}

void GameFixture::SetUp() {

    if (!first_test) {

        CreateStartField();

        draw_offered_by = Color::EMPTY;
        current_turn_color = Color::WHITE;
        number_of_moves = 0;
        game_result_type = GameResultType::IN_PROCESS;
        start_timepoint = std::chrono::system_clock::now();

    }
    
}
    
void GameFixture::TearDown() {

    for (auto& line : board_cells) {

        line.clear();

    }

    board_cells.clear();

    first_test = false;

}




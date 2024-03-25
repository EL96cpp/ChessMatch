#include "Game.h"
#include "ClientConnection.h"
#include "ChessFigure.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"


Game::Game() : white_player(nullptr), black_player(nullptr), draw_offered_by(Color::EMPTY) {}


Game::Game(std::shared_ptr<ClientConnection>& white_player, 
           std::shared_ptr<ClientConnection>& black_player) : white_player(white_player), black_player(black_player),
                                                              current_turn_color(Color::WHITE), draw_offered_by(Color::EMPTY) {

    white_player->SetIsWaiting(false);
    black_player->SetIsWaiting(false);

    CreateStartField();
    SendStartGameMessages();
    
    start_timepoint = std::chrono::system_clock::now();

}

void Game::CreateStartField() {

    //Black figures

    std::vector<std::shared_ptr<ChessFigure>> black_figures;

    black_figures.push_back(std::make_shared<ChessFigure>(Rook(Color::BLACK, 0, 0)));    
    black_figures.push_back(std::make_shared<ChessFigure>(Knight(Color::BLACK, 0, 1))); 
    black_figures.push_back(std::make_shared<ChessFigure>(Color::BLACK, FigureType::BISHOP, 0, 2));
    black_figures.push_back(std::make_shared<ChessFigure>(Color::BLACK, FigureType::QUEEN, 0, 3)); 
    black_figures.push_back(std::make_shared<ChessFigure>(Color::BLACK, FigureType::KING, 0, 4)); 
    black_figures.push_back(std::make_shared<ChessFigure>(Color::BLACK, FigureType::BISHOP, 0, 5)); 
    black_figures.push_back(std::make_shared<ChessFigure>(Knight(Color::BLACK, 0, 6))); 
    black_figures.push_back(std::make_shared<ChessFigure>(Rook(Color::BLACK, 0, 7))); 

    board_cells.push_back(black_figures);
    

    //Black pawns

    std::vector<std::shared_ptr<ChessFigure>> black_pawns;

    for (int i = 0; i < 8; ++i) {

        black_pawns.push_back(std::make_shared<ChessFigure>(Pawn(Color::BLACK, 1, i)));

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

        white_pawns.push_back(std::make_shared<ChessFigure>(Pawn(Color::WHITE, 6, i)));

    }

    board_cells.push_back(white_pawns);

    
    //White figures

    std::vector<std::shared_ptr<ChessFigure>> white_figures;
    
    white_figures.push_back(std::make_shared<ChessFigure>(Rook(Color::WHITE, 7, 0)));
    white_figures.push_back(std::make_shared<ChessFigure>(Knight(Color::WHITE, 7, 1)));
    white_figures.push_back(std::make_shared<ChessFigure>(Color::WHITE, FigureType::BISHOP, 7, 2));
    white_figures.push_back(std::make_shared<ChessFigure>(Color::WHITE, FigureType::QUEEN, 7, 3));
    white_figures.push_back(std::make_shared<ChessFigure>(Color::WHITE, FigureType::KING, 7, 4));
    white_figures.push_back(std::make_shared<ChessFigure>(Color::WHITE, FigureType::BISHOP, 7, 5));
    white_figures.push_back(std::make_shared<ChessFigure>(Knight(Color::WHITE, 7, 6)));
    white_figures.push_back(std::make_shared<ChessFigure>(Rook(Color::WHITE, 7, 7)));
 
    board_cells.push_back(white_figures);


}

void Game::SendStartGameMessages() {


    boost::property_tree::ptree property_tree_white;
    property_tree_white.put("Method", "POST");
    property_tree_white.put("Resource", "Start_game");
    property_tree_white.put("Player_color", "White");

    std::ostringstream json_stream_white;
    boost::property_tree::write_json(json_stream_white, property_tree_white);        
    std::string json_string_white = json_stream_white.str();
    
    std::vector<uint8_t> message_body_white(json_string_white.begin(), json_string_white.end());

    std::shared_ptr<Message> new_message_white = std::make_shared<Message>();
    new_message_white->body = message_body_white;
    new_message_white->message_size = message_body_white.size();

    white_player->SendMessage(new_message_white);
    

    boost::property_tree::ptree property_tree_black;
    property_tree_black.put("Method", "POST");
    property_tree_black.put("Resource", "Start_game");
    property_tree_black.put("Player_color", "Black");

    std::ostringstream json_stream_black;
    boost::property_tree::write_json(json_stream_black, property_tree_black);        
    std::string json_string_black = json_stream_black.str();
    
    std::vector<uint8_t> message_body_black(json_string_black.begin(), json_string_black.end());

    std::shared_ptr<Message> new_message_black = std::make_shared<Message>();
    new_message_black->body = message_body_black;
    new_message_black->message_size = message_body_black.size();

    black_player->SendMessage(new_message_black);

}


Color Game::DrawOfferedBy() {

    return draw_offered_by;

}


Color Game::GetCurrentTurnPlayerColor() {

    return current_turn_color;

}

bool Game::CheckIfPlayerIsAGameMember(std::shared_ptr<ClientConnection>& player) {

    return (white_player == player || black_player == player);

}

bool Game::CheckIfMoveIsCorrect(const size_t& y_from, const size_t& x_from, const size_t& y_to, const size_t& x_to, const Color& player_color) {

    if (board_cells[y_from][x_from]->GetColor() == player_color) {

        std::vector<std::pair<size_t, size_t>> possible_moves = board_cells[y_from][x_from]->CalculatePossibleMoves(board_cells);



    } else {

        return false;

    } 

}


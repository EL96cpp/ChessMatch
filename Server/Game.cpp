#include "Game.h"
#include "ClientConnection.h"
#include "ChessFigure.h"


Game::Game() : white_player(nullptr), black_player(nullptr) {}


Game::Game(std::shared_ptr<ClientConnection>& white_player, 
           std::shared_ptr<ClientConnection>& black_player) : white_player(white_player), 
                                                              black_player(black_player) {

    white_player->SetIsWaiting(false);
    black_player->SetIsWaiting(false);

    CreateStartField();
    SendStartGameMessages();

    
}

void Game::CreateStartField() {

    //Black figures

    std::vector<std::shared_ptr<ChessFigure>> black_figures;

    black_figures.push_back(std::make_shared<ChessFigure>(FigureColor::BLACK, FigureType::ROOK));    
    black_figures.push_back(std::make_shared<ChessFigure>(FigureColor::BLACK, FigureType::KNIGHT)); 
    black_figures.push_back(std::make_shared<ChessFigure>(FigureColor::BLACK, FigureType::BISHOP));
    black_figures.push_back(std::make_shared<ChessFigure>(FigureColor::BLACK, FigureType::QUEEN)); 
    black_figures.push_back(std::make_shared<ChessFigure>(FigureColor::BLACK, FigureType::KING)); 
    black_figures.push_back(std::make_shared<ChessFigure>(FigureColor::BLACK, FigureType::BISHOP)); 
    black_figures.push_back(std::make_shared<ChessFigure>(FigureColor::BLACK, FigureType::KNIGHT)); 
    black_figures.push_back(std::make_shared<ChessFigure>(FigureColor::BLACK, FigureType::ROOK)); 

    board_cells.push_back(black_figures);
    

    //Black pawns

    std::vector<std::shared_ptr<ChessFigure>> black_pawns;

    for (int i = 0; i < 8; ++i) {

        black_pawns.push_back(std::make_shared<ChessFigure>(FigureColor::BLACK, FigureType::PAWN));

    }
    
    board_cells.push_back(black_pawns);


    //Empty fields

    for (int i = 0; i < 4; ++i) {
        
        std::vector<std::shared_ptr<ChessFigure>> empty_line;

        for (int j = 0; j < 8; ++j) {

            empty_line.push_back(std::make_shared<ChessFigure>(FigureColor::EMPTY, FigureType::EMPTY));

        }

        board_cells.push_back(empty_line);

    }
    
    
    //White pawns

    std::vector<std::shared_ptr<ChessFigure>> white_pawns;

    for (int i = 0; i < 8; ++i) {

        white_pawns.push_back(std::make_shared<ChessFigure>(FigureColor::WHITE, FigureType::PAWN));

    }

    board_cells.push_back(white_pawns);

    
    //White figures

    std::vector<std::shared_ptr<ChessFigure>> white_figures;
    
    white_figures.push_back(std::make_shared<ChessFigure>(FigureColor::WHITE, FigureType::ROOK));
    white_figures.push_back(std::make_shared<ChessFigure>(FigureColor::WHITE, FigureType::KNIGHT));
    white_figures.push_back(std::make_shared<ChessFigure>(FigureColor::WHITE, FigureType::BISHOP));
    white_figures.push_back(std::make_shared<ChessFigure>(FigureColor::WHITE, FigureType::QUEEN));
    white_figures.push_back(std::make_shared<ChessFigure>(FigureColor::WHITE, FigureType::KING));
    white_figures.push_back(std::make_shared<ChessFigure>(FigureColor::WHITE, FigureType::BISHOP));
    white_figures.push_back(std::make_shared<ChessFigure>(FigureColor::WHITE, FigureType::KNIGHT));
    white_figures.push_back(std::make_shared<ChessFigure>(FigureColor::WHITE, FigureType::ROOK));
 
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


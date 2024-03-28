#include "GamesManager.h"
#include "Game.h"
#include "GameMessage.h"
#include "ClientConnection.h"


GamesManager::GamesManager(ThreadSafeQueue<GameResult>& game_results) : game_results(game_results) {

    board_navigation_map["1"] = 0;
    board_navigation_map["2"] = 1;
    board_navigation_map["3"] = 2;
    board_navigation_map["4"] = 3;
    board_navigation_map["5"] = 4;
    board_navigation_map["6"] = 5;
    board_navigation_map["7"] = 6;
    board_navigation_map["8"] = 7;

    board_navigation_map["a"] = 0;
    board_navigation_map["b"] = 1;
    board_navigation_map["c"] = 2;
    board_navigation_map["d"] = 3;
    board_navigation_map["e"] = 4;
    board_navigation_map["f"] = 5;
    board_navigation_map["g"] = 6;
    board_navigation_map["h"] = 7;

}

void GamesManager::Start() {


    games_creation_thread = std::thread(&GamesManager::MatchOpponents, this);
    messages_processing_thread = std::thread(&GamesManager::ProcessGameMessages, this);    

    games_creation_thread.join();
    messages_processing_thread.join();

}


void GamesManager::AddWaitingPlayer(std::shared_ptr<ClientConnection>& player) {

    waiting_players.push_back(player);

    std::cout << "Start waiting for opponent, player: " << player->GetNickname() << "\n";

    player->SetIsWaiting(true);

}


ThreadSafeGameMessagesQueue& GamesManager::GetGameMessagesReference() {

    return game_messages;

}


void GamesManager::MatchOpponents() {


    while (true) {

        waiting_players.wait_for_players();        

        std::vector<std::shared_ptr<Game>> new_games = waiting_players.create_games();
        
        for (auto& new_game : new_games) {

            games.push_back(new_game);

        }

        new_games.clear();


    }

}


void GamesManager::ProcessGameMessages() {


    while (true) {

        game_messages.wait(); 

        while (!game_messages.empty()) {

            std::shared_ptr<GameMessage> game_message = game_messages.pop_back();

            std::string message_str(game_message->body.begin(), game_message->body.end());
        
            std::stringstream message_stream(message_str);
            boost::property_tree::ptree root;
            boost::property_tree::read_json(message_stream, root);

            if (root.empty()) {

                std::cout << "empty\n";

            } else {

                if (game_message->game->CheckIfPlayerIsAGameMember(game_message->sender)) {


                    std::string action = root.get<std::string>("Action");

                    if (action == "Make_move") {

                        if (game_message->sender->GetPlayerColor() == game_message->game->GetCurrentTurnPlayerColor()) {
                            
                            std::string letter_from_str = root.get<std::string>("Letter_from");
                            std::string index_from_str = root.get<std::string>("Index_from");
                            std::string letter_to_str = root.get<std::string>("Letter_to");
                            std::string index_to_str = root.get<std::string>("Index_to");
                            
                            size_t y_from = board_navigation_map[letter_from_str];
                            size_t x_from = board_navigation_map[index_from_str];
                            size_t y_to = board_navigation_map[letter_to_str];
                            size_t x_to = board_navigation_map[index_to_str];

                            if (game_message->game->MakeMove(y_from, x_from, y_to, x_to, game_message->sender->GetPlayerColor())) {
                                                        
                                boost::property_tree::ptree move_confirm;
                                move_confirm.put("Method", "POST");
                                move_confirm.put("Action", "Move_accepted");
                                move_confirm.put("Letter_from", letter_from_str);
                                move_confirm.put("Index_from", index_from_str);
                                move_confirm.put("Letter_to", letter_to_str);
                                move_confirm.put("Index_to", index_to_str);


                                std::ostringstream move_confirm_json_stream;
                                boost::property_tree::write_json(move_confirm_json_stream, move_confirm);        
                                std::string move_confirm_json_string = move_confirm_json_stream.str();
                                
                                std::vector<uint8_t> move_confirm_message_body(move_confirm_json_string.begin(), 
                                                                               move_confirm_json_string.end());

                                std::shared_ptr<Message> move_confirm_message = std::make_shared<Message>();
                                move_confirm_message->body = move_confirm_message_body;
                                move_confirm_message->message_size = move_confirm_message_body.size();

                                game_message->game->SendMessageToAll(move_confirm_message);         
                                
                            } else {

                                boost::property_tree::ptree move_error;
                                move_error.put("Method", "POST");
                                move_error.put("Action", "Move_error");
                                move_error.put("Letter_from", letter_from_str);
                                move_error.put("Index_from", index_from_str);
                                move_error.put("Letter_to", letter_to_str);
                                move_error.put("Index_to", index_to_str);


                                std::ostringstream move_error_json_stream;
                                boost::property_tree::write_json(move_error_json_stream, move_error);        
                                std::string move_error_json_string = move_error_json_stream.str();
                                
                                std::vector<uint8_t> move_error_message_body(move_error_json_string.begin(), 
                                                                             move_error_json_string.end());

                                std::shared_ptr<Message> move_error_message = std::make_shared<Message>();
                                move_error_message->body = move_error_message_body;
                                move_error_message->message_size = move_error_message_body.size();
                                
                                game_message->sender->SendMessage(move_error_message);

                            }


                        } else {



                        }


                    } else if (action == "Eat_figure") {
                    
                        
                        std::string letter_from_str = root.get<std::string>("Letter_from");
                        std::string index_from_str = root.get<std::string>("Index_from");
                        std::string letter_to_str = root.get<std::string>("Letter_to");
                        std::string index_to_str = root.get<std::string>("Index_to");


                        if (game_message->sender->GetPlayerColor() == game_message->game->GetCurrentTurnPlayerColor()) {

                           
                            size_t y_from = board_navigation_map[letter_from_str];
                            size_t x_from = board_navigation_map[index_from_str];
                            size_t y_to = board_navigation_map[letter_to_str];
                            size_t x_to = board_navigation_map[index_to_str];

                               




                        } else {

                            boost::property_tree::ptree eat_figure_error;
                            eat_figure_error.put("Method", "POST");
                            eat_figure_error.put("Action", "Eat_figure_error");
                            eat_figure_error.put("Letter_from", letter_from_str);
                            eat_figure_error.put("Index_from", index_from_str);
                            eat_figure_error.put("Letter_to", letter_to_str);
                            eat_figure_error.put("Index_to", index_to_str);


                            std::ostringstream eat_figure_error_json_stream;
                            boost::property_tree::write_json(eat_figure_error_json_stream, eat_figure_error);        
                            std::string eat_figure_error_json_string = eat_figure_error_json_stream.str();
                            
                            std::vector<uint8_t> eat_figure_error_message_body(eat_figure_error_json_string.begin(), 
                                                                           eat_figure_error_json_string.end());

                            std::shared_ptr<Message> eat_figure_error_message = std::make_shared<Message>();
                            eat_figure_error_message->body = eat_figure_error_message_body;
                            eat_figure_error_message->message_size = eat_figure_error_message_body.size();
                            
                            game_message->sender->SendMessage(eat_figure_error_message);

                        }



                    } else if (action == "Make_castling") {


                        std::string letter_from_str = root.get<std::string>("Letter_from");
                        std::string index_from_str = root.get<std::string>("Index_from");
                        std::string letter_to_str = root.get<std::string>("Letter_to");
                        std::string index_to_str = root.get<std::string>("Index_to");


                        if (game_message->sender->GetPlayerColor() == game_message->game->GetCurrentTurnPlayerColor()) { 

                            
                            size_t y_from = board_navigation_map[letter_from_str];
                            size_t x_from = board_navigation_map[index_from_str];
                            size_t y_to = board_navigation_map[letter_to_str];
                            size_t x_to = board_navigation_map[index_to_str];

                            
                            if (game_message->game->MakeCastling(y_from, x_from, y_to, x_to, game_message->sender->GetPlayerColor())) {


                            }


                        } else {

                            

                        }
                        

                    } else if (action == "Transform_pawn") {


                        std::string letter_from_str = root.get<std::string>("Letter_from");
                        std::string index_from_str = root.get<std::string>("Index_from");
                        std::string letter_to_str = root.get<std::string>("Letter_to");
                        std::string index_to_str = root.get<std::string>("Index_to");


                        if (game_message->sender->GetPlayerColor() == game_message->game->GetCurrentTurnPlayerColor()) {

                            size_t y_from = board_navigation_map[letter_from_str];
                            size_t x_from = board_navigation_map[index_from_str];
                            size_t y_to = board_navigation_map[letter_to_str];
                            size_t x_to = board_navigation_map[index_to_str];

                        } else {




                        }

                    } else if (action == "Offer_draw") {

                        if (game_message->game->DrawOfferedBy() == Color::EMPTY) {

                            game_message->game->SetDrawOfferedBy(game_message->sender->GetPlayerColor());    
                            
                            boost::property_tree::ptree property_tree;
                            property_tree.put("Action", "Draw_offered");
                            property_tree.put("Nickname", game_message->sender->GetNickname());
                            
                            std::ostringstream json_stream;
                            boost::property_tree::write_json(json_stream, property_tree);        
                            std::string json_string = json_stream.str();
                            
                            std::vector<uint8_t> message_body(json_string.begin(), json_string.end());

                            std::shared_ptr<Message> new_message = std::make_shared<Message>();
                            new_message->body = message_body;
                            new_message->message_size = message_body.size();

                            game_message->game->SendMessageToAll(new_message);           
                                    

                        }


                    } else if (action == "Accept_draw") {

                    

                    } else if (action == "Cancel_draw") {

                        

                    } else if (action == "Resign") {

                        

                    }

                } else {




                }

            
            }



        }


    }    


}



#include "GamesManager.h"
#include "Game.h"
#include "GameMessage.h"
#include "ClientConnection.h"


GamesManager::GamesManager(ThreadSafeQueue<GameResult>& game_results, ThreadSafeClientsQueue& client_connections) : game_results(game_results), 
                                                                                                                    client_connections(client_connections),
                                                                                                                    waiting_players(game_results) {

    board_navigation_map["1"] = 7;
    board_navigation_map["2"] = 6;
    board_navigation_map["3"] = 5;
    board_navigation_map["4"] = 4;
    board_navigation_map["5"] = 3;
    board_navigation_map["6"] = 2;
    board_navigation_map["7"] = 1;
    board_navigation_map["8"] = 0;

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

            std::cout << "game messages queue is not empty!\n";

            std::shared_ptr<GameMessage> game_message = game_messages.pop_back();

            std::string message_str(game_message->body.begin(), game_message->body.end());
        
            std::stringstream message_stream(message_str);
            boost::property_tree::ptree root;
            boost::property_tree::read_json(message_stream, root);

            if (root.empty()) {

                std::cout << "empty\n";

            } else {


                if (game_message->game != nullptr && game_message->game->CheckIfPlayerIsAGameMember(game_message->sender)) {


                    std::cout << message_str << "\n";

                    std::cout << "Parse game message action\n";

                    std::string action = root.get<std::string>("Action");


                    if (action == "Make_move") {

                        
                        std::cout << "Make move message\n";


                        std::string letter_from_str = root.get<std::string>("Letter_from");
                        std::string index_from_str = root.get<std::string>("Index_from");
                        std::string letter_to_str = root.get<std::string>("Letter_to");
                        std::string index_to_str = root.get<std::string>("Index_to");

                        std::cout << letter_from_str + index_from_str << " " << letter_to_str + index_to_str << "\n";


                        if (game_message->sender->GetPlayerColor() == game_message->game->GetCurrentTurnPlayerColor()) {
                            
                           
                            size_t y_from = board_navigation_map[index_from_str];
                            size_t x_from = board_navigation_map[letter_from_str];
                            size_t y_to = board_navigation_map[index_to_str];
                            size_t x_to = board_navigation_map[letter_to_str];


                            if (game_message->game->MakeMove(y_from, x_from, y_to, x_to, game_message->sender->GetPlayerColor())) {

                                
                                std::cout << "Make move accepted\n";

                                boost::property_tree::ptree move_confirm;
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

                                
                                std::cout << "Make move error\n";

                                boost::property_tree::ptree move_error;
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

                            std::cout << "Incorrect player color!\n";

                            boost::property_tree::ptree move_error;
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


                    } else if (action == "Eat_figure") {
                    
                        
                        std::string letter_from_str = root.get<std::string>("Letter_from");
                        std::string index_from_str = root.get<std::string>("Index_from");
                        std::string letter_to_str = root.get<std::string>("Letter_to");
                        std::string index_to_str = root.get<std::string>("Index_to");
                        std::string transformation_type = root.get<std::string>("Transformation_type");


                        if (game_message->sender->GetPlayerColor() == game_message->game->GetCurrentTurnPlayerColor()) {

                           
                            size_t y_from = board_navigation_map[index_from_str];
                            size_t x_from = board_navigation_map[letter_from_str];
                            size_t y_to = board_navigation_map[index_to_str];
                            size_t x_to = board_navigation_map[letter_to_str];


                            if (game_message->game->EatFigure(y_from, x_from, y_to, x_to, 
                                game_message->sender->GetPlayerColor(), transformation_type)) {


                                if (game_message->game->GetGameResultType() == GameResultType::IN_PROCESS) {


                                    boost::property_tree::ptree eat_figure_accepted;
                                    eat_figure_accepted.put("Action", "Eat_figure_accepted");
                                    eat_figure_accepted.put("Letter_from", letter_from_str);
                                    eat_figure_accepted.put("Index_from", index_from_str);
                                    eat_figure_accepted.put("Letter_to", letter_to_str);
                                    eat_figure_accepted.put("Index_to", index_to_str);
                                    eat_figure_accepted.put("Transformation_type", transformation_type);


                                    std::ostringstream eat_figure_accepted_json_stream;
                                    boost::property_tree::write_json(eat_figure_accepted_json_stream, eat_figure_accepted);        
                                    std::string eat_figure_accepted_json_string = eat_figure_accepted_json_stream.str();
                                    
                                    std::vector<uint8_t> eat_figure_accepted_message_body(eat_figure_accepted_json_string.begin(), 
                                                                                   eat_figure_accepted_json_string.end());

                                    std::shared_ptr<Message> eat_figure_accepted_message = std::make_shared<Message>();
                                    eat_figure_accepted_message->body = eat_figure_accepted_message_body;
                                    eat_figure_accepted_message->message_size = eat_figure_accepted_message_body.size();
                                    
                                    game_message->game->SendMessageToAll(eat_figure_accepted_message);


                                } else if (game_message->game->GetGameResultType() == GameResultType::WHITE_WINS) {


                                    boost::property_tree::ptree game_over;
                                    game_over.put("Action", "Game_over");
                                    game_over.put("Result", "White_wins");


                                    std::ostringstream game_over_json_stream;
                                    boost::property_tree::write_json(game_over_json_stream, game_over);        
                                    std::string game_over_json_string = game_over_json_stream.str();
                                    
                                    std::vector<uint8_t> game_over_message_body(game_over_json_string.begin(), 
                                                                                   game_over_json_string.end());

                                    std::shared_ptr<Message> game_over_message = std::make_shared<Message>();
                                    game_over_message->body = game_over_message_body;
                                    game_over_message->message_size = game_over_message_body.size();
                                    

                                    SendUpdatedRatingToPlayer(game_message->game->GetWhitePlayer());
                                    SendUpdatedRatingToPlayer(game_message->game->GetBlackPlayer());

                                    game_message->game->SendMessageToAll(game_over_message);

 
                                } else if (game_message->game->GetGameResultType() == GameResultType::BLACK_WINS) {


                                    boost::property_tree::ptree game_over;
                                    game_over.put("Action", "Game_over");
                                    game_over.put("Result", "Black_wins");


                                    std::ostringstream game_over_json_stream;
                                    boost::property_tree::write_json(game_over_json_stream, game_over);        
                                    std::string game_over_json_string = game_over_json_stream.str();
                                    
                                    std::vector<uint8_t> game_over_message_body(game_over_json_string.begin(), 
                                                                                   game_over_json_string.end());

                                    std::shared_ptr<Message> game_over_message = std::make_shared<Message>();
                                    game_over_message->body = game_over_message_body;
                                    game_over_message->message_size = game_over_message_body.size();

                                    SendUpdatedRatingToPlayer(game_message->game->GetWhitePlayer());
                                    SendUpdatedRatingToPlayer(game_message->game->GetBlackPlayer());

                                    game_message->game->SendMessageToAll(game_over_message);


                                }


                            } else {

                            
                                boost::property_tree::ptree eat_figure_error;
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


                        } else {

                            
                            boost::property_tree::ptree eat_figure_error;
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

                            
                            size_t y_from = board_navigation_map[index_from_str];
                            size_t x_from = board_navigation_map[letter_from_str];
                            size_t y_to = board_navigation_map[index_to_str];
                            size_t x_to = board_navigation_map[letter_to_str];

                            
                            if (game_message->game->MakeCastling(y_from, x_from, y_to, x_to, game_message->sender->GetPlayerColor())) {

                                
                                std::cout << "Castling accepted!\n";


                                boost::property_tree::ptree make_castling_accepted;
                                make_castling_accepted.put("Action", "Make_castling_accepted");
                                make_castling_accepted.put("Letter_from", letter_from_str);
                                make_castling_accepted.put("Index_from", index_from_str);
                                make_castling_accepted.put("Letter_to", letter_to_str);
                                make_castling_accepted.put("Index_to", index_to_str);


                                std::ostringstream make_castling_accepted_json_stream;
                                boost::property_tree::write_json(make_castling_accepted_json_stream, make_castling_accepted);        
                                std::string make_castling_accepted_json_string = make_castling_accepted_json_stream.str();
                                
                                std::vector<uint8_t> make_castling_accepted_message_body(make_castling_accepted_json_string.begin(), 
                                                                                         make_castling_accepted_json_string.end());

                                std::shared_ptr<Message> make_castling_accepted_message = std::make_shared<Message>();
                                make_castling_accepted_message->body = make_castling_accepted_message_body;
                                make_castling_accepted_message->message_size = make_castling_accepted_message_body.size();
 
                                game_message->game->SendMessageToAll(make_castling_accepted_message);


                            } else {


                                boost::property_tree::ptree make_castling_error;
                                make_castling_error.put("Action", "Make_castling_error");
                                make_castling_error.put("Letter_from", letter_from_str);
                                make_castling_error.put("Index_from", index_from_str);                                
                                make_castling_error.put("Letter_to", letter_to_str);
                                make_castling_error.put("Index_to", index_to_str);


                                std::ostringstream make_castling_error_json_stream;
                                boost::property_tree::write_json(make_castling_error_json_stream, make_castling_error);        
                                std::string make_castling_error_json_string = make_castling_error_json_stream.str();
                                
                                std::vector<uint8_t> make_castling_error_message_body(make_castling_error_json_string.begin(), 
                                                                                      make_castling_error_json_string.end());

                                std::shared_ptr<Message> make_castling_error_message = std::make_shared<Message>();
                                make_castling_error_message->body = make_castling_error_message_body;
                                make_castling_error_message->message_size = make_castling_error_message_body.size();
                                
                                game_message->sender->SendMessage(make_castling_error_message);
     

                            }


                        } else {


                            boost::property_tree::ptree make_castling_error;
                            make_castling_error.put("Action", "Make_castling_error");
                            make_castling_error.put("Letter_from", letter_from_str);
                            make_castling_error.put("Index_from", index_from_str);
                            make_castling_error.put("Letter_to", letter_to_str);
                            make_castling_error.put("Index_to", index_to_str);


                            std::ostringstream make_castling_error_json_stream;
                            boost::property_tree::write_json(make_castling_error_json_stream, make_castling_error);        
                            std::string make_castling_error_json_string = make_castling_error_json_stream.str();
                            
                            std::vector<uint8_t> make_castling_error_message_body(make_castling_error_json_string.begin(), 
                                                                                  make_castling_error_json_string.end());

                            std::shared_ptr<Message> make_castling_error_message = std::make_shared<Message>();
                            make_castling_error_message->body = make_castling_error_message_body;
                            make_castling_error_message->message_size = make_castling_error_message_body.size();
                            
                            game_message->sender->SendMessage(make_castling_error_message);
                           

                        }
                        

                    } else if (action == "Transform_pawn") {


                        std::string letter_from_str = root.get<std::string>("Letter_from");
                        std::string index_from_str = root.get<std::string>("Index_from");
                        std::string letter_to_str = root.get<std::string>("Letter_to");
                        std::string index_to_str = root.get<std::string>("Index_to");
                        std::string figure_type_str = root.get<std::string>("Figure_type");


                        if (game_message->sender->GetPlayerColor() == game_message->game->GetCurrentTurnPlayerColor()) {


                            size_t y_from = board_navigation_map[index_from_str];
                            size_t x_from = board_navigation_map[letter_from_str];
                            size_t y_to = board_navigation_map[index_to_str];
                            size_t x_to = board_navigation_map[letter_to_str];

                            if (game_message->game->TransformPawn(y_from, x_from, y_to, x_to, game_message->sender->GetPlayerColor(), figure_type_str)) {


                                boost::property_tree::ptree transform_pawn_accepted;
                                transform_pawn_accepted.put("Action", "Transform_pawn_accepted");
                                transform_pawn_accepted.put("Letter_from", letter_from_str);
                                transform_pawn_accepted.put("Index_from", index_from_str);
                                transform_pawn_accepted.put("Letter_to", letter_to_str);
                                transform_pawn_accepted.put("Index_to", index_to_str);
                                transform_pawn_accepted.put("Figure_type", figure_type_str);


                                std::ostringstream transform_pawn_accepted_json_stream;
                                boost::property_tree::write_json(transform_pawn_accepted_json_stream, transform_pawn_accepted);        
                                std::string transform_pawn_accepted_json_string = transform_pawn_accepted_json_stream.str();
                                
                                std::vector<uint8_t> transform_pawn_accepted_message_body(transform_pawn_accepted_json_string.begin(), 
                                                                                          transform_pawn_accepted_json_string.end());

                                std::shared_ptr<Message> transform_pawn_accepted_message = std::make_shared<Message>();
                                transform_pawn_accepted_message->body = transform_pawn_accepted_message_body;
                                transform_pawn_accepted_message->message_size = transform_pawn_accepted_message_body.size();                                

                                game_message->game->SendMessageToAll(transform_pawn_accepted_message);


                            } else {


                                boost::property_tree::ptree transform_pawn_error;
                                transform_pawn_error.put("Action", "Transform_pawn_error");
                                transform_pawn_error.put("Letter_from", letter_from_str);
                                transform_pawn_error.put("Index_from", index_from_str);
                                transform_pawn_error.put("Letter_to", letter_to_str);
                                transform_pawn_error.put("Index_to", index_to_str);
                                transform_pawn_error.put("Figure_type", figure_type_str);


                                std::ostringstream transform_pawn_error_json_stream;
                                boost::property_tree::write_json(transform_pawn_error_json_stream, transform_pawn_error);        
                                std::string transform_pawn_error_json_string = transform_pawn_error_json_stream.str();
                                
                                std::vector<uint8_t> transform_pawn_error_message_body(transform_pawn_error_json_string.begin(), 
                                                                                       transform_pawn_error_json_string.end());

                                std::shared_ptr<Message> transform_pawn_error_message = std::make_shared<Message>();
                                transform_pawn_error_message->body = transform_pawn_error_message_body;
                                transform_pawn_error_message->message_size = transform_pawn_error_message_body.size();
                                
                                game_message->sender->SendMessage(transform_pawn_error_message);
     

                            }



                        } else {


                            boost::property_tree::ptree transform_pawn_error;
                            transform_pawn_error.put("Action", "Transform_pawn_error");
                            transform_pawn_error.put("Letter_from", letter_from_str);
                            transform_pawn_error.put("Index_from", index_from_str);
                            transform_pawn_error.put("Letter_to", letter_to_str);
                            transform_pawn_error.put("Index_to", index_to_str);
                            transform_pawn_error.put("Figure_type", figure_type_str);


                            std::ostringstream transform_pawn_error_json_stream;
                            boost::property_tree::write_json(transform_pawn_error_json_stream, transform_pawn_error);        
                            std::string transform_pawn_error_json_string = transform_pawn_error_json_stream.str();
                            
                            std::vector<uint8_t> transform_pawn_error_message_body(transform_pawn_error_json_string.begin(), 
                                                                                   transform_pawn_error_json_string.end());

                            std::shared_ptr<Message> transform_pawn_error_message = std::make_shared<Message>();
                            transform_pawn_error_message->body = transform_pawn_error_message_body;
                            transform_pawn_error_message->message_size = transform_pawn_error_message_body.size();
                            
                            game_message->sender->SendMessage(transform_pawn_error_message);
 

                        }


                    } else if (action == "Offer_draw") {


                        if (game_message->game->DrawOfferedBy() == Color::EMPTY) {


                            game_message->game->SetDrawOfferedBy(game_message->sender->GetPlayerColor());    
                            
                            boost::property_tree::ptree offer_draw;
                            offer_draw.put("Action", "Draw_offered");
                            offer_draw.put("Nickname", game_message->sender->GetNickname());
                            
                            std::ostringstream offer_draw_json_stream;
                            boost::property_tree::write_json(offer_draw_json_stream, offer_draw);        
                            std::string offer_draw_json_string = offer_draw_json_stream.str();
                            
                            std::vector<uint8_t> offer_draw_message_body(offer_draw_json_string.begin(), offer_draw_json_string.end());

                            std::shared_ptr<Message> new_message = std::make_shared<Message>();
                            new_message->body = offer_draw_message_body;
                            new_message->message_size = offer_draw_message_body.size();

                            game_message->game->SendMessageToAll(new_message);           
                                    

                        }


                    } else if (action == "Accept_draw") {

                        
                        if (game_message->game->AcceptDraw(game_message->sender)) {


                            boost::property_tree::ptree game_over;
                            game_over.put("Action", "Game_over");
                            game_over.put("Result", "Draw");

                            std::ostringstream game_over_json_stream;
                            boost::property_tree::write_json(game_over_json_stream, game_over);        
                            std::string game_over_json_string = game_over_json_stream.str();
                            
                            std::vector<uint8_t> game_over_message_body(game_over_json_string.begin(), 
                                                                           game_over_json_string.end());

                            std::shared_ptr<Message> game_over_message = std::make_shared<Message>();
                            game_over_message->body = game_over_message_body;
                            game_over_message->message_size = game_over_message_body.size();

                            
                            SendUpdatedRatingToPlayer(game_message->game->GetWhitePlayer());
                            SendUpdatedRatingToPlayer(game_message->game->GetBlackPlayer());

                            game_message->game->SendMessageToAll(game_over_message);


                        } else {


                            boost::property_tree::ptree offer_draw;
                            offer_draw.put("Action", "Accept_draw_error");
                            
                            std::ostringstream offer_draw_json_stream;
                            boost::property_tree::write_json(offer_draw_json_stream, offer_draw);        
                            std::string offer_draw_json_string = offer_draw_json_stream.str();
                            
                            std::vector<uint8_t> offer_draw_message_body(offer_draw_json_string.begin(), offer_draw_json_string.end());

                            std::shared_ptr<Message> new_message = std::make_shared<Message>();
                            new_message->body = offer_draw_message_body;
                            new_message->message_size = offer_draw_message_body.size();

                            game_message->sender->SendMessage(new_message);


                        }


                    } else if (action == "Cancel_draw") {
                        
                        
                        if (game_message->game->DrawOfferedBy() != Color::EMPTY && game_message->game->DrawOfferedBy() != game_message->sender->GetPlayerColor()) {

                            
                            boost::property_tree::ptree cancel_draw;
                            cancel_draw.put("Action", "Cancel_draw_accepted");
                            
                            std::ostringstream cancel_draw_json_stream;
                            boost::property_tree::write_json(cancel_draw_json_stream, cancel_draw);        
                            std::string cancel_draw_json_string = cancel_draw_json_stream.str();
                            
                            std::vector<uint8_t> cancel_draw_message_body(cancel_draw_json_string.begin(), cancel_draw_json_string.end());

                            std::shared_ptr<Message> new_message = std::make_shared<Message>();
                            new_message->body = cancel_draw_message_body;
                            new_message->message_size = cancel_draw_message_body.size();

                            game_message->game->SendMessageToAll(new_message);


                        } else {


                            boost::property_tree::ptree cancel_draw;
                            cancel_draw.put("Action", "Cancel_draw_error");
                            
                            std::ostringstream cancel_draw_json_stream;
                            boost::property_tree::write_json(cancel_draw_json_stream, cancel_draw);        
                            std::string cancel_draw_json_string = cancel_draw_json_stream.str();
                            
                            std::vector<uint8_t> cancel_draw_message_body(cancel_draw_json_string.begin(), cancel_draw_json_string.end());

                            std::shared_ptr<Message> new_message = std::make_shared<Message>();
                            new_message->body = cancel_draw_message_body;
                            new_message->message_size = cancel_draw_message_body.size();

                            game_message->sender->SendMessage(new_message);


                        }
                        
                        
                    } else if (action == "Resign") {

                        
                        game_message->game->Resign(game_message->sender);
                        
                        if (game_message->sender->GetPlayerColor() == Color::WHITE) {


                            boost::property_tree::ptree game_over;
                            game_over.put("Action", "Game_over");
                            game_over.put("Result", "Black_wins");

                            std::ostringstream game_over_json_stream;
                            boost::property_tree::write_json(game_over_json_stream, game_over);        
                            std::string game_over_json_string = game_over_json_stream.str();
                            
                            std::vector<uint8_t> game_over_message_body(game_over_json_string.begin(), 
                                                                           game_over_json_string.end());

                            std::shared_ptr<Message> game_over_message = std::make_shared<Message>();
                            game_over_message->body = game_over_message_body;
                            game_over_message->message_size = game_over_message_body.size();

                            
                            SendUpdatedRatingToPlayer(game_message->game->GetWhitePlayer());
                            SendUpdatedRatingToPlayer(game_message->game->GetBlackPlayer());

                            game_message->game->SendMessageToAll(game_over_message);


                        } else if (game_message->sender->GetPlayerColor() == Color::BLACK) {

 
                            boost::property_tree::ptree game_over;
                            game_over.put("Action", "Game_over");
                            game_over.put("Result", "White_wins");

                            std::ostringstream game_over_json_stream;
                            boost::property_tree::write_json(game_over_json_stream, game_over);        
                            std::string game_over_json_string = game_over_json_stream.str();
                            
                            std::vector<uint8_t> game_over_message_body(game_over_json_string.begin(), 
                                                                           game_over_json_string.end());

                            std::shared_ptr<Message> game_over_message = std::make_shared<Message>();
                            game_over_message->body = game_over_message_body;
                            game_over_message->message_size = game_over_message_body.size();


                            SendUpdatedRatingToPlayer(game_message->game->GetWhitePlayer());
                            SendUpdatedRatingToPlayer(game_message->game->GetBlackPlayer());

                            game_message->game->SendMessageToAll(game_over_message);
                           

                        }
                        

                    } else if (action == "Disconnect") {

                        
                        std::cout << game_message->sender->GetNickname() << " disconnected from server\n";

                        waiting_players.delete_connection(game_message->sender->GetNickname());
                        client_connections.delete_connection(game_message->sender->GetNickname()); 


                    }


                } else if (game_message->game == nullptr){

                    
                    std::string action = root.get<std::string>("Action");
               
                    if (action == "Stop_waiting") {

                        
                        if (waiting_players.contains_nickname(game_message->sender->GetNickname())) {

                            
                            std::cout << "Stop waitng for opponent accepted!\n";

                            game_message->sender->SetIsWaiting(false);
                            
                            waiting_players.delete_connection(game_message->sender->GetNickname());

                            boost::property_tree::ptree stop_waiting;
                            stop_waiting.put("Action", "Stop_waiting_accepted");

                            std::ostringstream stop_waiting_json_stream;
                            boost::property_tree::write_json(stop_waiting_json_stream, stop_waiting);        
                            std::string stop_waiting_json_string = stop_waiting_json_stream.str();
                            
                            std::vector<uint8_t> stop_waiting_message_body(stop_waiting_json_string.begin(), 
                                                                           stop_waiting_json_string.end());

                            std::shared_ptr<Message> stop_waiting_message = std::make_shared<Message>();
                            stop_waiting_message->body = stop_waiting_message_body;
                            stop_waiting_message->message_size = stop_waiting_message_body.size();

                            game_message->sender->SendMessage(stop_waiting_message);


                        } else {


                            boost::property_tree::ptree stop_waiting_error;
                            stop_waiting_error.put("Action", "Stop_waiting_error");

                            std::ostringstream stop_waiting_error_json_stream;
                            boost::property_tree::write_json(stop_waiting_error_json_stream, stop_waiting_error);        
                            std::string stop_waiting_error_json_string = stop_waiting_error_json_stream.str();
                            
                            std::vector<uint8_t> stop_waiting_error_message_body(stop_waiting_error_json_string.begin(), 
                                                                           stop_waiting_error_json_string.end());

                            std::shared_ptr<Message> stop_waiting_error_message = std::make_shared<Message>();
                            stop_waiting_error_message->body = stop_waiting_error_message_body;
                            stop_waiting_error_message->message_size = stop_waiting_error_message_body.size();

                            game_message->sender->SendMessage(stop_waiting_error_message);



                        }


                    } else if (action == "Disconnect") {


                        std::cout << game_message->sender->GetNickname() << " disconnected from server!\n";
            
                        waiting_players.delete_connection(game_message->sender->GetNickname());
                        client_connections.delete_connection(game_message->sender->GetNickname());


                    }


                } 

            
            }



        }


    }    


}

void GamesManager::SendUpdatedRatingToPlayer(const std::shared_ptr<ClientConnection>& player) {


    size_t new_rating = player->GetRating();

    boost::property_tree::ptree new_rating_tree;
    new_rating_tree.put("Action", "Update_rating");
    new_rating_tree.put("New_rating", new_rating);

    std::ostringstream new_rating_json_stream;
    boost::property_tree::write_json(new_rating_json_stream, new_rating_tree);        
    std::string new_rating_json_string = new_rating_json_stream.str();
    
    std::vector<uint8_t> new_rating_message_body(new_rating_json_string.begin(), 
                                                   new_rating_json_string.end());

    std::shared_ptr<Message> new_rating_message = std::make_shared<Message>();
    new_rating_message->body = new_rating_message_body;
    new_rating_message->message_size = new_rating_message_body.size();

    player->SendMessage(new_rating_message);


}

